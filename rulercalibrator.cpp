#include "rulercalibrator.h"

RulerCalibrator::RulerCalibrator(cv::Mat& inputFrame, const double& realDistance, double& autoPPMM)
{
    frame = inputFrame.clone();
    distanceInMM = realDistance;
    preprocessing();
    thresholding();
    getContours();
    autoPPMM = PPMM;
    inputFrame = frame;
}

void RulerCalibrator::preprocessing()
{
    cv::Rect ROI = Rect(400, 400, 1150, 750);
    roiFrame = frame(ROI);
    cv::cvtColor(roiFrame, grayFrame, COLOR_BGR2GRAY);
    cv::GaussianBlur(grayFrame, grayFrame, Size(3, 3), 0);
}

void RulerCalibrator::thresholding()
{
    //cv::Canny(grayFrame, cannyImage, 80, 200, 3); // edge pnly
    cv::threshold(grayFrame, thresholdImage, 135, 210, THRESH_BINARY_INV);
    //cv::dilate(thresholdImage, thresholdImage, Mat());
    //cv::erode(thresholdImage, thresholdImage, Mat());
}

void RulerCalibrator::getContours()
{
    std::vector<vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    cv::findContours(thresholdImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    //std::vector<cv::Rect> boundRect( contours.size() );
    std::vector<vector<Point> > contours_poly( contours.size() );
    std::vector<Point2f> center( contours.size() );
    std::vector<float> radius( contours.size() );
    for (size_t i = 0; i < contours.size(); i++)
    {
        //cv::drawContours(roiFrame, contours, i, Scalar(255, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
        cv::approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i]);
        cv::circle(roiFrame, center[i], (int)radius[i], Scalar(0, 255, 0), 2, 8, 0 );
        cv::line(roiFrame, Point(center[i].x-radius[i], center[i].y), Point(center[i].x+radius[i], center[i].y), Scalar(255, 0, 0), 1, 8);
        cv::circle(roiFrame, center[i], 1, Scalar(0, 0, 255), 3, 8, 0 );

        // Create an output string stream
        std::ostringstream streamObj3;
        // Set Fixed -Point Notation
        streamObj3 << std::fixed;
        // Set precision to 1 digits
        streamObj3 << std::setprecision(1);
        //Add double to stream
        streamObj3 << radius[i];
        // Get string from output string stream
        std::string strObj3 = streamObj3.str();
        string printDistance = "R:" + strObj3;
        cv::putText(roiFrame, printDistance, center[i], 1, 1, Scalar(0, 155, 255), 1, 8);
        //boundRect[i] = boundingRect(contours[i]);
        PPMM += radius[i];
    }
    PPMM /= int(contours.size());
    PPMM /= distanceInMM;

    cv::namedWindow("test", 1);
    cv::imshow("test", frame);
    cv::waitKey(0);
}

void RulerCalibrator::drawContours()
{

}