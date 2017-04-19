#pragma once

#include <vector>

#include "opencv2\core.hpp"
#include "opencv2\world.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect.hpp"

namespace colorSearching
{
	enum class ColorSpace
	{
		cieLAB, BGR, cieXYZ
	};

	class ColorSpaceConverter
	{
	public:
		ColorSpaceConverter ();
		ColorSpaceConverter ( const cv::Mat image, ColorSpace colorSpace ) : _sourceImage ( image.clone () ),
			_colorSpace ( colorSpace ), _convertedImage ( image.clone () )
		{ };

		virtual cv::Mat convert ( cv::Mat sourceImage ) = 0;
		virtual cv::Mat convert () = 0;

		virtual cv::Mat convertBack () = 0;
		virtual float pivot ( const float value ) = 0;

		virtual ~ColorSpaceConverter ();

	protected:
		cv::Mat _sourceImage;
		cv::Mat _convertedImage;

		ColorSpace _colorSpace;
	};

	class XYZConverter : public ColorSpaceConverter
	{
	public:
		XYZConverter ()
		{ };
		XYZConverter ( const XYZConverter &newObject )
		{ };
		XYZConverter ( const cv::Mat &image ) :
			ColorSpaceConverter ( image, ColorSpace::cieXYZ )
		{ };

		cv::Mat convert ( cv::Mat sourceImage ) override;
		cv::Mat convert () override;

		cv::Mat convertBack () override;
		
		float pivot (const float value) override;

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
		LABConverter ( const cv::Mat &image ) :
			ColorSpaceConverter ( image, ColorSpace::cieLAB )
		{ };

		cv::Mat convert ( cv::Mat sourceImage ) override;
		cv::Mat convert () override;

		cv::Mat convertBack () override;

		float pivot ( const float value ) override;

		~LABConverter ()
		{ };
	};
}

