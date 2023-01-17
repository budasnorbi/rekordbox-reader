#pragma once
#include "ServerConnection.h"


class TcpClient : public ServerConnection
{

public:

	TcpClient()
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

	~TcpClient()
	{
		closesocket(tcpSock);
		WSACleanup();
	}

private:
	sockaddr_in server;
	SOCKET tcpSock;
	void connectToServer() override;

};

