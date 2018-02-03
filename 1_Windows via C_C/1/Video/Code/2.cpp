#include <iostream>
#include <Windows.h>

int main()
{
	DWORD dwError = GetLastError();
	SetLastError(0xdfff);
	dwError = GetLastError();
	return 0;
}
