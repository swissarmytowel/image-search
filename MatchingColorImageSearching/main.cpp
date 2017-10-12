#include "ImageSearchingCore.h"

#include<opencv2/imgproc.hpp>

#include <iostream>
#include <string>

int main ( int argc, char** argv )
{
	if ( argc != 5 )
	{
		std::cerr << "Wrong number of arguments!\nUsage: <image name> <search_folder> <dE threshold> <display mode>\n" <<
			"Display modes:\n\t1 - print image names\n\t2 - display images\n\t3 - display images with highlighting matching regions\n\t - display images with bounding boxes for matching regions\n";
		return EXIT_FAILURE;
	}

	std::string imageName = argv[ 1 ];

	cv::String imageFolder = argv[ 2 ];

	double deltaThreshold = std::stod ( argv[ 3 ] );

	unsigned short displayMode = std::stoi ( argv[ 4 ] );

	SearchEngine eng ( imageName, imageFolder, deltaThreshold, displayMode );

	eng.runSearch ();

	return EXIT_SUCCESS;
}