#pragma once

#include <cstddef>

namespace droplet
{
    struct Droplet
    {
        double x = 0.0, y = 0.0;

        std::size_t area_pixels = 0;
        double equivalent_diameter_pixels = 0.0;

        Droplet(double _x, double _y, std::size_t _area_pixels, double _equivalent_diameter_pixels)
            : x(_x), y(_y), area_pixels(_area_pixels), equivalent_diameter_pixels(_equivalent_diameter_pixels) { }
    };
}