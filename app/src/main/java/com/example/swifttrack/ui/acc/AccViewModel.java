package com.example.swifttrack.ui.acc;

import android.graphics.Color;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import org.apache.commons.math3.geometry.euclidean.twod.Line;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class AccViewModel extends ViewModel {
    public enum OutTypes {
        velocity, acceleration, velocity2dist, acceleration2velocity, acceleration2dist
    }

//    private static MutableLiveData<LineDataSet> v = new MutableLiveData<>();
//    private static MutableLiveData<LineDataSet> a = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> v2d = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> waveform_chart = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> resp_chart = new MutableLiveData<>();

    public static int counter = 0;

    public AccViewModel(){
//        v = new MutableLiveData<>();
////        v.Dataset
////        v.add
//        a = new MutableLiveData<>();
//        v2d = new MutableLiveData<>();
//        a2v = new MutableLiveData<>();
//        a2d = new MutableLiveData<>();
    }

    public static MutableLiveData<LineDataSet> getLiveLineData(int type) {
        switch (type){
            case 1:
                return resp_chart;
            case 2:
                return waveform_chart;
            case 3:
                return v2d;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, double[] next_waveform, double[] resp_waveform, boolean is_new_waveform, AccViewModel.OutTypes type) {

        String label = " ";
        switch (type){
            case velocity:
                label = "Velocity";
                break;
            case acceleration:
//                a.postValue(PlotUtil.getLineDataSet(values, "Acceleration", Color.BLUE));
                label = "Acceleration";
                break;
            case velocity2dist:
//                v2d.postValue(PlotUtil.getLineDataSet(values, "Distance (SwiftTrack)", Color.CYAN));
                label = "Distance (SwiftTrack)";
                break;
            case acceleration2velocity:
//                a2v.postValue(PlotUtil.getLineDataSet(values, "Acceleration 2 Velocity", Color.RED));
                label = "Acceleration 2 Velocity";
                break;
            case acceleration2dist:
//                a2d.postValue(PlotUtil.getLineDataSet(values, "Distance (Strata)", Color.RED));
                label = "Distance (Strata)";
                break;
            default:
                break;
        }

        getLiveLineData(3).postValue(PlotUtil.getLineDataSet(values, "Distance change of chest caused by respiration", Color.RED));
//        if(is_new_waveform){
        getLiveLineData(2).postValue(PlotUtil.getLineDataSet(next_waveform, "The Last Waveform", Color.RED));
        getLiveLineData(1).postValue(PlotUtil.getLineDataSet(resp_waveform, "Averaged Respiration Waveform", Color.RED));
//        }









    }
}
