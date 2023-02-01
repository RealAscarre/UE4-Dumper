#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>

#include "Offsets.h"

using namespace std;

int GetProcessID(const char* module_name) {
	int pid = 0;
	int threadCount = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);
	while (Process32Next(hSnap, &pe)) {
		if (_tcsicmp(pe.szExeFile, _T(module_name)) == 0) {
			if ((int)pe.cntThreads > threadCount) {
				threadCount = pe.cntThreads;
				pid = pe.th32ProcessID;
			}
		}
	}

	return pid;
}

uintptr_t GetBaseAddress(DWORD Pid, const char* TargetProcess) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, Pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!strcmp(modEntry.szModule, TargetProcess)) {
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

template<typename T> T Read(SIZE_T address) {
	T buffer;
	ReadProcessMemory(Offsets::Handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template <typename T>
constexpr void Write(const uintptr_t& address, const T& value) {
	::WriteProcessMemory(Offsets::Handle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
}

string ReadStringNew(uintptr_t address, unsigned int size) {
	string name(size, '\0');
	ReadProcessMemory(Offsets::Handle, (LPCVOID)address, (void*)name.data(), size * sizeof(char), NULL);
	name.shrink_to_fit();
	return name;
}

string ReadString(uintptr_t address, unsigned int size) {
	char* data = new char[size];
	memset(data, '\0', size);

	for (int i = 0; i < size; i++) {
		ReadProcessMemory(Offsets::Handle, (void*)(address + (sizeof(char) * i)), (void*)(&data[0] + i), sizeof(char),NULL);
		if (data[i] == 0x0) {
			break;
		}
	}

	string name(data);
	name.shrink_to_fit();
	return name;
}

uintptr_t FindPattern(const char* Pattern, const char* Mask) {
	uintptr_t result = 0;
	HMODULE hMods[1024];
	DWORD cbNeeded;
	if (EnumProcessModules(Offsets::Handle, hMods, sizeof(hMods), &cbNeeded)) {
		MODULEINFO module_info;
		if (GetModuleInformation(Offsets::Handle, hMods[0], &module_info, sizeof(module_info))) {
			uintptr_t module_addr = (uintptr_t)module_info.lpBaseOfDll;
			uint32_t module_size = module_info.SizeOfImage;
			char* buffer = new char[module_size];
			if (ReadProcessMemory(Offsets::Handle, (LPCVOID)module_addr, buffer, module_size, 0)) {
				for (size_t i = 0; i < module_size - strlen(Mask) + 1; i++) {
					size_t j;
					for (j = 0; j < strlen(Mask); j++) {
						if (Mask[j] == '?') continue;
						if (buffer[i + j] != Pattern[j]) break;
					}
					if (j == strlen(Mask)) {
						result = module_addr + i;
						break;
					}
				}
			}
			delete[] buffer;
		}
	}
	return result;
}

/*
string ReadString(uintptr_t StrPtr, int StrLength) {
	char Names[1028];
	for (int x = 0; x < StrLength; ++x) {
		Names[x] = Read<char>(StrPtr + x);
	}
	Names[StrLength] = '\0';
	string ClsNames = string(Names);
	return ClsNames;
}
*/