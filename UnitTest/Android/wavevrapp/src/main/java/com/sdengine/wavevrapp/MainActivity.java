package com.sdengine.wavevrapp;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;

import com.htc.vr.sdk.VRActivity;

public class MainActivity extends VRActivity {

    public final String TAG = "SDWaveActivity";

    static {
        System.loadLibrary("sdengine_native");
    }

    public static native void InitializeApplication(AssetManager assetMgr);
    public static native void Resume();
    public static native void Pause();
    public static native void TerminateApplication();

    public MainActivity() {
    }

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        InitializeApplication(getResources().getAssets());
    }

    @Override
    protected void onResume() {
        super.onResume();
        Resume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        Pause();
    }

    @Override
    protected void onDestroy() {
        TerminateApplication();
        super.onDestroy();
        Log.i(TAG, "onDestory");
    }
}
