#include "droplet/Synthetic.hpp"

namespace droplet
{
    Image generate_circle(std::size_t width, std::size_t height, double center_x, double center_y, double radius)
    {
        Image image(width, height);

        for (std::size_t i = 0; i < height; ++i)
            for (std::size_t j = 0; j < width; ++j)
            {
                double dx = static_cast<double>(j) - center_x,
                    dy = static_cast<double>(i) - center_y;

                if (dx * dx + dy * dy <= radius * radius)
                    image.at(j, i) = 0;
            }
        
        return image;
    }
}