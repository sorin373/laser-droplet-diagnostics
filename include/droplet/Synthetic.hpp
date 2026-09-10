#pragma once

#include "core/Image.hpp"
#include <cstddef>

namespace droplet
{
    Image generate_circle(std::size_t width, std::size_t height, double center_x, double center_y, double radius);
}