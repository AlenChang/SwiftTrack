package com.example.swifttrack.ui.ml;

import android.content.Context;
import android.os.Bundle;
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
import com.example.swifttrack.databinding.FragmentMlBinding;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

public class MLFragment extends Fragment {
    private static final String TAG = "MLFragment";
    private static final String MODEL_NAME = "model.ptl";

    private MLViewModel mlViewModel;
    private FragmentMlBinding binding;
    private AudioPlayer audioPlayer;
    private AudioRecorder audioRecorder;
    private AudioProcessor audioProcessor;
    private Model model;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        mlViewModel =
                new ViewModelProvider(this).get(MLViewModel.class);
        model = new Model(getActivity());

        binding = FragmentMlBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(MainActivity.CHANNEL_MASK);
        audioProcessor.init(AudioProcessor.ActivityID.mlFragment);

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data
        binding.mlMl.setData(new LineData());
        binding.mlMl.setBorderWidth(20.0f);

        binding.mlTof.setData(new LineData());
        binding.mlTof.setBorderWidth(20.0f);

        binding.mlSwift.setData(new LineData());
        binding.mlSwift.setBorderWidth(20.0f);

        binding.mlCir.setData(new LineData());
        binding.mlCir.setBorderWidth(20.0f);

        //=====================
        // set button onclick listener
        binding.startMl.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                switchState(HomeFragment.State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();

                mlViewModel.setTimestamp(timestamp);
                mlViewModel.start();

                Toast.makeText(getActivity(),"Waiting for speaker warm up!",Toast.LENGTH_SHORT).show();
            }
        });

        binding.stopMl.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.STOP);
                audioPlayer.stop();
                audioRecorder.stop();
                audioProcessor.stop();
                mlViewModel.stop();
            }
        });

        binding.resetMl.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.INIT);
                audioPlayer.reset();
                audioRecorder.reset();
                audioProcessor.reset();
                mlViewModel.reset();
            }
        });

        binding.saveMl.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.SAVED);
                audioPlayer.save();
                audioRecorder.save();
                audioProcessor.save();
                mlViewModel.save();
            }
        });

        mlViewModel.getLiveLineData(MLViewModel.OutTypes.ML).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.mlMl, lineDataSet);
            }
        });

        mlViewModel.getLiveLineData(MLViewModel.OutTypes.TOF).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.mlTof, lineDataSet);
            }
        });

        mlViewModel.getLiveLineData(MLViewModel.OutTypes.SWIFT_TRACK).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.mlSwift, lineDataSet);
            }
        });

        mlViewModel.getLiveLineData(MLViewModel.OutTypes.CIR).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
//                setChart(binding.xCir, lineDataSet);
                LineData newData = new LineData();
                newData.addDataSet(lineDataSet);
                binding.mlCir.setData(newData);

                binding.mlCir.notifyDataSetChanged();
                binding.mlCir.invalidate();
            }
        });

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
    private void switchState(HomeFragment.State state) {
        if (state == HomeFragment.State.RUNNING) {
            binding.startMl.setEnabled(false);
            binding.stopMl.setEnabled(true);
            binding.resetMl.setEnabled(false);
            binding.saveMl.setEnabled(false);
        } else if (state == HomeFragment.State.STOP) {
            binding.startMl.setEnabled(false);
            binding.stopMl.setEnabled(false);
            binding.resetMl.setEnabled(true);
            binding.saveMl.setEnabled(true);
        } else if (state == HomeFragment.State.INIT) {
            binding.startMl.setEnabled(true);
            binding.stopMl.setEnabled(false);
            binding.resetMl.setEnabled(false);
            binding.saveMl.setEnabled(false);
        } else {
            binding.startMl.setEnabled(false);
            binding.stopMl.setEnabled(false);
            binding.resetMl.setEnabled(true);
            binding.saveMl.setEnabled(false);
        }
    }


}
