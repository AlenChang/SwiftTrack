package com.example.swifttrack;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

import com.example.swifttrack.utils.FileUtil;

import org.jtransforms.fft.DoubleFFT_1D;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class AudioPlayer {

    // public variable can be read by other classes
    public static final int N_ZC = 59;
    public static final int N_ZC_UP = 480;
    public static final int FC = 20000;

    public static boolean USE_FILE = false;
    public static int SAMPLE_RATE = 48000;
    public static int BUFFER_SIZE = 4800;


    private static final boolean USE_WINDOW = false;
    private static final int U = 1;
    private static final double SCALE = 0.9;
    private static final byte CHANNEL_MASK = 0x02;
    private static final double[][] TX_SEQ = new double[N_ZC_UP][2];

    private static FileOutputStream fileOutputStream;
    private static OutputStreamWriter outputStreamWriter;
    private static BufferedWriter bufferedWriter;
    private static long timestamp;
    private static boolean needSave;


    private static abstract class Speaker extends Thread {
        protected final float[] buffer = new float[AudioPlayer.BUFFER_SIZE];
        protected final AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                AudioPlayer.SAMPLE_RATE,
                AudioFormat.CHANNEL_OUT_STEREO,
                AudioFormat.ENCODING_PCM_FLOAT,
                AudioPlayer.BUFFER_SIZE,
                AudioTrack.MODE_STREAM
        );
        protected boolean running;
        protected final Lock lock = new ReentrantLock();

        public abstract void terminate();
    }


    private static class SeqSpeaker extends Speaker {
        public SeqSpeaker() { }

        @Override
        public void run() {
            audioTrack.play();

            running = true;

            while (running) {
                prepareBuffer();

                if (lock.tryLock()) {
                    audioTrack.write(buffer, 0, AudioPlayer.BUFFER_SIZE, AudioTrack.WRITE_BLOCKING);
                    FileUtil.streamWriteMusic(bufferedWriter, buffer);
                    lock.unlock();
                } else {
                    break;
                }
            }
        }

        @Override
        public void terminate() {
            running = false;

            lock.lock();

            audioTrack.stop();
            audioTrack.release();

            lock.unlock();
        }

        private void prepareBuffer() {
            for (int i = 0; i < AudioPlayer.BUFFER_SIZE / 2; i++) {
                buffer[2 * i] = (float) AudioPlayer.TX_SEQ[i % AudioPlayer.N_ZC_UP][0];
                buffer[2 * i + 1] = (float) AudioPlayer.TX_SEQ[i % AudioPlayer.N_ZC_UP][1];
            }
        }
    }



    private Speaker speaker;

    public AudioPlayer() { }

    public void setTimestamp(long timestamp) {
        AudioPlayer.timestamp = timestamp;
    }

    public void init() {
        needSave = false;

        if (!USE_FILE) {
            genZCSeq();
            speaker = new SeqSpeaker();
        }
    }

    public void start() {
        if (!USE_FILE) {
            fileOutputStream = FileUtil.getFileOutputStream("player/tx_" + timestamp + ".txt");
            outputStreamWriter = new OutputStreamWriter(fileOutputStream, StandardCharsets.UTF_8);
            bufferedWriter = new BufferedWriter(outputStreamWriter);
            speaker.start();
        }


    }

    public void stop() {


        try {
            if (!USE_FILE) {
                bufferedWriter.close();
                outputStreamWriter.close();
                fileOutputStream.close();
                speaker.terminate();
            }
        } catch (IOException e) {
            Log.e("AudioPlayer", e.getMessage());
        }
    }

    public void save() {
        needSave = true;
    }

    public void reset() {
        if (!needSave) {
            FileUtil.deleteFile("player/tx_" + timestamp + ".txt");
        } else {
            needSave = false;
        }

        if (!USE_FILE) {
            speaker = new SeqSpeaker();
        }
    }

    private void genZCSeq() {
        // Generate raw zc seq
        double[] rawZC = new double[2 * N_ZC];
        for (int i = 0; i < N_ZC; i++) {
            double theta = -Math.PI * U * i * (i + 1) / N_ZC;
            rawZC[2 * i] = Math.cos(theta);
            rawZC[2 * i + 1] = Math.sin(theta);
        }

        // Transfer raw zc to freq domain
        DoubleFFT_1D fft = new DoubleFFT_1D(N_ZC);
        fft.complexForward(rawZC);

        // Apply hann window based on the reference url: https://ww2.mathworks.cn/help/signal/ref/hann.html
        if (USE_WINDOW) {
            for (int i = 0; i < N_ZC; i++) {
                rawZC[i] *= 0.5 * (1 - Math.cos(2 * Math.PI * i / (N_ZC - 1)));
            }
        }

        // Padding zeros in freq domain
        double[] freqPadZC = new double[2 * N_ZC_UP];
        int len = N_ZC_UP - N_ZC;
        for (int i = 0; i < N_ZC; i++) {
            if (i < (N_ZC + 1) / 2) {
                freqPadZC[2 * i] = rawZC[2 * i];
                freqPadZC[2 * i + 1] = rawZC[2 * i + 1];
            } else {
                freqPadZC[2 * (i + len)] = rawZC[2 * i];
                freqPadZC[2 * (i + len) + 1] = rawZC[2 * i + 1];
            }
        }

        // Back to time domain
        DoubleFFT_1D ifft = new DoubleFFT_1D(N_ZC_UP);
        ifft.complexInverse(freqPadZC, true);

        // Up conversion
        double[] seqFrame = new double[N_ZC_UP];
        double maxValue = 0.0;
        for (int i = 0; i < N_ZC_UP; i++) {
            double theta = -2 * Math.PI * FC * (i + 1) / SAMPLE_RATE;
            double real = Math.cos(theta), imag = Math.sin(theta);
            seqFrame[i] = freqPadZC[2 * i] * real + freqPadZC[2 * i + 1] * imag;
            maxValue = Math.max(maxValue, Math.abs(seqFrame[i]));
        }

        // Scaling
        for (int i = 0; i < N_ZC_UP; i++) {
            for (int j = 0; j < 2; j++) {
                if (((j + 1) & CHANNEL_MASK) != 0) {
                    TX_SEQ[i][j] = seqFrame[i % N_ZC_UP] / maxValue * SCALE;
                }
            }
        }
    }
}
