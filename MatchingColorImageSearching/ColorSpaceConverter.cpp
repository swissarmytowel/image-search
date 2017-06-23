#include "ColorSpaceConverter.h"

using namespace colorConvertions;

ColorSpaceConverter::ColorSpaceConverter ()
{}

void colorConvertions::ColorSpaceConverter::set ( cv::Scalar pixel )
{
    _sourcePixel = pixel;
}


ColorSpaceConverter::~ColorSpaceConverter ()
{}

cv::Scalar LABConverter::convert ()
{
    XYZConverter  xyzConverter ( _sourcePixel );

    _convertedPixel = xyzConverter.convert ();

    double x ( normalize ( _convertedPixel[ 0 ] / _whitePoint[ 0 ] ) ),
        y ( normalize ( _convertedPixel[ 1 ] / _whitePoint[ 1 ] ) ),
        z ( normalize ( _convertedPixel[ 2 ] / _whitePoint[ 2 ] ) );

    _convertedPixel[ 0 ] = std::max ( 0.0, 116.0 * y - 16.0 );
    _convertedPixel[ 1 ] = 500.0 * ( x - y );
    _convertedPixel[ 2 ] = 200.0 * ( y - z );

    return _convertedPixel;
}

cv::Scalar LABConverter::convertBack ()
{
    double y ( ( _convertedPixel[ 0 ] + 16.0 ) / 116.0 ),
        x ( _convertedPixel[ 1 ] / 500.0 + y ),
        z ( y - _convertedPixel[ 2 ] / 200.0 );

    _sourcePixel[ 0 ] = ( _whitePoint[ 0 ] )* ( denormalize ( x ) );
    _sourcePixel[ 1 ] = ( _whitePoint[ 1 ] ) * ( _convertedPixel[ 0 ] > ( 0.008856 ) ? pow ( ( _convertedPixel[ 0 ] + 16.0 ) / 116.0, 3 ) : _convertedPixel[ 0 ] );
    _sourcePixel[ 2 ] = ( _whitePoint[ 2 ] ) * ( denormalize ( z ) );

    XYZConverter converter ( _sourcePixel );

    return converter.convertBack ();
}

double LABConverter::normalize ( const double value )
{
    return  ( value > 0.008856 ? pow ( value, ( 1.0 / 3.0 ) ) : ( 7.787 * value + 16.0 ) / 116.0 );
}

double colorConvertions::LABConverter::denormalize ( const double value )
{
    return ( pow ( value, 3.0 ) > 0.008856 ? pow ( value, 3.0 ) : ( value / 7.787 - 16.0 / 116.0 ) );
}

cv::Scalar XYZConverter::convert ()
{
    double red ( normalize ( _sourcePixel[ 2 ] ) ), green ( normalize ( _sourcePixel[ 1 ] ) ),
        blue ( normalize ( _sourcePixel[ 0 ] ) );

    _convertedPixel[ 0 ] = ( red * 0.412424 ) + ( green * 0.357579 ) + ( blue * 0.180464 );
    _convertedPixel[ 1 ] = ( red * 0.212656 ) + ( green * 0.715158 ) + ( blue * 0.0721856 );
    _convertedPixel[ 2 ] = ( red * 0.0193324 ) + ( green * 0.119193 ) + ( blue * 0.950444 );

    return _convertedPixel;
}


cv::Scalar XYZConverter::convertBack ()
{
    double x ( _convertedPixel[ 0 ] / 100.0 ), y ( _convertedPixel[ 1 ] / 100.0 ),
        z ( _convertedPixel[ 2 ] / 100.0 );


    _convertedPixel[ 2 ] = denormalize ( x * 3.2406 - 1.5372 * y - 0.4986 *z );
    _convertedPixel[ 1 ] = denormalize ( -x * 0.9689 + y * 1.8758 + z * 0.0415 );
    _convertedPixel[ 0 ] = denormalize ( x * 0.0557 - 0.2040 * y + z * 1.0570 );

    return _convertedPixel;
}

double XYZConverter::normalize ( const double value )
{
    return ( value > 0.04045 ? ( pow ( ( ( value + 0.055 ) / 1.055 ), 2.4 )* 100.0 ) : ( value * 100.0 ) / 12.92 );
}

double XYZConverter::denormalize ( const double value )
{
    return value > 0.0031308 ? 1.055 * pow ( value, 1.0 / 2.40 ) - 0.0550 : 12.920 * value;
}

cv::Scalar colorConvertions::HSVConverter::convert ()
{
    double red ( _sourcePixel[ 2 ] ), green (  _sourcePixel[ 1 ]  ),
        blue ( _sourcePixel[ 0 ] );

    double min = std::min ( red, std::min ( green, blue ) );
    double max = std::max ( red, std::max ( green, blue ) );   

    double chroma = max - min;

    double hue = 0, saturation = 0, value = 0;

    value = max;

    if ( chroma == 0 )
    {
        hue = 0;
        saturation = 0;
    }
    else
    {
        saturation = chroma / max;

        if ( red == max )
        {
            hue = ( green - blue ) / chroma;
            if ( green < blue )
            {
                hue += 6;
            }
        }
        else if ( green == max )
        {
            hue = 2 + ( ( blue - red ) / chroma );
        }
        else if ( blue == max )
        {
            hue = 4 + ( ( red - green ) / chroma );
        }

        hue *= 60;
    }

    hue /= 2;

    return cv::Scalar ( floor(hue), normalize(saturation), normalize(value) );
}

cv::Scalar colorConvertions::HSVConverter::convertBack ()
{
    return cv::Scalar ();
}

double colorConvertions::HSVConverter::normalize ( const double value )
{
    double normalizedResult = std::max ( 0.0, std::min ( 1.0, value ) );
    return floor ( normalizedResult == 1.0 ? 255 : normalizedResult * 256.0 );
}

double colorConvertions::HSVConverter::denormalize ( const double value )
{
    return 0.0;
}
