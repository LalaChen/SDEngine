package com.sdengine.unittest_androidapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    public final String TAG = "SDE_AndroidActivity";

    static {
        System.loadLibrary("sdengine_native");
    }

    public native void InitializeNative(AssetManager assetMgr, Surface surface, int format, int w, int h);

    //------------------------ Interface about surfaceHolder.callback
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG,"surfaceCreated");
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.i(TAG,"surfaceChanged ");
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG,"surfaceDestroyed ");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG,"onCreate ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView sv = (SurfaceView)findViewById(R.id.surfaceView);
        sv.getHolder().addCallback(this);
    }

    @Override
    protected void onStart() {
        Log.i(TAG,"onStart ");
        super.onStart();
    }

    @Override
    protected void onResume() {
        Log.i(TAG,"onResume ");
        super.onResume();
    }

    @Override
    protected void onPause() {
        Log.i(TAG,"onPause ");
        super.onPause();
    }

    @Override
    protected void onStop() {
        Log.i(TAG,"onStop ");
        super.onStop();
    }
}
