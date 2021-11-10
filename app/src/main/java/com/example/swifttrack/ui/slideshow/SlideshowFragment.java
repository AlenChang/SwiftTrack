package com.example.swifttrack.ui.slideshow;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import com.example.swifttrack.AudioPlayer;
import com.example.swifttrack.AudioProcessor;
import com.example.swifttrack.AudioRecorder;
import com.example.swifttrack.MainActivity;
import com.example.swifttrack.R;
import com.example.swifttrack.databinding.FragmentSlideshowBinding;
import com.example.swifttrack.ui.acc.AccViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

public class SlideshowFragment extends Fragment {

    private SlideshowViewModel slideshowViewModel;
    private FragmentSlideshowBinding binding;

    private AudioPlayer audioPlayer;
    public AudioRecorder audioRecorder;
    public AudioProcessor audioProcessor;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        slideshowViewModel =
                new ViewModelProvider(this).get(SlideshowViewModel.class);

        binding = FragmentSlideshowBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(MainActivity.CHANNEL_MASK);
        audioProcessor.init(AudioProcessor.ActivityID.slideFragment);

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data

        binding.imageView.setImageBitmap(Bitmap.createBitmap(SlideshowViewModel.width, SlideshowViewModel.height, Bitmap.Config.ARGB_8888));
        binding.xChart.setData(new LineData());
        binding.xChart.setBorderWidth(20.0f);

        //=====================
        // set button onclick listener
        binding.startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                switchState(HomeFragment.State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();

                Toast.makeText(getActivity(),"Waiting for speaker warm up!",Toast.LENGTH_SHORT).show();


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
                slideshowViewModel.resetBitmap();
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

        slideshowViewModel.getLiveBitmapData().observe(getViewLifecycleOwner(), new Observer<Bitmap>() {
            @Override
            public void onChanged(Bitmap bitmap) {
                binding.imageView.setImageBitmap(bitmap);
            }
        });

        slideshowViewModel.getLiveLineData().observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xChart, lineDataSet);
            }
        });



        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
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
    private void setChart(LineChart chart, LineDataSet lineDataSet){
        LineData lineData = chart.getLineData();
        lineData.removeDataSet(lineData.getDataSetCount() - 1);
        lineData.addDataSet(lineDataSet);

        chart.notifyDataSetChanged();
        chart.invalidate();
    }
}