#include "droplet/io/TIFF.hpp"
#include "droplet/Processing.hpp"
#include "droplet/core/Droplet.hpp"
#include "droplet/detection/Droplet_Detector.hpp"

#include <iostream>

int main()
{
    try
    {
        droplet::Image image = droplet::loadTIFF(
                "data/test/24072026_20um_0.2ml_min_105.98.73Hz_Vpp2_delph_0.012_PM_MOD_87.5kHz/ns_probe__22665537__20260724_183446439_0653.tiff"
            );

        std::cout << "Loaded TIFF\n";

        droplet::threshold(image, 128);

        std::cout << "Applied threshold\n";
        
        droplet::Droplet_Set droplets = droplet::detect_droplets(image);

        std::cout << "Detected droplets: "
          << droplets.size()
          << '\n';

    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}