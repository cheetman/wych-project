package com.tes.project;

import android.os.Bundle;
import android.os.Environment;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.tes.project.adapters.RvFilesAdapter;
import com.tes.project.databinding.ActivityFilesBinding;

import java.util.ArrayList;
import java.util.List;

public class FilesActivity extends AppCompatActivity {



    private ActivityFilesBinding binding;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityFilesBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());



        binding.rvFiles.setLayoutManager(new LinearLayoutManager(this));
        binding.rvFiles.setItemAnimator(new DefaultItemAnimator());
        binding.rvFiles.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));

        List<String> list = new ArrayList<>();
        list.add("sss");
        list.add("sss2");
        RvFilesAdapter rvAdapter = new RvFilesAdapter(this,list);
        binding.rvFiles.setAdapter(rvAdapter);





    }

}
