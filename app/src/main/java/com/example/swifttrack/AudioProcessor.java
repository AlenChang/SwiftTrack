package com.example.swifttrack;


import android.graphics.Color;
import android.util.Log;
import android.widget.Toast;

import com.example.swifttrack.ui.acc.AccViewModel;
import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.example.swifttrack.ui.ml.MLViewModel;
import com.example.swifttrack.ui.ml.Model;
import com.example.swifttrack.ui.slideshow.SlideshowFragment;
import com.example.swifttrack.ui.slideshow.SlideshowViewModel;
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

    private static int FRAME_SIZE;
    private static boolean[] CHANNEL_MASK = MainActivity.CHANNEL_MASK;

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

    public static class ActivityID{
        public static final int homeFragment = 0;
        public static final int galleryFragment = 1;
        public static final int accFragment = 2;
        public static final int slideFragment = 3;
        public static final int mlFragment = 4;
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

        private int counter = 0;

        Timer timer = new Timer();

        public Engine() { fragID = ActivityID.homeFragment;}
        public Engine(int inFragID) {fragID = inFragID;}

        private void prepareDataForHomeFragment(double[] xLeft, double[] vLeft, double[] xRight, double[] vRight, int winLen){
            if (CHANNEL_MASK[inputChannel.LEFT]) {
                getHistoryData(inputChannel.LEFT, xLeft, winLen, deployMethods.swifttrack, HistoryType.dist_v);
//                getDistHistory(inputChannel.LEFT, xLeft, winLen);
                getHistoryData(inputChannel.LEFT, vLeft, winLen, deployMethods.swifttrack, HistoryType.velocity_);
//                getVelocityHistory(inputChannel.LEFT, vLeft, winLen);
                HomeViewModel.setLineData(xLeft, HomeViewModel.OutTypes.LEFT_DIST);
                HomeViewModel.setLineData(vLeft, HomeViewModel.OutTypes.LEFT_V);
            }
            if (CHANNEL_MASK[inputChannel.RIGHT]) {
                getHistoryData(inputChannel.RIGHT, xRight, winLen, deployMethods.swifttrack, HistoryType.dist_v);
//                getDistHistory(inputChannel.RIGHT, xRight, winLen);
                getHistoryData(inputChannel.RIGHT, vRight, winLen, deployMethods.swifttrack, HistoryType.velocity_);
//                getVelocityHistory(inputChannel.RIGHT, vRight, winLen);
                HomeViewModel.setLineData(xRight, HomeViewModel.OutTypes.RIGHT_DIST);
                HomeViewModel.setLineData(vRight, HomeViewModel.OutTypes.RIGHT_V);
            }
        }

        private void prepareDataForGalleryFragment(int winLen){
            double[] xWindow1 = new double[winLen];
            double[] xWindow2 = new double[winLen];
            if (CHANNEL_MASK[inputChannel.RIGHT]) {

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.SWIFT_TRACK);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.tof, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.TOF);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.strata, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow2, GalleryViewModel.OutTypes.STRATA);

                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.RIGHT, cir_abs, FRAME_SIZE);
                GalleryViewModel.setLineData(cir_abs, GalleryViewModel.OutTypes.CIR);

            } else {
                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.SWIFT_TRACK);

                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.tof, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.TOF);

                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.strata, HistoryType.dist_v);
                GalleryViewModel.setLineData(xWindow1, GalleryViewModel.OutTypes.STRATA);

                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.LEFT, cir_abs, FRAME_SIZE);
                GalleryViewModel.setLineData(cir_abs, GalleryViewModel.OutTypes.CIR);

            }
        }

        private void prepareDataForAccFragment(int winLen){
            double[] xWindow1 = new double[winLen];
            double[] xWindow2 = new double[winLen];
            if (CHANNEL_MASK[inputChannel.RIGHT]) {

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.velocity_);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.velocity);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.acceleration_);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.velocity2dist);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.velocity_a);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration2velocity);

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.dist_a);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration2dist);

                double[] time_count = new double[2];
                getTime(inputChannel.RIGHT, time_count);
                Log.d("C_TIME_COUNT", ""+time_count[0]);

            } else {
                getHistoryData(inputChannel.LEFT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.velocity_);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.velocity);

                getHistoryData(inputChannel.LEFT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.acceleration_);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration);

                getHistoryData(inputChannel.LEFT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.velocity2dist);

                getHistoryData(inputChannel.LEFT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.velocity_a);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration2velocity);

                getHistoryData(inputChannel.LEFT, xWindow2, winLen, deployMethods.swifttrack, HistoryType.dist_a);
                AccViewModel.setLineData(xWindow2, AccViewModel.OutTypes.acceleration2dist);

                double[] time_count = new double[2];
                getTime(inputChannel.LEFT, time_count);
                Log.d("C_TIME_COUNT", ""+time_count[0]);

            }
        }

        private void prepareDataForSlideFragment(int winLen){
            if(CHANNEL_MASK[inputChannel.RIGHT]){
                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.RIGHT, cir_abs, FRAME_SIZE);
                SlideshowViewModel.draw(cir_abs);

                double[] xWindow1 = new double[winLen];
                getHistoryData(inputChannel.RIGHT, xWindow1, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                SlideshowViewModel.setLineData(xWindow1);
            }else{
                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.LEFT, cir_abs, FRAME_SIZE);
                SlideshowViewModel.draw(cir_abs);

                double[] xWindow1 = new double[winLen];
                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.swifttrack, HistoryType.dist_v);
                SlideshowViewModel.setLineData(xWindow1);
            }


        }

        private void prepareDataForMLFragment(int winLen){
            double[] xWindow1 = new double[winLen];
            double[] xWindow2 = new double[winLen];
            if (CHANNEL_MASK[inputChannel.RIGHT]) {

                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.tof, HistoryType.dist_v);
                MLViewModel.setLineData(xWindow2, MLViewModel.OutTypes.TOF);

                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.RIGHT, cir_abs, FRAME_SIZE);
                MLViewModel.setLineData(cir_abs, MLViewModel.OutTypes.CIR);

                double[] ml_result = Model.getHistoryData(cir_abs, winLen);
                MLViewModel.setLineData(ml_result, MLViewModel.OutTypes.ML);
            } else {
                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.tof, HistoryType.dist_v);
                MLViewModel.setLineData(xWindow1, MLViewModel.OutTypes.TOF);

                double[] cir_abs = new double[FRAME_SIZE];
                getCIR(inputChannel.LEFT, cir_abs, FRAME_SIZE);
                MLViewModel.setLineData(cir_abs, MLViewModel.OutTypes.CIR);

                double[] ml_result = Model.getHistoryData(cir_abs, winLen);
                MLViewModel.setLineData(ml_result, MLViewModel.OutTypes.ML);
            }
        }

        private void prepareFinalDataForMLFragment(int winLen){
            double[] xWindow1 = new double[winLen];
            double[] xWindow2 = new double[winLen];
            if (CHANNEL_MASK[inputChannel.RIGHT]) {
                getHistoryData(inputChannel.RIGHT, xWindow2, winLen, deployMethods.tof, HistoryType.dist_v);
                MLViewModel.saveTOF(xWindow2);
            } else {
                getHistoryData(inputChannel.LEFT, xWindow1, winLen, deployMethods.tof, HistoryType.dist_v);
                MLViewModel.saveTOF(xWindow1);
            }
        }

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
            }, 5*1000);


            while (running) {
                try {
                    if (lock.tryLock()) {
                        float[] data = AudioRecorder.rxQueue.poll(INTERVAL, TimeUnit.MILLISECONDS);


                        if (data != null & warmUpFlag) {
//                            Log.d("Timer", "Start processing audios.");
//                            Log.d("datasize", "data length " + data.length);
//                            Log.d("test", "data shape: " + data.length);
                            for (int i = 0; i < data.length / 2 / FRAME_SIZE; i++) {
//                                Log.d("frame_number", " " + i + " out of " + data.length / 2 / FRAME_SIZE );
                                for (int j = 0; j < FRAME_SIZE; j++) {
                                    frame1[j] = data[2 * (i * FRAME_SIZE + j)];
                                    frame2[j] = data[2 * (i * FRAME_SIZE + j) + 1];
//                                    Log.d("frame_data", frame1[j] + " " + frame2[j]);
                                }
//                                long startTime = System.currentTimeMillis();
                                long startTime = System.nanoTime();
                                if (CHANNEL_MASK[inputChannel.LEFT]) {
                                    processFrame(inputChannel.LEFT, frame1, FRAME_SIZE, AudioPlayer.N_ZC_UP);
                                }
                                if (CHANNEL_MASK[inputChannel.RIGHT]) {
                                    processFrame(inputChannel.RIGHT, frame2, FRAME_SIZE, AudioPlayer.N_ZC_UP);
                                }
                                long endTime = System.nanoTime();
                                if( counter % 10 == 0){
                                    Log.d("TimeCount", " " + (endTime - startTime)/1000000.0 + " ms");
                                }
                                counter++;
                            }

                            frameCount += data.length / 2 / FRAME_SIZE;

                            switch (fragID){
                                case ActivityID.homeFragment:
                                    // TODO: memory leak
                                    double[] xWindow1 = new double[WINDOW_SIZE];
                                    double[] xWindow2 = new double[WINDOW_SIZE];
                                    double[] vWindow1 = new double[WINDOW_SIZE];
                                    double[] vWindow2 = new double[WINDOW_SIZE];
                                    prepareDataForHomeFragment(xWindow1, vWindow1, xWindow2, vWindow2, WINDOW_SIZE);
                                    break;
                                case ActivityID.galleryFragment:
                                    prepareDataForGalleryFragment(WINDOW_SIZE);

                                    break;
                                case ActivityID.accFragment:
                                    prepareDataForAccFragment(WINDOW_SIZE);
                                    break;
                                case ActivityID.slideFragment:
                                    prepareDataForSlideFragment(WINDOW_SIZE);
                                    break;
                                case ActivityID.mlFragment:
                                    prepareDataForMLFragment(WINDOW_SIZE);
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
            Log.d("terminate frameCount", String.valueOf(frameCount));

            running = false;

            lock.lock();

            double[] xHistory1 = new double[frameCount];
            double[] vHistory1 = new double[frameCount];
            double[] xHistory2 = new double[frameCount];
            double[] vHistory2 = new double[frameCount];

            double[][] result = new double[frameCount][4];

            switch (fragID){
                case ActivityID.homeFragment:
                    prepareDataForHomeFragment(xHistory1, vHistory1, xHistory2, vHistory2, frameCount);
                    for (int i = 0; i < frameCount; i++) {
                        result[i][0] = xHistory1[i];
                        result[i][1] = xHistory2[i];
                        result[i][2] = vHistory1[i];
                        result[i][3] = vHistory2[i];
                    }
                    FileUtil.streamWriteResult(bufferedWriter, result);
                    break;
                case ActivityID.galleryFragment:
                    prepareDataForGalleryFragment(frameCount);

                    break;
                case ActivityID.accFragment:
                    prepareDataForAccFragment(frameCount);
                    break;
                case ActivityID.slideFragment:
                    prepareDataForSlideFragment(frameCount);
                    break;
                case ActivityID.mlFragment:
                    prepareFinalDataForMLFragment(frameCount);
                    break;
                default:
                    break;
            }



            lock.unlock();
        }
    }




    private Engine engine;


    public AudioProcessor() {}
    public AudioProcessor(boolean[] mask) {
        CHANNEL_MASK = mask;
        FRAME_SIZE = MainActivity.N_ZC_UP;
    }

    public void setTimestamp(long timestamp) {
        AudioProcessor.timestamp = timestamp;
    }

    public void init() {
        needSave = false;
        fragID = 0;

        if (CHANNEL_MASK[inputChannel.LEFT]) {
            reset(inputChannel.LEFT, AudioPlayer.N_ZC_UP);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT, AudioPlayer.N_ZC_UP);
        }

        engine = new Engine();
    }


    // 0 -> home view
    // 1 -> gallery view
    // 2 -> slideshow view
    public void init(int inFragID) {
        needSave = false;

        if (CHANNEL_MASK[inputChannel.LEFT]) {
            reset(inputChannel.LEFT, AudioPlayer.N_ZC_UP);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT, AudioPlayer.N_ZC_UP);
        }
        fragID = inFragID;
        engine = new Engine(fragID);
    }

    public void start() {
        Log.d("test", "ready to start!");
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
            reset(inputChannel.LEFT, AudioPlayer.N_ZC_UP);
        }
        if (CHANNEL_MASK[inputChannel.RIGHT]) {
            reset(inputChannel.RIGHT, AudioPlayer.N_ZC_UP);
        }
        AudioRecorder.rxQueue.clear();
        engine = new Engine(fragID);
    }


    private static native void processFrame(int id, double[] data, int n, int N_ZC_UP);

    private static native void getCIR(int id, double[] cir_abs, int n);

    private static native void reset(int id, int N_ZC_UP);

    private static native void getHistoryData(int id, double[] history, int n, int history_id, int history_type);

    private static native void getTime(int id, double[] time_count);
}
