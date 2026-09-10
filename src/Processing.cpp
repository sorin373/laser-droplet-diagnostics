#include "droplet/Processing.hpp"
#include "droplet/detection/Droplet_Detector.hpp"

namespace droplet
{
    void threshold(Image &image, std::uint8_t limit)
    {
        for (std::size_t i = 0; i < image.height; ++i)
            for (std::size_t j = 0; j < image.width; ++j)
            {
                std::uint8_t temp = image.at(j, i);

                if (temp >= limit)
                    image.at(j, i) = 255;
                else
                    image.at(j, i) = 0;
            }
    }

    void fill_holes(Image &image)
    {
        if (image.width == 0 || image.height == 0)
            return;

        const std::size_t count = image.width * image.height;

        bool *outside = new bool[count];

        for (std::size_t i = 0; i < count; ++i)
            outside[i] = false;

        Point *stack = new Point[count];
        std::size_t stack_size = 0;

        // top and bottom border
        for (std::size_t x = 0; x < image.width; ++x)
        {
            if (image.at(x, 0) == 255)
            {
                std::size_t index = x;

                if (!outside[index])
                {
                    outside[index] = true;
                    stack[stack_size++] = {x, 0};
                }
            }

            if (image.height > 1 && image.at(x, image.height - 1) == 255)
            {
                std::size_t index = (image.height - 1) * image.width + x;

                if (!outside[index])
                {
                    outside[index] = true;
                    stack[stack_size++] = {x, image.height - 1};
                }
            }
        }

        // left and right border
        for (std::size_t y = 0; y < image.height; ++y)
        {
            if (image.at(0, y) == 255)
            {
                std::size_t index = y * image.width;

                if (!outside[index])
                {
                    outside[index] = true;
                    stack[stack_size++] = {0, y};
                }
            }

            if (image.width > 1 && image.at(image.width - 1, y) == 255)
            {
                std::size_t index = y * image.width + (image.width - 1);

                if (!outside[index])
                {
                    outside[index] = true;
                    stack[stack_size++] = {image.width - 1, y};
                }
            }
        }

        while (stack_size > 0)
        {
            Point p = stack[--stack_size];

            // left
            if (p.x > 0)
            {
                std::size_t nx = p.x - 1;
                std::size_t ny = p.y;
                std::size_t index = ny * image.width + nx;

                if (!outside[index] && image.at(nx, ny) == 255)
                {
                    outside[index] = true;
                    stack[stack_size++] = {nx, ny};
                }
            }

            // right
            if (p.x + 1 < image.width)
            {
                std::size_t nx = p.x + 1;
                std::size_t ny = p.y;
                std::size_t index = ny * image.width + nx;

                if (!outside[index] && image.at(nx, ny) == 255)
                {
                    outside[index] = true;
                    stack[stack_size++] = {nx, ny};
                }
            }

            // up
            if (p.y > 0)
            {
                std::size_t nx = p.x;
                std::size_t ny = p.y - 1;
                std::size_t index = ny * image.width + nx;

                if (!outside[index] && image.at(nx, ny) == 255)
                {
                    outside[index] = true;
                    stack[stack_size++] = {nx, ny};
                }
            }

            // down
            if (p.y + 1 < image.height)
            {
                std::size_t nx = p.x;
                std::size_t ny = p.y + 1;
                std::size_t index = ny * image.width + nx;

                if (!outside[index] && image.at(nx, ny) == 255)
                {
                    outside[index] = true;
                    stack[stack_size++] = {nx, ny};
                }
            }
        }

        // any white pixel not connected to the border is a hole
        for (std::size_t y = 0; y < image.height; ++y)
        {
            for (std::size_t x = 0; x < image.width; ++x)
            {
                std::size_t index = y * image.width + x;

                if (image.at(x, y) == 255 && !outside[index])
                    image.at(x, y) = 0;
            }
        }

        delete[] stack;
        delete[] outside;
    }
}