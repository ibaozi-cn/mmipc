package com.zzy.mmipc

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.zzy.mmipc.ui.main.MainFragment
import java.util.concurrent.CountDownLatch

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)
        if (savedInstanceState == null) {
            supportFragmentManager.beginTransaction()
                .replace(R.id.container, MainFragment.newInstance())
                .commitNow()
        }
        startActivity(Intent(this, OtherProcessActivity::class.java))
    }

    override fun onResume() {
        super.onResume()
        testMultiThreadWriteMMIPC()
    }

    private fun testMultiThreadWriteMMIPC() {
        "set data".print(getProcessName())
        val countDownLatch = CountDownLatch(2)
        Thread {
            var index = 10000
            repeat(10000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
            countDownLatch.countDown()
        }.start()
        Thread {
            var index = 20000
            repeat(10000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
            countDownLatch.countDown()
        }.start()
        countDownLatch.await()
        MMIPC.getData("").length.toString().print(getProcessName())
    }

    fun startOtherProcessActivity(view: View) {
        startActivity(Intent(this, OtherProcessActivity::class.java))
    }

    override fun onRestart() {
        super.onRestart()
        MMIPC.getData("3", "4")
    }

}
