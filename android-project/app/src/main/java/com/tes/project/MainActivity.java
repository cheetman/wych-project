package com.tes.project;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.os.Environment;
import android.os.Process;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.tes.project.databinding.ActivityMainBinding;
import com.tes.project.utils.WychUtils;

import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.OnNeverAskAgain;
import permissions.dispatcher.OnPermissionDenied;
import permissions.dispatcher.OnShowRationale;
import permissions.dispatcher.PermissionRequest;
import permissions.dispatcher.RuntimePermissions;

@RuntimePermissions
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


            MainActivityPermissionsDispatcher.showFileWithPermissionCheck(this);
            //showFile();



        });

    }


    @NeedsPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
    public void showFile(){




//        var ff =  Environment.getExternalStorageDirectory().getAbsolutePath();
//
//        var f  =   Environment.getExternalStorageDirectory().listFiles();


        Intent intent = new Intent(this, FilesActivity.class);
        startActivity(intent);

    }

    @OnShowRationale(Manifest.permission.READ_EXTERNAL_STORAGE)
    public void  showRationaleForStorage(PermissionRequest request) {
       new AlertDialog.Builder(this)
               .setMessage("提示用户为何开启此权限")
                       .setPositiveButton("知道了", new DialogInterface.OnClickListener() {
                           @Override
                           public void onClick(DialogInterface dialogInterface, int i) {
                               request.proceed();
                           }
                       }).show();

    }


    @OnPermissionDenied(Manifest.permission.READ_EXTERNAL_STORAGE)
    public void   onStorageDenied() {
        Toast.makeText(this, "fffff2",Toast.LENGTH_SHORT).show();
    }

    @OnNeverAskAgain(Manifest.permission.READ_EXTERNAL_STORAGE)
    public void   onStorageNeverAskAgain() {
        Toast.makeText(this, "fffff3", Toast.LENGTH_SHORT).show();
    }


    @Override
    public  void onRequestPermissionsResult(int requestCode, String[] permissions,int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        // NOTE: delegate the permission handling to generated function
        MainActivityPermissionsDispatcher.onRequestPermissionsResult(this,requestCode,grantResults);
    }


    /**
     * A native method that is implemented by the 'project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native String sendString(String string ,int a);

}