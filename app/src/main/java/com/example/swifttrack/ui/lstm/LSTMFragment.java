package com.example.swifttrack.ui.lstm;

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
import com.example.swifttrack.databinding.FragmentLstmBinding;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

public class LSTMFragment extends Fragment {
    private static final String TAG = "LSTMFragment";

    private LSTMViewModel lstmViewModel;
    private FragmentLstmBinding binding;
    private AudioPlayer audioPlayer;
    private AudioRecorder audioRecorder;
    private AudioProcessor audioProcessor;
    private LSTMModel model;


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        lstmViewModel =
                new ViewModelProvider(this).get(LSTMViewModel.class);
        model = new LSTMModel(getActivity());

        binding = FragmentLstmBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(MainActivity.CHANNEL_MASK);
        audioProcessor.init(AudioProcessor.ActivityID.lstmFragment);

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data
        binding.lstmLstm.setData(new LineData());
        binding.lstmLstm.setBorderWidth(20.0f);

        binding.lstmTof.setData(new LineData());
        binding.lstmTof.setBorderWidth(20.0f);

        binding.lstmSwift.setData(new LineData());
        binding.lstmSwift.setBorderWidth(20.0f);

        binding.lstmCir.setData(new LineData());
        binding.lstmCir.setBorderWidth(20.0f);

        //=====================
        // set button onclick listener
        binding.startLstm.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                switchState(HomeFragment.State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();

                lstmViewModel.setTimestamp(timestamp);
                lstmViewModel.start();

                Toast.makeText(getActivity(),"Waiting for speaker warm up!",Toast.LENGTH_SHORT).show();
            }
        });

        binding.stopLstm.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.STOP);
                audioPlayer.stop();
                audioRecorder.stop();
                audioProcessor.stop();
                lstmViewModel.stop();
            }
        });

        binding.resetLstm.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.INIT);
                audioPlayer.reset();
                audioRecorder.reset();
                audioProcessor.reset();
                lstmViewModel.reset();
            }
        });

        binding.saveLstm.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.SAVED);
                audioPlayer.save();
                audioRecorder.save();
                audioProcessor.save();
                lstmViewModel.save();
            }
        });

        lstmViewModel.getLiveLineData(LSTMViewModel.OutTypes.LSTM).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.lstmLstm, lineDataSet);
            }
        });

        lstmViewModel.getLiveLineData(LSTMViewModel.OutTypes.TOF).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.lstmTof, lineDataSet);
            }
        });

        lstmViewModel.getLiveLineData(LSTMViewModel.OutTypes.SWIFT_TRACK).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.lstmSwift, lineDataSet);
            }
        });

        lstmViewModel.getLiveLineData(LSTMViewModel.OutTypes.CIR).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
//                setChart(binding.xCir, lineDataSet);
                LineData newData = new LineData();
                newData.addDataSet(lineDataSet);
                binding.lstmCir.setData(newData);

                binding.lstmCir.notifyDataSetChanged();
                binding.lstmCir.invalidate();
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
            binding.startLstm.setEnabled(false);
            binding.stopLstm.setEnabled(true);
            binding.resetLstm.setEnabled(false);
            binding.saveLstm.setEnabled(false);
        } else if (state == HomeFragment.State.STOP) {
            binding.startLstm.setEnabled(false);
            binding.stopLstm.setEnabled(false);
            binding.resetLstm.setEnabled(true);
            binding.saveLstm.setEnabled(true);
        } else if (state == HomeFragment.State.INIT) {
            binding.startLstm.setEnabled(true);
            binding.stopLstm.setEnabled(false);
            binding.resetLstm.setEnabled(false);
            binding.saveLstm.setEnabled(false);
        } else {
            binding.startLstm.setEnabled(false);
            binding.stopLstm.setEnabled(false);
            binding.resetLstm.setEnabled(true);
            binding.saveLstm.setEnabled(false);
        }
    }


}
