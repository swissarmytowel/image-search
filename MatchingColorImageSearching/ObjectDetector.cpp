#include "ObjectDetector.h"

ObjectDetector::ObjectDetector ()
{ }

ObjectDetector::ObjectDetector ( cv::Mat & image )
{
	_sourceImage = image;

	_hasAlphaChanel = image.channels () == 3 ? false : true;
}

cv::Mat ObjectDetector::getSourceImage ()
{
	return _sourceImage;
}

void ObjectDetector::setSourceImage ( const cv::Mat & newImage )
{
	_sourceImage = newImage;
}

void ObjectDetector::selectROI ( cv::Mat & image )
{ }

cv::Mat ObjectDetector::preprocess ()
{
	cv::Mat temporaryImage ( _sourceImage );

	cv::ColorConversionCodes conversionCode = _hasAlphaChanel ? cv::ColorConversionCodes::COLOR_BGRA2GRAY : cv::ColorConversionCodes::COLOR_BGR2GRAY;

	//Converts image to grayscale
	cv::cvtColor ( temporaryImage, temporaryImage, conversionCode );
	cv::Mat s;
	//cv::bilateralFilter ( temporaryImage, s, 17 ,50.0, 10.0 );

	cv::GaussianBlur ( temporaryImage, temporaryImage, cv::Size ( 3, 3 ), 0 );

	
	double minGray, maxGray;

	cv::minMaxLoc ( temporaryImage, &minGray, &maxGray );

	minGray = 115;

	double inputRange = maxGray - minGray;

	double alpha = 255.0 / inputRange; // alpha expands current range. MaxGray will be 255
	double beta = -minGray * alpha;    // beta shifts current range so that minGray will go to 0

	temporaryImage.convertTo ( temporaryImage, -1, alpha, beta );

	cv::morphologyEx ( temporaryImage, temporaryImage, cv::MORPH_CLOSE, cv::getStructuringElement ( cv::MORPH_RECT, cv::Size2d ( 21, 21 ) ) );

	cv::threshold ( temporaryImage, temporaryImage, 0, 255, cv::THRESH_BINARY );

	//

	return temporaryImage;
}


cv::Mat ObjectDetector::detectROI ( const cv::Mat & preprocessedImage )
{
	cv::Mat tmp, tp,tp1;

	cv::cvtColor ( preprocessedImage, tp, cv::ColorConversionCodes::COLOR_GRAY2BGR );

	tmp = _sourceImage - tp;

	//cv::Canny ( tmp, tmp, 50, 255 );

	return tmp;
}

ObjectDetector::~ObjectDetector ()
{ }
