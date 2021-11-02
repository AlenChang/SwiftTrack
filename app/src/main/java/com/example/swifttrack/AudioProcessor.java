package com.example.swifttrack;

import static com.example.swifttrack.ui.home.HomeViewModel.OutTypes;

import android.graphics.Color;
import android.util.Log;

import com.example.swifttrack.ui.home.HomeFragment;
import com.example.swifttrack.utils.FileUtil;
import com.example.swifttrack.utils.PlotUtil;

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




    private static class Engine extends Thread {
        private static final int INTERVAL = 50;
        private static final int WINDOW_SIZE = 500;

        private final Lock lock = new ReentrantLock();
        private final double[] frame1 = new double[FRAME_SIZE];
        private final double[] frame2 = new double[FRAME_SIZE];

        private int frameCount;

        private boolean running;

        public Engine() { }

        @Override
        public void run() {
            frameCount = 0;

            running = true;

            while (running) {
                try {
                    if (lock.tryLock()) {
                        float[] data = MainActivity.rxQueue.poll(INTERVAL, TimeUnit.MILLISECONDS);
                        if (data != null) {
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

                            if (CHANNEL_MASK[0]) {
                                getDistHistory(1, xWindow1, WINDOW_SIZE);
                                getVelocityHistory(1, vWindow1, WINDOW_SIZE);
                                HomeViewModel.setLineData(xWindow1, OutTypes.LEFT_DIST);
                                HomeViewModel.setLineData(vWindow1, OutTypes.LEFT_V);
//                                 Log.d("C1", Arrays.toString(xWindow1));
//                                PlotUtil.drawX1(xWindow1, "X1", Color.RED);
//                                PlotUtil.drawV1(vWindow1, "V1", Color.BLUE);
                            }
                            if (CHANNEL_MASK[1]) {
                                getDistHistory(2, xWindow2, WINDOW_SIZE);
                                getVelocityHistory(2, vWindow2, WINDOW_SIZE);
                                HomeViewModel.setLineData(xWindow2, OutTypes.RIGHT_DIST);
                                HomeViewModel.setLineData(vWindow2, OutTypes.RIGHT_V);
//                                 Log.d("C2", Arrays.toString(xWindow2));
//                                PlotUtil.drawX2(xWindow2, "X2", Color.RED);
//                                PlotUtil.drawV2(vWindow2, "V2", Color.BLUE);
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

            if (CHANNEL_MASK[0]) {
                getDistHistory(1, xHistory1, frameCount);
                getVelocityHistory(1, vHistory1, frameCount);
                HomeViewModel.setLineData(xHistory1, OutTypes.LEFT_DIST);
                HomeViewModel.setLineData(vHistory1, OutTypes.LEFT_V);
//                PlotUtil.drawX1(xHistory1, "X1", Color.RED);
//                PlotUtil.drawV1(vHistory1, "V1", Color.BLUE);
            }

            if (CHANNEL_MASK[1]) {
                getDistHistory(2, xHistory2, frameCount);
                getVelocityHistory(2, vHistory2, frameCount);
                HomeViewModel.setLineData(xHistory2, OutTypes.RIGHT_DIST);
                HomeViewModel.setLineData(vHistory2, OutTypes.RIGHT_V);
//                PlotUtil.drawX2(xHistory2, "X2", Color.RED);
//                PlotUtil.drawV2(vHistory2, "V2", Color.BLUE);
            }

            double[][] result = new double[frameCount][4];
            for (int i = 0; i < frameCount; i++) {
                result[i][0] = xHistory1[i];
                result[i][1] = xHistory2[i];
                result[i][2] = vHistory1[i];
                result[i][3] = vHistory2[i];
            }
            FileUtil.streamWriteResult(bufferedWriter, result);

            lock.unlock();
        }
    }




    private Engine engine;

    public AudioProcessor() { }

    public void setTimestamp(long timestamp) {
        AudioProcessor.timestamp = timestamp;
    }

    public void init() {
        needSave = false;

        if (CHANNEL_MASK[0]) {
            reset(1);
        }
        if (CHANNEL_MASK[1]) {
            reset(2);
        }

        engine = new Engine();
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
        engine = new Engine();
    }

    private static native void processFrame(int id, double[] data, int n);

    private static native void getVelocityHistory(int id, double[] history, int n);

    private static native void getDistHistory(int id, double[] history, int n);

    private static native void reset(int id);
}
