#include <iostream>
#include <string>
#include <vector>
#include "ObjectDetector.h"
#include "AverageColorExtractor.h"


int main ( int argc, char** argv )
{
	cv::Mat image;

	try
	{
		image = cv::imread ( "woman.jpg" );

		if ( image.empty () )
		{
			throw  cv::Exception ( 0, " Can't read image", "main() ", "main.cpp", 14 );
		}
	}
	catch ( cv::Exception error )
	{
		std::cerr << error.what () << std::endl;

		return EXIT_FAILURE;
	}

	cv::Mat tmp;

	image.convertTo ( tmp, CV_32F, 1.f / 255.f );

	ObjectDetector od ( tmp );

	AverageColorExtractor extr ( tmp, od.detectROI ( od.preprocess () ) );

	cv::namedWindow ( "result", cv::WindowFlags::WINDOW_AUTOSIZE );

	cv::namedWindow ( "source", cv::WINDOW_AUTOSIZE );

	cv::imshow ( "result", extr.get () );

	cv::imshow ( "source", image );

	cv::waitKey ();

	return EXIT_SUCCESS;
}