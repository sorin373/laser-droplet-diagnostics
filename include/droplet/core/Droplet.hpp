#pragma once

#include <cstddef>

namespace droplet
{
    struct Droplet
    {
        std::size_t area = 0;

        double sum_x = 0.0, sum_y = 0.0;

        double center_x = 0.0, center_y = 0.0;

        std::size_t min_x = 0, max_x = 0, min_y = 0, max_y = 0;

        std::size_t width = 0, height = 0;
        double aspect_ratio = 0.0;

        std::size_t perimeter = 0;
        double circularity = 0.0;
        double equivalent_diameter_pixels;

        std::size_t seed_x = 0, seed_y = 0;
    };

    class Droplet_Set
    {
    public:
        Droplet_Set()
            : _droplets(new Droplet[5]), _size(0), _capacity(5) { }

        void resize(std::size_t new_size)
        {
            Droplet *temp = new Droplet[new_size];

            for (std::size_t i = 0; i < this->_size; ++i)
                temp[i] = this->_droplets[i];

            delete[] _droplets;

            this->_droplets = temp;
            this->_capacity = new_size;
        }

        void push_back(const Droplet &droplet)
        {
            if (this->_size >= this->_capacity)
                this->resize(this->_capacity + (this->_capacity + 1) / 2);
            
            this->_droplets[this->_size++] = droplet;
        }

        std::size_t size() const noexcept
        { return this->_size; }

        std::size_t capacity() const noexcept
        { return this->_capacity; }

        Droplet* droplets() noexcept
        { return this->_droplets; }

        const Droplet* droplets() const noexcept
        { return this->_droplets; }

        ~Droplet_Set()
        {
            delete[] _droplets;
            this->_droplets = nullptr;
        }

    private:
        Droplet     *_droplets;
        std::size_t _size;
        std::size_t _capacity;
    };
}