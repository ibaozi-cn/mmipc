package com.zzy.mmipc

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import java.util.concurrent.CountDownLatch

class OtherProcessActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_other_process)
        testMMIPC()
    }

    private fun testMMIPC() {
//        MMIPC.getData().print(getProcessName())
        val countDownLatch = CountDownLatch(2)
        Thread {
            var index = 30000
            repeat(30000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
            countDownLatch.countDown()
        }.start()
        Thread {
            var index = 40000
            repeat(30000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
            countDownLatch.countDown()
        }.start()
        countDownLatch.await()
        Handler(Looper.getMainLooper()).postDelayed({
            MMIPC.getData("").length.toString().print(getProcessName())
        }, 5000)
    }
}