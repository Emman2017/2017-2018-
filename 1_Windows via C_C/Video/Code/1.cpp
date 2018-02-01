#include <iostream>
#include <Windows.h>

int main()
{
	// 忽略代码细节  我们只是想制造个错误！
	HANDLE hfile = CreateFile("C:\\124", 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	// 1.调试
	// 2.监视变量 hfile
	//	 监视错误 @err,hr
	return 0;
}
