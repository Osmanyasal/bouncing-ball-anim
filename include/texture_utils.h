#ifndef _INCLUDE__TEXTURE_UTILS_H
#define _INCLUDE__TEXTURE_UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "ppm.h"

/**
 * @brief Very simple shader utils class that reads shaders from the location ./shaders/ by default
 * 
 */
class TextureUtils
{
public:
    static PPM load_ppm(std::string file_name);

private:
    TextureUtils();
    ~TextureUtils();
};

#endif