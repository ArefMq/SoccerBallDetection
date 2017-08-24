#include <iostream>
#include <atlimage.h>
#include <atltypes.h>

#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"

#include "balldetector.h"
#include "image.h"

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

#define STATIC_PATH_TO_IMAGE "C:\\Users\\Orbi\\workspace\\SoccerBallDetection\\interfaces\\offline\\images\\b-short.png"

MVision::Image loadImage(const char* filename)
{
	using namespace MVision;

	CImage cimage;
	cimage.Load(CString(filename));
	if (cimage.IsNull())
		throw("can not load image");

	const unsigned width = cimage.GetWidth();
	const unsigned height = cimage.GetHeight();

	Image result(height, width);
	for (int x = 0; x < (int)width; ++x)
		for (int y = 0; y < (int)height; ++y)
		{
			COLORREF c = cimage.GetPixel(x, y);
			
			const unsigned char r = GetRValue(c);
			const unsigned char g = GetGValue(c);
			const unsigned char b = GetBValue(c);

			const Image::Pixel p(CRGB2Y(r, g, b), CRGB2Cb(r, g, b), CRGB2Cr(r, g, b));
			result.getPixel(y, x) = p;
		}
	return result;
}

cv::Mat image2cvImage(const MVision::Image& img)
{
	using namespace cv;
	using namespace MVision;

	std::cout << "[sub routine image2cvimage] image size:: " << img.width() << ", " << img.height() << "\n";

	Mat result(img.width(), img.height(), DataType<Vec3b>::type);
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

int main()
{
	using namespace MVision;
	using namespace std;

	Image image;
	try
	{
		image = loadImage(STATIC_PATH_TO_IMAGE);
	}
	catch (const char* exp)
	{
		cout << exp << "\n";
		for (;;);
		return -1;
	}

	cv::Mat monitor = image2cvImage(image);

	BallDetector ballDetector;
	ballDetector.update(image);

	typedef vector<Ball> Balls;
	const Balls& results = ballDetector.getResults();
	for (Balls::const_iterator itr = results.begin(); itr<results.end(); itr++)
	{
		const Ball& b = *itr;
		cv::Point pt = cv::Point(b.PositionInImage()._translation.x, b.PositionInImage()._translation.y);
		cv::circle(monitor, pt, b.PositionInImage()._radious, cv::Scalar(0, 0, 200), 2);
	}

	cv::namedWindow("Monitor", cv::WINDOW_AUTOSIZE);
	imshow("Monitor", monitor);

	cv::waitKey(0);
	return 0;
}
