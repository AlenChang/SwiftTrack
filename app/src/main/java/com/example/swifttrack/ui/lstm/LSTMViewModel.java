package com.example.swifttrack.ui.lstm;

import android.graphics.Color;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.utils.FileUtil;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class LSTMViewModel extends ViewModel {
    public enum OutTypes {
        LSTM, TOF, CIR, SWIFT_TRACK
    }

    private static MutableLiveData<LineDataSet> lstmLSTM; // channel 1 distance
    private static MutableLiveData<LineDataSet> lstmTOF;
    private static MutableLiveData<LineDataSet> lstmCIR; // channel 1 velocity
    private static MutableLiveData<LineDataSet> lstmSwiftTrack;

    private static int paintingCount;

    private static FileOutputStream fileOutputStreamTOF;
    private static OutputStreamWriter outputStreamWriterTOF;
    private static BufferedWriter bufferedWriterTOF;

    private static FileOutputStream fileOutputStreamSwiftTrack;
    private static OutputStreamWriter outputStreamWriterSwiftTrack;
    private static BufferedWriter bufferedWriterSwiftTrack;

    private static FileOutputStream fileOutputStreamCIR;
    private static OutputStreamWriter outputStreamWriterCIR;
    private static BufferedWriter bufferedWriterCIR;

    private static long timestamp;
    private static boolean needSave;
    private static int tofCount;

    public LSTMViewModel() {
        lstmLSTM = new MutableLiveData<>();
        lstmTOF = new MutableLiveData<>();
        lstmCIR = new MutableLiveData<>();
        lstmSwiftTrack = new MutableLiveData<>();
        paintingCount = 0;
        tofCount = 0;
    }

    public LiveData<LineDataSet> getLiveLineData(com.example.swifttrack.ui.lstm.LSTMViewModel.OutTypes type) {
        switch (type){
            case LSTM:
                return lstmLSTM;
            case TOF:
                return lstmTOF;
            case CIR:
                return lstmCIR;
            case SWIFT_TRACK:
                return lstmSwiftTrack;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, com.example.swifttrack.ui.lstm.LSTMViewModel.OutTypes type) {
        switch (type){
            case LSTM:
                if (paintingCount == 0) {
                    lstmLSTM.postValue(PlotUtil.getLineDataSet(values, "LSTM", Color.RED));
                }
                paintingCount = (paintingCount + 1) % 5;
                break;
            case TOF:
                if (paintingCount == 0) {
                    lstmTOF.postValue(PlotUtil.getLineDataSet(values, "TOF", Color.CYAN));
                }
                break;
            case SWIFT_TRACK:
                if (paintingCount == 0) {
                    lstmSwiftTrack.postValue(PlotUtil.getLineDataSet(values, "SwiftTrack", Color.CYAN));
                }
                break;
            case CIR:
                FileUtil.streamWriteCIR(bufferedWriterCIR, values);
                if (paintingCount == 0) {
                    lstmCIR.postValue(PlotUtil.getLineDataSet(values, "CIR", Color.RED));
                }
            default:
                break;
        }
    }

    public static void saveTOF(double[] values) {
        FileUtil.streamWriteTOF(bufferedWriterTOF, values);
    }

    public static void saveSwift(double[] values) {
        FileUtil.streamWriteTOF(bufferedWriterSwiftTrack, values);
    }

    public void setTimestamp(long timestamp) {
        com.example.swifttrack.ui.lstm.LSTMViewModel.timestamp = timestamp;
    }

    public void start() {
        fileOutputStreamTOF = FileUtil.getFileOutputStream("lstm/tof_" + timestamp + ".txt");
        outputStreamWriterTOF = new OutputStreamWriter(fileOutputStreamTOF, StandardCharsets.UTF_8);
        bufferedWriterTOF = new BufferedWriter(outputStreamWriterTOF);

        fileOutputStreamCIR = FileUtil.getFileOutputStream("lstm/cir_" + timestamp + ".txt");
        outputStreamWriterCIR = new OutputStreamWriter(fileOutputStreamCIR, StandardCharsets.UTF_8);
        bufferedWriterCIR = new BufferedWriter(outputStreamWriterCIR);

        fileOutputStreamSwiftTrack = FileUtil.getFileOutputStream("lstm/swift_" + timestamp + ".txt");
        outputStreamWriterSwiftTrack = new OutputStreamWriter(fileOutputStreamSwiftTrack, StandardCharsets.UTF_8);
        bufferedWriterSwiftTrack = new BufferedWriter(outputStreamWriterSwiftTrack);
    }

    public void stop() {
        try {
            bufferedWriterTOF.close();
            outputStreamWriterTOF.close();
            fileOutputStreamTOF.close();

            bufferedWriterCIR.close();
            outputStreamWriterCIR.close();
            fileOutputStreamCIR.close();

            bufferedWriterSwiftTrack.close();
            outputStreamWriterSwiftTrack.close();
            fileOutputStreamSwiftTrack.close();
        } catch (IOException e) {
            Log.e("LSTMViewModel", e.getMessage());
        }
    }

    public void save() {
        needSave = true;
    }

    public void reset() {
        if (!needSave) {
            FileUtil.deleteFile("lstm/tof_" + timestamp + ".txt");
            FileUtil.deleteFile("lstm/cir_" + timestamp + ".txt");
            FileUtil.deleteFile("lstm/swift_" + timestamp + ".txt");
        } else {
            needSave = false;
        }
    }
}

