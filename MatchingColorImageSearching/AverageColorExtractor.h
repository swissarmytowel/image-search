#pragma once

#include <vector>

#include"opencv2\core.hpp"
#include"opencv2\world.hpp"
#include "opencv2\cvconfig.h"
#include "opencv2\opencv.hpp"
#include"opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect.hpp"
#include "opencv2\opencv_modules.hpp"

class AverageColorExtractor
{
public:
	AverageColorExtractor ();
	
	AverageColorExtractor (const cv::Mat &image, std::vector<cv::Point> nonBlackPixelsLocation);

	cv::Scalar getAverageColor ();

	cv::Scalar calculateAverageColor (std::vector<cv::Point> nonBlackPixelsLocation);

	~AverageColorExtractor ();

	cv::Mat get ();

	static 	std::vector<cv::Scalar> _colors;

private:
	cv::Mat _image;

	cv::Scalar _extractedColor;
};
