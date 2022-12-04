#pragma once
#include <Windows.h>
#include <vector>
#include <tlhelp32.h>

namespace proc {
	DWORD getProcId(const wchar_t* procName);
	DWORD getModuleBaseAddr(DWORD procId, const wchar_t* modName);
}

namespace mem {
	template <class dataType>
	dataType readMem(HANDLE hProc, DWORD addr);
	template <class dataType>
	void writeMem(HANDLE hProc, DWORD addr, dataType val);
	
	DWORD findDMAAddy(HANDLE hProc, DWORD ptr, std::vector<unsigned int> offsets);
}