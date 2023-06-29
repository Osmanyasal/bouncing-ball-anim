#include <fstream>
#include "shader_utils.h"

const std::string ShaderUtils::load_shader(std::string shader_name,std::string extension, std::string shader_folder)
{
    std::string result = "";
    std::ifstream file(shader_folder + shader_name + extension);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            result += line + "\n";
        }
        file.close();
    }
    return result;
}