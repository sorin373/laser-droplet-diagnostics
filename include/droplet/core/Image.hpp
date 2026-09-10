#pragma once

#include <cstdint>
#include <new>

namespace droplet
{
	class Image
	{
	public:
		typedef std::uint8_t 		value_type;
		typedef std::size_t 		size_type;
		typedef value_type* 		pointer;
		typedef value_type& 		reference;
		typedef const value_type&   const_reference;

		size_type width;
		size_type height;

		Image() noexcept
			: width(0), height(0), pixels(nullptr) { }

		Image(value_type w, value_type h)
			: width(w), height(h)
		{
			this->pixels = new value_type[width * height];

			if (pixels == nullptr)
				throw std::bad_alloc();

			for (std::size_t i = 0, N = width * height; i < N; ++i)
				pixels[i] = 255;
		}

		Image(const Image& image)
		{
			this->width = image.width;
			this->height = image.height;
			this->pixels = image.pixels;
		}
		
		const_reference operator=(const Image&) = delete;

		~Image()
		{
			delete[] pixels;
		}

		reference at(size_type x, size_type y)
		{ return this->pixels[y * width + x]; }

		const_reference at(size_type x, size_type y) const
		{ return this->pixels[y * width + x]; }

	private:
		pointer pixels = nullptr;
	};
}

