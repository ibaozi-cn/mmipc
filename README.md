## mmipc
使用mmap实现的跨进程通信组件，也可用于写入数据较多的场景。

## 背景
说的跨进程通信，你可能对Binder有着莫名的崇拜之情，因为它几乎贯穿了整个Android系统的跨进程通信任务，如果你探究到Binder是通过mmap实现的时候，你是否跟我一样，想用mmap实现一个跨进程组件，这就是
我创建该项目的目的，对于你来说，这是一个不错的学习mmap的开源项目。

## 对比Binder跨进程

优势：
- 通信数据大小突破binder限制
- 轻量级，易用

缺点：
- 安全，目前未考虑安全性，后续可以加进来，比如对数据加密，对外部进程访问加入验证规则
- 

## mmap原理



## 实现Android跨进程通信

### 用例

初始化
```kotlin

class App : Application() {

    companion object {
        var mmipc: MMIPC = MMIPC()
    }

    override fun onCreate() {
        super.onCreate()
        val path: String = this.externalCacheDir!!.absolutePath + File.separator + "mmipc.arsc"
        "init".print(getProcessName())
        mmipc.initMMAP(path)
    }
}
```
保存数据
```kotlin
   App.mmipc.setData("1", "2")
   App.mmipc.setData("3", "4")
```
获取数据，不分进程
```kotlin
    App.mmipc.getData()
    App.mmipc.getData("key")
    App.mmipc.getData("key","defaultValue")
```

### 部门源码分析
待补

## 参考
<https://github.com/Tencent/MMKV>
