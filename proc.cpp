#include "proc.h"


DWORD proc::getProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

DWORD proc::getModuleBaseAddr(DWORD procId, const wchar_t* modName)
{
	DWORD modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (DWORD)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

DWORD mem::findDMAAddy(HANDLE hProc, DWORD ptr, std::vector<unsigned int> offsets)
{
	DWORD addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

template<class dataType>
dataType mem::readMem(HANDLE hProc, DWORD addr)
{
	dataType val;
	ReadProcessMemory(hProc, (BYTE*)addr, &val, sizeof(val), 0);
	return val;
}

template <class dataType>
void mem::writeMem(HANDLE hProc, DWORD addr, dataType val)
{
	WriteProcessMemory(hProc, (BYTE*)addr, &val, sizeof(val), 0);
}