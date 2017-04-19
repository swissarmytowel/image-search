#include "ObjectDetector.h"

using namespace colorSearching;

ObjectDetector::ObjectDetector ()
{ }

ObjectDetector::ObjectDetector ( cv::Mat & image )
{
	_sourceImage = image;
}

cv::Mat ObjectDetector::getSourceImage ()
{
	return _sourceImage;
}

void ObjectDetector::setSourceImage ( const cv::Mat & newImage )
{
	_sourceImage = newImage;
}

cv::Mat ObjectDetector::_preprocess ()
{
	cv::Mat temporaryImage ( _sourceImage );

	cv::ColorConversionCodes conversionCode = _hasAlphaChanel ? cv::ColorConversionCodes::COLOR_BGRA2GRAY : cv::ColorConversionCodes::COLOR_BGR2GRAY;

	//Converts image to grayscale
	cv::cvtColor ( temporaryImage, temporaryImage, cv::ColorConversionCodes::COLOR_BGR2GRAY );

	cv::GaussianBlur ( temporaryImage, temporaryImage, cv::Size ( 3, 3 ), 0 );

	double minimalGrayValue, maximalGrayValue;

	cv::minMaxLoc ( temporaryImage, &minimalGrayValue, &maximalGrayValue );

	//Magic number specified after number of tests to give the most accurate, yet undistorted result
	minimalGrayValue = 0.4555;

	double inputRange = maximalGrayValue - minimalGrayValue;

	double alpha = 1.0/inputRange; // alpha expands current range. MaxGray will be 255
	double beta = -minimalGrayValue * alpha;    // beta shifts current range so that minGray will go to 0

	temporaryImage.convertTo ( temporaryImage, -1, alpha, beta );

	cv::morphologyEx ( temporaryImage, temporaryImage, cv::MORPH_CLOSE, cv::getStructuringElement ( cv::MORPH_RECT, cv::Size2d ( 21, 21 ) ) );

	cv::threshold ( temporaryImage, temporaryImage, 0, 255, cv::THRESH_BINARY );

	return temporaryImage;
}


std::vector<cv::Point>  ObjectDetector::detectROI ()
{
	std::vector<cv::Point>  foundNonBlackLocations;

	cv::Mat temporaryImage, preprocessedImage(_preprocess());

	preprocessedImage.convertTo ( temporaryImage, CV_8UC1 );

	for ( int i = 0; i < temporaryImage.rows; ++i )
	{
		for ( int j = 0; j < temporaryImage.cols; j++ )
		{
			if ( temporaryImage.at<uchar> ( i, j ) == 0 )
			{
				foundNonBlackLocations.push_back ( cv::Point ( i, j ) );
			}
		}
	}

	return foundNonBlackLocations;
}

ObjectDetector::~ObjectDetector ()
{ }
