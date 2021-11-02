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

import com.example.swifttrack.R;
import com.example.swifttrack.databinding.FragmentHomeBinding;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;



public class HomeFragment extends Fragment {

    private HomeViewModel homeViewModel;
    private FragmentHomeBinding binding;

    public enum State {
        INIT, RUNNING, STOP, SAVED
    }

    // ******************************
    // A queue to to store received dataframes
    // ******************************
//    public static final ArrayBlockingQueue<float[]> rxQueue = new ArrayBlockingQueue<>(AudioPlayer.BUFFER_SIZE);
//
//    public AudioPlayer audioPlayer;
//    public AudioRecorder audioRecorder;
//    public AudioProcessor audioProcessor;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        // initialize chart view

        binding.xChart2.setData(new LineData());
        binding.vChart1.setData(new LineData());
        binding.vChart2.setData(new LineData());

//        { // initialize button view
//            startButton = binding.startButton;
//            stopButton = binding.stopButton;
//            resetButton = binding.resetButton;
//            saveButton = binding.saveButton;
//        }
        binding.xChart1.setData(homeViewModel.getLineData());
        binding.xChart2.setData(homeViewModel.getLineData());

        binding.startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.RUNNING);

                Log.d("Thread", String.valueOf(Thread.currentThread()));
                homeViewModel.setLineData(binding.xChart1, "X1", Color.RED);
                binding.xChart1.notifyDataSetChanged();
                binding.xChart1.invalidate();
            }
        });

        binding.stopButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.STOP);

                Log.d("Thread", String.valueOf(Thread.currentThread()));
                homeViewModel.setLineData(binding.xChart2, "X2", Color.BLUE);
                binding.xChart2.notifyDataSetChanged();
                binding.xChart2.invalidate();
            }
        });

        binding.resetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.INIT);

                Log.d("Thread", String.valueOf(Thread.currentThread()));
                homeViewModel.setLineData(binding.vChart1, "V1", Color.RED);
                binding.vChart1.notifyDataSetChanged();
                binding.vChart1.invalidate();
            }
        });

        binding.saveButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(State.SAVED);

                Log.d("Thread", String.valueOf(Thread.currentThread()));
                homeViewModel.setLineData(binding.vChart2, "V2", Color.RED);
                binding.vChart2.notifyDataSetChanged();
                binding.vChart2.invalidate();
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

    private void switchState(State state) {
        if (state == State.RUNNING) {
            binding.startButton.setEnabled(false);
            binding.stopButton.setEnabled(true);
            binding.resetButton.setEnabled(false);
            binding.saveButton.setEnabled(false);
        } else if (state == State.STOP) {
            binding.startButton.setEnabled(false);
            binding.stopButton.setEnabled(false);
            binding.resetButton.setEnabled(true);
            binding.saveButton.setEnabled(true);
        } else if (state == State.INIT) {
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

}