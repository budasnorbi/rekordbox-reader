#pragma once
#include <vector>
#ifndef reader_hpp
#define reader_hpp
#include <stdio.h>
#endif

struct Changes
{
    std::vector<unsigned char> data = {};
};

extern "C" {
    Changes *getData();
}
