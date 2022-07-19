package com.zzy.mmipc

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import com.zzy.mmipc.ui.main.MainFragment
import java.io.File
import java.sql.Time
import java.util.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)
        if (savedInstanceState == null) {
            supportFragmentManager.beginTransaction()
                .replace(R.id.container, MainFragment.newInstance())
                .commitNow()
        }
        "set data".print(getProcessName())
        Thread {
            var index = 10000
            repeat(5000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
        }.start()
        Thread {
            var index = 20000
            repeat(5000) {
                index++
                MMIPC.setData(index.toString(), index.toString())
            }
        }.start()
        startActivity(Intent(this, OtherProcessActivity::class.java))
    }

    fun startOtherProcessActivity(view: View) {
        startActivity(Intent(this, OtherProcessActivity::class.java))
    }

    override fun onRestart() {
        super.onRestart()
        MMIPC.getData("3", "4")
    }

}
