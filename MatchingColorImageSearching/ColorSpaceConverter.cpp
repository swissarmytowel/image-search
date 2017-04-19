#include "ColorSpaceConverter.h"

using namespace colorSearching;

ColorSpaceConverter::ColorSpaceConverter ()
{ }


ColorSpaceConverter::~ColorSpaceConverter ()
{ }

cv::Mat colorSearching::LABConverter::convert ( cv::Mat sourceImage )
{
	cv::Mat backupImage ( _sourceImage ), temporaryImage;

	_sourceImage = sourceImage;

	temporaryImage = convert ();

	_sourceImage = backupImage;

	return temporaryImage;
}

cv::Mat colorSearching::LABConverter::convert ()
{
	XYZConverter  xyzConverter ( _sourceImage );

	_convertedImage = xyzConverter.convert ();

	for ( int i = 0; i < _convertedImage.rows; ++i )
	{
		for ( int j = 0; j < _convertedImage.cols; ++j )
		{
			cv::Vec3f sourcePixel ( _convertedImage.at<cv::Vec3f> ( i, j )),
				resultPixel;

			float x ( sourcePixel [ 0 ] / 95.047f  ),
				  y ( sourcePixel [ 1 ] / 100.f  ),
				  z ( sourcePixel [ 2 ] / 108.883f );

			x = pivot ( x );
			y = pivot ( y );
			z = pivot ( z );

			resultPixel [ 0 ] = std::max ( 0.f, 116.f * y - 16.f );
			resultPixel [ 1 ] = 500.f * ( x - y );
			resultPixel [ 2 ] = 200.f * ( y - z );

			_convertedImage.at<cv::Vec3f> ( i, j ) = resultPixel;
		}
	}

	return _convertedImage;
}

cv::Mat colorSearching::LABConverter::convertBack ()
{
	return cv::Mat ();
}

float colorSearching::LABConverter::pivot ( const float value )
{
	return  ( value > 0.008856f ? pow ( value, ( 1.0f / 3.f ) ) : ( 7.787f * value + 16.0f ) / 116.f );
}

cv::Mat colorSearching::XYZConverter::convert ( cv::Mat sourceImage )
{
	cv::Mat backupImage ( _sourceImage ), temporaryImage;

	_sourceImage = sourceImage;

	temporaryImage = convert ();

	_sourceImage = backupImage;

	return temporaryImage;
}

cv::Mat colorSearching::XYZConverter::convert ()
{
	cv::Mat temporaryImage ( _sourceImage.clone () );

	for ( int i = 0; i < _sourceImage.rows; ++i )
	{
		for ( int j = 0; j < _sourceImage.cols; ++j )
		{
			cv::Vec3f sourcePixel ( _sourceImage.at<cv::Vec3f> ( i, j ) ),
				resultPixel;

			float red ( sourcePixel [ 2 ] ), green ( sourcePixel [ 1 ]  ), blue ( sourcePixel [ 0 ]  );

			red = pivot ( red );
			green = pivot ( green );
			blue = pivot ( blue );

			resultPixel [ 0 ] = ( red * 0.412424f ) + ( green * 0.357579f ) + ( blue * 0.180464f );
			resultPixel [ 1 ] = ( red * 0.212656f ) + ( green * 0.715158f ) + ( blue * 0.0721856f );
			resultPixel [ 2 ] = ( red * 0.0193324f ) + ( green * 0.119193f ) + ( blue * 0.950444f );

			_convertedImage.at<cv::Vec3f> ( i, j ) = resultPixel;
		}
	}

	return _convertedImage;
}


cv::Mat colorSearching::XYZConverter::convertBack ()
{
	return cv::Mat ();
}

float colorSearching::XYZConverter::pivot ( const float value )
{
	return ( value > 0.04045f ? ( pow ( ( ( value + 0.055f ) / 1.055f ), 2.4f )* 100.f ) : ( value * 100.f ) / 12.92f );
}


