#include "droplet/Synthetic.hpp"
#include "droplet/Processing.hpp"
#include "droplet/io/PGM.hpp"
#include <iostream>

int main()
{
    droplet::Image image =
        droplet::generate_circle(
            100,
            100,
            50.0,
            50.0,
            20.0
        );

    // create an artificial white hole
    for (std::size_t y = 47; y <= 53; ++y)
    {
        for (std::size_t x = 47; x <= 53; ++x)
        {
            image.at(x, y) = 255;
        }
    }

    droplet::savePGM(
        image,
        "results/before_fill.pgm"
    );

    std::cout << "Fill\n";
    droplet::fill_holes(image);

    std::cout << "Finished!\n";

    droplet::savePGM(
        image,
        "results/after_fill.pgm"
    );

    return 0;
}