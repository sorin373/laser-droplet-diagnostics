#include "droplet/detection/Droplet_Detector.hpp"

#include <iostream>
#include <stdexcept>
#include <cmath>

namespace droplet
{
    constexpr double PI = 3.14159265358979323846;

    Droplet find_component(const Image &image, bool *visited, std::size_t start_x, std::size_t start_y)
    {
        Droplet component;

        component.min_x = component.max_x = start_x;
        component.min_y = component.max_y = start_y;

        component.seed_x = start_x;
        component.seed_y = start_y;

        Point *stack = new Point[image.width * image.height];

        std::size_t stack_size = 0;

        stack[stack_size] = {start_x, start_y};
        ++stack_size;

        while(stack_size > 0)
        {
            Point p = stack[--stack_size];
            std::size_t index = p.y * image.width + p.x;

            if (visited[index])
                continue;

            visited[index] = true;

            if (image.at(p.x, p.y) != 0)
                continue;

            ++component.area;

            component.sum_x += p.x;
            component.sum_y += p.y;

            if (p.x < component.min_x)
                component.min_x = p.x;
            else if (p.x > component.max_x)
                component.max_x = p.x;

            if (p.y < component.min_y)
                component.min_y = p.y;
            else if (p.y > component.max_y)
                component.max_y = p.y;

            if (p.x > 0)
                stack[stack_size++] = {p.x - 1, p.y};

            if (p.x + 1 < image.width)
                stack[stack_size++] = {p.x + 1, p.y};
            
            if (p.y > 0)
                stack[stack_size++] = {p.x, p.y - 1};

            if (p.y + 1 < image.height)
                stack[stack_size++] = {p.x, p.y + 1};

            if (p.x == 0 || image.at(p.x - 1, p.y) != 0)
                ++component.perimeter;
            
            if (p.x + 1 >= image.width || image.at(p.x + 1, p.y) != 0)
                ++component.perimeter;

            if (p.y == 0 || image.at(p.x, p.y - 1) != 0)
                ++component.perimeter;

            if (p.y + 1 >= image.height || image.at(p.x, p.y + 1) != 0)
                ++component.perimeter;
        }

        delete[] stack;

        if (component.area > 0)
        {
            component.center_x = component.sum_x / component.area;
            component.center_y = component.sum_y / component.area;

            component.width = component.max_x - component.min_x + 1;
            component.height = component.max_y - component.min_y + 1;

            component.aspect_ratio = static_cast<double>(component.width) / static_cast<double>(component.height);

            component.circularity = 4.0 * PI * static_cast<double>(component.area) / (static_cast<double>(component.perimeter) * static_cast<double>(component.perimeter));
        }

        return component;
    }

    Droplet_Set detect_droplets(const Image &image)
    {
        Droplet_Set ds;

        const std::size_t count = image.width * image.height;

        bool *visited = new bool[count];

        for (std::size_t i = 0; i < count; ++i)
            visited[i] = false;

        for (std::size_t i = 0; i < image.height; ++i)
            for (std::size_t j = 0; j < image.width; ++j)
            {
                const std::size_t index = i * image.width + j;

                if (visited[index] || image.at(j, i) != 0)
                    continue;

                Droplet c = find_component(image, visited, j, i);

                bool plausible_droplet = c.area > 100 && c.aspect_ratio > 0.8 && c.aspect_ratio < 1.2 && c.circularity > 0.2;

                if (plausible_droplet)
                    ds.push_back(c);
            }

        delete[] visited;

        if (ds.size() == 0)
            throw std::runtime_error("No droplet found!\n");

        return ds;
    }

    Image isolate_droplets(const Image &image, const Droplet &droplet)
    {
        Image result(image.width, image.height);

        std::size_t count = image.width * image.height;

        bool *visited = new bool[count];

        for (std::size_t i; i < count; ++i)
            visited[i] = false;

        Point *stack = new Point[count];
        std::size_t stack_size = 0;

        std::size_t start_x = droplet.seed_x;
        std::size_t start_y = droplet.seed_y;

        stack[stack_size] = {start_x, start_y};
        ++stack_size;

        while (stack_size > 0)
        {
            Point p = stack[--stack_size];

            std::size_t index = p.y * image.width + p.x;

            if (visited[index])
                continue;

            visited[index] = true;

            if (image.at(p.x, p.y) != 0)
                continue;

            result.at(p.x, p.y) = 0;

            if (p.x > 0)
                stack[stack_size++] = {p.x - 1, p.y};

            if (p.x + 1 < image.width)
                stack[stack_size++] = {p.x + 1, p.y};

            if (p.y > 0)
                stack[stack_size++] = {p.x, p.y - 1};

            if (p.y + 1 < image.height)
                stack[stack_size++] = {p.x, p.y + 1};
        }

        delete[] stack;
        delete[] visited;
        
        return result;
    }
}