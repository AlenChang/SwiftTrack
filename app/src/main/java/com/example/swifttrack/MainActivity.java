package com.example.swifttrack;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;
import com.google.android.material.navigation.NavigationView;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.appcompat.app.AppCompatActivity;

import com.example.swifttrack.databinding.ActivityMainBinding;

import com.example.swifttrack.utils.FileUtil;
import java.lang.Math;

import java.util.concurrent.ArrayBlockingQueue;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration mAppBarConfiguration;
    private ActivityMainBinding binding;

    // tunable parameters
    public static boolean[] CHANNEL_MASK = {true, false}; // for processor
    public static final boolean[] SPEAKER_CHANNEL_MAKS = {false, true};
    public static boolean USE_FILE = false;
    public static int N_ZC_UP = 960;
    public static int FS = 48000;
    public static int FC = 20000;
    public static int BW = 4000;
    public static int N_ZC = N_ZC_UP * BW / FS - 1;
    public static int ZC_ROOT = (N_ZC - 1) / 2;
    public static int DOWN_SAMPLE_FACTOR = N_ZC_UP;





    // ******************************
    // required permissions
    // ******************************
    public static final String[] PERMISSIONS = {
            Manifest.permission.WAKE_LOCK,
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.READ_EXTERNAL_STORAGE
    };






    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setupPermission();

        showSystemParameter();

        if(Build.MODEL.equals("GM1910")){
            CHANNEL_MASK = new boolean[]{false, true};
        }
        if(Build.MODEL.equals("Redmi K20 Pro Premium Edition")){
            CHANNEL_MASK = new boolean[]{true, false};
        }
        if(Build.MODEL.equals("Pixel 3")){
            CHANNEL_MASK = new boolean[]{true, false};
        }


        FileUtil.init(this);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.appBarMain.toolbar);
//        binding.appBarMain.fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });
        DrawerLayout drawer = binding.drawerLayout;
        NavigationView navigationView = binding.navView;
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        mAppBarConfiguration = new AppBarConfiguration.Builder(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow, R.id.nav_acc, R.id.nav_ml, R.id.nav_lstm, R.id.nav_setting)
                .setOpenableLayout(drawer)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        NavigationUI.setupActionBarWithNavController(this, navController, mAppBarConfiguration);
        NavigationUI.setupWithNavController(navigationView, navController);


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
//        NavController navController = Navigation.findNavController(this, R.id.acc);
        return NavigationUI.navigateUp(navController, mAppBarConfiguration)
                || super.onSupportNavigateUp();
    }

    // ******************************
    // button actions
    // ******************************
    public void start(View view) {
        Log.i("button", "press start");
    }
    public void stop(View view) {
        Log.i("button", "press stop");
    }
    public void reset(View view) {
        Log.i("button", "press reset");
    }
    public void save(View view) {
        Log.i("button", "press save");
    }

    // ******************************
    // setup permissions
    // ******************************
    private void setupPermission() {
        while (getPermission() != PackageManager.PERMISSION_GRANTED) {
            setupPermission();
        }
    }

    private int getPermission() {
        ActivityCompat.requestPermissions(this, PERMISSIONS, 1);

        for (String permission : PERMISSIONS) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                return PackageManager.PERMISSION_DENIED;
            }
        }

        return PackageManager.PERMISSION_GRANTED;
    }

    public void setToast(CharSequence text){
        Context context = getApplicationContext();
//        CharSequence text = "Hello toast!";
        int duration = Toast.LENGTH_SHORT;

        Toast.makeText(context, text, duration).show();
    }

    public void showSystemParameter(){
        String TAG = "系统参数： ";
        Log.e(TAG, "手机型号： " + Build.MODEL);
    }

}