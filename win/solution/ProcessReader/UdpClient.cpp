#include "UdpClient.h"

int UdpClient::sendPacket(char* message, int packetSize)
{
	return sendto(this->udpSocket, message, packetSize, 0, (sockaddr*)&server, sizeof(server));
}

void UdpClient::connectToServer()
{
	this->udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	this->server.sin_family = AF_INET;
	this->server.sin_port = htons(UDP_SERVER_PORT);

	inet_pton(AF_INET, this->server_host.c_str(), &this->server.sin_addr);
}
