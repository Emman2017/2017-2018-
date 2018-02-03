#include <iostream>
#include <Shlwapi.h>
#include <tchar.h>
#include <Windows.h>
int main()
{
	// ANSI/Unicode通用源代码 _tcs
	TCHAR name[10] = TEXT("Em小小晓");
	int len = _tcsclen(name);
	_tcscat_s(name, TEXT("2017"));
	int rsValue = _tcscmp(name, TEXT("Emman2017"));
	_tcscpy_s(name, TEXT("_EM_Cpper"));
	return 0;
}
