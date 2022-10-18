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

    private static MutableLiveData<LineDataSet> v = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> a = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> v2d = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> a2v = new MutableLiveData<>();
    private static MutableLiveData<LineDataSet> a2d = new MutableLiveData<>();

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

    public static MutableLiveData<LineDataSet> getLiveLineData(OutTypes type) {
        switch (type){
            case velocity:
                return v;
            case acceleration:
                return a;
            case velocity2dist:
                return v2d;
            case acceleration2velocity:
                return a2v;
            case acceleration2dist:
                return a2d;
            default:
                return null;
        }

    }

    public static void setLineData(double[] values, AccViewModel.OutTypes type) {

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

        getLiveLineData(type).postValue(PlotUtil.getLineDataSet(values, label, Color.RED));



    }
}
