package com.example.swifttrack.ui.ml;

import android.graphics.Color;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

public class MLViewModel extends ViewModel {
    public enum OutTypes {
        ML, SWIFT_TRACK, CIR
    }

    private static MutableLiveData<LineDataSet> mlML; // channel 1 distance
    private static MutableLiveData<LineDataSet> mlSWIFTTRACK; // channel 1 velocity
    private static MutableLiveData<LineDataSet> mlCIR; // channel 1 velocity

    public MLViewModel() {
        mlML = new MutableLiveData<>();
        mlSWIFTTRACK = new MutableLiveData<>();
        mlCIR = new MutableLiveData<>();
    }

    public LiveData<LineDataSet> getLiveLineData(MLViewModel.OutTypes type) {
        switch (type){
            case ML:
                return mlML;
            case SWIFT_TRACK:
                return mlSWIFTTRACK;
            case CIR:
                return mlCIR;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, MLViewModel.OutTypes type) {

        switch (type){
            case ML:
                mlML.postValue(PlotUtil.getLineDataSet(values, "ML", Color.RED));
                break;
            case SWIFT_TRACK:
                mlSWIFTTRACK.postValue(PlotUtil.getLineDataSet(values, "SwiftTrack", Color.CYAN));
                break;
            case CIR:
                mlCIR.postValue(PlotUtil.getLineDataSet(values, "CIR", Color.RED));
            default:
                break;
        }



    }
}
