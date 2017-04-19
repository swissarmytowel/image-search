#include <iostream>
#include <string>
#include <vector>

#include "ObjectDetector.h"
#include "AverageColorExtractor.h"


int main ( int argc, char** argv )
{
	cv::Mat image;

	std::string imageName = "some image.jpg";

	try
	{
		image = cv::imread ( imageName );

		if ( image.empty () )
		{
			throw  cv::Exception ( 0, " Can't read image \"" + imageName + "\" ", "int main ( int argc, char** argv ) ", "main.cpp", 17 );
		}
	}
	catch ( cv::Exception error )
	{
		std::cerr << error.what () << std::endl;

		return EXIT_FAILURE;
	}

	cv::Mat temporaryImage;

	image.convertTo ( temporaryImage, CV_32F, 1.f / 255.f );

	colorSearching::ObjectDetector detector ( temporaryImage );

	colorSearching::AverageColorExtractor extractor ( temporaryImage, detector.detectROI () );

	cv::namedWindow ( "result", cv::WindowFlags::WINDOW_AUTOSIZE );

	cv::namedWindow ( "source", cv::WINDOW_AUTOSIZE );

	cv::imshow ( "result", ( extractor.getAverageColorPlate () ) );

	cv::imshow ( "source", image );

	cv::waitKey ();

	return EXIT_SUCCESS;
}

