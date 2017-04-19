#pragma once

#include <vector>

#include "opencv2\core.hpp"
#include "opencv2\world.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect.hpp"

namespace colorSearching
{
	class ObjectDetector
	{
	public:
		ObjectDetector ();
		ObjectDetector ( cv::Mat &image );

		cv::Mat getSourceImage ();

		void setSourceImage ( const cv::Mat &newImage );

		std::vector<cv::Point>  detectROI ();

		~ObjectDetector ();

	private:
		cv::Mat _sourceImage;

		bool _hasAlphaChanel;

		cv::Mat _preprocess ();
	};
}
