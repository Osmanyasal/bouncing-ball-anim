#ifndef _INCLUDE__PPM_H
#define _INCLUDE__PPM_H

#include <vector>
struct PPM
{
    // Global variables
    int width, height;
    std::vector<unsigned char> pixels;
};

#endif