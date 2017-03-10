#ifndef IMAGE_H
#define IMAGE_H


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

    inline const Pixel& getPixel(unsigned int x, unsigned int y) const { return data[y * _width + x]; }
    inline Pixel& getPixel(unsigned int x, unsigned int y) { return data[y * _width + x]; }

    unsigned int width() const;
    unsigned int height() const;

protected:
    unsigned int _totalSize;
    unsigned int _width, _height;

private:
    Pixel* data;
};

#endif // IMAGE_H
