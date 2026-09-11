#include "droplet/io/TIFF.hpp"

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace droplet
{
    namespace
    {
        enum class ByteOrder
        {
            LittleEndian,
            BigEndian
        };

        std::uint16_t read_u16(std::ifstream &file, ByteOrder order)
        {
            unsigned char bytes[2];

            file.read(reinterpret_cast<char*>(bytes), 2);

            if (!file)
                throw std::runtime_error("Unexpected EOTF!\n");

            if (order == ByteOrder::LittleEndian)
                return static_cast<std::uint16_t>(bytes[0]) | (static_cast<std::uint16_t>(bytes[1]) << 8);

            return (static_cast<std::uint16_t>(bytes[0]) << 8) | static_cast<std::uint16_t>(bytes[1]);            
        }

        std::uint32_t read_u32(std::ifstream &file, ByteOrder order)
        {
            unsigned char bytes[4];

            file.read(reinterpret_cast<char*>(bytes), 4);

            if (!file)
                throw std::runtime_error("Unexpected EOTF!\n");

            if (order == ByteOrder::LittleEndian)
                return static_cast<std::uint32_t>(bytes[0]) |
                    (static_cast<std::uint32_t>(bytes[1]) << 8) |
                    (static_cast<std::uint32_t>(bytes[2]) << 16) |
                    (static_cast<std::uint32_t>(bytes[3]) << 24);

            return (static_cast<std::uint32_t>(bytes[0]) << 24) |
                (static_cast<std::uint32_t>(bytes[1]) << 16) |
                (static_cast<std::uint32_t>(bytes[2]) << 8) |
                static_cast<std::uint32_t>(bytes[3]);
        }
    }

    struct IFDEntry
    {
        std::uint16_t tag;
        std::uint16_t type;
        std::uint32_t count;
        std::uint32_t value_or_offset;
    };    

    std::uint32_t get_inline_value(const IFDEntry &entry, ByteOrder order)
    {
        if (entry.type == 3 && entry.count == 1)
        {
            if (order == ByteOrder::BigEndian)
                return entry.value_or_offset >> 16;

            return entry.value_or_offset & 0xFFFF;
        }

        if (entry.type == 4 && entry.count == 1)
            return entry.value_or_offset;

        throw std::runtime_error("Unsupported inline TIFF value!\n");
    }

    std::uint32_t* read_TIFF_array(std::ifstream &file, ByteOrder order, std::uint32_t offset, std::uint32_t count, std::uint16_t type)
    {
        std::cout
        << "read_TIFF_array called with "
        << "offset=" << offset
        << " count=" << count
        << " type=" << type
        << '\n';

        file.clear();
        file.seekg(offset);

        if (!file)
            throw std::runtime_error("Invalid TIFF array offset!\n");

        std::uint32_t *values = new std::uint32_t[count];

        for (std::uint32_t i = 0; i < count; ++i)
            if (type == 3)
                values[i] = read_u16(file, order);
            else if (type == 4)
                values[i] = read_u32(file, order);
            else
            {
                delete[] values;

                throw std::runtime_error("Unsupported TIFF array type!\n");
            }

        return values;
    }

    Image loadTIFF(const char *path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file)
            throw std::runtime_error("Could not open TIFF file!\n");

        char byte_order_chars[2];

        file.read(byte_order_chars, 2);

        if (!file)
            throw std::runtime_error("Invalid TIFF header!\n");

        ByteOrder order;

        if (byte_order_chars[0] == 'I' && byte_order_chars[1] == 'I')
            order = ByteOrder::LittleEndian;
        else if (byte_order_chars[0] == 'M' && byte_order_chars[1] == 'M')
            order = ByteOrder::BigEndian;
        else
            throw std::runtime_error("Invalid TIFF byte order!\n");

        const std::uint16_t magic = read_u16(file, order);

        if (magic != 42)
            throw std::runtime_error("Invalid TIFF magic number!\n");

        const std::uint32_t ifd_offset = read_u32(file, order);

        file.seekg(ifd_offset);

        std::uint16_t entry_count = read_u16(file, order);

        std::uint32_t width = 0;
        std::uint32_t height = 0;

        std::uint16_t bits_per_sample = 0;
        std::uint16_t compression = 0;
        std::uint16_t photometric = 0;
        std::uint16_t samples_per_pixel = 0;

        std::uint32_t rows_per_strip = 0;

        std::uint32_t strip_offsets_offset = 0;
        std::uint32_t strip_offsets_count = 0;
        std::uint16_t strip_offsets_type = 0;

        std::uint32_t strip_byte_counts_offset = 0;
        std::uint32_t strip_byte_counts_count = 0;
        std::uint16_t strip_byte_counts_type = 0;

        for (std::uint16_t i = 0; i < entry_count; ++i)
        {
            IFDEntry entry;

            entry.tag = read_u16(file, order);
            entry.type = read_u16(file, order);
            entry.count = read_u32(file, order);
            entry.value_or_offset = read_u32(file, order);

            switch (entry.tag)
            {
                case 256:
                    width = get_inline_value(entry, order);
                    break;
                case 257:
                    height = get_inline_value(entry, order);
                    break;
                case 258:
                    bits_per_sample = static_cast<std::uint16_t>(get_inline_value(entry, order));
                    break;
                case 259:
                    compression = static_cast<std::uint16_t>(get_inline_value(entry, order));
                    break;
                case 262:
                    photometric = static_cast<std::uint16_t>(get_inline_value(entry, order));
                    break;
                case 273:
                    strip_offsets_offset = entry.value_or_offset;
                    strip_offsets_count = entry.count;
                    strip_offsets_type = entry.type;
                    break;
                case 277:
                    samples_per_pixel = static_cast<std::uint16_t>(get_inline_value(entry, order));
                    break;
                case 278:
                    rows_per_strip = get_inline_value(entry, order);
                    break;
                case 279:
                    strip_byte_counts_offset = entry.value_or_offset;
                    strip_byte_counts_count = entry.count;
                    strip_byte_counts_type = entry.type;
                    break;

                default:
                    break;
            }
        }

        if (width == 0 || height == 0)
            throw std::runtime_error("Invalid TIFF dimensions");

        if (bits_per_sample != 8)
            throw std::runtime_error("Only 8-bit TIFF is supported");

        if (compression != 1)
            throw std::runtime_error("Only uncompressed TIFF is supported");

        if (samples_per_pixel != 1)
            throw std::runtime_error("Only grayscale TIFF is supported");

        if (photometric != 1)
            throw std::runtime_error("Only BlackIsZero TIFF is supported");

        if (rows_per_strip == 0)
            throw std::runtime_error("Invalid RowsPerStrip");

        if (strip_offsets_count == 0 || strip_byte_counts_count == 0)
            throw std::runtime_error("Missing TIFF strip data");

        if (strip_offsets_count != strip_byte_counts_count)
            throw std::runtime_error("StripOffsets and StripByteCounts count mismatch");

        std::uint32_t *strip_offsets = read_TIFF_array(file, order, strip_offsets_offset, strip_offsets_count, strip_offsets_type);

        std::uint32_t *strip_byte_counts = read_TIFF_array(file, order, strip_byte_counts_offset, strip_byte_counts_count, strip_byte_counts_type);

        Image image(width, height);

        std::uint32_t current_row = 0;

        for (std::uint32_t strip = 0; strip < strip_offsets_count; ++strip)
        {
            file.seekg(static_cast<std::streamoff>(strip_offsets[strip]), std::ios::beg);

            if (!file)
            {
                delete[] strip_offsets;
                delete[] strip_byte_counts;

                throw std::runtime_error("Invalid TIFF strip offset!\n");
            }

            const std::uint32_t byte_count = strip_byte_counts[strip];

            unsigned char *buffer = new unsigned char[byte_count];

            file.read(reinterpret_cast<char*>(buffer), byte_count);

            if (!file)
            {
                delete[] buffer;
                delete[] strip_offsets;
                delete[] strip_byte_counts;

                throw std::runtime_error("Could not read TIFF strip!\n");
            }

            for (std::uint32_t row = 0; row < rows_per_strip && current_row < height; ++row)
            {
                for (std::uint32_t x = 0; x < width; ++x)
                    image.at(x, current_row) = buffer[row * width + x];

                ++current_row;
            }
            
            delete[] buffer;
        }

        delete[] strip_offsets;
        delete[] strip_byte_counts;

        std::cout << "FInished!\n";

        return image;
    }
}