package com.tes.project;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.tes.infrastructure.utils.ToastUtils;
import com.tes.project.adapters.RvFilesAdapter;
import com.tes.project.databinding.ActivityFilesBinding;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

public class FilesActivity extends AppCompatActivity {



    private ActivityFilesBinding binding;

    private RvFilesAdapter rvAdapter;

    //    private File[] files;
    private List<File> files2 = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityFilesBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());



        binding.rvFiles.setLayoutManager(new LinearLayoutManager(this));
        binding.rvFiles.setItemAnimator(new DefaultItemAnimator());
        binding.rvFiles.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));


        rvAdapter = new RvFilesAdapter(this);
        binding.rvFiles.setAdapter(rvAdapter);

        rvAdapter.setOnItemClickListener(new RvFilesAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                var file = files2.get(position);
                ToastUtils.showShort(position +"" + file.getName());
                if(file.isDirectory()){
                    show(file);
                }
            }

            @Override
            public void onItemLongClick(View view, int position) {

            }
        });

        show(null);
    }

    private void show(File file){
        // 外部存储

        if(file == null){

            File[]  files  =   Environment.getExternalStorageDirectory().listFiles();
            if(files != null){
                files2 = Arrays.stream(files).sorted(Comparator.comparing(File::getName)).collect(Collectors.toList());
            }
        }else{
            files2 = Arrays.stream(file.listFiles()).sorted(Comparator.comparing(File::getName)).collect(Collectors.toList());
        }

        rvAdapter.setList(files2);



    }



}
