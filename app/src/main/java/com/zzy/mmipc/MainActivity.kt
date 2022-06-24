package com.zzy.mmipc

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
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
        val path: String = this.externalCacheDir!!.absolutePath + File.separator + "mmipc.arsc"
        Log.d("MMIPC->",path);
        NativeLib().initMMAP(path)
    }
}