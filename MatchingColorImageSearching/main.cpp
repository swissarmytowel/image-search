#include <iostream>
#include <string>
#include <vector>
#include "ObjectDetector.h"


int main ( int argc, char** argv )
{
	cv::Mat image;

	try
	{
		image = cv::imread ( "woman.jpg" );

		if ( image.empty () )
		{
			throw  cv::Exception ( 0, " Can't read image", "main() ", "main.cpp", 17 );
		}
	}
	catch ( cv::Exception error )
	{
		std::cerr << error.what () << std::endl;

		return EXIT_FAILURE;
	}

	ObjectDetector od ( image );

	cv::namedWindow ( "result", cv::WindowFlags::WINDOW_KEEPRATIO );

	cv::imshow ( "result", od.detectROI ( od.preprocess () ) );

	cv::waitKey ();

	return EXIT_SUCCESS;
}