package com.example.swifttrack.ui.respiration;

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
import com.example.swifttrack.MainActivity;
import com.example.swifttrack.databinding.FragmentGalleryBinding;
import com.example.swifttrack.ui.gallery.GalleryViewModel;
import com.example.swifttrack.ui.home.HomeFragment;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.Timer;

public class RSFragment extends Fragment {

    private RSViewModel rsViewModel;
    private FragmentGalleryBinding binding;

    public enum State {
        INIT, RUNNING, STOP, SAVED
    }

    private AudioPlayer audioPlayer;
    private AudioRecorder audioRecorder;
    private AudioProcessor audioProcessor;
    private Timer timer;



    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        rsViewModel =
                new ViewModelProvider(this).get(RSViewModel.class);

        binding = FragmentGalleryBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        //===================== Initialization
        // initialization of components
        audioPlayer = new AudioPlayer();
        audioPlayer.init();

        audioRecorder = new AudioRecorder();
        audioRecorder.init();

        audioProcessor = new AudioProcessor(MainActivity.CHANNEL_MASK);
        audioProcessor.init(AudioProcessor.ActivityID.galleryFragment);

        // set a timer
        timer = new Timer();

        // init button state
        switchState(HomeFragment.State.INIT);

        // initialization of chart data

        binding.xTof.setData(new LineData());
        binding.xTof.setBorderWidth(20.0f);

        binding.xStrata.setData(new LineData());
        binding.xStrata.setBorderWidth(20.0f);

        binding.xSwifttrack.setData(new LineData());
        binding.xSwifttrack.setBorderWidth(20.0f);

        binding.xCir.setData(new LineData());
        binding.xCir.setBorderWidth(20.0f);

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

                Toast.makeText(getActivity(),"Waiting for speaker warm up!",Toast.LENGTH_SHORT).show();

//                timer.schedule(new TimerTask() {
//                    @Override
//                    public void run() {
//                        getActivity().runOnUiThread(new Runnable() {
//                            public void run() {
//                                Toast.makeText(getActivity(),"Calibration!",Toast.LENGTH_SHORT).show();
//                            }
//                        });
//                    }
//                }, 2*1000);


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

        rsViewModel.getLiveLineData(RSViewModel.OutTypes.TOF).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xTof, lineDataSet);
            }
        });

        rsViewModel.getLiveLineData(RSViewModel.OutTypes.STRATA).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xStrata, lineDataSet);
            }
        });

        rsViewModel.getLiveLineData(RSViewModel.OutTypes.SWIFT_TRACK).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
                setChart(binding.xSwifttrack, lineDataSet);
            }
        });

        rsViewModel.getLiveLineData(RSViewModel.OutTypes.CIR).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(LineDataSet lineDataSet) {
//                setChart(binding.xCir, lineDataSet);
                LineData newData = new LineData();
                newData.addDataSet(lineDataSet);
                binding.xCir.setData(newData);
                binding.xCir.notifyDataSetChanged();
                binding.xCir.invalidate();

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
//        LineData lineData = new LineData();
//        lineData.addDataSet(lineDataSet);
//        chart.setData(lineData);

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