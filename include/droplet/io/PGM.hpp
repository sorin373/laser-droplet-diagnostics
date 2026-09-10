#pragma once

#include "droplet/core/Image.hpp"

namespace droplet
{
    Image loadPGM(const char *path);

    void savePGM(const Image& image, const char *path);
}