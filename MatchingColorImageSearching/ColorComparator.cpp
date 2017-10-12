#include "ColorComparator.h"

using namespace imageAnalysis;

cv::Scalar imageAnalysis::ColorComparator::getBaseColor ()
{
    return _standartColor;
}

void imageAnalysis::ColorComparator::setBaseColor ( cv::Scalar color )
{
    _standartColor = color;
}

void imageAnalysis::ColorComparator::displayColorPlate ()
{
    cv::Mat plate ( 400, 400, CV_32FC3 );

    plate = _standartColor;

    cv::imshow ( "Base color", plate );
    cv::waitKey ();
}

bool imageAnalysis::ColorComparator::checkCMCColorDifference ( const cv::Scalar sampleColor )
{
    //Pixel data
    double referenceL = _standartColor[ 0 ];
    double referenceA = _standartColor[ 1 ];
    double referenceB = _standartColor[ 2 ];

    double sampleL = sampleColor[ 0 ];
    double sampleA = sampleColor[ 1 ];
    double sampleB = sampleColor[ 2 ];

    //Lightness, chroma and hue value (or related difference) calculation

    double referenceC = calculateMagnitude ( referenceA, referenceB );
    double sampleC = calculateMagnitude ( sampleA, sampleB );

    double deltaL = referenceL - sampleL;
    double deltaC = referenceC - sampleC;
    double deltaH = sqrt ( ( referenceA - sampleA )*( referenceA - sampleA ) + ( referenceB - sampleB )*( referenceB - sampleB ) - deltaC*deltaC );

    //Compensations calculation
    double lightnessCompensation = referenceL < 16.0 ? 0.511 : ( 0.040975 * referenceL ) / ( 1.0 + 0.01765 * referenceL );

    double chromaCompensation = ( 0.0638*referenceC ) / ( 1.0 + 0.0131*referenceC ) + 0.638;

    //F value calculation (specified in CMC formula)
    double fourthPowerReferenceC = std::pow ( referenceC, 4.0 );

    double f = sqrt ( fourthPowerReferenceC / ( fourthPowerReferenceC + 1900.0 ) );

    double pi = 3.14159265;

    //Hue must be in degrees. Standart math library functions return radians, thus back and forth conversion is implemented
    double hue = atan2 ( referenceB, referenceA ) * ( 180.0 / pi );

    hue = hue >= 0.0 ? hue : hue + 360.0;

    //T value calculation (specified in CMC formula)
    double t = hue >= 164.0  &&  hue <= 345.0 ? 0.56 + abs ( 0.2*cos ( ( hue + 168.0 ) * ( pi / 180.0 ) ) ) :
        0.36 + ( abs ( 0.4*cos ( ( hue + 35.0 ) * ( pi / 180.0 ) ) ) );

    double hueCompensation = chromaCompensation * ( f * t + 1.0 - f );

    _deltaE = calculateMagnitude ( deltaL / ( 2.0 * lightnessCompensation ), deltaC / chromaCompensation, deltaH / hueCompensation );

#ifdef DEBUG
    std::cout << "Delta E ( CMC(2:1) ) = " << deltaE << std::endl;
#endif // DEBUG

    return _deltaE <= _deltaEThreshold;
}

bool imageAnalysis::ColorComparator::checkEuclideanDifference ( const cv::Scalar sampleColor )
{
    double sum = 0;

    for ( size_t i = 0; i < 3; i++ )
    {
        double delta = ( sampleColor[ i ] - _standartColor[ i ] );

        sum = sum + delta*delta;
    }

    _deltaE = std::sqrt ( sum );

    return _deltaE <= 3.4;
}

double imageAnalysis::ColorComparator::calculateMagnitude ( const double first, const double second )
{
    return sqrt ( first*first + second*second );
}

double imageAnalysis::ColorComparator::calculateMagnitude ( const double first, const double second, const double third )
{
    return sqrt ( first*first + second*second + third*third );
}

double imageAnalysis::ColorComparator::getDeltaE ()
{
    return _deltaE;
}

ColorComparator::~ColorComparator ()
{}
