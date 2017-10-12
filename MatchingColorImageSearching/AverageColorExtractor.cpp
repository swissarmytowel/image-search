#include "AverageColorExtractor.h"

using namespace imageAnalysis;

AverageColorExtractor::AverageColorExtractor ()
{}

AverageColorExtractor::AverageColorExtractor ( const cv::Mat &image )
{
	_image = image;
	_extractedAverageColor = calculateAverageColor ();
}

cv::Scalar AverageColorExtractor::getAverageColor ()
{
	return _extractedAverageColor;
}

cv::Scalar AverageColorExtractor::calculateAverageColor ()
{
	cv::Scalar averageColor ( cv::Scalar::all ( 0.0 ) );

	cv::Vec3b white ( 255, 255, 255 );

	cv::Mat tmp;

	cv::bilateralFilter ( _image, tmp, 9, 140, 140 );
	double size = 0.0;

	for ( size_t i = 0; i < tmp.rows; i++ )
	{
		for ( size_t j = 0; j < tmp.cols; j++ )
		{
			if ( tmp.at<cv::Vec3b> ( i, j ) != white )
			{
				for ( int k = 0; k < 3; ++k )
				{
					averageColor[ k ] += static_cast< double >( tmp.at<cv::Vec3b> ( i, j )[ k ] / 255.0 );
				}

				size += 1.0;
			}
		}
	}
	averageColor /= size;

	return averageColor;
}

AverageColorExtractor::~AverageColorExtractor ()
{}


