## C/C++运行期库的考虑

Visual C++配有6个C/C++运行期库。

- LibC.lib		用于单线程应用程序的静态链接库的发行版本

- LibCD.lib        用于单线程应用程序的静态链接库的调试版本

- LibCMt.lib      用于多线程应用程序的静态链接库的发行版本

- LibCMtD.lib   用于多线程应用程序的静态链接库的调试版本

- MSVCRt.lib    用于动态链接库MSVCRt.dll库的发行版的输入库

- MSVCRtD.lib 用于动态链接库MSVCRtD.dll的调试版的输入库。支持多线程和单线程

  ​

在多线程环境中存在问题的C/C++运行期库变量和函数有好几个...

当调用C/C++运行期库时，这些库函数必须能够查看线程的数据块，每个线程需要关联必要的数据块

，为此，若要创建一个新线程，不要使用操作系统的CreateThread函数，必须调用C/C++运行期库函数_beginthreadex:

```c++
unsigned long _beginthreadex(

void*	security,
 unsigned stack_size,
  unsigned (*start_address)(void*),
  void*	arglist,
  unsigned initflag,
  unsigned*	thrdaddr
);
// unsigned long 相当于句柄
// 参数和CreateThread相同  类型不依赖于Windows系统 因此需要一定的类型转换
// _beginthreadex只存在于C/C++运行期库多线程版本中
// 相对于CraeteThread 增加了绑定数据块
```

关于_beiginthreadex的要点：

1. 每个线程均获得由C/C++运行期库的堆栈分配的自己的tiddata内存结构（数据块）（位于Mtdall.h）
2. 传递给_beginthreadex的线程函数的地址保存在tiddata内存块中。_传递给该__beginthreadex函数的参数也在该数据块中。
3. _beginthreadex确释也从内部调用了CreateThread
4. 当内部调用CraeteThread时，它被告知通过调用_threadstartex而不是pfnStartAddr来启动执行新线程。还有，传递给线程函数的参数是tiddata结构而不是pvParam的地址。
5. 如果一切顺利，就会像CreateThread那样返回线程句柄。如果失败返回NULL


既然为新线程指定了tiddata结构，并且对该结构进行了初始化，那么我们需要了解一下该结构与线程之间是如何关联起来的。

关于_threadstartex的一些重点:

1. 新线程开始从BaseThreadStart执行，然后转到_threadstartex
2. tiddata块的地址作为唯一参数传递给_threadstartex
3. TlsSetValue是个操作系统函数，负责将一个值与调用线程关联起来。称之为线程本地存储器(TLS),_threadstartex将tiddata与线程关联起来。
4. 一个SEH帧被放置在需要的线程函数周围，这个帧负责处理与运行期库相关的许多事情，例如运行期错误（未捕获的C++异常）和C/C++运行期库的signal函数。
5. 调用必要的线程函数，传递必要的参数，函数和参数地址都在_beignthreadex中的数据块保存。
6. 最后是调用_endthreadex撤销tiddata结构传递退出代码



关于_endthreadex的一些要点：

1. C运行期库的_getptd函数内部调用TlsGetValue函数，该函数检索调用线程的tiddata内存地址

2. 然后该数据块被释放，操作系统的ExitThread函数被调用，以便真正撤销该线程。

   ​

**_beginthreadex调用------------线程获得tidata数据块并初始化**

**CreateThread被调用，线程内核对象被初始化---------上下文，堆栈，计数等设置**

**BaseThreadStart被启动-------现在不是调用线程函数了，而是调用_threadstartex**

**_threadstartex被调用-----------操作系统线程本地存储器将tiddata与线程关联起来**

**_endthreadex被调用----------tiddata数据块被撤销**

**ExitThread被调用---------线程终止运行**





C/C++运行期库的启动代码为应用程序的主线程分配了数据块，并且对数据块进行了初始化，这样，主线程就能安全地调用C/C++运行期函数中的任何函数。当主线程从他的进入点函数返回时，C/C++运行期库就会释放相关的数据块。此外，启动代码设置了相应的结构话异常处理代码，以便主线程能够成功的调用C/C++运行期库的signal函数。



不应该使用的C/C++运行期库函数

_beginthread(...)

_endthread(...)