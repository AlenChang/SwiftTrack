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

        binding.v.setData(new LineData());
        binding.v.setBorderWidth(20.0f);
        binding.v.getXAxis().setDrawLabels(false);
        binding.v.getAxisLeft().setDrawLabels(false);
        binding.v.getAxisRight().setDrawLabels(false);
        binding.v.getDescription().setEnabled(false);
//        binding.v.setNoDataText("No Data");
//        binding.v.
//        binding.v.setVisible

        binding.acc.setData(new LineData());
        binding.acc.setBorderWidth(20.0f);
        binding.acc.getXAxis().setDrawLabels(false);
        binding.acc.getAxisLeft().setDrawLabels(false);
        binding.acc.getAxisRight().setDrawLabels(false);
        binding.acc.getDescription().setEnabled(false);

        binding.acc2v.setData(new LineData());
        binding.acc2v.setBorderWidth(20.0f);
        binding.acc2v.getXAxis().setDrawLabels(false);
        binding.acc2v.getAxisLeft().setDrawLabels(false);
        binding.acc2v.getAxisRight().setDrawLabels(false);
        binding.acc2v.getDescription().setEnabled(false);

        binding.v2d.setData(new LineData());
        binding.v2d.setBorderWidth(20.0f);
        binding.v2d.getXAxis().setDrawLabels(false);
        binding.v2d.getAxisLeft().setDrawLabels(false);
        binding.v2d.getAxisRight().setDrawLabels(false);
        binding.v2d.getDescription().setEnabled(false);

        binding.acc2d.setData(new LineData());
        binding.acc2d.setBorderWidth(20.0f);
        binding.acc2d.getXAxis().setDrawLabels(false);
        binding.acc2d.getAxisLeft().setDrawLabels(false);
        binding.acc2d.getAxisRight().setDrawLabels(false);
        binding.acc2d.getDescription().setEnabled(false);

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

                Toast.makeText(getActivity(),"Calibration!",Toast.LENGTH_SHORT).show();
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
                }, 6000);


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

        AccViewModel.getLiveLineData(AccViewModel.OutTypes.velocity).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.v, lineDataSet);
//                AccViewModel.v.postValue(null);
//                accViewModel.getLiveLineData(AccViewModel.OutTypes.velocity).postValue(null);
            }
        });
//
        AccViewModel.getLiveLineData(AccViewModel.OutTypes.acceleration).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.acc, lineDataSet);
//                AccViewModel.a.postValue(null);
            }
        });

        AccViewModel.getLiveLineData(AccViewModel.OutTypes.velocity2dist).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.v2d, lineDataSet);
//                AccViewModel.v2d.postValue(null);
            }
        });

        AccViewModel.getLiveLineData(AccViewModel.OutTypes.acceleration2velocity).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.acc2v, lineDataSet);
//                AccViewModel.a2v.postValue(null);
            }
        });

        AccViewModel.getLiveLineData(AccViewModel.OutTypes.acceleration2dist).observe(getViewLifecycleOwner(), new Observer<LineDataSet>() {
            @Override
            public void onChanged(@NonNull LineDataSet lineDataSet) {
                setChart(binding.acc2d, lineDataSet);
//                AccViewModel.a2d.
//                AccViewModel.a2d.postValue(null);
            }
        });





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

    public void setLineData(double[] y, LineDataSet lineDataSet){
        for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++){
            lineDataSet.getEntryForIndex(ti).setY((float) y[ti]);
        }
    }

    // update chart
    public void setChart(LineChart chart, LineDataSet lineDataSet){
        if(lineDataSet != null){
            // check dataset
//            double ymean = 0.0;
//            double[] y = new double[lineDataSet.getEntryCount()];
//            for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++){
//                y[ti] = lineDataSet.getEntryForIndex(ti).getY();
//            }

            if(Objects.equals(lineDataSet.getLabel(), "Distance (SwiftTrack)")){
                double ymin = lineDataSet.getYMin();
                double ymax = lineDataSet.getYMax();
                int entry_count = lineDataSet.getEntryCount();
                double[] y = new double[entry_count];
                for(int ti = 0; ti < entry_count; ti++){
                    y[ti] = lineDataSet.getEntryForIndex(ti).getY();
                }

                // check abnormal flag
                if(ymax - ymin > 5){// if abnormal, set all values to zero
                    abnormal_flag = true;
                    for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++){
                        lineDataSet.getEntryForIndex(ti).setY((float) 0.0);
                    }
                }else{
                    abnormal_flag = false;
                    AudioProcessor.detrend(y);
//                    setLineData(y, lineDataSet);
                    for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++){
                        lineDataSet.getEntryForIndex(ti).setY((float) y[ti]);
                    }
//                    double a = 0.0;
//                    double b = 0.0;
//                    double ybar = 0.0;
//                    double xbar = y.length / 2;
//                    for(int ti = 0; ti < y.length; ti++){
//
//                    }

//                    ymean = ymean / lineDataSet.getEntryCount();
                }



            }else{ // if other datasets
                if(abnormal_flag){
                    for(int ti = 0; ti < lineDataSet.getEntryCount(); ti++) {
                        lineDataSet.getEntryForIndex(ti).setY((float) 0.0);
                    }
                }
            }


//            Log.d("ChartLabel", lineDataSet.getLabel());
            LineData lineData = chart.getLineData();
            if(lineData.getDataSetCount() == 0){
                lineData.addDataSet(lineDataSet);
            }
            else{
                for(int ti = 0; ti < lineData.getDataSetByIndex(0).getEntryCount(); ti++){
                    lineData.getDataSetByIndex(0).getEntryForIndex(ti).setY(lineDataSet.getEntryForIndex(ti).getY());
                }
            }
            lineData.getDataSetByIndex(0).calcMinMax();
            lineData.notifyDataChanged();
            chart.notifyDataSetChanged();
            chart.invalidate();
        }




    }
}
