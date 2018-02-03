#include <tchar.h>
#include <iostream>
#include <Windows.h>

int main()
{
	/*
		ANSI 与 Unicode 转换
	*/
	char	*myName = "大Emman2017"; // ANSI
	int asLen = strlen(myName);
	int ucLen = MultiByteToWideChar(CP_ACP, 0, myName, asLen + 1, NULL, 0);
	if (ucLen)
	{
		wchar_t* pUc = new wchar_t[ucLen]; // Unicode
		int rsValue = MultiByteToWideChar(CP_ACP, 0, myName, asLen + 1, pUc, ucLen);
		delete[] pUc;
	}
	// Unicode 到 ANSI
	//WCHAR myName[15] = L"大Emman2017";
	//int ucLen = lstrlenW(myName);
	//int asLen = WideCharToMultiByte(CP_ACP, 0, myName, ucLen + 1, NULL,0, NULL,NULL);
	//if (asLen)
	//{
	//	char *pAs = new char[asLen];
	//	int rsValue = WideCharToMultiByte(CP_ACP, 0, myName, ucLen + 1, pAs, asLen, NULL, NULL);
	//	delete[] pAs;
	//}
	return 0;
}
