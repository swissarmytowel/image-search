#include "AverageColorExtractor.h"

std::vector<cv::Scalar> AverageColorExtractor::_colors = std::vector<cv::Scalar> ();

AverageColorExtractor::AverageColorExtractor ()
{ }

AverageColorExtractor::AverageColorExtractor ( const cv::Mat &image, std::vector<cv::Point> nonBlackPixelsLocation )
{
	_image = image;

	_extractedColor = calculateAverageColor ( nonBlackPixelsLocation );
}

cv::Scalar AverageColorExtractor::getAverageColor ()
{
	return _extractedColor;
}

cv::Scalar AverageColorExtractor::calculateAverageColor ( std::vector<cv::Point> nonBlackPixelsLocation )
{
	cv::Scalar averageColor ( 0.0 );

	for ( auto &it : nonBlackPixelsLocation )
	{
		for ( int i = 0; i < 3; ++i )
		{
			averageColor [ i ] += _image.at<cv::Vec3f> ( it.x, it.y ) [ i ];
		}
	}

	averageColor /= static_cast< double >( nonBlackPixelsLocation.size () );

	return averageColor;
}


AverageColorExtractor::~AverageColorExtractor ()
{ }


cv::Mat AverageColorExtractor::get ()
{
	cv::Mat colorPlate ( cv::Size ( 200, 200 ), _image.type () );

	colorPlate = _extractedColor;

	return colorPlate;
}
