# Windows 2000 与 Unicode

> Windows 2000

Windows 2000使用Unicode从头进行开发，用于创建窗口、显示文本、进行字符串操作的所有核心函数都是需要Unicode字符串。

->	调用一个Windows函数 传递给它一个ANSI字符串

->	系统将字符串转换成为Unicode （消耗时间和空间）

->	将转换后的Unicode字符串返回给你的应用程序

