#pragma once

#include <vector>

#include <opencv2\opencv.hpp>

namespace imageAnalysis
{
    enum class SelectionState
    {
        STANDBY, PROCESSED, SET
    };

    static void mouseCallback ( int mouseEvent, int x, int y, int flags, void* userdata );

	class ObjectDetector
	{
	public:
		ObjectDetector ();

		ObjectDetector ( cv::Mat &image );

        ObjectDetector ( const ObjectDetector & newDetector );

		cv::Mat getSourceImage () const;

		void setSourceImage ( const cv::Mat &newImage );

        void selectROI (std::string windowName);

        void setMaskValue ( int flag, cv::Point location );

		cv::Mat detectObject ();

        cv::Point startingRectanglePoint, finalRectanglePoint;

        cv::Mat mask;

        cv::Mat displayedSelection;

        cv::Mat roi;

        cv::Mat foreground;

        std::string windowName;

        cv::Rect boundingRectangle;

        SelectionState rectangleState;

        SelectionState backgroundEdditingState;

        SelectionState foregroundEdditingState;

		~ObjectDetector ();

	private:

        std::vector<cv::Point> objectCoords;

		cv::Mat _sourceImage;

		bool _hasAlphaChanel;
	};
}


