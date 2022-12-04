#include <iostream>
#include <Windows.h>
#include "proc.h"

int main()
{
	DWORD procId = proc::getProcId(L"csgo.exe");
	uintptr_t moduleBase = proc::getModuleBaseAddr(procId, L"client.dll");
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t localPlayerPtr, healthAddr;
	if (procId && moduleBase)
	{
		// SET LocalPlayerPointer
		localPlayerPtr = moduleBase + 0x00DE8964;

		// Base Address of Pointer Chain
		healthAddr = mem::findDMAAddy(hProc, localPlayerPtr, { 0x100 });
	}	
	else
	{
		std::cout << "Process or ModuleBase not found, press enter to exit" << std::endl;
		getchar();
		return 0;
	}

	while (true)
	{
		int iHealth;
		ReadProcessMemory(hProc, (LPCVOID)(healthAddr), &iHealth, sizeof(iHealth), nullptr);
		std::cout << "health Value : " <<  iHealth << std::endl;
		Sleep(100);
	}

	return 0;
}

