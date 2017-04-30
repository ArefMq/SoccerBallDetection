#ifndef IMAGE_H
#define IMAGE_H

#ifdef DEBUG
# include <vector>
# include <iostream>
#endif

namespace MVision {

class Image
{
public:
    class Pixel
    {
    public:
        Pixel(unsigned char Y, unsigned char Cb, unsigned char Cr) : y(Y), cb(Cb), cr(Cr) {}
        Pixel() : y(0), cb(0), cr(0) {}
        unsigned char y, cb, cr;
    };

    Image();
    Image(unsigned int width, unsigned int height);
    ~Image();

    void resize(unsigned int width, unsigned int height);

    inline const Pixel& getPixel(unsigned int x, unsigned int y) const
    {
#ifdef DEBUG
        if (x>=_width || y>=_height)
            std::cerr << x << ", " << y << " << Invalid indexing...\n";
        return data.at(y * _width + x);
#else
        return data[y * _width + x];
#endif
    }

    inline Pixel& getPixel(unsigned int x, unsigned int y)
    {
#ifdef DEBUG
        if (x>=_width || y>=_height)
            std::cerr << x << ", " << y << " << Invalid indexing...\n";
        return data.at(y * _width + x);
#else
        return data[y * _width + x];
#endif
    }


    unsigned int width() const;
    unsigned int height() const;

protected:
    unsigned int _totalSize;
    unsigned int _width, _height;

private:
#ifdef DEBUG
    std::vector<Pixel> data;
#else
    Pixel* data;
#endif
};
}

#endif // IMAGE_H
