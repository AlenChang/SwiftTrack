package com.example.swifttrack.ui.home;

import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;

import com.example.swifttrack.utils.PlotUtil;
import java.util.Random;

public class HomeViewModel extends ViewModel {

    private MutableLiveData<String> mText;
    private LineData lineData;


    public HomeViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is home fragment");

        lineData = new LineData();
    }

    public LiveData<String> getText() {
        return mText;
    }
    public LineData getLineData() {return lineData; }

    public void setLineData(LineChart chartName, String label, int color) {

        Log.d("Thread", String.valueOf(Thread.currentThread()));


        Random rand = new Random();
        double[] values = new double[500];
        for (int i = 0; i < 500; i++){
            values[i] = rand.nextDouble();
        }

        lineData = PlotUtil.draw(chartName, values, label, color);



    }
}