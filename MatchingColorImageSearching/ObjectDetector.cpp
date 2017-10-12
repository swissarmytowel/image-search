#include <iostream>

#include "ObjectDetector.h"

#define DEBUG


using namespace imageAnalysis;

ObjectDetector::ObjectDetector ()
{}

ObjectDetector::ObjectDetector ( cv::Mat & image )
{
    _sourceImage = image;
}

imageAnalysis::ObjectDetector::ObjectDetector ( const ObjectDetector & newDetector )
{
    _sourceImage = newDetector.getSourceImage ();
    windowName = newDetector.windowName;
    rectangleState = newDetector.rectangleState;
}

cv::Mat ObjectDetector::getSourceImage () const
{
    return _sourceImage;
}

void ObjectDetector::setSourceImage ( const cv::Mat & newImage )
{
    _sourceImage = newImage;
}

cv::Mat ObjectDetector::detectObject ()
{
    cv::Mat backgroundPixels, foregroundPixels, foregroundOnlyMask;

    if ( !_sourceImage.empty () && !mask.empty () && boundingRectangle.area () != 0 )
    {
        cv::grabCut ( _sourceImage, mask, boundingRectangle, 
                      backgroundPixels, foregroundPixels, 1, cv::GC_INIT_WITH_MASK );
    }
    else
    {
        return _sourceImage;
    }

    foregroundOnlyMask = mask & 1;

    cv::Mat foreground ( _sourceImage.size (), CV_8UC3, cv::Scalar ( 255, 255, 255 ) );

    _sourceImage.copyTo ( foreground, foregroundOnlyMask );

#ifdef DEBUG
    cv::imshow ( windowName, foreground );

    cv::waitKey ();
#endif // DEBUG

    return foreground;
}

ObjectDetector::~ObjectDetector ()
{}

void imageAnalysis::ObjectDetector::selectROI ( std::string _windowName )
{
    std::cout << "\n====USAGE====\n" << 
        "Select region in rectangle\nAdjust with lines using:\n\t" <<
        "Shift key for foreground\n\tCtrl key for background\n";

    mask = cv::Mat ( _sourceImage.size (), CV_8UC1 );
    mask.setTo ( cv::Scalar::all ( cv::GrabCutClasses::GC_BGD ) );

    displayedSelection = _sourceImage;
    windowName = _windowName;

    cv::namedWindow ( windowName );
    cv::imshow ( windowName, _sourceImage );

    rectangleState = SelectionState::STANDBY;
    foregroundEdditingState = SelectionState::STANDBY;
    backgroundEdditingState = SelectionState::STANDBY;

    while ( true )
    {
        cv::setMouseCallback ( windowName, mouseCallback, this );

        if ( cv::waitKey () )
        {
            break;
        }
    }
}

void imageAnalysis::mouseCallback ( int mouseEvent, int x, int y, int flags, void * userdata )
{
    ObjectDetector &detector ( *static_cast < ObjectDetector* > ( userdata ) );

    switch ( mouseEvent )
    {
    case cv::MouseEventTypes::EVENT_LBUTTONDOWN:
    {
        if ( flags & cv::MouseEventFlags::EVENT_FLAG_CTRLKEY )
        {
            if ( detector.backgroundEdditingState != SelectionState::PROCESSED )
            {
                detector.backgroundEdditingState = SelectionState::PROCESSED;
            }
        }
        else
            if ( flags & cv::MouseEventFlags::EVENT_FLAG_SHIFTKEY )
            {
                if ( detector.foregroundEdditingState != SelectionState::PROCESSED )
                {
                    detector.foregroundEdditingState = SelectionState::PROCESSED;
                }
            }
            else
                if ( detector.rectangleState != SelectionState::PROCESSED )
                {
                    detector.startingRectanglePoint = cv::Point ( x, y );

                    detector.rectangleState = SelectionState::PROCESSED;
                }

        break;
    }
    case cv::MouseEventTypes::EVENT_MOUSEMOVE:
    {
        if ( detector.rectangleState == SelectionState::PROCESSED )
        {
            cv::Mat tmp ( detector.getSourceImage ().clone () );

            detector.finalRectanglePoint = cv::Point ( x, y );

            cv::rectangle ( tmp, detector.startingRectanglePoint, 
                            detector.finalRectanglePoint, cv::Scalar ( 0, 255, 0 ), 1 );

            detector.displayedSelection = tmp;

            cv::imshow ( detector.windowName, tmp );
        }
        else
        {
            if ( detector.backgroundEdditingState == SelectionState::PROCESSED ||
                 detector.foregroundEdditingState == SelectionState::PROCESSED )
            {
                detector.setMaskValue ( flags, cv::Point ( x, y ) );

                cv::Scalar color = 
                    ( flags & cv::MouseEventFlags::EVENT_FLAG_CTRLKEY ) ?
                                                                        cv::Scalar ( 0, 255, 0 ) :
                                                                        cv::Scalar ( 0, 0, 255 );

                cv::circle ( detector.displayedSelection, cv::Point ( x, y ), 1, color );

                cv::imshow ( detector.windowName, detector.displayedSelection );
            }
        }

        break;
    }
    case cv::MouseEventTypes::EVENT_LBUTTONUP:
    {
        if ( detector.rectangleState == SelectionState::PROCESSED )
        {
            detector.finalRectanglePoint = cv::Point ( x, y );

            bool invalidRectangleX = ( detector.startingRectanglePoint.x > detector.finalRectanglePoint.x );
            bool invalidRectangleY = ( detector.startingRectanglePoint.y > detector.finalRectanglePoint.y );

            if ( invalidRectangleX )
            {
                std::swap ( detector.finalRectanglePoint.x, detector.startingRectanglePoint.x );
            }
            if ( invalidRectangleY )
            {
                std::swap ( detector.finalRectanglePoint.y, detector.startingRectanglePoint.y );
            }

            detector.boundingRectangle = cv::Rect ( detector.startingRectanglePoint.x, 
                                                    detector.startingRectanglePoint.y, 
                                                    detector.finalRectanglePoint.x - detector.startingRectanglePoint.x, 
                                                    detector.finalRectanglePoint.y - detector.startingRectanglePoint.y );

            detector.rectangleState = SelectionState::SET;

            detector.mask ( detector.boundingRectangle ).setTo ( cv::GC_PR_FGD );
        }
        else
        {
            if ( detector.backgroundEdditingState == SelectionState::PROCESSED ||
                 detector.foregroundEdditingState == SelectionState::PROCESSED )
            {
                cv::Mat tmp ( detector.displayedSelection.clone () );

                detector.setMaskValue ( flags, cv::Point ( x, y ) );

                cv::imshow ( detector.windowName, tmp );

                detector.backgroundEdditingState = 
                    detector.backgroundEdditingState == SelectionState::PROCESSED ?
                    SelectionState::SET : 
                    detector.backgroundEdditingState;

                detector.foregroundEdditingState = 
                    detector.foregroundEdditingState == SelectionState::PROCESSED ? 
                    SelectionState::SET :
                    detector.foregroundEdditingState;
            }
        }
        break;
    }
    default:
        break;
    }
}

void ObjectDetector::setMaskValue ( int flag, cv::Point location )
{
    if ( flag & cv::MouseEventFlags::EVENT_FLAG_CTRLKEY )
    {
        cv::rectangle ( mask, location, cv::Point ( location + cv::Point ( 1, 1 ) ), cv::GrabCutClasses::GC_BGD, -1 );
    }

    else if ( flag & cv::MouseEventFlags::EVENT_FLAG_SHIFTKEY )
    {
        cv::rectangle ( mask, location, cv::Point ( location + cv::Point ( 1, 1 ) ), cv::GrabCutClasses::GC_FGD, -1 );
    }
}
