#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "proc.h"
#include "stdafx.h"

#define UDP_SERVER_PORT 54000
#define TCP_SERVER_PORT 54001

class Packet {
public:
	unsigned int size;
	unsigned char opcode;
	uintptr_t addr;
	std::vector<unsigned char> data;

	Packet(unsigned char _opcode, unsigned int _size, uintptr_t _addr) {
		opcode = _opcode;
		addr = _addr;
		data = std::vector<unsigned char>(_size);
		size = _size;
	}
};

int main()
{
	std::cout << "REKORDBOX READER v0.2 (for Windows)" << std::endl;
	std::cout << "Supports rekordbox 6.6.8" << std::endl;

	std::string SERVERS_HOST = "127.0.0.1";

	// TCP CLIENT
	WSAData data2;
	WORD ver2 = MAKEWORD(2, 2);

	int wsResult2 = WSAStartup(ver2, &data2);
	if (wsResult2 != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult2 << std::endl;
		return 0;
	}

	SOCKET tcpSock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpSock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(TCP_SERVER_PORT);
	inet_pton(AF_INET, SERVERS_HOST.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(tcpSock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(tcpSock);
		WSACleanup();
		return 0;
	}


	// UDP CLIENT SOCKET
	WSAData data;
	WORD version = MAKEWORD(2, 2);

	int wsResult = WSAStartup(version, &data);
	if (wsResult != 0) {
		return 0;
	}

	SOCKET udpSock = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(UDP_SERVER_PORT);
	inet_pton(AF_INET, SERVERS_HOST.c_str(), &server.sin_addr);

	//MEMORY READING

	// Get ProcId of the target process
	DWORD procId = GetProcId(L"rekordbox.exe");

	// Get module base address
	uintptr_t base = GetModuleBaseAddress(procId, L"rekordbox.exe");

	// Get Handle to Process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	std::vector<Packet> packets = {
		// D1 CHANNEL FADER
		Packet(0, sizeof(double),         FindDMAAddy(hProcess, base + 0x03F950A8, {})),
		// D1 CFX KNOB
		Packet(1, sizeof(double),         FindDMAAddy(hProcess, base + 0x03F626C8, { 0x28, 0x50, 0x128, 0x100, 0x0, 0xE0, 0xB0 })),
		// D1 LOW FILTER KNOB
		Packet(2, sizeof(double),         FindDMAAddy(hProcess, base + 0x03EA7BA8, {})),
		// D1 MID FILTER KNOB
		Packet(3, sizeof(double),         FindDMAAddy(hProcess, base + 0x03EA7BA0, {})),
		// D1 HIGH FILTER KNOB
		Packet(4, sizeof(double),         FindDMAAddy(hProcess, base + 0x03EA7B98, {})),
		// D1 TRIM KNOB
		Packet(5, sizeof(double),         FindDMAAddy(hProcess, base + 0x03F95098, {})),
		// D1 TEMPO
		Packet(6, sizeof(double),         FindDMAAddy(hProcess, base + 0x03FBF5E0, { 0x160, 0x40, 0x78, 0xE0, 0xB0})),
		// D1 SONG ID
		Packet(7, sizeof(4),              FindDMAAddy(hProcess, base + 0x03FCB738, { 0x8, 0x240, 0xF0, 0x58, 0x0, 0x1C0, 0x1C })),
		// D1 IS PLAYING
		Packet(8, sizeof(unsigned char),  FindDMAAddy(hProcess, base + 0x03FCA690, { 0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x1A4 })),
		// D1 CURRENT TIME
		Packet(9, sizeof(double),         FindDMAAddy(hProcess, base + 0x03FCA690, { 0x138, 0x38, 0x68, 0x3E0, 0xF8, 0x0, 0x138 })),

		// D2 CHANNEL FADER
		Packet(10, sizeof(double),        FindDMAAddy(hProcess, base + 0x03F950B0, {})),
		// D2 CFX KNOB
		Packet(11, sizeof(double),        FindDMAAddy(hProcess, base + 0x03F626C8, { 0x28, 0x50, 0x128, 0x40, 0x10, 0xE0, 0xB0 })),
		// D2 LOW FILTER KNOB
		Packet(12, sizeof(double),        FindDMAAddy(hProcess, base + 0x03EA7BC0, {})),
		// D2 MID FILTER KNOB
		Packet(13, sizeof(double),        FindDMAAddy(hProcess, base + 0x03EA7BB8, {})),
		// D2 HIGH FILTER KNOB
		Packet(14, sizeof(double),        FindDMAAddy(hProcess, base + 0x03EA7BB0, {})),
		// D2 TRIM KNOB
		Packet(15, sizeof(double),        FindDMAAddy(hProcess, base + 0x03F950A0, {})),
		// D2 TEMPO
		Packet(16, sizeof(double),        FindDMAAddy(hProcess, base + 0x03F626C8, { 0x70, 0x10, 0x18, 0x40, 0x78, 0xE0, 0xB0 })),
		// D2 SONG ID
		Packet(17, sizeof(4),             FindDMAAddy(hProcess, base + 0x03F626C8, { 0x98, 0x20, 0x18, 0x10, 0x8, 0x3C0 })),
		// D2 IS PLAYING
		Packet(18, sizeof(unsigned char), FindDMAAddy(hProcess, base + 0x03FCA690, { 0x1D8, 0x8, 0x10, 0x60, 0x40, 0x150, 0x188 })),
		// D2 CURRENT TIME
		Packet(19, sizeof(double),        FindDMAAddy(hProcess, base + 0x03FACDB8, { 0xF0, 0x3E0, 0xF8, 0x8, 0xA0, 0x108, 0x138, })),
	};

	int sendResult = NULL;
	Packet* packet;
	std::vector<unsigned char> new_data;

	while (true) {
		for (unsigned int i = 0; i < packets.size(); i++) {
			packet = &packets[i];
			new_data.resize(packet->size);

			ReadProcessMemory(hProcess, (unsigned char*)packet->addr, new_data.data(), new_data.size(), nullptr);

			if (std::equal(packet->data.begin(), packet->data.end(), new_data.begin()) == FALSE) {
				packet->data = new_data;

				new_data.insert(new_data.begin(), packet->opcode);
				
				if (packet->opcode == 7 || packet->opcode == 8 || packet->opcode == 17 || packet->opcode == 18) {
					// TCP PACKETS
					sendResult = send(tcpSock, (char*)new_data.data(), new_data.size(), 0);
				}
				else {
					// UDP PACKETS
					sendResult = sendto(udpSock, (char*)new_data.data(), new_data.size(), 0, (sockaddr*)&server, sizeof(server));
				}
				
			}
		}

		if (sendResult == SOCKET_ERROR)
		{
			std::cout << "SOCKET_ERROR\n" << WSAGetLastError() << std::endl;
			return 0;
		}

		new_data.clear();
	}

	closesocket(tcpSock);
	closesocket(udpSock);
	WSACleanup();
	return 0;
}
