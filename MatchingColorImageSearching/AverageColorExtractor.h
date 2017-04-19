#pragma once

#include <vector>

#include "opencv2\core.hpp"
#include "opencv2\world.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect.hpp"

#include "ColorSpaceConverter.h"

namespace colorSearching
{
	class AverageColorExtractor
	{
	public:
		AverageColorExtractor ();

		AverageColorExtractor ( const cv::Mat &image, std::vector<cv::Point> nonBlackPixelsLocation );

		cv::Scalar getAverageColor ();

		cv::Scalar calculateAverageColor ( std::vector<cv::Point> nonBlackPixelsLocation );

		~AverageColorExtractor ();

		cv::Mat getAverageColorPlate ();

		static 	std::vector<cv::Scalar> nonBlackColors;

	private:
		cv::Mat _image;

		cv::Scalar _extractedAverageColor;
	};
}
