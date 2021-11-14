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
        if(files == null) {
            Log.e("error", "空目录");
        }
        List<File> s = new ArrayList<>();
        for(int i = 0; i < files.length; i++){
            s.add(files[i]);
        }
        List<File> ss = sortFileByName(s, "asc");
        return ss.get(0).getAbsolutePath();
    }

    public static List<File> sortFileByName(List<File> files, final String orderStr) {
        if (!orderStr.equalsIgnoreCase("asc") && orderStr.equalsIgnoreCase("desc")) {
            return files;
        }
        File[] files1 = files.toArray(new File[0]);
        Arrays.sort(files1, new Comparator<File>() {
            public int compare(File o1, File o2) {
                int n1 = extractNumber(o1.getName());

                int n2 = extractNumber(o2.getName());
                if(orderStr == null || orderStr.length() < 1 || orderStr.equalsIgnoreCase("asc")) {
                    return n1 - n2;
                } else {
                    //降序
                    return n2 - n1;
                }
            }
        });
        return new ArrayList<File>(Arrays.asList(files1));
    }

    private static int extractNumber(String name) {
        int i;
        try {
            String regEx="[^0-9]";
            Pattern p = Pattern.compile(regEx);
            Matcher m = p.matcher(name);
            String number = m.replaceAll("").trim();

            i = Integer.parseInt(number);
            Log.d("files", number);
        } catch (Exception e) {
            i = 0;
        }


        return i;
    }




}