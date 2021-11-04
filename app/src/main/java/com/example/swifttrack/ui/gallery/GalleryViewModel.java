package com.example.swifttrack.ui.gallery;

import android.graphics.Color;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.ui.home.HomeViewModel;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

public class GalleryViewModel extends ViewModel {
    public enum OutTypes {
        TOF, STRATA, SWIFT_TRACK, CIR
    }

    private static MutableLiveData<LineDataSet> xTOF; // channel 1 distance
    private static MutableLiveData<LineDataSet> xSTRATA; // channel 2 distance
    private static MutableLiveData<LineDataSet> xSWIFTTRACK; // channel 1 velocity
    private static MutableLiveData<LineDataSet> xCIR; // channel 1 velocity

    public GalleryViewModel() {
        xTOF = new MutableLiveData<>();
        xSTRATA = new MutableLiveData<>();
        xSWIFTTRACK = new MutableLiveData<>();
        xCIR = new MutableLiveData<>();
    }

    public LiveData<LineDataSet> getLiveLineData(GalleryViewModel.OutTypes type) {
        switch (type){
            case TOF:
                return xTOF;
            case STRATA:
                return xSTRATA;
            case SWIFT_TRACK:
                return xSWIFTTRACK;
            case CIR:
                return xCIR;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, GalleryViewModel.OutTypes type) {

        switch (type){
            case TOF:
                xTOF.postValue(PlotUtil.getLineDataSet(values, "TOF", Color.RED));
                break;
            case STRATA:
                xSTRATA.postValue(PlotUtil.getLineDataSet(values, "Basic Channel Estimation", Color.BLUE));
                break;
            case SWIFT_TRACK:
                xSWIFTTRACK.postValue(PlotUtil.getLineDataSet(values, "SwiftTrack", Color.CYAN));
                break;
            case CIR:
                xCIR.postValue(PlotUtil.getLineDataSet(values, "CIR", Color.RED));
            default:
                break;
        }



    }


}