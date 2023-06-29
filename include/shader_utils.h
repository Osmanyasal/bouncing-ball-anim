#ifndef _INCLUDE__SHADER_UTILS_H
#define _INCLUDE__SHADER_UTILS_H

#include <iostream>
#include <string>

/**
 * @brief Very simple shader utils class that reads shaders from the location ./shaders/ by default
 * 
 */
class ShaderUtils
{
public:
    static const std::string load_shader(std::string shader_name,std::string extension = ".glsl", std::string shader_folder = "./shaders/");

private:
    ShaderUtils();
    ~ShaderUtils();
};

#endif