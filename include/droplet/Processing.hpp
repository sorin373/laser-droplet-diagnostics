#pragma once

#include "core/Image.hpp"
#include <cstdint>

namespace droplet
{
    void threshold(Image &image, std::uint8_t limit);

    void fill_holes(Image &image);
}