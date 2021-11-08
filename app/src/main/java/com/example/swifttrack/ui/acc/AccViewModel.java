package com.example.swifttrack.ui.acc;

import android.graphics.Color;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.data.LineDataSet;

public class AccViewModel extends ViewModel {
    public enum OutTypes {
        velocity, acceleration, velocity2dist, acceleration2velocity, acceleration2dist
    }

    private static MutableLiveData<LineDataSet> v;
    private static MutableLiveData<LineDataSet> a;
    private static MutableLiveData<LineDataSet> v2d;
    private static MutableLiveData<LineDataSet> a2v;
    private static MutableLiveData<LineDataSet> a2d;

    public AccViewModel(){
        v = new MutableLiveData<>();
        a = new MutableLiveData<>();
        v2d = new MutableLiveData<>();
        a2v = new MutableLiveData<>();
        a2d = new MutableLiveData<>();
    }

    public LiveData<LineDataSet> getLiveLineData(OutTypes type) {
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

        switch (type){
            case velocity:
                v.postValue(PlotUtil.getLineDataSet(values, "Velocity", Color.RED));
                break;
            case acceleration:
                a.postValue(PlotUtil.getLineDataSet(values, "Acceleration", Color.BLUE));
                break;
            case velocity2dist:
                v2d.postValue(PlotUtil.getLineDataSet(values, "Velocity 2 Dist", Color.CYAN));
                break;
            case acceleration2velocity:
                a2v.postValue(PlotUtil.getLineDataSet(values, "Acceleration 2 Velocity", Color.RED));
                break;
            case acceleration2dist:
                a2d.postValue(PlotUtil.getLineDataSet(values, "Acceleration 2 Dist", Color.RED));
                break;
            default:
                break;
        }



    }
}
