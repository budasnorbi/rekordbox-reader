#include "ProcessReader.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>
#include <thread>



void readMemory(bool* running, ProcessReader* reader)
{
	while (*running)
	{
		reader->ReadMemory();
	}
}

// Sends packet on UDP server with artificial sleep
void sendUDP(bool* running, ProcessReader* reader)
{

	while (*running)
	{
		reader->sendOnUdpSocket();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

}

// Sends packet on TCP ( if awailable
void sendTCP(bool* running, ProcessReader* reader)
{

	while (*running)
	{
		reader->sendOnTcpSocket();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

int main() 
{

	std::cout << "REKORDBOX READER v0.5 (for Windows)" << std::endl;
	std::cout << "Supports rekordbox 6.6.8" << std::endl;

	bool running = true;
	std::vector<std::thread> threadpool = std::vector<std::thread>(3);

	ProcessReader reader = ProcessReader(L"rekordbox.exe");

	// Starting threads
	threadpool[0] = std::thread(readMemory, &running, &reader);
	threadpool[1] = std::thread(sendUDP, &running, &reader);
	threadpool[2] = std::thread(sendTCP, &running, &reader);

	while (running)
	{

		std::cout << "Enter exit for terminating the program" << std::endl;

		std::string text;

		std::getline(std::cin, text);

		std::cout << "You entered: " << text << std::endl;

		if(text == "exit" || text == "Exit")
		{
			running = false;
		}

		system("cls"); // System call for clearing console

	}

	for (size_t i = 0; i < threadpool.size(); i++)
	{
		threadpool[i].join();
	}

	return 0;

}