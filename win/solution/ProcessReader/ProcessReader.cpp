#include "ProcessReader.h"

void ProcessReader::ReadMemory()
{
	this->readBinaryValues();
	this->readDoubleValues();
}

void ProcessReader::printToConsole()
{
	this->printBinaryValues();
	this->printDoubleValues();
}

void ProcessReader::sendOnUdpSocket()
{

	for (size_t i = 0; i < this->recordBoxDoubles.size(); i++)
	{
		if (!recordBoxDoubles[i].locked && recordBoxDoubles[i].shouldSend) {


			recordBoxDoubles[i].shouldSend = false;

			std::vector<unsigned char> sendData = this->recordBoxDoubles[i].data;
			sendData.insert(sendData.begin(), this->recordBoxDoubles[i].opCode);

			int result = this->udpClient.sendPacket((char*)sendData.data(), sendData.size());

			if (result == SOCKET_ERROR)
			{
				std::cout << "SOCKET_ERROR\n" << WSAGetLastError() << std::endl;

				throw "Error";
			}

		}

	}

}

void ProcessReader::sendOnTcpSocket()
{
	for (size_t i = 0; i < this->recordBoxBites.size(); i++)
	{
		if (!recordBoxBites[i].locked && this->tcpClient.connected && recordBoxBites[i].shouldSend) {

			recordBoxBites[i].shouldSend = false;

			std::vector<unsigned char> sendData = this->recordBoxBites[i].data;
			sendData.insert(sendData.begin(), this->recordBoxBites[i].opCode);

			int result = this->tcpClient.sendPacket((char*)sendData.data(), sendData.size());

			if (result == SOCKET_ERROR)
			{
				std::cout << "SOCKET_ERROR\n" << WSAGetLastError() << std::endl;

				throw "Error";
			}

		}
	}
}

DWORD ProcessReader::GetProcessId()
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, processName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}

	CloseHandle(hSnap);
	return procId;
}

uintptr_t ProcessReader::GetModuleBaseAddress()
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry)) {
			do
			{
				if (!_wcsicmp(modEntry.szModule, this->processName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t ProcessReader::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(this->hProcess, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void ProcessReader::readBinaryValues()
{
	for (size_t i = 0; i < this->recordBoxBites.size(); i++)
	{

		recordBoxBites[i].locked = true;


		if (recordBoxBites[i].locked)
		{

			BYTE* _addr = (BYTE*)recordBoxBites[i].addr;

			std::vector<unsigned char> new_data;

			new_data.resize(recordBoxBites[i].size);

			ReadProcessMemory(hProcess, (BYTE*)recordBoxBites[i].addr, new_data.data(), new_data.size(), nullptr);

			if (!std::equal(recordBoxBites[i].data.begin(), recordBoxBites[i].data.end(), new_data.begin()))
			{
				recordBoxBites[i].shouldSend = true;
				recordBoxBites[i].data = new_data;
			}

			recordBoxBites[i].locked = false;
		}

	}
}

void ProcessReader::readDoubleValues()
{

	for (size_t i = 0; i < this->recordBoxDoubles.size(); i++)
	{
		recordBoxDoubles[i].locked = true;

		if (recordBoxDoubles[i].locked)
		{

			BYTE* _addr = (BYTE*)recordBoxDoubles[i].addr;

			std::vector<unsigned char> new_data;

			new_data.resize(recordBoxDoubles[i].size);

			ReadProcessMemory(hProcess, (BYTE*)recordBoxDoubles[i].addr, new_data.data(), new_data.size(), nullptr);

			if(!std::equal(recordBoxDoubles[i].data.begin(), recordBoxDoubles[i].data.end(),new_data.begin()))
			{
				recordBoxDoubles[i].shouldSend = true;
				recordBoxDoubles[i].data = new_data;
			}

			

			recordBoxDoubles[i].locked = false;
		}



	}
}

void ProcessReader::printBinaryValues()
{
	for (size_t i = 0; i < this->recordBoxBites.size(); i++)
	{
		bool d_data;

		memcpy(&d_data, recordBoxBites[i].data.data(), recordBoxBites[i].size);

		std::cout << recordBoxBites[i].description << " : " << (d_data % 2) << std::endl; //Convert to binary
	}
}

void ProcessReader::printDoubleValues()
{
	for (size_t i = 0; i < this->recordBoxDoubles.size(); i++)
	{
		double d_data;

		memcpy(&d_data, recordBoxDoubles[i].data.data(), recordBoxDoubles[i].size);

		std::cout << recordBoxDoubles[i].description << " : " << d_data << std::endl; //Convert to binary
	}



}

