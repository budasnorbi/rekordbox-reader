#include "pch.h"
#include <TlHelp32.h>
#include <utility>
#include <vector>
#include "reader.h"
#include <iostream>
#include <variant>

DWORD GetProcId(const wchar_t *procName)
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

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t *modName)
{
	uintptr_t modBaseAddr = 0;
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
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE *)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

class Packet
{
public:
	unsigned int size;
	uintptr_t addr;

	Packet(unsigned int _size, uintptr_t _addr)
	{
		addr = _addr;
		size = _size;
	}
};

// DLL internal state variables:
static DWORD procId = GetProcId(L"rekordbox.exe");
static uintptr_t base = GetModuleBaseAddress(procId, L"rekordbox.exe");
static HANDLE hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);
static std::vector<Packet> packets = {
	// 0 D1 CHANNEL FADER
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x3FC5458, {})),
	// 1 D1 CFX KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F929E8, {0x28, 0x50, 0x128, 0x100, 0x0, 0xE0, 0xB0})),
	// 2 D1 LOW FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4EA8, {})),
	// 3 D1 MID FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4EA0, {})),
	// 4 D1 HIGH FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4E98, {})),
	// 5 D1 SONG ID
	Packet(sizeof(4), FindDMAAddy(hProcess, base + 0x03F929E8, {0x60, 0x88, 0x20, 0x2A0, 0x38})),
	// 7 D1 CURRENT TIME
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F964C8, {0x40, 0x110, 0x288, 0xA8, 0x40, 0x150, 0x138})),
	// 8 D1 TEMPO
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F929E8, {0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x0, 0x120})),

	// 9 D2 CHANNEL FADER
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x3FC5460, {})),
	// 10 D2 CFX KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F929E8, {0x28, 0x50, 0x128, 0x40, 0x10, 0xE0, 0xB0})),
	// 11 D2 LOW FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4EC0, {})),
	// 12 D2 MID FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4EB8, {})),
	// 13 D2 HIGH FILTER KNOB
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03ED4EB0, {})),
	// 14 D2 SONG ID
	Packet(sizeof(4), FindDMAAddy(hProcess, base + 0x03F929E8, {0x70, 0x10, 0x18, 0x200, 0x198, 0x8, 0x3B8})),
	// 16 D2 CURRENT TIME
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F929E8, {
																		0x28,
																		0x10,
																		0x68,
																		0x3E0,
																		0xF8,
																		0x8,
																		0x138,
																	})),
	// 17 D2 TEMPO
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03F929E8, {0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x8, 0x120})),

	// 18 CROSSFADER
	Packet(sizeof(double), FindDMAAddy(hProcess, base + 0x03FA3638, {
																		0x18,
																		0x18,
																		0x18,
																		0x8,
																		0x38,
																		0x20,
																		0x178,
																	})),
};
Packet *packet;

Changes *getData()
{
	Changes *changes = new Changes();

	for (size_t i = 0; i < packets.size(); i++)
	{
		packet = &packets[i];

		std::vector<unsigned char> readedData;
		readedData.resize(packet->size);

		ReadProcessMemory(hProcess, (unsigned char *)packet->addr, readedData.data(), readedData.size(), nullptr);
		changes->data.insert(changes->data.end(), readedData.begin(), readedData.end());
		readedData.clear();
	}

	return changes;
}
