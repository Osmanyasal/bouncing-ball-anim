#include "texture_utils.h"

PPM TextureUtils::load_ppm(std::string file_name)
{   
    PPM ppm;
    std::ifstream file(file_name, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return {};
    }

    std::string format;
    file >> format;
    if (format != "P3")
    {
        std::cerr << "Invalid PPM format: " << format << std::endl;
        return {};
    }

    file >> ppm.width >> ppm.height;
    int maxValue;
    file >> maxValue;
    file.ignore(1); // Ignore the newline character

    // Resize the pixel buffer
    ppm.pixels.resize(ppm.width * ppm.height * 3);

     // Read the pixel data
    for (int i = 0; i < ppm.width * ppm.height * 3; ++i) {
        int value;
        file >> value;
        ppm.pixels[i] = static_cast<unsigned char>(value);
    }
    if (!file)
    {
        std::cerr << "Failed to read pixel data from file: " << file_name << std::endl;
        return {};
    }

    return ppm;
}