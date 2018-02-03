# 如何编写Unicode源代码

> 设定UNICODE和_UNICODE宏就可以重新编译源文件

## C运行期库对Unicode的支持

typedef unsigned short wchar_t;

> ANSI函数和等价Unicode函数

```
char*		strcat(char*, const char*); // 拼接两个字符串
wchar_t*	wcscat(wchar_t*, const wchar_t*);

char*		strchr(const char*,int); // 考察字符首次出现的位置
wchar_t*	wcschr(const wchar_t*, const wchar_t*);

int 		strcmp(const char*, const char*);// 比较两个字符串
int			wcscmp(const wchar_t*, const wchar_t*);

char*		strcpy(char*, const char*);// 拷贝字符串
wchar_t*	wcscpy(wchar_t*,const wchar_t*);

size_t		strlen(const char*); // 获取字符串长度
size_t		wcslen(const wchar_t*);
```



> 通用的ANSI/Unicode源代码文件
>
> UNICODE宏用于Windows头文件  _UNICODE用于C运行期头文件（一般一起指定）

<tchar.h>中的宏可扩展为ANSI版本和Unicode版本。

TCHAR 	通用字符型		char、wchar_t

TEXT	通用字符处理宏	”“	、L”“

PTSTR	

PCTSTR

> 字符串函数 <tchar.h>

_tcs

_tcslen

_tcscmp

...

