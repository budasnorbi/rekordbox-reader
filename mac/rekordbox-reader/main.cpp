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
#define HOST "127.0.0.1"

class Packet{
public:
    unsigned int size;
    char opcode;
    uintptr_t addr;
    std::vector<std::byte> data;
    
    Packet(char _opcode, unsigned int _size, uintptr_t _addr){
        opcode = _opcode;
        addr = _addr;
        data = std::vector<std::byte>(_size);
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
    std::cout << "REKORDBOX READER v0.1 (for macOS)" << std::endl;
    std::cout << "Supported rekordbox version: 6.6.8" << std::endl;
    
    int sockfd;
    struct sockaddr_in cli_addr;
    //socklen_t addr_size;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cout << "ERROR: creating an endpoint for communication" << std::endl;
    }
    
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(UDP_SERVER_PORT);
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // INIT PROCESS READER
    Process *process = new Process();
    process->Open((char*)"rekordbox");
    //pid_t pid = process->get_pid();
    
    uint64_t base = process->get_base_address();
    
    std::vector packets = {
        // D1 CHANNEL FADER
        Packet(0, sizeof(double), FindDMAAddy(*process, base + 0x03F8DDA0, {0x190, 0x10, 0x20, 0x18, 0x40, 0x28, 0x248})),
        // D1 LOW FILTER KNOB
        Packet(1, sizeof(double), FindDMAAddy(*process, base + 0x03F8DDA0, {0x150,0x10,0x68,0x38,0x40,0x10,0x778})),
        // D1 MID FILTER KNOB
        Packet(2, sizeof(double), FindDMAAddy(*process, base + 0x03FA0CF0, {0xD0, 0x40, 0x10, 0x168, 0x0, 0x10, 0x770})),
        // D1 HIGH FILTER KNOB
        Packet(3, sizeof(double), FindDMAAddy(*process, base + 0x03FA0CF0, {0xE0, 0x18, 0x40, 0x18, 0x40, 0x18, 0x4F8})),
        // D1 CURRENT TIME
        Packet(4, sizeof(double), FindDMAAddy(*process, base + 0x03F9FA58, {0x18, 0x188, 0x140, 0x8, 0x158, 0x140, 0x128})),
        // D1 IS PLAYING
        Packet(5, sizeof(std::byte), FindDMAAddy(*process, base + 0x03F92DC8, {0x60, 0x688, 0x248, 0x150, 0x58, 0xD44})),
        // D2 CHANNEL FADER
        Packet(6, sizeof(double), FindDMAAddy(*process, base + 0x03FA0CF0, {0x838, 0x18, 0x40, 0x0, 0x40, 0x40, 0x248}))
    };
    
    while(true){
        ssize_t sendResult = NULL;
        for(unsigned int i = 0; i < packets.size(); i++){
            Packet &packet = packets[i];
            
            std::vector<std::byte> data(packet.size);
            process->Read(packet.addr, data.data(), data.size());
            if(packet.data != data){
                packet.data = data;
                
                data.insert(data.begin(), (std::byte)packet.opcode);
                sendResult = sendto(sockfd, data.data(), data.size(), 0, (sockaddr*)&cli_addr, sizeof(cli_addr));
            }
        }
        
        if(sendResult < 0){
            close(sockfd);
            return 0;
        }
    };
}
