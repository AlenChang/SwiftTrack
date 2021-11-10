package com.example.swifttrack.ui.setting;

import androidx.lifecycle.ViewModelProvider;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;


import com.example.swifttrack.R;
import com.example.swifttrack.databinding.SettingFragmentBinding;


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

        String courseList[] = {"C-Programming", "Data Structure", "Database", "Python",
                "Java", "Operating System", "Compiler Design", "Android Development"};

        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getActivity(),
                R.layout.item_view, R.id.itemTextView1, courseList);

        ArrayAdapter<String> arrayAdapter2 = new ArrayAdapter<String>(getActivity(),
                R.layout.item_view, R.id.itemTextView2, courseList);
        binding.listView.setAdapter(arrayAdapter);
//        binding.listView.setAdapter(arrayAdapter2);

        return root;
    }



}