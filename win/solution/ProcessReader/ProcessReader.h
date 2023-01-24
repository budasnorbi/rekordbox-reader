#pragma once
#include <string>
#define _WINSOCKAPI_  // Need to define it to handle multiple windows.h includes
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "RekordBoxData.h"
#include <iostream>
#include "UdpClient.h"
#include "TcpClient.h"


class ProcessReader
{
public:
	const wchar_t* processName;
	DWORD procId;

	ProcessReader(const wchar_t* _processName)
	{
		this->processName = _processName;
		this->procId = this->GetProcessId();

		if (this->procId == 0)
			throw "Process not found!";

		this->baseAddress = this->GetModuleBaseAddress();

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, this->procId);

		// Define read bites, these are send on TCP
		recordBoxBites =
		{
			RekordBoxData<bool>("D1 Playing",8, FindDMAAddy(this->baseAddress + 0x03FCA690, { 0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x1A4 })),
			RekordBoxData<bool>("D2 Playing",18, FindDMAAddy(this->baseAddress + 0x03FCA690, { 0x1D8, 0x8, 0x10, 0x60, 0x40, 0x150, 0x188 }))
		};

		// Define read doubles, these are send on UDP
		recordBoxDoubles =
		{
			RekordBoxData<double>("D1 Channel Fader", 0,  FindDMAAddy(this->baseAddress + 0x03F950A8, {})),
			RekordBoxData<double>("D1 CFX Knob", 1,  FindDMAAddy(this->baseAddress + 0x03F626C8,  { 0x28, 0x50, 0x128, 0x100, 0x0, 0xE0, 0xB0 })),
			RekordBoxData<double>("D1 Low Filter Knob", 2,  FindDMAAddy(this->baseAddress + 0x03EA7BA8,  {})),
			RekordBoxData<double>("D1 Mid Filter Knob", 3,  FindDMAAddy(this->baseAddress + 0x03EA7BA0,  {})),
			RekordBoxData<double>("D1 High Filter Knob", 4,  FindDMAAddy(this->baseAddress + 0x03EA7B98,  {})),
			RekordBoxData<double>("D1 Trim Knob", 5,  FindDMAAddy(this->baseAddress + 0x03F95098,  {})),
			RekordBoxData<double>("D1 Tempo Slider", 6,  FindDMAAddy(this->baseAddress + 0x03FBF5E0, { 0x160, 0x40, 0x78, 0xE0, 0xB0})),
			RekordBoxData<double>("D1 Current Time", 9,  FindDMAAddy(this->baseAddress + 0x03FCA690, { 0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x138 })),

			RekordBoxData<double>("D2 Channel Fader", 10,  FindDMAAddy(this->baseAddress + 0x03F950B0, {})),
			RekordBoxData<double>("D2 CFX Knob", 11,  FindDMAAddy(this->baseAddress + 0x03F626C8, { 0x28, 0x50, 0x128, 0x40, 0x10, 0xE0, 0xB0 })),
			RekordBoxData<double>("D2 Low Filter Knob", 12,  FindDMAAddy(this->baseAddress + 0x03EA7BC0,  {})),
			RekordBoxData<double>("D2 Mid Filter Knob", 13,  FindDMAAddy(this->baseAddress + 0x03EA7BB8,  {})),
			RekordBoxData<double>("D2 High Filter Knob", 14,  FindDMAAddy(this->baseAddress + 0x03EA7BB0,  {})),
			RekordBoxData<double>("D2 Trim Knob", 15,  FindDMAAddy(this->baseAddress + 0x03F950A0,  {})),
			RekordBoxData<double>("D2 Tempo Slider", 16,  FindDMAAddy(this->baseAddress + 0x03F626C8, { 0x70, 0x10, 0x18, 0x40, 0x78, 0xE0, 0xB0 })),
			RekordBoxData<double>("D2 Current Time", 19,  FindDMAAddy(this->baseAddress + 0x03FACDB8, { 0xF0, 0x3E0, 0xF8, 0x8, 0xA0, 0x108, 0x138, })),

			RekordBoxData<double>("Crossfader", 20,  FindDMAAddy(this->baseAddress + 0x03F626C8, { 0x60, 0x670, 0x38, 0x38, 0x0, 0x120, 0x2C8, })),

			// Calculated values
			RekordBoxData<double>("D1 Calculated Tempo", 30,  FindDMAAddy(this->baseAddress + 0x03FBF5E0, { 0x10, 0x40, 0x0, 0x58, 0x278, 0xB0 })),
			RekordBoxData<double>("D1 Calculated First Beat Time", 31,  FindDMAAddy(this->baseAddress + 0x03F626C8, { 0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x0, 0x140 })),

			RekordBoxData<double>("D2 Calculated Tempo", 40,  FindDMAAddy(this->baseAddress + 0x03FBF5E0, { 0x18, 0x40, 0x0, 0x58, 0x278, 0xB0 })),
			RekordBoxData<double>("D2 Calculated First Beat Time", 41,  FindDMAAddy(this->baseAddress + 0x03F626C8, { 0x28, 0x10, 0x68, 0x3E0, 0xF8, 0x8, 0x140 })),
		};


		//TODO: Song ID : 7 , 17 -> INT 
	}

	// Reads the rekordbox.exe data, addresses are defined in the constructor
	void ReadMemory();

	// Prints the read data to console ( only for debug purposes )
	void printToConsole();

	// Sends recordBoxDoubles on UDP ( Immidiate data )
	void sendOnUdpSocket();

	// Sends recordBoxBites on TCP ( Reliable data )
	void sendOnTcpSocket();

private:
	DWORD GetProcessId();
	uintptr_t GetModuleBaseAddress();

	uintptr_t baseAddress;
	HANDLE hProcess;

	std::vector<RekordBoxData<bool>> recordBoxBites;
	std::vector<RekordBoxData<double>> recordBoxDoubles;

	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	UdpClient udpClient;
	TcpClient tcpClient;

	void readBinaryValues();
	void readDoubleValues();
	
	void printBinaryValues();
	void printDoubleValues();

};

