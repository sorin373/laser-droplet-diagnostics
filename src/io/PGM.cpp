#include "droplet/io/PGM.hpp"

#include <fstream>
#include <stdexcept>

namespace droplet
{
    Image loadPGM(const char *path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file)
            throw std::runtime_error("Could not opern PGM file!\n");

        char magic[3];

        file >> magic;

        if (magic[0] != 'P' || magic[1] != '5')
            throw std::runtime_error("Unsupported PGM format!\n");

        std::size_t width, height;
        int max_value;

        file >> width >> height;
        file >> max_value;

        if (max_value != 255)
            throw std::runtime_error("Only 8-bit PGM files are supported!\n");        

        file.get();

        Image image(width, height);

        for (std::size_t i = 0; i < height; ++i)
            for (std::size_t j = 0; j < width; ++j) // j -> x
            {
                char pixel;

                file.read(&pixel, 1);

                if (!file)
                    throw std::runtime_error("Unexpected EOF!\n");
                
                image.at(j, i) = static_cast<std::uint8_t>(static_cast<unsigned char>(pixel));
            }

        return image;
    }

    void savePGM(const Image& image, const char *path)
    {
        std::ofstream file(path, std::ios::binary);

        if (!file)
            throw std::runtime_error("Could not create PGM file!\n");

        file << "P5\n" << image.width << " " << image.height << "\n" << "255\n";

        for (std::size_t i = 0; i < image.height; ++i)
            for (std::size_t j = 0; j < image.width; ++j)
            {
                const char pixel = static_cast<char>(image.at(j, i));

                file.write(&pixel, 1);
            }

        if (!file)
            throw std::runtime_error("Could not write PGM file!\n");
    }
}