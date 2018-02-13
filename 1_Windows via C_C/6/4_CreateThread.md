## CreateThread

让已经运行的线程来调用CreateThread:

```c++
HANDLE	CreateThread(
	PSECURITY_ATTRIBUTES psa, // 安全属性描述
  	DWORD	cbStack,
  	PTHREAD_START_ROUTINE	pfnStartAddr,
  	PVOID	pvParam,
  	DWORD	fdwCreate,
  	PDWORD	pdwThreadID
);
/*
	当CreateThread被调用时，系统创建一个线程内核对象，系统从进程的地址空间中分配内存供线程的堆栈使用，新线程运行的进程环境和创建线程的环境相同。
	因此，新线程可以访问进程的内核对象的所有句柄、进程中的所有内存、相同进程中的所有其他线程的堆栈。
	这使得多个线程可以非常容易的进行通信。
*/
```

编译器应该有自己的替代函数来替代CreateThread，如Microsoft的_beginthreadex。



### 1、pas

默认安全则设置为NULL

希望所有子进程都能继承该线程内核对象的句柄则需要指定结构成员bInheritHandle为TRUE



### 2、cbStack

cbStack用于设定线程使用多少进程的地址空间用于自己的堆栈。

每个线程拥有自己的堆栈。



当CreateProcess启动一个进程时，它在内部调用CreateThread对进程的主线程进行初始化，对于cbStack来说，CreateProcess使用存放在可执行文件中的一个值，可以在链接开关中的/STACK中指定：

/STACK:reserve][,commit]

默认是1MB

commit是用于堆栈保留区的物理存储器的容量，默认是一页。



调用CreateThread时，如果传递的值不是0，就能使该函数将所有的存储器保留并分配给线程的堆栈。

由于所有的存储器都先作了分配，因此可以确保线程拥有指定容量的可用堆栈存储器。



### 3、pfnStartAddr和pvParam

指明新线程执行的线程函数的地址，线程函数的pvParam参数与传递给CreatThread的pvParam参数是相同的。CreateThread使用该参数只是当线程启动时将参数传递给线程函数而已。

创建多个线程，使这些线程拥有与起始点相同的函数地址。

线程共享问题需要使用真正的线程同步技术（8，9，10将介绍）



### 4、fdwCreate

如果是0就马上进行调度。

如果是CREATE_SUSPENDED就完成初始化后暂停线程的运行，这样就无法进行调度。

这样做是为了有机会执行代码之前修改线程的某些属性。



### 5、pdwThreadID

存储新线程的ID

