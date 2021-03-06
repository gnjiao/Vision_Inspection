// CalibratorForm class: Do all calibration steps
// Author: @ZC
// Date: created on 05 Sep 2018
#ifndef CALIBRATORFORM_H
#define CALIBRATORFORM_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFile>

#include <unistd.h> // for usleep()
#include <math.h>
#include <iostream>
using namespace std;

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

namespace Ui {
class CalibratorForm;
}

class CalibratorForm : public QWidget
{
    Q_OBJECT

public:
    explicit CalibratorForm(QWidget *parent = nullptr);
    ~CalibratorForm();

signals:
    void sendFrameRequest();
    void sendFrameToShow(cv::Mat frame);
    void sendUpdateConfig();
    void sendCaliCommand(QString command);

public slots:
    void receiveFrame(cv::Mat frame);
    void receiveMousePressedPosition(QPoint& pos);

private slots:
    void on_pushButtonBGStart_clicked();
    void on_pushButtonRulerStart_clicked();
    void on_pushButtonCalculate_clicked();
    void receiveCancelForm();
    void receiveOkForm();
    void receiveSetButtonVisible(QString input);
    void on_pushButtonRulerSelect_clicked();
    void on_pushButtonRulerSelectStop_clicked();

private:
    Ui::CalibratorForm *ui;
    cv::Mat frameCopy, roiBGFrame, roiRLFrame, roiRLFrame2Show;
    cv::Mat thresholdImg, contourImg;
    cv::Scalar bgMeanBGR, bgMeanHSV, bgMeanGS;
    cv::Scalar objMeanBGR, objMeanHSV, objMeanGS;
    cv::Rect ROI;
    bool newFrameAvaviable = false;
    double pixelDistance, pixelPERmm;
    void initialSetup();
    void extractBGColorMean();
    void extractObjColorMean(cv::Mat image);
    void autoCalibrateRuler();
    void manualCalibrateRuler();
    void hsvThreshold();
    void diffThreshold();
    void grayscaleThreshold();
    void getContour();
    void writeCaliConf();
};

#endif // CALIBRATORFORM_H
