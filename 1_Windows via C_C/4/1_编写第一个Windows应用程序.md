# 进程

> 本章介绍系统如何管理所有正在运行中的程序

1. 什么是进程？
2. 系统如何创建进程内核对象 管理每个进程？
3. 如何使用相关的内核对象来对进程进行操作？
4. 进程的各种不同属性？
5. 修改查询属性的函数？
6. 辅助进程？

**定义**：一个正在运行的程序的实例

**组成**：管理进程的内核对象、地址空间（所有可执行模块、DLL模块代码和数据、动态内存分配空间）

**进程是不活泼的**

->	若要使进程完成某项操作，他必须拥有在它环境中运行的线程，该线程负责执行包含在进程的地址空间中的代码。若干线程“同时”执行进程地址空间中的代码，为此，线程有他自己的一组CPU寄存器和堆栈。每个进程至少拥有一个线程，如果没有线程，进程就没有存在的必要了，系统将自动撤销该进程和他的地址空间。

->	操作系统为每个线程安排一定的CPU时间，以循环为线程提供时间片，造成所有线程同时运行的假象。

多个CPU的话更复杂，得考虑线程负载平衡。

->	当创建一个进程时，系统会自动创建他的第一个线程（主线程），主线程可以创建其他的线程，这些线程又可以创建更多的线程...

## 编写第一个Windows应用程序

> Windows支持的两种应用程序

图形用户界面应用程序（GUI）：

拥有图形前端程序：创建窗口、拥有菜单、通过对话框和用户打交道、使用所有标志Windows组件

控制台用户界面应用程序（CUI）：

它们通常不能创建窗口或者处理消息，并且它们不需要图形用户界面。

->	当使用Microsoft Visual C++来创建应用程序时，这种集成环境安装了许多不同的链接程序开关，这样，链接程序就可以把相应的子系统嵌入产生的可执行程序。CUI应用程序开关/SUBSYSTEM:CONSOLE,GUI应用程序

开关/SUBSYSTEM:WINDOWS

->	当用户运行一个应用程序时，操作系统的加载程序就会查看可执行图形程序的标题，抓取子系统的值，如果该值指明一个CUI应用程序，那么加载程序就会自动保证为该应用程序创建控制台窗口。如果该值指明是一个GUI应用程序，那么加载程序不创建控制台窗口，而只是加载应用程序。一旦程序启动运行，操作系统就不再考虑应用程序拥有什么类型的用户界面了。

->	Windows应用程序必须有一个应用程序启动运行时调用的进入口函数。

四个：

```c++
int	WINAPI	WinMain(HINSTANCE	hinstExe,HINSTANCE	h,PSTR	pszCmdLine,	int	nCmdShow);
int	WINAPI	wWinMain(HINSTANCE	hinstExe,HINSTANCE	h,PWSTR	pszCmdLine,	int	nCmdShow);
int	__cdecl	main(int argc,char *argv[],char *envp[]);
int __cdecl	wmain(int argc,wchar_t *argv[],wchar_t *envp[]);
```

->	操作系统实际上并不调用你编写的进入点函数，他调用的是C/C++运行期启动函数。该函数负责对C/C++运行期库进行初始化，这样就可以调用malloc和free之类的函数。还能确保已经声明的任何全局对象和静态C++对象能够在代码执行前被正确的创建。

->	源代码中可以实现哪个进入点以及何时使用该进入点

应用程序类型								进入点					嵌入可执行文件的启动函数

- 需要ANSI字符和字符串的GUI应用程序	WinMain				         WinMainCRTStartup
- 需要Unicode字符和字符串的GUI程序        wWinMain                              wWinMainCRTStartup
- 需要ANSI字符和字符串的CUI应用程序       main                                       mainCRTStartup
- 需要Unicode字符和字符串的CUI程序         wmain                                   wmainCRTStartup

进行链接程序（选择运行期函数初始化）------进行加载程序（获取开关值）执行操作

链接程序负责在他连接可执行文件时选择相应的C/C++运行期启动函数。

如果设定了/SUBSYSTEM:WINDOWS连接程序开关，那么该连接程序期望找到一个WinMain或者wWinMain，假如都不存在则链接程序返回一个“未转换的外部符号”的错误消息。否则他将选择WinMainCRTStartup或者wWinMainCRTStartup函数。

// 从应用程序中全部删除/SUBSYSTEM链接程序开关，链接程序就自动确定子系统，即根据入口点函数判断，然后嵌入相应的C/C++运行期启动函数

**注意**：假如我们创建项目时选择了Win32应用程序，但是入口点是main函数，那么会发生错误，因为链接程序开关是被默认设置为/SUBSYSTEM:WINDOWS的，即找不到WinMain和wWinMain发生的错误。

**注意**：运行期函数指出了应该调用什么入口点函数，，，而入口点函数和链接开关都可以指出调用的何种运行期启动函数。

**启动函数的功能归纳**

- 检索指向新进程的完整命令行的指针
- 检索指向新进程的环境变量的指针
- 对C/C++运行期的全局变量进行初始化
- 对C运行期内存单元分配函数 malloc calloc和底层输入输出例程使用的内存栈进行初始化
- 为所有全局和静态C++类对象调用构造函数

当执行完这些操作后，启动函数调用应用程序的进入点函数。

int  nMainRetVal = main（...);

当进入口函数返回时，启动函数便调用C运行期的exit函数，将返回值传递给它exit(nMainRetval)

Exit函数负责下面的操作：

- 调用由_onexit函数的调用而注册的任何函数
- 为所有全局的和静态的C++类对象调用析构函数
- 调用操作系统的ExitProcess函数，将nMainRetVal传递给他，使得操作系统能够撤销进程并设置exit代码

#### 进程的实例句柄

加载到进程地址空间的每个可执行文件或DLL文件均被赋予一个独一无二的实例句柄。对于加载资源的函数调用来说，通常需要该句柄的值，HANDLE和HMODULE是一样的，区别在于16位Windows系统中用于不同的东西。

->	WinMain的hinstExe参数的实际值是系统将可执行文件的映像加载到进程的地址空间时使用的基地址。例如，系统打开了可执行文件并且将他的内容加载到地址0x00400000中，那么WinMain的hinstExe的参数就是0x00400000。

->	可执行文件的映像加载到基地址是由链接程序决定的，不同的链接程序可以使用不同的默认基地址。基地址可以由链接程序中的/BASE:address链接程序开关改变。

->	GetModuleHandle函数返回可执行文件或者DLL文件加载到进程的地址空间时用到的实例句柄/基地址。

该函数只查看调用进程的地址空间，如果是DLL，不使用，传递DLL的名字也是得到NULL，尽管该DLL可能被加载到了其他进程的地址空间，因此，通过DLL中的代码返回的是可执行文件的基地址，而不是DLL文件的基地址。

#### 进程的前一个实例句柄

C/C++运行期启动函数总是将NULL传递给WinMain的hinstExePrev参数。该参数只在16位Windows系统中有用，因此，除非我们编写16位应用程序，否则，不要使用该参数（因为是NULL）

#### 进程的命令行

该命令行永远不会为空，至少用于创建新进程的可执行文件的名字是命令行的第一个标记，（CreateProcess韩指出进程能够接受单个字符组成的命令行，即字符串结尾处的0）。当C运行期启动代码开始运行的时候，它要检索进程的命令行，**跳过可执行文件的名字，并将指向命令行的其余部分的指针传递给WinMain的pszCmdLine。**

->	获得指向进程的完整的命令行的指针 GetCommandLine(A or W)

->	只有Unicode版的命令行字符串参数转为传统的字符串数组（切割）CommandLineToArgvW(...)

该切割后返回的是首地址，该空间的内存是系统分配的，我们可以等待程序结束，系统自动释放，如果想自己释放，HeadpFree(GetProcessHeap(),0,首地址);

#### 进程的环境变量

跟句柄表一样，环境块也是进程相关的。环境块是进程的地址空间中分配的一个内存块。每个环境块都包含**一组字符串**：

varname1=varvalue1\0

varname2=varvalue2\0

...

`即：`环境变量名=值  组成的以\0结束的字符串

**注意**：

1. 所有字符串按环境变量名字母顺序排序
2. 变量名不允许有等号  可以有空格（要区分）

让有关程序更新他们的环境块：

`SendMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,(LPARAM)TEXT("Environment"));`

子进程基础一组与父进程相同的环境变量。但是父进程可以控制继承什么环境变量。（子进程可以添加修改自己的环境`变量）`

环境变量的基本职能：用户创建环境变量并初始化----------用户启动应用程序---------应用程序查看环境块----找到该变量的话----调整自己的行为特性。

**使用环境变量**：

`DWORD GetEnvironmentVariable(PCTSTR pszName,PTSTR pszValue,DWORD cchValue);`

功能：**确定某个环境变量**是否存在以及它的值是多少

参数：变量名字、临时缓存存储变量值（都是字符串）、缓存的大小

返回值：找到---返回字符串长度，找不到---返回0

有时，我们可以看到%USERPROFILE%\My Documents

%%直接是可扩展的名字，环境变量的值USERPROFILE应该被放入该字符串，我们可以使用

函数，**逆向由值得到环境变量**

`DWORD ExpandEnvironmentStrings(PCTSTR pszSrc,PSTR pszDst,DWORD nSize);`

参数：源字符串地址、临时缓存存储新字符串、缓存大小

**添加修改删除变量**：

`BOOL SetEnvironmentVariable(PCRSTR pszName,PCTSTR pszValue);`

功能：将变量pszName设置pszValue值

注意：如果pszName不存在，就创建，如果pszName存在就修改值，如果pszValue那么就删除pszName，够智能了！



### 进程的亲缘性

一般来说，进程中的线程可以在主计算机中的任何一个CPU上执行。但是一个进程的线程可能被强制在可用的CPU的子集上运行，这称为进程的亲缘性，将在后面介绍（子进程继承父进程的亲缘性）。



#### 进程的错误模式

与进程相关联的还有：一组标志

用于告诉系统，进程对严重错误应该作出什么反应，包括：磁盘介质故障、未处理的异常、文件查找失败、数据没有对齐等。

进程可以告诉系统如何处理每一种错误，方法是调用：SetErrorMode

`UINT SetErrorMode(UINT fuErrorMode);`

fuErrorMode的值：

```c++
SEM_FAILCRITICALERRORS              系统不显示关键错误句柄消息框，将错误返回给调用线程
SEM_NOGOFAULTERRORBOX               系统不显示一般保护故障消息框。本标志只应该由采用异常情况处理程序来处理一般保护 故障的调试应用程序来设定
SEM_NOOPENFILEERRORBOX				当系统找不到消息框时，不显示消息框
SEM_NOALIGNMENTFAULTEXCEPT			系统自动排除内存没有对齐的故障，并使应用程序看不到这些故障，对X86处理器不起作用
```

父进程可以防止子进程继承他的错误模式：CreateProcess时设定：CREATE_DEFAULT_ERROR_MODE



#### 进程的当前驱动器和目录

当不提供全路径名时，Windows的各个函数就会在当前驱动器的当前目录中查找文件和目录。

必须指明的是，该信息是进程相关的，因此，改变当前驱动器或目录的进程中的线程，就可以为所有线程改变这些信息。

线程能够获得和设置他的进程的当前驱动器和目录：

`DWORD GetCurrentDirectory(DWORD cchCurDir,PTSTR pszCurDir);`

`BOOL SetCurrentDirectory(PCTSTR pszCurDir);`





#### 进程的当前目录

系统对进程的当前驱动器和目录保持跟踪，但是不跟每个驱动器的当前目录。

如果支持对多个驱动器的当前目录的处理，则是通过进程的环境字符串来提供的，

=C:=C:\Utility\Bin            驱动器C的进程的当前目录\Utility\Bin

=D:=D:\Program Files     驱动器D的进程的当前目录\Program Files 

调用一个函数，传递一个驱动器全限定名（如：D:ReadMe.txt）,系统会去查看进程的环境块，找出与指定驱动器相关的变量（=D）。如果存在则使用变量的值作为当前驱动器，若不存在，系统假设驱动器的当前目录是他的根目录。

例如：进程的当前目录的C:\Utility\Bin，并且你调用CreateFile，打开D:ReadMe.txt,那么系统会去查看进程的环境变量=D，因为=D变量存在，因此系统去从D:\Program Files目录打开该ReadMe.txt。如果=D变量不存在，系统将试图从驱动器D的根目录打开ReadMe.txt。



> 如果父进程创建了一个想传递给子进程的环境块，那该如何？

子进程的环境块不会自动继承父进程的当前目录。相反，子进程的当前目录将默认为每个驱动器的根目录，如果想要让子进程继承父进程的根目录，该父进程必须创建这些驱动器名的环境变量，并在子进程创建前将它们添加给环境块。（通过使用GetFullPathName  父进程可以获取当前目录）(环境块是继承的但是当前目录不是继承的  必须写入环境块才能继承)

`DWORD GetFullPathName(PCTSTR pszFile,DWORD cchPath,PTSTR pszPath,PTSTR *ppszFilePart);`

例如：获取驱动器C的当前目录

```c++
TCHAR szCurDir[MAX_PATH];
DWORD GetrFullPathName(TEXT("C:"),MAX_PATH,szCurDir,NULL);
```



#### 系统版本

应用程序通常需要确定用户运行的是哪个Windows版本。（进行对比 返回TRUE则正确）

GetVersion（）：高位是次要版本，低位是主要版本（老函数）

`BOOL	GetVersionEx(POSVERSIONINFO pVersionInfo);`

```c++
typedef	struct
  {
  DWORD	dwOSVersionInfosize;	// 	使用前设置  sizoef(OSVERSIONINFOEX)
  DWORD dwMajorVersion;			// 	主系统的主版本号
  DWORD	dwMinorVersion;			// 	次系统的次要版本号
  DWORD dwPlatformId;			// 	当前系统的构建号
  TCHAR	szCSDVersion[128];		// 	附加文本
  WORD	wServicePackMajor;		//	服务程序包的主要版本
  WORD	wServicePackMinor;		//	服务程序包的次要版本
  WORD	wSuitMask;				//	标识系统上存在哪个程序组
  BYTE	wProductType;			//	标识装了下面哪几个操作系统
  BYTE	wReserved;				// 	保留
}OSVERSIONINFOEX,*POSVERSIONINFOEX
```

替代函数：BOOL VerifyVersionInfo(上面的结构，DWORD dwTypeMask，DWORDLONG dwl)

mask指明哪些成员进行了初始化 ，dwl参数描述如何比较，使用宏VER_SET_CONDITION来解决

具体参考《windows核心编程第四章》系统版本。