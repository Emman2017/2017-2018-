## 对自己的ID概念应该有所了解

当线程运行时，它们常常想要调用Windows函数来改变它们的运行环境。例如，线程可能想要改变他的优先级或者他的进程的优先级（7中会介绍）。

因此，线程可以很容易的引用他的进程的内核对象:

`HANDLE	GetCurrentProcess();`

`HANDLE GetCurrentThread();`

返回的是伪句柄。因此对使用计数没有影响。

如果调用CloseHandle传递伪句柄，那么将忽视调用，返回FALSE



但是，当调用一个需要进程句柄或者线程句柄的Windows函数时，可以传递一个伪句柄执行相应的操作。

只有少数Windows函数需要传递独一无二的ID来标识进程或者线程。

`DWORD	GetCurrentProcessId();`

`DWORD	GetCurrentThreadId();`

可以说伪句柄是当前线程的句柄，因此，当传递给调用函数的参数是句柄时，我们需要将句柄变成实句柄。

DuplicateHandle可以实现这一转换。即拷贝伪句柄后将拷贝出实句柄...

所有计数次数也增加了。

CloseHandle应该放在最后不再使用的调用函数里面。