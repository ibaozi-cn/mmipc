## mmipc

纯手写的mmap进程间通信组件

## 背景

说到跨进程通信，你可能对Binder有着莫名的崇拜之情，因为它几乎贯穿了整个Android系统的跨进程通信任务，如果你探究到Binder是通过mmap实现的时候，你是否跟我一样，想用mmap实现一个跨进程组件，这就是
我创建该项目的目的，对于你来说，这是一个不错的学习mmap的开源项目。

## 对比Binder

优势：
- 通信数据大小突破binder限制
- 轻量级，易用

缺点：
- 安全，目前未考虑安全性，后续可以加进来，比如对数据加密，对外部进程访问加入验证规则

进行中：
 - 跨线程、跨进程同步，已完成
 - pb+加密，doing

## Demo用例

初始化
```kotlin

class App : Application() {

    override fun onCreate() {
        super.onCreate()
        "init".print(getProcessName())
        MMIPC.initMMAP(this)
    }
}
```
保存数据
```kotlin
   MMIPC.setData("1", "2")
   MMIPC.setData("3", "4")
```
获取数据，不分进程
```kotlin
    MMIPC.getData()
    MMIPC.getData("key")
    MMIPC.getData("key","defaultValue")
```

## 进程间系列文章

该文章讲的很细，也有补充知识，推荐查看。

> https://www.yuque.com/ibaozi/dhzi9k

核心实现：<https://github.com/ibaozi-cn/mmipc/tree/main/mmipc/src/main/cpp>  

欢迎加微信交流
![img.png](ibaozi_cn.png)

## 组织

[官网](http://jetpack.net.cn)

## 参考
<https://github.com/Tencent/MMKV>  
<https://mp.weixin.qq.com/s/YjwxeLCwvJxXn7HPOF4Ayw>  
<https://mp.weixin.qq.com/s/xTKQjb45gzRIUjKzSBOKsg>
