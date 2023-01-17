#include "TcpClient.h"

int TcpClient::sendPacket(char* message, int packetSize)
{
	return  send(tcpSock, message, packetSize, 0);
}

void TcpClient::connectToServer()
{

	this->connected = true; // Assume we connect successfully

	tcpSock = socket(AF_INET, SOCK_STREAM, 0);

	if (tcpSock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		

		throw "Error";
	}

	this->server.sin_family = AF_INET;
	this->server.sin_port = htons(TCP_SERVER_PORT);
	inet_pton(AF_INET, this->server_host.c_str(), &this->server.sin_addr);

	
	int connResult = connect(tcpSock, (sockaddr*)&this->server, sizeof(this->server));

	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Cannot connect to TCP server, error code: " << WSAGetLastError() << std::endl;
		closesocket(tcpSock);
		WSACleanup();
		
		this->connected = false;		
	}

	
	return;

}
