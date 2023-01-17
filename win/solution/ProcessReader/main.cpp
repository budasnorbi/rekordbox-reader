#include "ProcessReader.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>



std::vector<std::thread> threadpool;
bool running;
ProcessReader reader;


int main() {

	running = false;
	threadpool = std::vector<std::thread>(3);

	reader = ProcessReader(L"rekordbox.exe");

	while (true)
	{
		reader.ReadMemory();
		//reader.printToConsole();

		reader.sendOnUdpSocket();
		reader.sendOnTcpSocket();

	}

	return 0;

}