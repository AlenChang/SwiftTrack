package com.example.swifttrack.ui.acc;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import com.example.swifttrack.AudioPlayer;
import com.example.swifttrack.AudioProcessor;
import com.example.swifttrack.AudioRecorder;

import com.example.swifttrack.MainActivity;
import com.example.swifttrack.databinding.FragmentAccBinding;
import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.DataSet;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Timer;

public class AccFragment extends Fragment {
    private AccViewModel accViewModel;
    private FragmentAccBinding binding;

    private AudioPlayer audioPlayer;
    private AudioRecorder audioRecorder;
    private AudioProcessor audioProcessor;

    private boolean abnormal_flag = false;
    private boolean toast_flag = false;
//    private Boolean chartFlat = false;
//    private int counter = 0;


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        accViewModel =
                new ViewModelProvider(this).get(AccViewModel.class);

        binding = FragmentAccBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(MainActivity.CHANNEL_MASK);
        audioProcessor.init(AudioProcessor.ActivityID.accFragment);

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data

        resetChart(binding.v);
        resetChart(binding.acc);
        resetChart(binding.acc2v);

//        binding.v2d.setData(new LineData());
//        binding.v2d.setBorderWidth(20.0f);
//        binding.v2d.getXAxis().setDrawLabels(false);
//        binding.v2d.getAxisLeft().setDrawLabels(false);
//        binding.v2d.getAxisRight().setDrawLabels(false);
//        binding.v2d.getDescription().setEnabled(false);
//
//        binding.acc2d.setData(new LineData());
//        binding.acc2d.setBorderWidth(20.0f);
//        binding.acc2d.getXAxis().setDrawLabels(false);
//        binding.acc2d.getAxisLeft().setDrawLabels(false);
//        binding.acc2d.getAxisRight().setDrawLabels(false);
//        binding.acc2d.getDescription().setEnabled(false);

        //=====================
        // set button onclick listener
        binding.startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                resetChart(binding.v);
                resetChart(binding.acc);
                resetChart(binding.acc2v);

                switchState(HomeFragment.State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();

                Toast.makeText(getActivity(),"Analyzing!",Toast.LENGTH_LONG).show();
//                new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
//                    @Override
//                    public void run() {
//                        //Do something after 100ms
//                        Toast.makeText(getActivity(),"Calibration!",Toast.LENGTH_SHORT).show();
//                    }
//                }, 5000);

                new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        //Do something after 100ms
                        Toast.makeText(getActivity(),"Start Monitoring!",Toast.LENGTH_SHORT).show();
                    }
                }, 5000);


            }
        });

        binding.stopButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.STOP);
                audioPlayer.stop();
                audioRecorder.stop();
                audioProcessor.stop();
            }
        });

        binding.resetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.INIT);
                audioPlayer.reset();
                audioRecorder.reset();
                audioProcessor.reset();

            }
        });

        binding.saveButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.SAVED);
                audioPlayer.save();
                audioRecorder.save();
                audioProcessor.save();
            }
        });

        AccViewModel.getLiveLineData(3).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.v, lineDataSet, true);
            }
        });
//
        AccViewModel.getLiveLineData(2).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.acc, lineDataSet, false);
            }
        });

        AccViewModel.getLiveLineData(1).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.acc2v, lineDataSet, false);
            }
        });

//        AccViewModel.getLiveLineData(AccViewModel.OutTypes.acceleration2velocity).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
//            @Override
//            public void onChanged(@NonNull LineDataSet lineDataSet) {
//                setChart(binding.acc2v, lineDataSet);
////                AccViewModel.a2v.postValue(null);
//            }
//        });
//
//        AccViewModel.getLiveLineData(AccViewModel.OutTypes.acceleration2dist).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
//            @Override
//            public void onChanged(@NonNull LineDataSet lineDataSet) {
//                setChart(binding.acc2d, lineDataSet);
////                AccViewModel.a2d.
////                AccViewModel.a2d.postValue(null);
//            }
//        });





        return root;
    }

    private void resetChart(LineChart chart){
        chart.setData(new LineData());
        chart.setBorderWidth(20.0f);
        chart.getXAxis().setDrawLabels(false);
        chart.getAxisLeft().setDrawLabels(false);
        chart.getAxisRight().setDrawLabels(false);
        chart.getDescription().setEnabled(false);
        chart.getXAxis().setDrawGridLines(false);
        chart.getAxisLeft().setDrawGridLines(false);
        chart.getAxisRight().setDrawGridLines(false);
    }

    // change state of buttons
    private void switchState(HomeFragment.State state) {
        if (state == HomeFragment.State.RUNNING) {
            binding.startButton.setEnabled(false);
            binding.stopButton.setEnabled(true);
            binding.resetButton.setEnabled(false);
            binding.saveButton.setEnabled(false);
        } else if (state == HomeFragment.State.STOP) {
            binding.startButton.setEnabled(false);
            binding.stopButton.setEnabled(false);
            binding.resetButton.setEnabled(true);
            binding.saveButton.setEnabled(true);
        } else if (state == HomeFragment.State.INIT) {
            binding.startButton.setEnabled(true);
            binding.stopButton.setEnabled(false);
            binding.resetButton.setEnabled(false);
            binding.saveButton.setEnabled(false);
        } else {
            binding.startButton.setEnabled(false);
            binding.stopButton.setEnabled(false);
            binding.resetButton.setEnabled(true);
            binding.saveButton.setEnabled(false);
        }
    }

    public void setLineData(double[] y, LineDataSet lineDataSet){
        for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++){
            lineDataSet.getEntryForIndex(ti).setY((float) y[ti]);
        }
    }

    // update chart
    public void setChart(LineChart chart, LineDataSet lineDataSet, boolean setRange){
        if(lineDataSet != null){

            LineData lineData = chart.getLineData();
            Highlight highlight;
            int source_num;
            int new_num;


            if(lineData.getDataSetCount() == 0){
                lineData.addDataSet(lineDataSet);
                source_num = lineData.getDataSetByIndex(0).getEntryCount();
                new_num = lineDataSet.getEntryCount();
            }
            else{
                source_num = lineData.getDataSetByIndex(0).getEntryCount();
                new_num = lineDataSet.getEntryCount();
                for(int ti = 0; ti < source_num; ti++){
                    lineData.getDataSetByIndex(0).getEntryForIndex(ti).setY(lineDataSet.getEntryForIndex(ti).getY());
                }
                if(source_num < new_num){
                    for(int mi = source_num; mi < new_num; mi++){
                        lineData.getDataSetByIndex(0).addEntry(new Entry(mi * 1.0f, lineDataSet.getEntryForIndex(mi).getY()));
                    }
                }

                if(setRange){
                    highlight = new Highlight(new_num*1.0f, lineData.getDataSetByIndex(0).getEntryForIndex(new_num-1).getY(), 0);
                    chart.highlightValue(highlight, false); // highlight this value, don't call listener
                }
            }


            lineData.getDataSetByIndex(0).calcMinMax();
            lineData.notifyDataChanged();
            if(setRange && AudioProcessor.getRunnningStatus()){
                chart.setVisibleXRange(0, AudioProcessor.WIN_LENGTH);
            }

            if(setRange && !AudioProcessor.getRunnningStatus()){
                chart.setVisibleXRange(0, new_num);
            }

            chart.notifyDataSetChanged();
            chart.invalidate();
        }




    }
}
