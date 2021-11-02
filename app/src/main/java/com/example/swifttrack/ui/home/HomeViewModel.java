package com.example.swifttrack.ui.home;

import android.graphics.Color;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;

import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

import org.apache.commons.math3.geometry.euclidean.twod.Line;

import java.util.Random;

public class HomeViewModel extends ViewModel {

    public enum OutTypes {
            LEFT_DIST, RIGHT_DIST, LEFT_V, RIGHT_V
    }

    private MutableLiveData<String> mText;

    private static MutableLiveData<LineDataSet> xChart1; // channel 1 distance
    private static MutableLiveData<LineDataSet> xChart2; // channel 2 distance
    private static MutableLiveData<LineDataSet> vChart1; // channel 1 velocity
    private static MutableLiveData<LineDataSet> vChart2; // channel 2 velocity


    public HomeViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is home fragment");

        xChart1 = new MutableLiveData<>();
        xChart2 = new MutableLiveData<>();
        vChart1 = new MutableLiveData<>();
        vChart2 = new MutableLiveData<>();
    }

    public LiveData<String> getText() {
        return mText;
    }
    public LiveData<LineDataSet> getLiveLineData(OutTypes type) {
        switch (type){
            case LEFT_DIST:
                return xChart1;
            case RIGHT_DIST:
                return xChart2;
            case LEFT_V:
                return vChart1;
            case RIGHT_V:
                return vChart2;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, OutTypes type) {

        switch (type){
            case LEFT_DIST:
                xChart1.postValue(PlotUtil.getLineDataSet(values, "Channel 1 Distance", Color.RED));
                break;
            case RIGHT_DIST:
                xChart2.postValue(PlotUtil.getLineDataSet(values, "Channel 2 Distance", Color.BLUE));
                break;
            case LEFT_V:
                vChart1.postValue(PlotUtil.getLineDataSet(values, "Channel 1 Velocity", Color.RED));
                break;
            case RIGHT_V:
                vChart2.postValue(PlotUtil.getLineDataSet(values, "Channel 2 Velocity", Color.BLUE));
                break;
            default:
                break;
        }



    }
}