#include "reader.hpp"
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

static Process *process = new Process();
static kern_return_t hProcess = process->Open((char*)"rekordbox");
static uint64_t base = process->get_base_address();

static std::vector<Packet> packets = {
        // 0 D1 CHANNEL FADER
        Packet(sizeof(double), FindDMAAddy(*process, base + 0x041931E0, {0x400, 0x0, 0x1B0, 0x20, 0x8, 0x18, 0x248})),
        // 5 D1 SONG ID
        Packet(4, FindDMAAddy(*process, base + 0x0415CA68, {0x70, 0x1A0, 0x8, 0x0, 0x1F8, 0x18})),
        // 7 D1 CURRENT TIME
        Packet(sizeof(double), FindDMAAddy(*process, base + 0x415CA68, {0x0, 0x250, 0x178, 0x0, 0x48, 0x108, 0x128})),

        // 9 D2 CHANNEL FADER
        Packet(sizeof(double), FindDMAAddy(*process, base + 0x041931E0, {0x400, 0x0, 0x1B0, 0x28, 0x8, 0x18, 0x248})),
        // 14 D2 SONG ID
        Packet(4, FindDMAAddy(*process, base + 0x0415CA68, {0x8, 0x2B0, 0x18, 0x658, 0x0, 0x1F8, 0x18})),
        // 16 D2 CURRENT TIME
        Packet(sizeof(double), FindDMAAddy(*process, base + 0x415CA68, {0x8, 0x1E0, 0x10, 0x110, 0x1F8, 0x148, 0x128}))
};
Packet *packet;

/*Changes *getData()
{

    Changes *changes = new Changes();
    
    for (size_t i = 0; i < packets.size(); i++)
    {
        packet = &packets[i];

        std::vector<unsigned char> readedData;
        readedData.resize(packet->size);

        process->Read(packet->addr, readedData.data(), readedData.size());
        changes->data.insert(changes->data.end(), readedData.begin(), readedData.end());
        readedData.clear();
    }
    
    return changes;
}*/

Changes *getData()
{
    Changes *changes = new Changes();
    std::vector<unsigned char> readedData;
    
    for (auto& packet : packets)
    {
        readedData.resize(packet.size);
        process->Read(packet.addr, readedData.data(), readedData.size());
        changes->data.insert(changes->data.end(), readedData.begin(), readedData.end());
    }
    
    return changes;
}
