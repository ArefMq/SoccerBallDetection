#include "image.h"
#include <iostream>
#include <string.h>

using namespace MVision;

Image::Image() :
    _totalSize(0), _width(0), _height(0), data(0)
{
}

Image::Image(unsigned int width, unsigned int height) :
    _width(0),
    _height(0)
#ifndef DEBUG
    , data(0)
#endif
{
    resize(width, height);
}

Image::~Image()
{
#ifdef DEBUG
    data.clear();
#else
    if (data)
        delete[] data;
#endif
}

Image::Image(const Image& other) :
	_width(other._width),
	_height(other._height),
	_totalSize(other._totalSize)
{
#ifdef DEBUG
	data = other.data;
#else
	data = new Pixel[_totalSize];
	std::memcpy(data, other.data, sizeof(Pixel) * _totalSize);
#endif
}

unsigned int Image::width() const
{
    return _width;
}

unsigned int Image::height() const
{
    return _height;
}

void Image::resize(unsigned int width, unsigned int height)
{
    if (width == _width && height == _height)
        return;

    _width = width; _height = height;
    _totalSize = _width * _height;

#ifdef DEBUG
    data.resize(_totalSize);
#else
    if (data)
        delete[] data;

    data = new Pixel[_totalSize];
#endif
}
