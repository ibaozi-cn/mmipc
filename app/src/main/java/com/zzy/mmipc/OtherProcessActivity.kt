package com.zzy.mmipc

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class OtherProcessActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_other_process)
        App.mmipc.getData().print(getProcessName())
    }
}