# 成为符合A N S I和U n i c o d e的应用程序

> 规则

- 将文本视为字符数组
- 将通用数据类型(TCHAT 和 PTSTR等)用于文本字符和字符串
- 将显示数据类型(BYTE和PBYTE等)用于字节、字节指针、数据缓存
- 将TEXT宏用于原义字符和字符串
- 执行全局替换（例如用PTSTR替换PSTR)
- 字符串计算注意区分字节大小和长度

> 是否是Unicode代码

DWORD IsTextUnicode(CONST PVOID pvBuffer,int cb,PINT pResult); // 并不是完全准确的

第一个参数：要测试的缓存的地址

第二个参数：字节数

第三个参数：一个整数的地址，可NULL表示全部测试

返回值：TRUE or FALSE

> Unicode与ASNI之间的转换

int	MutilByteToWideChar();// ANSI到Unicode

参数说明：

UINT		uCodePage：多字节代码页号   一般指定CP_ACP

DWORD		dwFlags:通常不使用 传递0

PCSTR		pMultiByteStr:ANSI字符串

int			cchMultiByte:多字节字符串长度，指定-1则计算出来

PWSTR		pWideCharStr:存储转换后的Unicode字符串

int			cchWideChar:Unicode字符串长度

使用步骤：

1. 设定最后两位参数为0 获取Unicode长度

2. 根据获取的大小分配足够空间

3. 设定最后两位参数，执行转换

   ​

int	WideCharToMutilByte();//Unicode到ANSI

注意：多出来的后面两个参数一般设置为NULL（默认无法找到该字符设置为?）