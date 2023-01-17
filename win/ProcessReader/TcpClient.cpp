#include "TcpClient.h"

int TcpClient::sendPacket(char* message, int packetSize)
{
	return  send(tcpSock, message, packetSize, 0);
}

void TcpClient::connectToServer()
{

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
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(tcpSock);
		WSACleanup();
		

		throw "Error";
	}

}
