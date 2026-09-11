#pragma once

#include "droplet/core/Image.hpp"
#include "droplet/core/Droplet.hpp"

namespace droplet
{
    struct Point
    {
        std::size_t x = 0;
        std::size_t y = 0;
    };

    Droplet_Set detect_droplets(const Image &image);
    
    Image isolate_droplets(const Image &image);
}