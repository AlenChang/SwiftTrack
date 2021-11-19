package com.example.swifttrack;

import android.annotation.SuppressLint;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import com.example.swifttrack.utils.FileUtil;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class AudioRecorder {

    private static boolean USE_FILE;
    private static int BUFFER_SIZE;

    private static final int SAMPLE_RATE = AudioPlayer.SAMPLE_RATE;
    private static final int CHANNEL_TYPE = AudioFormat.CHANNEL_IN_STEREO;
    private static final int ENCODING_TYPE = AudioFormat.ENCODING_PCM_FLOAT;


    private static FileInputStream fileInputStream;
    private static FileOutputStream fileOutputStream;
    private static InputStreamReader inputStreamReader;
    private static OutputStreamWriter outputStreamWriter;
    private static BufferedReader bufferedReader;
    private static BufferedWriter bufferedWriter;
    private static long timestamp;
    private static boolean needSave;


    public static String playFile = "atest1.txt";


    private static abstract class Phone extends Thread {
        protected final float[] buffer = new float[AudioRecorder.BUFFER_SIZE];
        protected boolean running;
        protected final Lock lock = new ReentrantLock();

        public abstract void terminate();
    }

    private static class Microphone extends Phone {
        @SuppressLint("MissingPermission")
        private final AudioRecord audioRecord = new AudioRecord(
                MediaRecorder.AudioSource.UNPROCESSED,
                AudioRecorder.SAMPLE_RATE,
                AudioRecorder.CHANNEL_TYPE,
                AudioRecorder.ENCODING_TYPE,
                AudioRecorder.BUFFER_SIZE
        );

        public Microphone() { }

        @Override
        public void run() {
            audioRecord.startRecording();

            running = true;

            while (running) {
                if (lock.tryLock()) {
                    audioRecord.read(buffer, 0, AudioRecorder.BUFFER_SIZE, AudioRecord.READ_BLOCKING);
                    processBuffer();
//                    Log.d("RecorderThread", String.valueOf(Thread.currentThread()));
                    lock.unlock();
                } else {
                    break;
                }
            }
        }

        public void terminate() {
            running = false;

            lock.lock();

            audioRecord.stop();
            audioRecord.release();

            lock.unlock();
        }

        private void processBuffer() {
            MainActivity.rxQueue.offer(buffer);

            FileUtil.streamWriteMusic(bufferedWriter, buffer);
        }
    }

    private static class FakerPhone extends Phone {
        public FakerPhone() { }

        @Override
        public void run() {
            running = true;

            while (running) {
                try {
                    if (lock.tryLock()) {
                        TimeUnit.MILLISECONDS.sleep(1000 * AudioRecorder.BUFFER_SIZE / 2 / AudioRecorder.SAMPLE_RATE);
                        int lineCount = FileUtil.streamReadMusic(bufferedReader, buffer);

                        if (lineCount * 2 < AudioRecorder.BUFFER_SIZE) {
                            Arrays.fill(buffer, lineCount * 2, AudioRecorder.BUFFER_SIZE, 0.0f);
                            running = false;
                        }

                        MainActivity.rxQueue.offer(buffer);

                        lock.unlock();
                    } else {
                        break;
                    }
                } catch (InterruptedException e) {
                    Log.e("FakerPhone", e.getMessage());
                }
            }
        }

        public void terminate() {
            running = false;

            lock.lock();

            try {
                fileInputStream.close();
            } catch (IOException e) {
                Log.e("FakerPhone", e.getMessage());
            }

            lock.unlock();
        }
    }



    private Phone phone;
    public AudioRecorder() {
        USE_FILE = MainActivity.USE_FILE;
        BUFFER_SIZE = MainActivity.N_ZC_UP * 10;
    }

    public void setTimestamp(long timestamp) {
        AudioRecorder.timestamp = timestamp;
    }

    public void init() {
        needSave = false;

        if (USE_FILE) {
            phone = new FakerPhone();
        } else {
            phone = new Microphone();
        }
    }

    public void start() {
        if (USE_FILE) {
            fileInputStream = FileUtil.getFileInputStream("recorder/"+playFile);
            inputStreamReader = new InputStreamReader(fileInputStream, StandardCharsets.UTF_8);
            bufferedReader = new BufferedReader(inputStreamReader);
        } else {
            fileOutputStream = FileUtil.getFileOutputStream("recorder/rx_" + timestamp + ".txt");
            outputStreamWriter = new OutputStreamWriter(fileOutputStream, StandardCharsets.UTF_8);
            bufferedWriter = new BufferedWriter(outputStreamWriter);
        }

        phone.start();
    }

    public static void setFileName(String fileName) {
        playFile = fileName;
    };

    public void stop() {
        phone.terminate();

        try {
            if (USE_FILE) {
                bufferedReader.close();
                inputStreamReader.close();
                fileInputStream.close();
            } else {
                bufferedWriter.close();
                outputStreamWriter.close();
                fileOutputStream.close();
            }
        } catch (IOException e) {
            Log.e("AudioRecorder", e.getMessage());
        }
    }

    public void save() {
        needSave = true;
    }

    public void reset() {
        if (!needSave) {
            FileUtil.deleteFile("recorder/rx_" + timestamp + ".txt");
        } else {
            needSave = false;
        }

        if (USE_FILE) {
            phone = new FakerPhone();
        } else {
            phone = new Microphone();
        }
    }
}
