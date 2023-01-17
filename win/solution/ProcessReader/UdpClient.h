#pragma once
#include "ServerConnection.h"




class UdpClient : public ServerConnection
{

public:

	UdpClient()
	{	

		WSAData data;
		WORD version = MAKEWORD(2, 2);

		int wsResult = WSAStartup(version, &data);
		if (wsResult != 0) {
			throw "Error";
		}

		this->connectToServer();
	}

	int sendPacket(char* message, int packetSize) override;

	~UdpClient()
	{
		closesocket(udpSocket);
		WSACleanup();
	}

private:
	sockaddr_in server;
	SOCKET udpSocket;

	void connectToServer() override;





};

