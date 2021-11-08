package com.example.swifttrack;


import android.graphics.Color;
import android.util.Log;
import android.widget.Toast;

import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.example.swifttrack.utils.FileUtil;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.example.swifttrack.ui.home.HomeViewModel;

import java.util.Timer;

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


    public static class deployMethods{
        public static final int tof = 0;
        public static final int strata = 1;
        public static final int swifttrack = 2;
    }

    public static class HistoryType{
        public static final int phase_v = 0;
        public static final int velocity_ = 1;
        public static final int dist_v = 2;
        public static final int acceleration_ = 3;
        public static final int velocity_a = 4;
        public static final int dist_a = 5;
        public static final int phase_acc = 6;
    };

    public static class inputChannel{
        public static final int LEFT = 0;
        public static final int RIGHT = 1;
    }



    private static class Engine extends Thread {
        private static final int INTERVAL = 50;
        private static final int WINDOW_SIZE = 500;

        private final Lock lock = new ReentrantLock();
        private final double[] frame1 = new double[FRAME_SIZE];
        private final double[] frame2 = new double[FRAME_SIZE];

        private int frameCount;
        private boolean running;

        private final int fragID;

        private boolean warmUpFlag = false;

        Timer timer = new Timer();

        public Engine() { fragID = 0;}
        public Engine(int inFragID) {fragID = inFragID;}

        @Override
        public void run() {
            frameCount = 0;

            running = true;

            Log.d("Timer", "Waiting for speaker warm up.");


            timer.schedule(new TimerTask() {
                @Override
                public void run() {
                    warmUpFlag = true;

                    Log.d("Timer", "Warm up finished.");
                }
            }, 2*1000);


            while (running) {
                try {
                    if (lock.tryLock()) {
                        float[] data = MainActivity.rxQueue.poll(INTERVAL, TimeUnit.MILLISECONDS);

                        if (data != null & warmUpFlag) {
//                            Log.d("Timer", "Start processing audios.");
//                            Log.d("datasize", "data length " + data.length);
                            for (int i = 0; i < data.length / 2 / FRAME_SIZE; i++) {
                                for (int j = 0; j < FRAME_SIZE; j++) {
                                    frame1[j] = data[2 * (i * FRAME_SIZE + j)];
                                    frame2[j] = data[2 * (i * FRAME_SIZE + j) + 1];
                                }
                                if (CHANNEL_MASK[inputChannel.LEFT]) {
                                    processFrame(inputChannel.LEFT, frame1, FRAME_SIZE);
                                }
                                if (CHANNEL_MASK[inputChannel.RIGHT]) {
                                    processFrame(inputChannel.RIGHT, frame2, FRAME_SIZE);
                                }
                            }

                            frameCount += data.length / 2 / FRAME_SIZE;

                            double[] xWindow1 = new double[WINDOW_SIZE];
                            double[] xWindow2 = new double[WINDOW_SIZE];
                            double[] vWindow1 = new double[WINDOW_SIZE];
                            double[] vWindow2 = new double[WINDOW_SIZE];

                            switch (fragID){
                                case 0:
                                    if (CHANNEL_MASK[inputChannel.LEFT]) {
                                        getDistHistory(inputChannel.LEFT, xWindow1, WINDOW_SIZE);
                                        getVelocityHistory(inputChannel.LEFT, vWindow1, WINDOW_SIZE);
                                        HomeViewModel.setLineData(xWindow1, HomeViewModel.OutTypes.LEFT_DIST);
                                        HomeViewModel.setLineData(vWindow1, HomeViewModel.OutTypes.LEFT_V);
                                    }
                                    if (CHANNEL_MASK[inputChannel.RIGHT]) {
                                        getDistHistory(inputChannel.RIGHT, xWindow2, WINDOW_SIZE);
                                        getVelocityHistory(inputChannel.RIGHT, vWindow2, WINDOW_SIZE);
                                        HomeViewModel.setLineData(xWindow2, HomeViewModel.OutTypes.RIGHT_DIST);
                                        HomeViewModel.setLineData(vWindow2, HomeViewModel.OutTypes.RIGHT_V);
                                    }
                                    break;
                                case 1:
                                    if (CHANNEL_MASK[inputChannel.LEFT]) {
                                        getHistoryData(inputChannel.LEFT, xWindow1, WINDOW_SIZE, deployMethods.swifttrack, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.SWIFT_TRACK);

                                        getHistoryData(inputChannel.LEFT, xWindow1, WINDOW_SIZE, deployMethods.tof, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.TOF);

                                        getHistoryData(inputChannel.LEFT, xWindow1, WINDOW_SIZE, deployMethods.strata, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.STRATA);
                                    }
                                    if (CHANNEL_MASK[inputChannel.RIGHT]) {
                                        getHistoryData(inputChannel.RIGHT, xWindow2, WINDOW_SIZE, deployMethods.swifttrack, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.SWIFT_TRACK);

                                        getHistoryData(inputChannel.RIGHT, xWindow2, WINDOW_SIZE, deployMethods.tof, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.TOF);

                                        getHistoryData(inputChannel.RIGHT, xWindow2, WINDOW_SIZE, deployMethods.strata, HistoryType.dist_v);
                                        GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.STRATA);

                                        double[] cir_abs = new double[FRAME_SIZE];
                                        getCIR(inputChannel.RIGHT, cir_abs, FRAME_SIZE);
                                        GalleryViewModel.setLineData(cir_abs, GalleryViewModel.OutTypes.CIR);
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
                    if (CHANNEL_MASK[inputChannel.LEFT]) {
                        getDistHistory(inputChannel.LEFT, xHistory1, frameCount);
                        getVelocityHistory(inputChannel.LEFT, vHistory1, frameCount);
                        HomeViewModel.setLineData(xHistory1, HomeViewModel.OutTypes.LEFT_DIST);
                        HomeViewModel.setLineData(vHistory1, HomeViewModel.OutTypes.LEFT_V);
                    }

                    if (CHANNEL_MASK[inputChannel.RIGHT]) {
                        getDistHistory(inputChannel.RIGHT, xHistory2, frameCount);
                        getVelocityHistory(inputChannel.RIGHT, vHistory2, frameCount);
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

                    if (CHANNEL_MASK[inputChannel.LEFT]) {
                        getHistoryData(inputChannel.LEFT, xHistory1, frameCount, deployMethods.swifttrack, HistoryType.dist_v);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.SWIFT_TRACK);

                        getHistoryData(inputChannel.LEFT, xHistory1, frameCount, deployMethods.tof, HistoryType.dist_v);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.TOF);

                        getHistoryData(inputChannel.LEFT, xHistory1, frameCount, deployMethods.strata, HistoryType.dist_v);
                        GalleryViewModel.setLineData(xHistory1, GalleryViewModel.OutTypes.STRATA);
                    }

                    if (CHANNEL_MASK[inputChannel.RIGHT]) {
                        getHistoryData(inputChannel.RIGHT, xHistory2, frameCount, deployMethods.swifttrack, HistoryType.dist_v);
                        GalleryViewModel.setLineData(xHistory2, GalleryViewModel.OutTypes.SWIFT_TRACK);

                        getHistoryData(inputChannel.RIGHT, xHistory2, frameCount, deployMethods.tof, HistoryType.dist_v);
                        GalleryViewModel.setLineData(xHistory2, GalleryViewModel.OutTypes.TOF);

                        getHistoryData(inputChannel.RIGHT, xHistory2, frameCount, deployMethods.strata, HistoryType.dist_v);
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

        if (CHANNEL_MASK[inputChannel.LEFT]) {
            reset(inputChannel.LEFT);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT);
        }

        engine = new Engine();
    }


    // 0 -> home view
    // 1 -> gallery view
    // 2 -> slideshow view
    public void init(int inFragID) {
        needSave = false;

        if (CHANNEL_MASK[inputChannel.LEFT]) {
            reset(inputChannel.LEFT);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT);
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

        if (CHANNEL_MASK[inputChannel.LEFT]) {
            reset(inputChannel.LEFT);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT);
        }
        MainActivity.rxQueue.clear();
        engine = new Engine(fragID);
    }


    private static native void processFrame(int id, double[] data, int n);

    private static native void getVelocityHistory(int id, double[] history, int n);

    private static native void getVelocityHistory2(int id, double[] history, int n, int history_id);

    private static native void getDistHistory(int id, double[] history, int n);

    private static native void getDistHistory2(int id, double[] history, int n, int history_id);

    private static native void getCIR(int id, double[] cir_abs, int n);

    private static native void reset(int id);

    private static native void getHistoryData(int id, double[] history, int n, int history_id, int history_type);
}
