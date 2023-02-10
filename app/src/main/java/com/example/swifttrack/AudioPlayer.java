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
    // tunable parameters








    // non-tunable parameters
    public static int N_ZC_UP;
    public static int N_ZC;
    public static int BW;
    public static int BUFFER_SIZE;
    public static boolean USE_FILE;
    public static boolean[] SPEAKER_CHANNEL_MASK;


    public static final int FC = MainActivity.FC;
    public static final int SAMPLE_RATE = MainActivity.FS;
    private static final boolean USE_WINDOW = false;
    private static final int U = MainActivity.ZC_ROOT;
    private static final double SCALE = 0.9;

    private static double[][] TX_SEQ;


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
//            audioTrack.play();

            running = true;

            while (running) {
                prepareBuffer();

                if (lock.tryLock()) {
//                    audioTrack.write(buffer, 0, AudioPlayer.BUFFER_SIZE, AudioTrack.WRITE_BLOCKING);
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

//            audioTrack.stop();
//            audioTrack.release();

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

    public AudioPlayer() {
        USE_FILE = MainActivity.USE_FILE;
        N_ZC_UP = MainActivity.N_ZC_UP;
        N_ZC = MainActivity.N_ZC;
        BW = MainActivity.BW;

        BUFFER_SIZE = N_ZC_UP * 10;
        SPEAKER_CHANNEL_MASK = MainActivity.SPEAKER_CHANNEL_MAKS;
        TX_SEQ = new double[N_ZC_UP][2];
    }

    public void setTimestamp(long timestamp) {
        AudioPlayer.timestamp = timestamp;
    }

    public void init() {
        needSave = false;
        Log.d("init_value", "player N_ZC_UP " + AudioPlayer.N_ZC_UP);
        Log.d("init_value", "player BUFFER_SIZE " + AudioPlayer.BUFFER_SIZE);
        Log.d("init_value", "player TX_SEQ size " + TX_SEQ.length);
        if (!USE_FILE) {
            AudioProcessor.genZCSeq(N_ZC,U,USE_WINDOW,N_ZC_UP,FC,SAMPLE_RATE,SPEAKER_CHANNEL_MASK,SCALE,TX_SEQ);
            speaker = new SeqSpeaker();
        }
        Log.d("Player", " " + SPEAKER_CHANNEL_MASK[0] + " " + SPEAKER_CHANNEL_MASK[1]);
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

}
