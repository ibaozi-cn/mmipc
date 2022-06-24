package com.zzy.mmipc

class MMIPC {

    /**
     * A native method that is implemented by the 'mmipc' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun initMMAP(rootDir: String)
    external fun setData(key: String, value: String = "")
    external fun getData(key: String = "", defaultValue: String = ""): String

    companion object {
        // Used to load the 'mmipc' library on application startup.
        init {
            System.loadLibrary("mmipc")
        }
    }
}