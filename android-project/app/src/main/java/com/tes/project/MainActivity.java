package com.tes.project;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.os.Process;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.tes.project.databinding.ActivityMainBinding;
import com.tes.project.utils.WychUtils;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'project' library on application startup.
    static {
        System.loadLibrary("project");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        // crc 校验
        String md5 =  WychUtils.getMD5(String.valueOf( WychUtils.getApkCRC(getApplicationContext())));
        if(!md5.equals(getString(R.string.classes_txt))){
            // crc32.exe classes.dex -> 0x72000000
            // 注意重打包
            // 校验
            //Process.killProcess(Process.myPid());
        }

        // 签名校验
        String md52 =  WychUtils.getMD5(String.valueOf( WychUtils.getSignature(getApplicationContext())));
        Toast toast = Toast.makeText(getApplicationContext(),md52, Toast.LENGTH_LONG);
        toast.show();
        if(!md52.equals("99b95b48e489b9c38d01430c0ebc0bbb")){
            // crc32.exe classes.dex -> 0x72000000
            // 注意重打包
            // 校验
            //Process.killProcess(Process.myPid());
        }
        if(1==2){
            toast.show();
        }
//        toast.show();




        // init
        binding.btnFiles.setOnClickListener(view -> {

            Intent intent = new Intent(this, FilesActivity.class);
            startActivity(intent);


        });

    }

    /**
     * A native method that is implemented by the 'project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native String sendString(String string ,int a);

}