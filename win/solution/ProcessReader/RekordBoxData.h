#pragma once
#include <windows.h>
#include <vector>
#include <string>

template<typename T>
class RekordBoxData
{
public:
	int opCode;
	unsigned int size;
	uintptr_t addr;
	std::vector<unsigned char> data;
	std::string description;
	bool locked;
	bool shouldSend;
	

	RekordBoxData(std::string _description, int _opCode, uintptr_t _addr)
	{
		this->description = _description;
		this->size = sizeof(T);
		this->addr = _addr;
		this->opCode = _opCode;

		this->locked = false;
		this->shouldSend = false;

		data = std::vector<unsigned char>(this->size);
	}




private:
	
};

