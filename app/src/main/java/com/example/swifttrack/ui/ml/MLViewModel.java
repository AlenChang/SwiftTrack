package com.example.swifttrack.ui.ml;

import android.graphics.Color;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.utils.FileUtil;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class MLViewModel extends ViewModel {
    public enum OutTypes {
        ML, TOF, CIR
    }

    private static MutableLiveData<LineDataSet> mlML; // channel 1 distance
    private static MutableLiveData<LineDataSet> mlTOF;
    private static MutableLiveData<LineDataSet> mlCIR; // channel 1 velocity

    private static int paintingCount;

    private static FileOutputStream fileOutputStreamTOF;
    private static OutputStreamWriter outputStreamWriterTOF;
    private static BufferedWriter bufferedWriterTOF;

    private static FileOutputStream fileOutputStreamCIR;
    private static OutputStreamWriter outputStreamWriterCIR;
    private static BufferedWriter bufferedWriterCIR;

    private static long timestamp;
    private static boolean needSave;

    public MLViewModel() {
        mlML = new MutableLiveData<>();
        mlTOF = new MutableLiveData<>();
        mlCIR = new MutableLiveData<>();
        paintingCount = 0;
    }

    public LiveData<LineDataSet> getLiveLineData(MLViewModel.OutTypes type) {
        switch (type){
            case ML:
                return mlML;
            case TOF:
                return mlTOF;
            case CIR:
                return mlCIR;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, MLViewModel.OutTypes type) {
        switch (type){
            case ML:
                if (paintingCount == 0) {
                    mlML.postValue(PlotUtil.getLineDataSet(values, "ML", Color.RED));
                }
                paintingCount = (paintingCount + 1) % 5;
                break;
            case TOF:
                Log.d("TOF", Arrays.toString(values));
                if (paintingCount == 0) {
                    mlTOF.postValue(PlotUtil.getLineDataSet(values, "TOF", Color.CYAN));
                }
                break;
            case CIR:
                FileUtil.streamWriteCIR(bufferedWriterCIR, values);
                if (paintingCount == 0) {
                    mlCIR.postValue(PlotUtil.getLineDataSet(values, "CIR", Color.RED));
                }
            default:
                break;
        }
    }

    public static void saveTOF(double[] values) {
        FileUtil.streamWriteTOF(bufferedWriterTOF, values);
    }

    public void setTimestamp(long timestamp) {
        MLViewModel.timestamp = timestamp;
    }

    public void start() {
        fileOutputStreamTOF = FileUtil.getFileOutputStream("ml/tof_" + timestamp + ".txt");
        outputStreamWriterTOF = new OutputStreamWriter(fileOutputStreamTOF, StandardCharsets.UTF_8);
        bufferedWriterTOF = new BufferedWriter(outputStreamWriterTOF);

        fileOutputStreamCIR = FileUtil.getFileOutputStream("ml/cir_" + timestamp + ".txt");
        outputStreamWriterCIR = new OutputStreamWriter(fileOutputStreamCIR, StandardCharsets.UTF_8);
        bufferedWriterCIR = new BufferedWriter(outputStreamWriterCIR);
    }

    public void stop() {
        try {
            bufferedWriterTOF.close();
            outputStreamWriterTOF.close();
            fileOutputStreamTOF.close();

            bufferedWriterCIR.close();
            outputStreamWriterCIR.close();
            fileOutputStreamCIR.close();
        } catch (IOException e) {
            Log.e("MLViewModel", e.getMessage());
        }
    }

    public void save() {
        needSave = true;
    }

    public void reset() {
        if (!needSave) {
            FileUtil.deleteFile("ml/tof_" + timestamp + ".txt");
            FileUtil.deleteFile("ml/cir_" + timestamp + ".txt");
        } else {
            needSave = false;
        }
    }
}
