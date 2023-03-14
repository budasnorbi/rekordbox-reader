#pragma once
#include <vector>
#include <variant>

#ifdef READER_EXPORTS
#define READER_API __declspec(dllexport)
#else
#define READER_API __declspec(dllimport)
#endif

struct Changes {
	std::vector<unsigned char> data = {};
};

extern "C" READER_API Changes *getData();
