package com.zzy.mmipc

import android.app.Application
import android.util.Log
import android.app.ActivityManager
import android.content.Context
import android.os.Build
import java.io.File
import java.nio.file.Files
import kotlin.io.path.Path


class App : Application() {

    override fun onCreate() {
        super.onCreate()
        "init".print(getProcessName())
        MMIPC.initMMAP(this)
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