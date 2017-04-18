#pragma once

#include <vector>
#include <functional>

#include"opencv2\core.hpp"
#include"opencv2\world.hpp"
#include "opencv2\cvconfig.h"
#include "opencv2\opencv.hpp"
#include"opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect.hpp"
#include "opencv2\opencv_modules.hpp"

class ObjectDetector
{
public:
	ObjectDetector ();
	ObjectDetector (cv::Mat &image);

	cv::Mat getSourceImage ();

	void setSourceImage (const cv::Mat &newImage );

	void selectROI ( cv::Mat &image );

	cv::Mat preprocess ();

	cv::Mat detectROI (const cv::Mat &preprocessedImage);

	~ObjectDetector ();

private:
	cv::Mat _sourceImage;
	std::vector<int> coords;
	bool _hasAlphaChanel;
};

