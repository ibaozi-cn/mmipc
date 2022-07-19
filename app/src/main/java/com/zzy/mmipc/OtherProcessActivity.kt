package com.zzy.mmipc

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class OtherProcessActivity : AppCompatActivity() {
    var int = 100;
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_other_process)
        MMIPC.getData().print(getProcessName())
        Thread {
            var index = 60000
            repeat(2000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
        }.start()
        Thread {
            var index = 50000
            repeat(8000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
            MMIPC.getData("").length.toString().print()
        }.start()
    }
}