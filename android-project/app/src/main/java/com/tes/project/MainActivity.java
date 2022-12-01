package com.tes.project;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.tes.project.databinding.ActivityMainBinding;

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
    }

    /**
     * A native method that is implemented by the 'project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native String sendString(String string );

}