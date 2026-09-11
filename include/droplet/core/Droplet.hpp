#pragma once

#include <cstddef>

namespace droplet
{
    struct Droplet
    {
        double center_x = 0.0;
        double center_y = 0.0;

        std::size_t area_pixels = 0;
        double equivalent_diameter_pixels = 0.0;
    };
}