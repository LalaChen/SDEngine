package com.sdengine.unittest_featuredeveloping;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("sdengine_native");
    }

    public static native void InitializeApplication(AssetManager assetMgr);
    public static native void ChangeSurface(Surface surface, int format, int width, int height);
    public static native void Pause();
    public static native void TerminateApplication();
    public static native void onMotionEventReceived(MotionEvent event);

    public final String TAG = "SDE_AndroidActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG,"onCreate");
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

        //-------------- set on touch listener
        sv.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                //Log.i(TAG, "Touch event !!! Event action is " + (event.getAction() & MotionEvent.ACTION_MASK) + " touch count = " + event.getPointerCount());
                onMotionEventReceived(event);
                return true;
            }
        });

        InitializeApplication(getAssets());
    }

    @Override
    protected void onStart() {
        Log.i(TAG,"onStart");
        super.onStart();
    }

    @Override
    protected void onResume() {
        Log.i(TAG,"onResume");
        super.onResume();
    }

    @Override
    protected void onPause() {
        Log.i(TAG,"onPause");
        super.onPause();
        Pause();
    }

    @Override
    protected void onStop() {
        Log.i(TAG,"onStop");
        super.onStop();
    }

	@Override
    protected void onDestroy() {
        Log.i(TAG,"onDestroy");
        super.onDestroy();
		TerminateApplication();
    }
}
