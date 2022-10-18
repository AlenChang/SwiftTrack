package com.example.swifttrack.ui.setting;

import androidx.lifecycle.ViewModelProvider;

import android.content.Context;
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
import com.example.swifttrack.MainActivity;
import com.example.swifttrack.R;
import com.example.swifttrack.databinding.SettingFragmentBinding;
import com.example.swifttrack.ui.home.HomeFragment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
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

        binding.switch2.setChecked(MainActivity.USE_FILE);
        binding.switch3.setChecked(MainActivity.CHANNEL_MASK[0]);
        binding.switch4.setChecked(MainActivity.CHANNEL_MASK[1]);
        binding.editTextNumberDecimal.setHint(" " + MainActivity.N_ZC_UP);
//        binding.editTextNumberDecimal.setText(MainActivity.N_ZC_UP);
//        binding.switch5.setChecked(AudioPlayer.CHANNEL_MASK[0]);
//        binding.switch6.setChecked(AudioPlayer.CHANNEL_MASK == 0x02);



        binding.switch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    MainActivity.USE_FILE = true;
                    listAssetFiles("");
//                    final String FILE_NAME = "new_seq.txt";
                    Log.d("USE_FILE", getActivity().getExternalFilesDir(null).toString());
                    final String FILE_NAME;
                    try {
                        FILE_NAME = assetFilePath(getActivity().getApplicationContext(), "new_seq.txt");
                        Log.d("USE_FILE", FILE_NAME);
                        AudioRecorder.setFileName(FILE_NAME);
//                        final String files = getFileName(getActivity().getApplicationContext().getFilesDir());
                    } catch (IOException e) {
                        Log.d("USE_FILE", "File not find.");
                        e.printStackTrace();
                    }

                } else {
                    MainActivity.USE_FILE = false;
                }
            }
        });

        binding.switch3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    MainActivity.CHANNEL_MASK[0] = true;
                    MainActivity.CHANNEL_MASK[1] = false;
                    binding.switch4.setChecked(false);
                } else {
                    MainActivity.CHANNEL_MASK[0] = false;
                    MainActivity.CHANNEL_MASK[1] = true;
                    binding.switch4.setChecked(true);
                }
            }
        });

        binding.switch4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    MainActivity.CHANNEL_MASK[0] = false;
                    MainActivity.CHANNEL_MASK[1] = true;
                    binding.switch3.setChecked(false);
                } else {
                    MainActivity.CHANNEL_MASK[0] = true;
                    MainActivity.CHANNEL_MASK[1] = false;
                    binding.switch3.setChecked(true);
                }
            }
        });

        binding.nZcUpSaveButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                String text = binding.editTextNumberDecimal.getText().toString();
                if(!text.isEmpty()){
                    int N_ZC_UP = Integer.parseInt(text);
                    Log.d("test_edit_number", text + " " + N_ZC_UP);
                    if(N_ZC_UP % 24 == 0 && N_ZC_UP > 48 && N_ZC_UP <=4000){
                        MainActivity.N_ZC_UP = N_ZC_UP;
                        MainActivity.N_ZC = MainActivity.N_ZC_UP * MainActivity.BW / MainActivity.FS - 1;
                        binding.editTextNumberDecimal.setHint(" " + MainActivity.N_ZC_UP);
                        Log.d("test_edit_number", "Sequence length is " + MainActivity.N_ZC_UP);
                    }
                }

            }
        });

//        binding.switch5.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
//            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                if (isChecked) {
//                    AudioPlayer.CHANNEL_MASK[0] = true;
//                    AudioPlayer.CHANNEL_MASK[1] = false;
////                    binding.switch6.setChecked(false);
//                } else {
//                    AudioPlayer.CHANNEL_MASK[1] = true;
//                    AudioPlayer.CHANNEL_MASK[0] = false;
////                    binding.switch6.setChecked(true);
//                }
//            }
//        });

//        binding.switch6.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
//            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                if (isChecked) {
//                    AudioPlayer.CHANNEL_MASK = 0x02;
//                    binding.switch5.setChecked(false);
//                } else {
//                    AudioPlayer.CHANNEL_MASK = 0x01;
//                    binding.switch5.setChecked(true);
//                }
//            }
//        });

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

    private String assetFilePath(Context context, String assetName) throws IOException {
        File file = new File(context.getFilesDir(), assetName);
        if (file.exists() && file.length() > 0) {
            return file.getAbsolutePath();
        }

        try (InputStream is = context.getAssets().open(assetName)) {
            try (OutputStream os = new FileOutputStream(file)) {
                byte[] buffer = new byte[4 * 1024];
                int read;
                while ((read = is.read(buffer)) != -1) {
                    os.write(buffer, 0, read);
                }
                os.flush();
            }
            return file.getAbsolutePath();
        }
    }

    private boolean listAssetFiles(String path) {

        String [] list;
        try {
            list = getActivity().getAssets().list(path);
            if (list.length > 0) {
                // This is a folder
                for (String file : list) {
                    if (!listAssetFiles(path + "/" + file))
                        return false;
                    else {
                        // This is a file
                        // TODO: add file name to an array list
                        Log.d("LIST_FILE", file);
                    }
                }
            }
        } catch (IOException e) {
            return false;
        }

        return true;
    }







}