#include <iostream>
#include <Windows.h>

int main()
{
	char* InjectionName = (char*)GetModuleHandle(L"Kernel32.dll") + 0xE8;

	CreateRemoteThread(
		OpenProcess(PROCESS_ALL_ACCESS, false, 19372),
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "LoadLibraryA"),
		InjectionName,
		0,
		NULL
	);

	return 0;
}