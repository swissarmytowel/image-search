#pragma once

#include <vector>

#include <opencv2\opencv.hpp>

#include "ColorSpaceConverter.h"

namespace imageAnalysis
{
    //------------------------------------------------------------------------
    //          Class for extracting average color from image
    //------------------------------------------------------------------------
	class AverageColorExtractor
	{
	public:
		AverageColorExtractor ();

		AverageColorExtractor ( const cv::Mat &image );

		cv::Scalar getAverageColor ();

		cv::Scalar calculateAverageColor ( );

		~AverageColorExtractor ();

	private:
		cv::Mat _image;

		cv::Scalar _extractedAverageColor;
	};
}
