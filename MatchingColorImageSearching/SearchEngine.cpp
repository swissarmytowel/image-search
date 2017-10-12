#include "SearchEngine.h"

SearchEngine::SearchEngine ( std::string sampleImageName, cv::String folderName, 
                             double deltaEThreshold,  unsigned short displayMode )
{
    try
    {
        _referenceImage = cv::imread ( sampleImageName );

        if ( _referenceImage.empty () )
        {
            throw  cv::Exception ( 0, " Can't read image \"" + sampleImageName + "\" ",
                                   "int main ( int argc, char** argv ) ", "SearchEngine.cpp", __LINE__ );
        }
    }
    catch ( cv::Exception error )
    {
        std::cerr << error.what () << std::endl;
        _errorsOccured = true;
        return;
    }

    _folderName = folderName;

    if ( _folderName[ _folderName.length () ] != '\\' )
    {
        _folderName += '\\';
    }

    cv::glob ( _folderName, _imageNamesFromFolder );

    _deltaEThreshold = deltaEThreshold;
    _displayMode = displayMode;
    _errorsOccured = false;
}

void SearchEngine::preprocessSampleImage ()
{
    imageAnalysis::ObjectDetector detector ( _referenceImage );

    detector.selectROI ( "Select region of interest" );

    imageAnalysis::AverageColorExtractor extractor ( detector.detectObject () );

    std::unique_ptr<colorConvertions::ColorSpaceConverter> labConverter =
        std::make_unique<colorConvertions::LABConverter> ( extractor.getAverageColor () );

    _averageColor = labConverter->convert ();
    _colorThreshold = cv::Scalar ( 40, 40, 40 ); //Hardcoded needs an improvement

    findColorBoundaries ();
}

cv::Mat SearchEngine::getColorRegion ( const cv::Mat& image )
{
    cv::Mat processedImage = image.clone();
    cv::Mat HSVImage, tmp, regionTemplate;
    cv::Mat morphologyStructure = cv::getStructuringElement ( cv::MORPH_RECT, cv::Size ( 11, 11 ) );

    cv::bilateralFilter ( processedImage, tmp, 9, 150, 150 );
    cv::cvtColor ( tmp, HSVImage, cv::COLOR_BGR2HSV );

    cv::inRange ( HSVImage, _lowerPixelValueBoundary,  _upperPixelValueBoundary, regionTemplate );

    morphologyEx ( regionTemplate, regionTemplate, cv::MORPH_OPEN, morphologyStructure );
    morphologyEx ( regionTemplate, regionTemplate, cv::MORPH_CLOSE, morphologyStructure );

    cv::Mat result ( image.size (), CV_8UC3, cv::Scalar ( 255, 255, 255 ) );
    image.copyTo ( result, regionTemplate );

    return result;
}

cv::Mat SearchEngine::getReferenceImage ()
{
    return _referenceImage;
}

void SearchEngine::runSearch ()
{
    if ( !_errorsOccured )
    {
        preprocessSampleImage ();
        index ();

        DisplayMode mode;

        switch ( _displayMode )
        {
        case 1:
        {
            mode = DisplayMode::DISPLAY_NAMES;
            break;
        }
        case 2:
        {
            mode = DisplayMode::DISPLAY_IMAGES;
            break;
        }
        case 3:
        {
            mode = DisplayMode::DISPLAY_IMAGES_HIGHLIGHTED;
            break;
        }
        case 4:
        {
            mode = DisplayMode::DISPLAY_BOUNDING_BOX;
            break;
        }
        default:
        {
            break;
        }
        }

        displayResults ( mode );
    }
    else
    {
        std::cerr << "Errors occured\n";
        return;
    }
}

void SearchEngine::displayResults ( DisplayMode mode )
{
    if ( indexedImages.size () == 0 )
    {
        std::cout << std::endl << "Found " << indexedImages.size () << " results\n";
        return;
    }

    char switcher;

    std::cout << std::endl << "Found " << indexedImages.size () << " results, show? (Y/n) ";
    std::cin >> switcher;

    if ( switcher == 'Y' || switcher == 'y' )
    {
        switch ( mode )
        {
        case DisplayMode::DISPLAY_NAMES:
        {
            std::cout << "\nMatching images names:" << std::endl << std::endl;

            for ( auto &result : indexedImages )
            {
                std::string delimiter = "\\";

                size_t nameStartingPos = result.first.find_last_of ( delimiter );

                std::string formattedImageName ( result.first.begin () + nameStartingPos + 1 );

                std::cout << formattedImageName << std::endl;
            }

            break;
        }
        case DisplayMode::DISPLAY_IMAGES:
        {
            cv::namedWindow ( "Search result", cv::WindowFlags::WINDOW_AUTOSIZE );

            for ( auto &result : indexedImages )
            {
                cv::Mat image = cv::imread ( result.first );

                cv::imshow ( "Search result", image );

                cv::waitKey ();
            }

            cv::destroyWindow ( "Search result" );

            break;
        }
        case DisplayMode::DISPLAY_IMAGES_HIGHLIGHTED:
        {
            cv::namedWindow ( "Search result", cv::WindowFlags::WINDOW_AUTOSIZE );

            for ( auto &result : indexedImages )
            {
                cv::Mat image = cv::imread ( result.first );

                for ( size_t i = 0; i < result.second.size(); i++ )
                {
                    cv::drawContours ( image, result.second, i, cv::Scalar ( 0, 0, 255 ), 2 );               
                }

                cv::imshow ( "Search result", image );

                cv::waitKey ();
            }

            cv::destroyWindow ( "Search result" );

            break;
        }
        case DisplayMode::DISPLAY_BOUNDING_BOX:
        {
            cv::namedWindow ( "Search result", cv::WindowFlags::WINDOW_AUTOSIZE );

            for ( auto &result : indexedImages )
            {
                cv::Mat image = cv::imread ( result.first );

                for ( size_t i = 0; i < result.second.size (); i++ )
                {
                    if ( result.second[ i ].size () >= 100 )
                    {
                        cv::Rect box = cv::boundingRect ( result.second[ i ] );
                        cv::rectangle ( image, box, cv::Scalar ( 0, 0, 255 ), 2 );
                    }
                }

                cv::imshow ( "Search result", image );

                cv::waitKey ();
            }

            cv::destroyWindow ( "Search result" );

            break;
        }
        default:
            break;
        }
    }
}

bool SearchEngine::findMatchingPixels ( const cv::Mat sampleImage )
{
    cv::Mat roi = getColorRegion ( sampleImage ).clone() ;

    if ( !roi.empty () )
    {
        imageAnalysis::AverageColorExtractor extractor ( roi );

        std::unique_ptr<colorConvertions::ColorSpaceConverter> converter = 
            std::make_unique<colorConvertions::LABConverter> ( extractor.calculateAverageColor () );

        imageAnalysis::ColorComparator comparator ( _averageColor, _deltaEThreshold );
        cv::Scalar pixel = converter->convert ();

        if ( comparator.checkCMCColorDifference ( pixel ) )
        {
            edges = findEdges ( roi );

            return true;
        }
    }

    return false;
}

std::vector<std::vector<cv::Point>> SearchEngine::findEdges ( const cv::Mat roi )
{
    std::vector < std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat processedImage = roi.clone (), edges;

    cv::GaussianBlur ( processedImage, processedImage, cv::Size ( 5, 5 ), 1 );
    cv::Canny ( processedImage, edges, 0, 255 );
    cv::findContours ( edges, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point ( 0, 0 ) );

    return contours;
}

void SearchEngine::findColorBoundaries ()
{   
    std::unique_ptr<colorConvertions::ColorSpaceConverter> labConverter =
        std::make_unique<colorConvertions::LABConverter> ( _averageColor );

    std::unique_ptr<colorConvertions::ColorSpaceConverter> hsvConverter =
        std::make_unique<colorConvertions::HSVConverter> ( labConverter->convertBack () );

    cv::Scalar averageHSVPixel = hsvConverter->convert ();

    applyThreshold ( averageHSVPixel );
}

void SearchEngine::index ()
{
    std::cout << "\nSearch in progress:\n.";

    for ( auto &it : _imageNamesFromFolder )
    {
        std::cout << " .";

        cv::Mat sampleImage = cv::imread ( it );

        if ( findMatchingPixels ( sampleImage ) )
        {
            indexedImages.emplace ( it, edges );
            edges.clear ();
        }
    }
    std::cout << std::endl;
}

void SearchEngine::applyThreshold ( const cv::Scalar  pixel )
{
    _lowerPixelValueBoundary = pixel - _colorThreshold;
    _upperPixelValueBoundary = pixel + _colorThreshold;

    for ( size_t i = 0; i < 3; i++ )
    {
        _lowerPixelValueBoundary[ i ] = _lowerPixelValueBoundary[ i ] >= 0 ? _lowerPixelValueBoundary[ i ] : 0;
    }
    _upperPixelValueBoundary[ 0 ] = _upperPixelValueBoundary[ 0 ] <= 180 ? _upperPixelValueBoundary[0] : 180;

    _upperPixelValueBoundary[ 1 ] = _upperPixelValueBoundary[ 1 ] <= 255 ? _upperPixelValueBoundary[1] : 255;
    _upperPixelValueBoundary[ 2 ] = _upperPixelValueBoundary[ 2 ] <= 255 ? _upperPixelValueBoundary[ 2 ] : 255;
}

bool SearchEngine::checkRange ( const cv::Scalar labpixel )
{
    bool lightnessIsInRange = labpixel[ 0 ] >= 0.0 && labpixel[ 0 ] <= 100.0;

    bool aIsInRange = labpixel[ 1 ] >= -128.0 && labpixel[ 1 ] <= 128.0;
    bool bIsInRange = labpixel[ 2 ] >= -128.0 && labpixel[ 2 ] <= 128.0;

    return lightnessIsInRange && aIsInRange && bIsInRange;
}

