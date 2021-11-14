package com.example.swifttrack.ui.home;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import com.example.swifttrack.AudioPlayer;
import com.example.swifttrack.AudioProcessor;
import com.example.swifttrack.AudioRecorder;
import com.example.swifttrack.R;
import com.example.swifttrack.databinding.FragmentHomeBinding;
import com.example.swifttrack.utils.PlotUtil;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import org.apache.commons.math3.geometry.euclidean.twod.Line;

import java.util.Random;



public class HomeFragment extends Fragment {

    private HomeViewModel homeViewModel;
    private FragmentHomeBinding binding;

    public enum State {
        INIT, RUNNING, STOP, SAVED
    }

    private AudioPlayer audioPlayer;
    private AudioRecorder audioRecorder;
    private AudioProcessor audioProcessor;

    private static final boolean[] CHANNEL_MASK = {true, true};


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(CHANNEL_MASK);
        audioProcessor.init();

        // init button state
        switchState(State.INIT);


        // initialization of chart data

        binding.xChart1.setData(new LineData());
        binding.xChart1.setBorderWidth(20.0f);

        binding.xChart2.setData(new LineData());
        binding.xChart2.setBorderWidth(20.0f);

        binding.vChart1.setData(new LineData());
        binding.vChart1.setBorderWidth(20.0f);

        binding.vChart2.setData(new LineData());
        binding.vChart2.setBorderWidth(20.0f);



        //=====================
        // set button onclick listener
        binding.startHome.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                switchState(State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();
            }
        });

        binding.stopHome.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.STOP);
                audioPlayer.stop();
                audioRecorder.stop();
                audioProcessor.stop();
            }
        });

        binding.resetHome.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.INIT);
                audioPlayer.reset();
                audioRecorder.reset();
                audioProcessor.reset();
            }
        });

        binding.saveHome.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.SAVED);
                audioPlayer.save();
                audioRecorder.save();
                audioProcessor.save();
            }
        });

        //=====================
        // set chart observer
        homeViewModel.getLiveLineData(HomeViewModel.OutTypes.LEFT_DIST).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xChart1, lineDataSet);
            }
        });

        homeViewModel.getLiveLineData(HomeViewModel.OutTypes.RIGHT_DIST).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xChart2, lineDataSet);
            }
        });

        homeViewModel.getLiveLineData(HomeViewModel.OutTypes.LEFT_V).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.vChart1, lineDataSet);
            }
        });

        homeViewModel.getLiveLineData(HomeViewModel.OutTypes.RIGHT_V).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.vChart2, lineDataSet);
            }
        });


//        final TextView textView = binding.textHome;
//        homeViewModel.getText().observe(getViewLifecycleOwner(), new Observer<String>() {
//            @Override
//            public void onChanged(@Nullable String s) {
//                textView.setText(s);
//            }
//        });
        return root;
    }



    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

    // update chart
    private void setChart(LineChart chart, LineDataSet lineDataSet){
        LineData lineData = chart.getLineData();
        lineData.removeDataSet(lineData.getDataSetCount() - 1);
        lineData.addDataSet(lineDataSet);
        chart.notifyDataSetChanged();
        chart.invalidate();
    }

    // change state of buttons
    private void switchState(State state) {
        if (state == State.RUNNING) {
            binding.startHome.setEnabled(false);
            binding.stopHome.setEnabled(true);
            binding.resetHome.setEnabled(false);
            binding.saveHome.setEnabled(false);
        } else if (state == State.STOP) {
            binding.startHome.setEnabled(false);
            binding.stopHome.setEnabled(false);
            binding.resetHome.setEnabled(true);
            binding.saveHome.setEnabled(true);
        } else if (state == State.INIT) {
            binding.startHome.setEnabled(true);
            binding.stopHome.setEnabled(false);
            binding.resetHome.setEnabled(false);
            binding.saveHome.setEnabled(false);
        } else {
            binding.startHome.setEnabled(false);
            binding.stopHome.setEnabled(false);
            binding.resetHome.setEnabled(true);
            binding.saveHome.setEnabled(false);
        }
    }


}