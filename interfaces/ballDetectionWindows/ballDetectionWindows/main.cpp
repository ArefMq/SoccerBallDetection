#include <iostream>
#include "balldetector.h"

#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "image.h"

using namespace std;
using namespace cv;
using namespace MVision;

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )


Mat image2cvImage(const Image& img)
{
	Mat result(img.width(), img.height(), DataType<unsigned char>::type);
	for (unsigned int y = 0; y<img.height(); ++y)
		for (unsigned int x = 0; x<img.width(); ++x)
		{
			Image::Pixel p = img.getPixel(x, y);

			result.at<cv::Vec3b>(x, y)[0] = (CYCbCr2R(p.y, p.cb, p.cr));
			result.at<cv::Vec3b>(x, y)[1] = (CYCbCr2G(p.y, p.cb, p.cr));
			result.at<cv::Vec3b>(x, y)[2] = (CYCbCr2B(p.y, p.cb, p.cr));
		}

	return result;
}

bool loadImage(Image& result)
{
	Mat image;
	image = imread("C:\\Users\\Orbi\\workspace\\SoccerBallDetection\\interfaces\\offline\\images\\a-short.png");

	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return false;
	}

	const unsigned width = (unsigned)image.size().width;
	const unsigned height = (unsigned)image.size().height;
	result.resize(width, height);
	for (unsigned int y = 0; y<height; ++y)
		for (unsigned int x = 0; x<width; ++x)
		{
			Image::Pixel p;
			const Vec3b& c = image.at<Vec3b>(x, y);

			p.y = CRGB2Y(c[0], c[1], c[2]);
			p.cr = CRGB2Cr(c[0], c[1], c[2]);
			p.cb = CRGB2Cb(c[0], c[1], c[2]);

			result.getPixel(x, y) = p;
		}

	return true;
}


int main()
{
	BallDetector ballDetector;

	Image image;
	if (!loadImage(image))
		return -1;
	Mat monitor = image2cvImage(image);

	ballDetector.update(image);
	
	typedef vector<Ball> Balls;
	const Balls& results = ballDetector.getResults();
	for (Balls::const_iterator itr = results.begin(); itr<results.end(); itr++)
	{
		const Ball& b = *itr;
		Point pt = Point(b.PositionInImage()._translation.x, b.PositionInImage()._translation.y);
		circle(monitor, pt, b.PositionInImage()._radious, Scalar(0, 0, 200), 5);
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", monitor);

	waitKey(0);
	return 0;
}
