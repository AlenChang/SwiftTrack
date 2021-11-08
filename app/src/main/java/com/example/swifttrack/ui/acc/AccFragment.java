package com.example.swifttrack.ui.acc;

import android.os.Bundle;
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

import com.example.swifttrack.databinding.FragmentAccBinding;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.Timer;

public class AccFragment extends Fragment {
    private AccViewModel accViewModel;
    private FragmentAccBinding binding;

    private AudioPlayer audioPlayer;
    public AudioRecorder audioRecorder;
    public AudioProcessor audioProcessor;


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

        audioProcessor = new AudioProcessor();
        audioProcessor.init(1);

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data

        binding.v.setData(new LineData());
        binding.v.setBorderWidth(20.0f);

        binding.acc.setData(new LineData());
        binding.acc.setBorderWidth(20.0f);

        binding.acc2v.setData(new LineData());
        binding.acc2v.setBorderWidth(20.0f);

        binding.v2d.setData(new LineData());
        binding.v2d.setBorderWidth(20.0f);

        binding.acc2d.setData(new LineData());
        binding.acc2d.setBorderWidth(20.0f);




        return root;
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
}
