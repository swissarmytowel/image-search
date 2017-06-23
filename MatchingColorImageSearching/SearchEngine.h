#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>

#include <opencv2/opencv.hpp>

#include "AverageColorExtractor.h"
#include "ColorComparator.h"
#include "ColorSpaceConverter.h"
#include "ObjectDetector.h"

enum class DisplayMode
{
    DISPLAY_NAMES, DISPLAY_IMAGES, DISPLAY_IMAGES_HIGHLIGHTED, DISPLAY_BOUNDING_BOX
};

class SearchEngine
{
public:
    SearchEngine ()
    {};

    SearchEngine ( std::string sampleImageName, cv::String folderName, double deltaEThreshold,  unsigned short displayMode );

    void runSearch ();

    void displayResults (DisplayMode mode);

    ~SearchEngine ()
    {};

private:
    bool findMatchingPixels ( const cv::Mat sampleImage );

    std::vector<std::vector<cv::Point>> findEdges ( const cv::Mat roi );

    void findColorBoundaries ();

    void index ();

    void applyThreshold ( const cv::Scalar  pixel );

    void preprocessSampleImage ();

    cv::Mat getColorRegion ( const cv::Mat& image );

    cv::Mat getReferenceImage ();

    cv::Mat _referenceImage;

    cv::String _folderName;
    
    cv::Scalar _averageColor;

    std::map<cv::String, std::vector<std::vector<cv::Point>>> indexedImages;

    std::vector<std::vector<cv::Point>> edges;

    std::vector<cv::String> _imageNamesFromFolder;

    cv::Scalar _lowerPixelValueBoundary;
    cv::Scalar _upperPixelValueBoundary;

    cv::Scalar _colorThreshold;

    bool checkRange ( const cv::Scalar labpixel );


    double _deltaEThreshold;

    unsigned short _displayMode;

    bool _errorsOccured;
};

