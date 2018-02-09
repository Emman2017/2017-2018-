# CreateProcess函数

```c++
BOOL	CreateProcess（
  PCTSTR	pszApplicationName，
  PTSTR		pszCommandLine，
  PSECURITY_ATTRIBUTE	psaProcess,
  PSECURITY_ATTRIBUTE	psaThread,
  BOOL		bInheritHandles,
  DWORD		fdwCreate,
  PVOID		pvEnvironment,
  PCTSTR	pszCurDir,
  PSTARTUPINFO			psiStartInfo,
  PPROCESS_INFORMATION	ppiProcInfo
); 
```

**使用该函数产生的固有效果**：创建进程内核对象，使用计数为1

**注意**：该进程对象不是进程本身，而是操作系统管理进程时使用的一个较小的数据结构（进程统计信息组成的较小数据结构）



1. 系统为该新进程创建一个虚拟地址空间，并将可执行文件或任何必要的DLL文件的代码和数据加载到该进程的地址空间。

2. 然后系统为新进程创建一个线程内核对象（使用计数为1）。与进程内核对象一样，线程内核对象也是操作系统用来管理线程的小型数据结构，执行C/C++运行期启动代码，主线程开始运行，最终调用了入口点函数，如果成功创建了新进程和主线程，CreateProcess便返回TRUE。

   ​

> 注意

在进程被完全初始化之前，CreateProcess返回TRUE。

如果是这样的话，操作系统加载程序还没找出所有的所需DLL，如果一个找不到，或者未能正确的进行初始化，那么该进程就终止运行，由于CreateProcess返回TRUE，因此，父进程不知道任何初始化问题！！



### 各个参数的介绍

#### pszApplicationName和pszCommandLine

分别用于设置可执行文件的名字和传递给新进程的命令行字符串

->	关于pszCommandLine

**pszApplicationName设置为NULL的情况下**

当CreateProcess函数分析该命令行参数是，它将查看字符串的第一个标记，并假设该标记是想运行的可执行文件的名字，没有扩展名则假设为.exe。如果非全路径则进行按顺序查找可执行文件：

1）包含调用进程的.exe文件目录

2）调用进程的当前目录

3）Windows的系统目录

4）Windows目录

5）PATH环境变量中列出的目录

一旦找到该可执行文件，就创建一个新进程，将可执行文件的代码和数据映射到新进程的地址空间中。然后系统系统C/C++运行期函数启动例程

**pszAllicationName设置限制**：全名（包括扩展名）、只在当前目录中查找（找不到则运行失败）



#### psaProcess、psaThread、binheritHandles

创建新进程，意味着创建进程内核对象和一个线程内核对象（主线程）

psaProcess设置进程内核对象安全性

pszThread设置线程内核对象安全性

binheritHandles继承父进程的可继承句柄



#### fdwCreate

用于标识标志，以便于规定如何创建新进程

- DEBUG_PROCESS  父进程调试子进程和子进程生成的任何进程，将情况通知父进程

- DEBUG_ONLY_THIS_PROCESS 父进程调试子进程，不调试子进程生成的进程

- CREATE_SUSPENDED新进程的主线程被挂起，父进程能够修改子进程的地址空间中的内存，改变子进程的主线程优先级，一旦父进程修改了子进程，父进程将允许子进程通过调用ResumeThread函数执行代码

- DETACHED_PROCESS用于阻止基于CUI的进程对它的父进程的控制台的访问，并告诉系统将他的输出发送到新的控制台窗口。如果基于CUI的进程是由基于CUI的进程创建，则新进程使用父进程的控制台窗口，而通过此标志位，新进程将输出发送到新控制台窗口。

- CREATE_NEW_CONSOLE创建一个新的控制台窗口，不能和DETACHED_PROCESS同时指定

- CREATE_NO_WINDOW告诉系统不要为应用程序创建任何控制台窗口。

- CREATE_NEW_PROCESS_GROUP修改用户在按下Ctrl C或者Ctrl Break时得到通知的进程列表。

- CREATE_DEFAULT_ERROR_MODE新进程使用默认错误模式 不继承父进程的错误模式

- CREATE_SEPARATE_WOW_VDM创建一个单独的DOS虚拟机

- CREATE_SHARED_WOW_VDM系统的共享VDM中运行16位Windows应用程序，需要修改注册表的WOW下的DefaultSeparate VDM 为yes。

- CREATE_UNICODE_ENVIRONMENT子进程环境块包含Unicode字符。（默认是ANSI）

- 强制系统运行嵌入的16位OS/2应用程序的MOS-DOS应用程序

- CREATE_BREAKWAY_FROM_JOB使作业中的进程生成一个与作业相关联的新进程

  ​

也可设置优先级

空闲		IDLE_PRIORITY_CLASS

低于正常	BELOW_NORMAL_PRIORITY_CLASS

正常		NORMAL_PRIORITY_CLASS

高于正常	ABOVE_NORMAL_PRIORITY_CLASS

高		HIGH_PRIORITY_CLASS

实时		REALTIME_PRIORITY_CLASS

优先级影响到进程中的线程如何相对与其他进程的线程来进行调度。



#### pvEnvironment

新进程要使用的环境字符串的内存块，大多数情况下传递NULL（使用父进程的环境字符串）。

GetEnvironmentStrings()获取调用进程使用的环境字符串数据块的地址。

释放内存块：`FreeEnvironmentStrings(PTSTR pszEnvironmentBlock);`



#### pszCurdir

设置子进程的驱动器和目录，设置NULL可以和父进程（应用程序）一致



#### psiStartInfo

指向一个要使用的STARTUPINFO结构

```c++
typedef struct_STARTUPINFO{
  DWORD	cb;		//结构体大小
  PSTR	lpReserved;
  PSTR	lpDesktop;
  PSTR 	lpTitle;
  DWORD	dwX;
  DWORD	dwY;
  DWORD	dwXSize;
  DWORD	dwYSize;
  DWORD	dwXCountChars;
  DWORD	dwYCountChars;
  DWORD	dwFillAttribute;
  DWORD	dwFlags;
  WORD	wShowWindow;
  WORD	cbReserved2;
  PBYTE	lpReserved2;
  HANDLE	hStdInput;
  HANDLE	hStdOutput;
  HANDLE	hStdError;
}STARTUPINFO,*LPSTARTUPINFO;
```

默认值则只设置cb（结构体大小）

成员		控制台/窗口

cb				两者兼有			sizeof(STARTUPINFO)

lpReserved		两者兼有			保留，必须为NULL

lpDesktop		两者兼有			存在则关联，不存在则创建默认属性桌面，NULL则当前桌面

lpTitle			控制台			NULL则使用执行文件的名字

dwX				两者兼有			左上角x

dwY				两者兼有			左上角y

dwXSize			两者兼有			宽度 像素为单位

dwYSize			两者兼有			高度

dwXCountChars	控制台			控制台窗口的宽度 字符为单位

dwYCountChars	控制台			控制台窗口的高度 字符为单位

dwFlags			两者兼有			

wShowWindow	窗口				

cbReserved2		两者兼有			保留。为0

lpReserved2		两者兼有			保留。为NULL

hStdInput		控制台			键盘缓存

hStdOutput		控制台			控制台窗口缓存

hStdError		控制台			控制台窗口缓存





dwFlag有一组标志，用于修改如何创建子进程。（使用什么成员）

STARTF_USESIZE				使用dwXSize和dwYSize

STARTF_USESHOWWINDOW	使用wShowWindow成员

STARTF_USEPOSITION			使用dwX和dwY成员

STARTF_USECOUNTCHARS		使用dwXCountChars和dwYCountchars成员

STARTF_USEFILLATTRIBUTE	使用dwFillAttribute成员

STARTF_USESTDHANDLES		使用三个句柄（hStdInput...）

STARTF_RUN_FULLSCREEN		控制台程序全屏幕运行

STARTF_FORCEONFEEDBACK	临时的光标转换（标识等待状态）



获取由父进程初始化的STARTUPINFO结构的拷贝

`VOID	GetStartUpInfo(LPSTARTUPINFO	pStartUpInfo);`



#### ppiProcInfo

指向要使用的PROCESS_INFORMATION,要进行初始化

```c++
typedef	struct	_PROCESS_INFORMATION{
  HANDLE	hProcess;
  HANDLE	hThread;
  DWORD		dwProcessId;
  DWORD		dwThreadId;
}PROCESS_INFOMATION;
```

在创建进程的时候，系统为每个对象赋予初始计数为1，然后在CreateProcess返回之前，将进程句柄和线程句柄放入成员中，因此使用计数变为2。（最后释放进程时，需要先终止进程，即2-1=1，父进程CloseHandle再减一为0）



当进程内核对象创建后，系统赋予该对象一个独一无二的标识号（ID），线程也是一样，进程ID和线程ID共享相同的号码池，这意味着进程和线程不可能拥有相同的ID。0无法作为ID。CreateProcess返回之前，ID被填入到成员中（ID的生命周期和进程或者线程是一致的，从创建到销毁）。



ToolHelp函数通过PPROCESSENTRY32的th32ParentProcessID获取父进程ID（如果父进程终止则错误无法预知，ID可能标志了另外的进程），和父进程通信可以使用内核对象或者窗口句柄等。