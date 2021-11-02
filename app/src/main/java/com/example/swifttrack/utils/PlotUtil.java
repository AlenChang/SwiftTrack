package com.example.swifttrack.utils;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.ArrayList;
import java.util.List;

public class PlotUtil {

    private static final float TAP = 0.01f;

    private static class Series {
        public List<Entry> entries;
        public String label;
        public int color;

        public Series(double[] values, String label, int color) {
            this.entries = new ArrayList<>();
            for (int i = 0; i < values.length; i++) {
                this.entries.add(new Entry((float) i * TAP, (float) values[i]));
            }
            this.label = label;
            this.color = color;
        }
    }

    public static LineDataSet getLineDataSet(double[] values, String label, int color) {
        Series series = new Series(values, label, color);

        LineDataSet lineDataSet = new LineDataSet(series.entries, label);
        lineDataSet.setColor(series.color);
        lineDataSet.setDrawCircles(true);

//        LineData lineData = chartName.getLineData();
//        lineData.removeDataSet(lineData.getDataSetCount() - 1);
//        lineData.addDataSet(lineDataSet);

        return lineDataSet;

    }


}
