#include "image.h"
#include <iostream>

Image::Image() :
    _totalSize(0), _width(0), _height(0), data(0)
{
}

Image::Image(unsigned int width, unsigned int height) :
    data(0)
{
    resize(width, height);
}

Image::~Image()
{
    if (data)
        delete[] data;
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

    if (data)
        delete[] data;

    data = new Pixel[_totalSize];
}
