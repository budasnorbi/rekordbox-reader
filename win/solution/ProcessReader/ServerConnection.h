#pragma once
#include<stdio.h>
#include<winsock2.h>
#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define UDP_SERVER_PORT 54000
#define TCP_SERVER_PORT 54001


struct ServerConnection
{

public:
	virtual int sendPacket(char* message, int packetSize) = 0;

private:
	virtual void connectToServer() = 0;

protected:
	std::string server_host = "127.0.0.1";

};

