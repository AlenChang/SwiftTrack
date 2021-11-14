package com.example.swifttrack.ui.setting;

import androidx.lifecycle.ViewModelProvider;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;


import com.example.swifttrack.AudioPlayer;
import com.example.swifttrack.AudioRecorder;
import com.example.swifttrack.R;
import com.example.swifttrack.databinding.SettingFragmentBinding;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class SettingFragment extends Fragment {

    private SettingViewModel mViewModel;
    private SettingFragmentBinding binding;

    public static SettingFragment newInstance() {
        return new SettingFragment();
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {

        mViewModel =
                new ViewModelProvider(this).get(SettingViewModel.class);
        binding = SettingFragmentBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        binding.switch2.setChecked(AudioPlayer.USE_FILE);


        binding.switch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    AudioPlayer.USE_FILE = true;
                    AudioRecorder.setFileName(getFileName("recorder/"));
                    Log.d("files", getFileName("recorder/"));
                } else {
                    AudioPlayer.USE_FILE = false;
                }
            }
        });

        return root;
    }

    public String getFileName(String path){
        File file = new File(getActivity().getExternalFilesDir(null), path);
        File[] files = file.listFiles();
        int counter = 0;
        long lastTime = 0;
        if(files == null) {
            Log.e("error", "空目录");
        }

        for(int i = 0; i < files.length; i++){
            if (files[i].lastModified() > lastTime){
                lastTime = files[i].lastModified();
                counter = i;
            }
        }

        return files[counter].getName();
    }







}