package com.sdengine.unittest_graphicsapi;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    public final String TAG = "SDE_AndroidActivity";

    static {
        System.loadLibrary("sdengine_native");
    }

    public static native void InitializeApplication(AssetManager assetMgr);
    public static native void ChangeSurface(Surface surface, int format, int width, int height);
    public static native void Pause();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG,"onCreate ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SurfaceView sv = (SurfaceView)findViewById(R.id.surfaceView);
        //-------------- set on surface callback
        sv.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Log.i(TAG,"surfaceCreated");
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.i(TAG,"surfaceChanged ");
                ChangeSurface(holder.getSurface(), format, width, height);
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.i(TAG,"surfaceDestroyed ");
            }
        });

        //-------------- set on touch linter
        sv.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                Log.i(TAG, "Touch event !!! Event action is " + (event.getAction() & MotionEvent.ACTION_MASK) + " touch count = " + event.getPointerCount());
                for (int touchID = 0; touchID < event.getPointerCount(); touchID++) {
                    Log.i(TAG, "T(" + event.getPointerId(touchID) + ")(" + event.getX(touchID) + "," + event.getY(touchID) + ")");
                }
                return true;
            }
        });

        InitializeApplication(getAssets());
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
        Pause();
    }

    @Override
    protected void onStop() {
        Log.i(TAG,"onStop ");
        super.onStop();
    }
}