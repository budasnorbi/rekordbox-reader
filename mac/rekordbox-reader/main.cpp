#include "vm_mem.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

#define UDP_SERVER_PORT 54000
#define TCP_SERVER_PORT 54001
#define HOST "127.0.0.1"

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

uintptr_t FindDMAAddy(Process process, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        addr = process.Read<uintptr_t>(addr);
        addr += offsets[i];
    }
    return addr;
}


int main()
{
    std::cout << "REKORDBOX READER v0.2 (for macOS)" << std::endl;
    std::cout << "Supported rekordbox version: 6.6.8" << std::endl;

    // UDP CLIENT
    struct sockaddr_in cli_addr;
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0) {
        std::cout << "ERROR: creating an udp client for communication" << std::endl;
        exit(1);
    }

    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(UDP_SERVER_PORT);
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // TCP CLIENT
    struct sockaddr_in serverAddr;
    int tcpSock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSock < 0) {
        std::cout << "ERROR: creating a tcp client for communication" << std::endl;
        exit(1);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TCP_SERVER_PORT);
    
    if(connect(tcpSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0){
        std::cout << "Connecting to server..." << std::endl;
    }
    
    // INIT PROCESS READER
    Process *process = new Process();
    process->Open((char*)"rekordbox");
    
    uint64_t base = process->get_base_address();
    
    std::vector packets = {
          // D1 CHANNEL FADER
          Packet(0, sizeof(double), FindDMAAddy(*process, base + 0x03F8DDA0, {0x190, 0x10, 0x20, 0x18, 0x40, 0x28, 0x248})),
          // D1 CFX KNOB
          Packet(1, sizeof(double), FindDMAAddy(*process, base + 0x03F92DC8, {0x28, 0x50, 0x130, 0x100, 0x40, 0xA0, 0xDC0})),
          // D1 LOW FILTER KNOB
          Packet(2, sizeof(double), FindDMAAddy(*process, base + 0x03EE2840, {})),
          // D1 MID FILTER KNOB
          Packet(3, sizeof(double), FindDMAAddy(*process, base + 0x03EE2838, {})),
          // D1 HIGH FILTER KNOB
          Packet(4, sizeof(double), FindDMAAddy(*process, base + 0x03EE2830, {})),
          // D1 TRIM KNOB
          Packet(5, sizeof(double), FindDMAAddy(*process, base + 0x03F8D6B8, {})),
          // D1 TEMPO
          Packet(6, sizeof(double), FindDMAAddy(*process, base + 0x03F92DC8, {0x60, 0x630, 0x258, 0x730, 0x40, 0x40, 0x320})),
          // D1 SONG ID
          Packet(7, 4, FindDMAAddy(*process, base + 0x03F928B0, {0x8, 0x40, 0x8})),
          // D1 IS PLAYING
          Packet(8, sizeof(unsigned char), FindDMAAddy(*process, base + 0x03F92DC8, {0x60, 0x688, 0x248, 0x150, 0x58, 0xD44})),
          // D1 CURRENT TIME
          Packet(9, sizeof(double), FindDMAAddy(*process, base + 0x03F9FA58, {0x18, 0x188, 0x140, 0x8, 0x158, 0x140, 0x128})),
        
          // D2 CHANNEL FADER
          Packet(10, sizeof(double), FindDMAAddy(*process, base + 0x03FA0CF0, {0x838, 0x18, 0x40, 0x0, 0x40, 0x40, 0x248})),
          // D2 CFX KNOB
          Packet(11, sizeof(double), FindDMAAddy(*process, base + 0x03F92DC8, {0x28, 0xC0, 0x18, 0x108, 0x8, 0xE8, 0xB0})),
          // D2 LOW FILTER KNOB
          Packet(12, sizeof(double), FindDMAAddy(*process, base + 0x3EE2860, {})),
          // D2 MID FILTER KNOB
          Packet(13, sizeof(double), FindDMAAddy(*process, base + 0x3EE2858, {})),
          // D2 HIGH FILTER KNOB
          Packet(14, sizeof(double), FindDMAAddy(*process, base + 0x3EE2850, {})),
          // D2 TRIM KNOB
          Packet(15, sizeof(double), FindDMAAddy(*process, base + 0x03F8D6C0, {})),
          // D2 TEMPO
          Packet(16, sizeof(double), FindDMAAddy(*process, base + 0x03F92DC8, {0x60, 0x688, 0x290, 0x70, 0x40, 0x78, 0x990 })),
          // D2 SONG ID
          Packet(17, 4, FindDMAAddy(*process, base + 0x03FA0CF0, {0x788, 0x18, 0x168, 0x118, 0x58, 0x28, 0x850})),
          // D2 IS PLAYING
          Packet(18, sizeof(unsigned char), FindDMAAddy(*process, base + 0x03F9FA28, {0x0, 0x18, 0x48, 0x178, 0xE0, 0x210, 0x7B8})),
          // D2 CURRENT TIME
          Packet(19, sizeof(double), FindDMAAddy(*process, base + 0x03F9FA28, {0x0, 0x30, 0x8, 0x100, 0x8, 0xE0, 0x988})),
        
          // CROSS FADER
          Packet(20, sizeof(double), FindDMAAddy(*process, base + 0x03F92DC8, {0x28, 0x10, 0x208, 0x48, 0x8, 0x18, 0xE00}))
    };
    
    ssize_t sendResult = NULL;
    Packet* packet;
    std::vector<unsigned char> new_data;
    
    while(true){
        for(unsigned int i = 0; i < packets.size(); i++){
            packet = &packets[i];
            new_data.resize(packet->size);
            
            process->Read(packet->addr, new_data.data(), new_data.size());
            if(std::equal(packet->data.begin(), packet->data.end(), new_data.begin()) == FALSE){
                packet->data = new_data;
                new_data.insert(new_data.begin(), packet->opcode);
                
                if(packet->opcode == 7 || packet->opcode == 17 || packet->opcode == 8|| packet->opcode == 18){
                    sendResult = sendto(tcpSock, (char*)new_data.data(), new_data.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
                } else {
                    sendResult = sendto(udpSock, (char*)new_data.data(), new_data.size(), 0, (sockaddr*)&cli_addr, sizeof(cli_addr));
                }
               
            }
        }
        
        if(sendResult < 0){
            close(udpSock);
            return 0;
        }
    };
}
