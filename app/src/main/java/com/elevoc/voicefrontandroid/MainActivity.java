/*
 * Copyright (C) 2018 Baidu, Inc. All Rights Reserved.
 */
package com.elevoc.voicefrontandroid;

//import com.vondear.rxtools.RxPermissionsTool;

import android.Manifest;
import android.app.Activity;
import android.os.Build;
import android.os.Environment;
//import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentActivity;

import com.hjq.permissions.OnPermissionCallback;
import com.hjq.permissions.Permission;
import com.hjq.permissions.XXPermissions;

import java.util.ArrayList;
import java.util.List;

//import com.tbruyelle.rxpermissions3.RxPermissions;

public class MainActivity extends FragmentActivity implements View.OnClickListener {

    private final String appid = "your_appid";
    private final String appkey = "your_key";

    private Button btn = null;
//    RxPermissions rxPermissions = null;


    private String WAV_DIR = Environment.getExternalStorageDirectory().getAbsolutePath() +
//            "/elevoc_putian_demo/";
            "/elevoc_qingtianshu_demo/";

    private final String[] permission = {Manifest.permission.RECORD_AUDIO,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};
    static {
        System.loadLibrary("elevoc_single_rn");
        System.loadLibrary("elevoc_test");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        rxPermissions = new RxPermissions(this);
//        rxPermissions.setLogging(true);
        btn = (Button)findViewById(R.id.btn);
        btn.setOnClickListener(this);
        initPermission();
    }

    private void initPermission() {
        List<String > permissionList =new ArrayList<>();
        permissionList.add(Permission.RECORD_AUDIO);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            permissionList.add(Permission.MANAGE_EXTERNAL_STORAGE);
        } else {
            permissionList.add(Permission.READ_EXTERNAL_STORAGE);
            permissionList.add(Permission.WRITE_EXTERNAL_STORAGE);
        }
        XXPermissions.with(this)
                .permission(permissionList)
                .request(new OnPermissionCallback() {
                    @Override
                    public void onGranted(List<String> permissions, boolean all) {
                        Toast.makeText(MainActivity.this,"授权"+all,1).show();
                    }
                });
    }


    @Override
    public void onClick(View v) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                start_process(appid, appkey, WAV_DIR);
            }
        }).start();
    }

    public native void start_process(String appid, String appkey, String wav_path);

}
