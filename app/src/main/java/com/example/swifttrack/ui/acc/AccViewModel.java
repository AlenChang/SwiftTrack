package com.example.swifttrack.ui.acc;

import android.graphics.Color;
import android.provider.MediaStore;
import android.util.Log;

import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.swifttrack.AudioProcessor;
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
    private static int LINE_COLOR = Color.parseColor("#7DB836");

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

    public static void setLineData(double[] values, int valid_length, double[] next_waveform, double[] resp_waveform, boolean is_new_waveform) {

//        double[] valid_values;
//        if(valid_length > AudioProcessor.WIN_LENGTH){
//            valid_length = AudioProcessor.WIN_LENGTH;
//            valid_values = new double[valid_length];
//            System.arraycopy(values, values.length - valid_length, valid_values, 0, valid_length);
//        }else{
//            valid_values = new double[valid_length];
//            System.arraycopy(values, 0, valid_values, 0, valid_length);
//        }

        getLiveLineData(3).postValue(PlotUtil.getLineDataSet(processValues(values, valid_length), "Distance change of chest caused by respiration", LINE_COLOR));
//        if(is_new_waveform){
//        if(is_new_waveform){
//            getLiveLineData(2).postValue(PlotUtil.getLineDataSet(next_waveform, "The Last Waveform", LINE_COLOR));
//            getLiveLineData(1).postValue(PlotUtil.getLineDataSet(resp_waveform, "Averaged Respiration Waveform", LINE_COLOR));
//        }
    }

    public static double[] processValues(double[] values, int valid_length){
        double[] valid_values;
        if(valid_length > AudioProcessor.WIN_LENGTH){
            valid_length = AudioProcessor.WIN_LENGTH;
            valid_values = new double[valid_length];
            System.arraycopy(values, values.length - valid_length, valid_values, 0, valid_length);
        }else{
            valid_values = new double[valid_length];
            System.arraycopy(values, 0, valid_values, 0, valid_length);
        }
        return valid_values;

    }
}
