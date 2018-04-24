#include <iostream>
#include "stdafx.h"
#include <xstring>
#include <windows.h>
#include <tlhelp32.h>

bool Inject(DWORD pId, char *dllName);

using namespace std;

void GetProcId(char* ProcName);

DWORD pid = 0; // THIS IS OUR GLOBAL VARIABLE FOR THE PROC ID;

char* wchar_to_string(TCHAR* widechar)
{
	int size = 0;
	while ((char)widechar[size] != ' ') {
		size++;
	}
	size++;
	char * charpointer = new char[size];
	wcstombs(charpointer, widechar, size);
	return charpointer;
}

void GetProcId(char* ProcName)
{
	PROCESSENTRY32   pe32;
	HANDLE         hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32))
	{
		do {
			if (strcmp(wchar_to_string(pe32.szExeFile), ProcName) == 0)
				break;
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	pid = pe32.th32ProcessID;
}


int main()
{
	char filename[MAX_PATH];
	DWORD size = GetModuleFileNameA(NULL, filename, MAX_PATH);

	char *pch;
	pch = strrchr(filename, '\\');
	strcpy(filename, filename + (int)(pch - filename) + 1);

	pch = strrchr(filename, '.');
	strtok(filename, ".");
	strcat(filename, ".dll");

	ShellExecute(NULL, L"open", L"et.exe", L"+connect etjump.twcclan.org +set com_zoneMegs 512", NULL, SW_SHOWDEFAULT);
	Sleep(5000);
	GetProcId("et.exe");
	system("title Chat BOT");
	printf("\n");
	printf("                     Chat BOT\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xC);
	printf("                                        |\n");
	printf("                  ,------------=--------|___________|\n");
	printf("--=============%%%|         |  |______|_|___________|\n");
	printf("                  | | | | | | ||| | | | |___________|\n");
	printf("                  `------------=--------|           |\n");
	printf("                                        |\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("\n----------------------------MADE BY MONSTER&777--------\n\n");
	TCHAR full_path[MAX_PATH];
	wstring wstrDLL;
	for (int i = 0; i < (int)strlen(filename); i++)
		wstrDLL += (wchar_t)filename[i];
	LPCWSTR lstrDLL = wstrDLL.c_str();
	GetFullPathName(lstrDLL, 260, full_path, NULL);
	Inject(pid, filename);
	//system("pause");
	return 0;
}

bool Inject(DWORD pId, char *dllName)
{
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, pId);
	if (h)
	{
		LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		printf("[!] Initialized Library\n");
		LPVOID dereercomp = VirtualAllocEx(h, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		printf("[!] Initialized memory allocation\n");
		WriteProcessMemory(h, dereercomp, dllName, strlen(dllName), NULL);
		printf("[!] Wrote dll name to memory: %d byte(s)\n", (int)strlen(dllName));
		HANDLE asdc = CreateRemoteThread(h, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
		printf("[!] Waiting for Dll exit...\n");
		WaitForSingleObject(asdc, INFINITE);
		VirtualFreeEx(h, dereercomp, strlen(dllName), MEM_RELEASE);
		printf("[!] Freeing memory\n");
		CloseHandle(asdc);
		CloseHandle(h);
		printf("[!] Closed all handles\n");
		return true;
		printf("[!] Complete!\n");
	}
	printf("[!] That process does not exist\n");
	return false;
}