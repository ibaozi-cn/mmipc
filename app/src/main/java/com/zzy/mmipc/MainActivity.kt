package com.zzy.mmipc

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import com.zzy.mmipc.ui.main.MainFragment
import java.io.File

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
        App.mmipc.setData("1", "2")
        App.mmipc.setData("3", "4")
    }

    fun startOtherProcessActivity(view: View) {
        startActivity(Intent(this, OtherProcessActivity::class.java))
    }

}
