#pragma once

#include <vector>
#include <math.h>

#include <opencv2\opencv.hpp>

namespace colorConvertions
{
	enum class ColorSpace
	{
		cieLAB, BGR, cieXYZ
	};
    
    //------------------------------------------------------------------------
    //      Class implementing abstract color space convertion interface
    //------------------------------------------------------------------------
	class ColorSpaceConverter
	{
	public:
		ColorSpaceConverter ();
		ColorSpaceConverter ( const cv::Scalar pixel, ColorSpace colorSpace ) : 
            _sourcePixel ( pixel ),
			_colorSpace ( colorSpace ), _convertedPixel ( pixel ),
            _whitePoint ( 95.047, 100.0, 108.883 )
		{ };

        void set (cv::Scalar pixel);

		virtual cv::Scalar convert () = 0;
		virtual cv::Scalar convertBack () = 0;

		virtual double normalize ( const double value ) = 0;
		virtual double denormalize ( const double value ) = 0;

		virtual ~ColorSpaceConverter ();

	protected:
		cv::Scalar _sourcePixel;
        cv::Scalar _convertedPixel;

		cv::Scalar _whitePoint;

		ColorSpace _colorSpace;
	};

    //------------------------------------------------------------------------
    //                  Derived classes
    //------------------------------------------------------------------------
	class XYZConverter : public ColorSpaceConverter
	{
	public:
		XYZConverter ()
		{ };
		XYZConverter ( const XYZConverter &newObject )
		{ };
		XYZConverter ( const cv::Scalar pixel ) :
			ColorSpaceConverter ( pixel, ColorSpace::cieXYZ )
		{ };

        cv::Scalar convert () override;

		cv::Scalar convertBack () override;

		double normalize ( const double value ) override;
        double denormalize ( const double value ) override;

		~XYZConverter ()
		{ };
	};

	class LABConverter : public ColorSpaceConverter
	{
	public:
		LABConverter ()
		{ };
		LABConverter ( const LABConverter &newObject )
		{ };
		LABConverter ( const cv::Scalar &pixel ) :
			ColorSpaceConverter ( pixel, ColorSpace::cieLAB )
		{ };

		cv::Scalar convert () override;

		cv::Scalar convertBack () override;

        double normalize ( const double value ) override;
        double denormalize ( const double value ) override;

		~LABConverter ()
		{ };
	};

    class HSVConverter : public ColorSpaceConverter
    {
    public:
        HSVConverter ()
        {};
        HSVConverter ( const cv::Scalar &pixel ) :
            ColorSpaceConverter ( pixel, ColorSpace::cieLAB )
        {};

        cv::Scalar convert () override;

        cv::Scalar convertBack () override;

        double normalize ( const double value ) override;
        double denormalize ( const double value ) override;
    };
}

