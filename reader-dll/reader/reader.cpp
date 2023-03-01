#include "pch.h" 
#include <TlHelp32.h>
#include <utility>
#include <vector>
#include "reader.h"
#include <iostream>
#define DOUBLE_SIZE     ((int)sizeof(double))

const int LITTLE_ENDIAN = 1234;
const int BIG_ENDIAN = 4321;

double ToDouble(const std::vector<unsigned char>& from, int byteOrder = BIG_ENDIAN)
{

	double retVal = 0;
	unsigned char* p = (unsigned char*)&retVal;

	if (byteOrder == BIG_ENDIAN) {
		for (int32_t i = 0; i < DOUBLE_SIZE; i++) {
			p[i] = from[(DOUBLE_SIZE - (i + 1)) + 0];
		}
	}

	if (byteOrder == LITTLE_ENDIAN) {
		for (int32_t i = 0; i < DOUBLE_SIZE; i++) {
			p[i] = from[i + 0];
		}
	}

	return retVal;
}

uint32_t ToUint32(std::vector<unsigned char> v) {
	uint32_t result = 0;
	result |= v[0] << 24;
	result |= v[1] << 16;
	result |= v[2] << 8;
	result |= v[3];
	return result;
}


DWORD GetProcId(const wchar_t* procName)
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

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
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
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

class Packet
{
public:
	unsigned int size;
	unsigned char opcode;
	uintptr_t addr;

	Packet(unsigned char _opcode, unsigned int _size, uintptr_t _addr)
	{
		opcode = _opcode;
		addr = _addr;
		size = _size;
	}
};

// DLL internal state variables:
static DWORD procId = GetProcId(L"rekordbox.exe");
static uintptr_t base = GetModuleBaseAddress(procId, L"rekordbox.exe");
static HANDLE hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);
static std::vector<Packet> packets = {
	// D1 CHANNEL FADER
	Packet(0, sizeof(double), FindDMAAddy(hProcess, base + 0x03F950A8, {})),
	// D1 CFX KNOB
	Packet(1, sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, { 0x28, 0x50, 0x128, 0x100, 0x0, 0xE0, 0xB0})),
	// D1 LOW FILTER KNOB
	Packet(2, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7BA8, {})),
	// D1 MID FILTER KNOB
	Packet(3, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7BA0, {})),
	// D1 HIGH FILTER KNOB
	Packet(4, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7B98, {})),
	// D1 TRIM KNOB
	Packet(5, sizeof(double), FindDMAAddy(hProcess, base + 0x03F95098, {})),
	// D1 TEMPO
	Packet(6, sizeof(double), FindDMAAddy(hProcess, base + 0x03FBF5E0, {0x160, 0x40, 0x78, 0xE0, 0xB0})),
	// D1 SONG ID
	Packet(7, sizeof(4), FindDMAAddy(hProcess, base + 0x03FCB738, {0x8, 0x240, 0xF0, 0x58, 0x0, 0x1C0, 0x1C})),
	// D1 IS PLAYING
	Packet(8, sizeof(unsigned char), FindDMAAddy(hProcess, base + 0x03FCA690, { 0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x1A4 })),
	// D1 CURRENT TIME
	Packet(9, sizeof(double), FindDMAAddy(hProcess, base + 0x03FCA690, {0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x138})),

	// D2 CHANNEL FADER
	Packet(10, sizeof(double), FindDMAAddy(hProcess, base + 0x03F950B0, {})),
	// D2 CFX KNOB
	Packet(11, sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, {0x28, 0x50, 0x128, 0x40, 0x10, 0xE0, 0xB0})),
	// D2 LOW FILTER KNOB
	Packet(12, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7BC0, {})),
	// D2 MID FILTER KNOB
	Packet(13, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7BB8, {})),
	// D2 HIGH FILTER KNOB
	Packet(14, sizeof(double), FindDMAAddy(hProcess, base + 0x03EA7BB0, {})),
	// D2 TRIM KNOB
	Packet(15, sizeof(double), FindDMAAddy(hProcess, base + 0x03F950A0, {})),
	// D2 TEMPO
	Packet(16, sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, {0x70, 0x10, 0x18, 0x40, 0x78, 0xE0, 0xB0})),
	// D2 SONG ID
	Packet(17, sizeof(4), FindDMAAddy(hProcess, base + 0x03F626C8, {0x98, 0x20, 0x18, 0x10, 0x8, 0x3C0})),
	// D2 IS PLAYING
	Packet(18, sizeof(unsigned char), FindDMAAddy(hProcess, base + 0x03FCA690, { 0x1D8, 0x8, 0x10, 0x60, 0x40, 0x150, 0x188 })),
	// D2 CURRENT TIME
	Packet(19,sizeof(double), FindDMAAddy(hProcess, base + 0x03FACDB8, {
																			0xF0,
																			0x3E0,
																			0xF8,
																			0x8,
																			0xA0,
																			0x108,
																			0x138,
																		})),
	// CROSSFADER
	Packet(20,sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, { 0x60, 0x670, 0x38, 0x38, 0x0, 0x120, 0x2C8, })),

	// D1 CALCULATED TEMPO
	Packet(30, sizeof(double), FindDMAAddy(hProcess, base + 0x03FBF5E0, { 0x10, 0x40, 0x0, 0x58, 0x278, 0xB0 } )),
	// D1 CALCULATED FIRST BEAT TIME
	Packet(31, sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, { 0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x0, 0x140 })),
	// D2 CALCULATED TEMPO
	Packet(40, sizeof(double), FindDMAAddy(hProcess, base + 0x03FBF5E0, { 0x18, 0x40, 0x0, 0x58, 0x278, 0xB0 })),
	// D2 CALCULATED FIRST BEAT TIME
	Packet(41, sizeof(double), FindDMAAddy(hProcess, base + 0x03F626C8, { 0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x8, 0x140 })),

};
Packet* packet;


Changes* getData() {
	Changes* changes = new Changes();

	for (unsigned int i = 0; i < packets.size(); i++)
	{
		packet = &packets[i];

		std::vector<unsigned char>data;
		data.resize(packet->size);

		ReadProcessMemory(hProcess, (unsigned char*)packet->addr, data.data(), data.size(), nullptr);

		switch (packet->opcode)
		{
		case 0: {
			changes->d1ChannelFader = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 1: {
			changes->d1CfxKnob = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 2: {
			changes->d1LowFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 3: {
			changes->d1MidFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 4: {
			changes->d1HighFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 5: {
			changes->d1TrimKnob = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 6: {
			changes->d1Tempo = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 7: {
			changes->d1SongId = ToUint32(data);
			break;
		}
		case 8: {
			changes->d1IsPlaying = data[0];
			break;
		}
		case 9: {
			changes->d1CurrentTime = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 10: {
			changes->d2ChannelFader = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 11: {
			changes->d2CfxKnob = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 12: {
			changes->d2LowFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 13: {
			changes->d2MidFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 14: {
			changes->d2HighFilter = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 15: {
			changes->d2TrimKnob = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 16: {
			changes->d2Tempo = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 17: {
			changes->d2SongId = ToUint32(data);
			break;
		}
		case 18: {
			changes->d2IsPlaying = data[0];
			break;
		}
		case 19: {
			changes->d2CurrentTime = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 20: {
			changes->crossfader = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 30: {
			changes->d1CalculatedTempo = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 31: {
			changes->d1CalculatedFristBeat = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 40: {
			changes->d2CalculatedTempo = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		case 41: {
			changes->d2CalculatedFristBeat = ToDouble(data, LITTLE_ENDIAN);
			break;
		}
		default:
			break;
		}
	}

	return changes;
}
