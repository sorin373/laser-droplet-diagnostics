#pragma once

#include "droplet/core/Image.hpp"
#include "droplet/core/Droplet.hpp"

namespace droplet
{
    struct Point
    {
        std::size_t x = 0;
        std::size_t y = 0;

        Point() = default;
    };

    struct Component
    {
        std::size_t area = 0;

        double sum_x = 0.0, sum_y = 0.0;
        double center_x = 0.0, center_y = 0.0;

        std::size_t min_x = 0, max_x = 0, min_y = 0, max_y = 0;

        std::size_t width = 0, height = 0;
        double aspect_ratio = 0.0;

        std::size_t perimeter = 0;
        double circularity = 0.0;

        std::size_t seed_x = 0, seed_y = 0;

        Component() = default;
    };

    Droplet detect_droplet(const Image &image);

    Component find_best_component(const Image &image);
    
    Image isolate_droplet(const Image &image);
}