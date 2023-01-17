#include "ProcessReader.h"
#include <iostream>
#include<stdio.h>
#include <string>


int main() {


	ProcessReader reader = ProcessReader(L"rekordbox.exe");

	while (true)
	{
		reader.ReadMemory();
		//reader.printToConsole();

		reader.sendOnUdpSocket();

	}

	return 0;

}