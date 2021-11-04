package com.example.swifttrack.ui.gallery;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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
import com.example.swifttrack.databinding.FragmentGalleryBinding;
import com.example.swifttrack.ui.home.HomeFragment;
import com.example.swifttrack.ui.home.HomeViewModel;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

public class GalleryFragment extends Fragment {

    private GalleryViewModel galleryViewModel;
    private FragmentGalleryBinding binding;

    public enum State {
        INIT, RUNNING, STOP, SAVED
    }

    private AudioPlayer audioPlayer;
    public AudioRecorder audioRecorder;
    public AudioProcessor audioProcessor;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        galleryViewModel =
                new ViewModelProvider(this).get(GalleryViewModel.class);

        binding = FragmentGalleryBinding.inflate(inflater, container, false);
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

        binding.xTof.setData(new LineData());
        binding.xTof.setBorderWidth(20.0f);

        binding.xStrata.setData(new LineData());
        binding.xStrata.setBorderWidth(20.0f);

        binding.xSwifttrack.setData(new LineData());
        binding.xSwifttrack.setBorderWidth(20.0f);

        //=====================
        // set button onclick listener
        binding.startGallery.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                switchState(HomeFragment.State.RUNNING);

                long timestamp = System.currentTimeMillis();
                audioPlayer.setTimestamp(timestamp);
                audioPlayer.start();

                audioRecorder.setTimestamp(timestamp);
                audioRecorder.start();

                audioProcessor.setTimestamp(timestamp);
                audioProcessor.start();
            }
        });

        binding.stopGallery.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.STOP);
                audioPlayer.stop();
                audioRecorder.stop();
                audioProcessor.stop();
            }
        });

        binding.resetGallery.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.INIT);
                audioPlayer.reset();
                audioRecorder.reset();
                audioProcessor.reset();
            }
        });

        binding.saveGallery.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                switchState(HomeFragment.State.SAVED);
                audioPlayer.save();
                audioRecorder.save();
                audioProcessor.save();
            }
        });

        galleryViewModel.getLiveLineData(GalleryViewModel.OutTypes.TOF).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xTof, lineDataSet);
            }
        });

        galleryViewModel.getLiveLineData(GalleryViewModel.OutTypes.STRATA).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xStrata, lineDataSet);
            }
        });

        galleryViewModel.getLiveLineData(GalleryViewModel.OutTypes.SWIFT_TRACK).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xSwifttrack, lineDataSet);
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
            binding.startGallery.setEnabled(false);
            binding.stopGallery.setEnabled(true);
            binding.resetGallery.setEnabled(false);
            binding.saveGallery.setEnabled(false);
        } else if (state == HomeFragment.State.STOP) {
            binding.startGallery.setEnabled(false);
            binding.stopGallery.setEnabled(false);
            binding.resetGallery.setEnabled(true);
            binding.saveGallery.setEnabled(true);
        } else if (state == HomeFragment.State.INIT) {
            binding.startGallery.setEnabled(true);
            binding.stopGallery.setEnabled(false);
            binding.resetGallery.setEnabled(false);
            binding.saveGallery.setEnabled(false);
        } else {
            binding.startGallery.setEnabled(false);
            binding.stopGallery.setEnabled(false);
            binding.resetGallery.setEnabled(true);
            binding.saveGallery.setEnabled(false);
        }
    }
}