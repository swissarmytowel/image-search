#pragma once

#include <cmath>

#include <opencv2\opencv.hpp>

namespace imageAnalysis
{
    class ColorComparator
    {
    public:
        ColorComparator () : _standartColor ( cv::Scalar::all ( 0.0 ) ), _deltaEThreshold(8.0)
        {};

        ColorComparator ( const cv::Scalar &color ) : _standartColor ( color ), _deltaEThreshold(8.0)
        {};

        ColorComparator ( const cv::Scalar &color, const double deltaEThreshold ) : _standartColor ( color ), _deltaEThreshold(deltaEThreshold)
        {};

        cv::Scalar getBaseColor ();

        void setBaseColor ( cv::Scalar color );

        void displayColorPlate ();

        bool checkCMCColorDifference ( const cv::Scalar sampleColor );

        bool checkEuclideanDifference ( const cv::Scalar sampleColor );

        double getDeltaE ();

        ~ColorComparator ();

    private:
        double calculateMagnitude ( const double first, const double second );

        double calculateMagnitude ( const double first, const double second, const double third );

        cv::Scalar _standartColor;
        double _deltaEThreshold;
        double _deltaE;
    };
}

