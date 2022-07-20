package com.zzy.mmipc

import android.app.Application
import android.os.Handler
import android.os.HandlerThread
import java.lang.Exception

class MMIPC private constructor() {

    @Throws(Exception::class)
    private external fun initMMAPJNI(rootDir: String)

    @Throws(Exception::class)
    private external fun setDataJNI(key: String, value: String = "")

    @Throws(Exception::class)
    private external fun getDataJNI(key: String = "", defaultValue: String = ""): String

    companion object {
        // Used to load the 'mmipc' library on application startup.
        val mmipc = MMIPC()

        private val handlerThread by lazy {
            HandlerThread("THREAD-MMIPC").apply { start() }
        }

        private val workHandler by lazy {
            Handler(handlerThread.looper)
        }

        init {
            System.loadLibrary("mmipc")
        }

        fun initMMAP(appContext: Application) {
            val path: String = appContext.filesDir.absolutePath
            try {
                mmipc.initMMAPJNI(path)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        fun setData(key: String, value: String = "") {
            try {
                workHandler.post {
                    mmipc.setDataJNI(key, value)
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        fun getData(key: String = "", defaultValue: String = ""): String {
            try {
                return mmipc.getDataJNI(key, defaultValue)
            } catch (e: Exception) {
                e.printStackTrace()
            }
            return defaultValue
        }
    }
}