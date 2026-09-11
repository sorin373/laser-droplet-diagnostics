#include <droplet/io/TIFF.hpp>
#include <droplet/io/PGM.hpp>

#include <iostream>

int main()
{
    try
    {
        std::cout << "Starting!\n";

        droplet::Image image =
            droplet::loadTIFF(
                "data/test/24072026_20um_0.2ml_min_105.98.73Hz_Vpp2_delph_0.012_PM_MOD_87.5kHz/ns_probe__22665537__20260724_183446439_0653.tiff"
            );

        std::cout << "Loaded TIFF\n";
        std::cout << "Width: " << image.width << '\n';
        std::cout << "Height: " << image.height << '\n';

        droplet::savePGM(
            image,
            "results/from_tiff.pgm"
        );

        std::cout << "Saved PGM\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}