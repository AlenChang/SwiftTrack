package com.example.swifttrack;


import android.graphics.Color;
import android.util.Log;

import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.example.swifttrack.utils.FileUtil;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.example.swifttrack.ui.home.HomeViewModel;

public class AudioProcessor {

    static {
        System.loadLibrary("acoustic");
    }

    private static final int FRAME_SIZE = AudioPlayer.N_ZC_UP;
    private static final boolean[] CHANNEL_MASK = { false, true };

    private static FileOutputStream fileOutputStream;
    private static OutputStreamWriter outputStreamWriter;
    private static BufferedWriter bufferedWriter;
    private static long timestamp;
    private static boolean needSave;
    private int fragID;





    private static class Engine extends Thread {
        private static final int INTERVAL = 50;
        private static final int WINDOW_SIZE = 500;

        private final Lock lock = new ReentrantLock();
        private final double[] frame1 = new double[FRAME_SIZE];
        private final double[] frame2 = new double[FRAME_SIZE];

        private int frameCount;
        private boolean running;

        private final int fragID;

        public Engine() { fragID = 0;}
        public Engine(int inFragID) {fragID = inFragID;}

        @Override
        public void run() {
            frameCount = 0;

            running = true;

            while (running) {
                try {
                    if (lock.tryLock()) {
                        float[] data = MainActivity.rxQueue.poll(INTERVAL, TimeUnit.MILLISECONDS);

                        if (data != null) {
                            Log.d("datasize", "data length " + data.length);
                            for (int i = 0; i < data.length / 2 / FRAME_SIZE; i++) {
                                for (int j = 0; j < FRAME_SIZE; j++) {
                                    frame1[j] = data[2 * (i * FRAME_SIZE + j)];
                                    frame2[j] = data[2 * (i * FRAME_SIZE + j) + 1];
                                }
                                if (CHANNEL_MASK[0]) {
                                    processFrame(1, frame1, FRAME_SIZE);
                                }
                                if (CHANNEL_MASK[1]) {
                                    processFrame(2, frame2, FRAME_SIZE);
                                }
                            }

                            frameCount += data.length / 2 / FRAME_SIZE;

                            double[] xWindow1 = new double[WINDOW_SIZE];
                            double[] xWindow2 = new double[WINDOW_SIZE];
                            double[] vWindow1 = new double[WINDOW_SIZE];
                            double[] vWindow2 = new double[WINDOW_SIZE];

                            switch (fragID){
                                case 0:
                                    if (CHANNEL_MASK[0]) {
                                        getDistHistory(1, xWindow1, WINDOW_SIZE);
                                        getVelocityHistory(1, vWindow1, WINDOW_SIZE);
                                        HomeViewModel.setLineData(xWindow1, HomeViewModel.OutTypes.LEFT_DIST);
                                        HomeViewModel.setLineData(vWindow1, HomeViewModel.OutTypes.LEFT_V);
                                    }
                                    if (CHANNEL_MASK[1]) {
                                        getDistHistory(2, xWindow2, WINDOW_SIZE);
                                        getVelocityHistory(2, vWindow2, WINDOW_SIZE);
                                        HomeViewModel.setLineData(xWindow2, HomeViewModel.OutTypes.RIGHT_DIST);
                                        HomeViewModel.setLineData(vWindow2, HomeViewModel.OutTypes.RIGHT_V);
                                    }
                                    break;
                                case 1:
                                    if (CHANNEL_MASK[0]) {
                                        getDistHistory2(1, xWindow1, WINDOW_SIZE, 2);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.SWIFT_TRACK);
                                        getDistHistory2(1, xWindow1, WINDOW_SIZE, 0);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.TOF);
                                        getDistHistory2(1, xWindow1, WINDOW_SIZE, 1);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.STRATA);
                                    }
                                    if (CHANNEL_MASK[1]) {
                                        getDistHistory2(2, xWindow2, WINDOW_SIZE, 2);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.SWIFT_TRACK);
                                        getDistHistory2(2, xWindow2, WINDOW_SIZE, 0);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.TOF);
                                        getDistHistory2(2, xWindow2, WINDOW_SIZE, 1);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.STRATA);
                                    }
                                    break;
                                default:
                                    break;
                            }


                        }

                        lock.unlock();
                    } else {
                        break;
                    }
                } catch (InterruptedException e) {
                    Log.e("Engine", e.getMessage());
                }
            }
        }

        public void terminate() {
            running = false;

            lock.lock();

            double[] xHistory1 = new double[frameCount];
            double[] vHistory1 = new double[frameCount];
            double[] xHistory2 = new double[frameCount];
            double[] vHistory2 = new double[frameCount];

            double[][] result = new double[frameCount][4];

            switch (fragID){
                case 0:
                    if (CHANNEL_MASK[0]) {
                        getDistHistory(1, xHistory1, frameCount);
                        getVelocityHistory(1, vHistory1, frameCount);
                        HomeViewModel.setLineData(xHistory1, HomeViewModel.OutTypes.LEFT_DIST);
                        HomeViewModel.setLineData(vHistory1, HomeViewModel.OutTypes.LEFT_V);
                    }

                    if (CHANNEL_MASK[1]) {
                        getDistHistory(2, xHistory2, frameCount);
                        getVelocityHistory(2, vHistory2, frameCount);
                        HomeViewModel.setLineData(xHistory2, HomeViewModel.OutTypes.RIGHT_DIST);
                        HomeViewModel.setLineData(vHistory2, HomeViewModel.OutTypes.RIGHT_V);
                    }


                    for (int i = 0; i < frameCount; i++) {
                        result[i][0] = xHistory1[i];
                        result[i][1] = xHistory2[i];
                        result[i][2] = vHistory1[i];
                        result[i][3] = vHistory2[i];
                    }
                    FileUtil.streamWriteResult(bufferedWriter, result);
                    break;
                case 1:

                    if (CHANNEL_MASK[0]) {
                        getDistHistory2(1, xHistory1, frameCount, 2);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.SWIFT_TRACK);
                        getDistHistory2(1, xHistory1, frameCount, 0);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.TOF);
                        getDistHistory2(1, xHistory1, frameCount, 1);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.STRATA);
                    }

                    if (CHANNEL_MASK[1]) {
                        getDistHistory2(2, xHistory2, frameCount, 2);
                        GalleryViewModel.setLineData(xHistory2, GalleryViewModel.OutTypes.SWIFT_TRACK);
                        getDistHistory2(2, xHistory2, frameCount, 0);
                        GalleryViewModel.setLineData(xHistory2, GalleryViewModel.OutTypes.TOF);
                        getDistHistory2(2, xHistory2, frameCount, 1);
                        GalleryViewModel.setLineData(xHistory2, GalleryViewModel.OutTypes.STRATA);

                    }

                    for (int i = 0; i < frameCount; i++) {
                        result[i][0] = xHistory1[i];
                        result[i][1] = xHistory2[i];
                        result[i][2] = vHistory1[i];
                        result[i][3] = vHistory2[i];
                    }
                    FileUtil.streamWriteResult(bufferedWriter, result);
                    break;
                default:
                    break;
            }



            lock.unlock();
        }
    }




    private Engine engine;


    public AudioProcessor() {}

    public void setTimestamp(long timestamp) {
        AudioProcessor.timestamp = timestamp;
    }

    public void init() {
        needSave = false;
        fragID = 0;

        if (CHANNEL_MASK[0]) {
            reset(1);
        }
        if (CHANNEL_MASK[1]) {
            reset(2);
        }

        engine = new Engine();
    }


    // 0 -> home view
    // 1 -> gallery view
    // 2 -> slideshow view
    public void init(int inFragID) {
        needSave = false;

        if (CHANNEL_MASK[0]) {
            reset(1);
        }
        if (CHANNEL_MASK[1]) {
            reset(2);
        }
        fragID = inFragID;
        engine = new Engine(fragID);
    }

    public void start() {
        fileOutputStream = FileUtil.getFileOutputStream("processor/est_" + timestamp + ".txt");
        outputStreamWriter = new OutputStreamWriter(fileOutputStream, StandardCharsets.UTF_8);
        bufferedWriter = new BufferedWriter(outputStreamWriter);

        engine.start();
    }

    public void stop() {
        engine.terminate();

        try {
            bufferedWriter.close();
            outputStreamWriter.close();
            fileOutputStream.close();
        } catch (IOException e) {
            Log.e("AudioProcessor", e.getMessage());
        }
    }

    public void save() {
        needSave = true;
    }

    public void reset() {
        if (!needSave) {
            FileUtil.deleteFile("processor/est_" + timestamp + ".txt");
        } else {
            needSave = false;
        }

        if (CHANNEL_MASK[0]) {
            reset(1);
        }
        if (CHANNEL_MASK[1]) {
            reset(2);
        }
        MainActivity.rxQueue.clear();
        engine = new Engine(fragID);
    }


    private static native void processFrame(int id, double[] data, int n);

    private static native void getVelocityHistory(int id, double[] history, int n);

    private static native void getDistHistory(int id, double[] history, int n);

    private static native void getDistHistory2(int id, double[] history, int n, int history_id);

    private static native void reset(int id);
}
