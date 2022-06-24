package com.zzy.mmipc

import android.app.Application
import android.util.Log
import java.io.File
import android.app.ActivityManager
import android.app.ActivityManager.RunningAppProcessInfo
import android.content.Context


class App : Application() {

    companion object {
        var mmipc: MMIPC = MMIPC()
    }

    override fun onCreate() {
        super.onCreate()
        val path: String = this.externalCacheDir!!.absolutePath + File.separator + "mmipc.arsc"
        "init".print(getProcessName())
        mmipc.initMMAP(path)
    }
}

fun String.print(processName: String = "com.zzy.mmipc") {
    Log.d("MMIPC->", "进程：$processName 日志： $this");
}

fun Context.getProcessName(): String {
    val am = this.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
    val runningApps = am.runningAppProcesses
    if (runningApps != null && !runningApps.isEmpty()) {
        for (procInfo in runningApps) {
            if (procInfo.pid == android.os.Process.myPid()) {
                return procInfo.processName
            }
        }
    }
    return ""
}