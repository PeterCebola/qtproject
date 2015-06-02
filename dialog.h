#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QDebug>
#include <QValidator>
#include <QMouseEvent>
#include <QEvent>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include <math.h>
//#include <array>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "armadillo"
//#include "libptp2/ptp.h"

#define ARMA_USE_LAPACK
#define ARMA_USE_BLAS
#define ARMA_BLAS_UNDERSCORE

#define CONTOUR_VEC_SIZE 2000

using namespace cv;
using namespace std;
//using namespace arma;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    cv::Mat img, img_hole;
    QImage *qimage;
    QValidator *int2048Validator, *int255Validator, *int65536Validator,
               *doubleValidator, *int180Validator, *int2147483647Validator;
    bool ellipsisCompare(cv::RotatedRect const &a, cv::RotatedRect const &b);

private slots:
    void on_loadImage_clicked();
    void on_savePushButton_clicked();
    void on_confirmPushButton_clicked();
    void on_printValuesPushButton_clicked();
    void on_radiusCalcPushButton_clicked();
    void on_centerCalcPushButton_clicked();
    void on_exportPushButton_clicked();
    void on_cameraPushButton_clicked();
    void on_setupCameraPushButton_clicked();
    void on_resetValuesPushButton_clicked();
    void on_leastSquareFitPushButton_clicked();
    void on_holeDetectHTPushButton_clicked();
    void on_holeDetectCPushButton_clicked();
    void on_holeDetectSplitPushButton_clicked();
    void on_functionChoice_activated(int index);
    void on_targetChoice_activated(int index);
    void on_keepDataCheckBox_toggled(bool checked);
    void on_contours1lineEdit_textEdited(const QString &arg1);
    void on_hough1lineEdit_textEdited(const QString &arg1);
    void on_hough2lineEdit_textEdited(const QString &arg2);
    void on_hough3lineEdit_textEdited(const QString &arg3);
    void on_hough4lineEdit_textEdited(const QString &arg4);
    void on_hough5lineEdit_textEdited(const QString &arg5);
    void on_hough6lineEdit_textEdited(const QString &arg6);
    void on_holeDetectHT1lineEdit_textEdited(const QString &arg1);
    void on_holeDetectHT2lineEdit_textEdited(const QString &arg2);
    void on_holeDetectHT3lineEdit_textEdited(const QString &arg3);
    void on_holeDetectHT4lineEdit_textEdited(const QString &arg4);
    void on_holeDetectHT5lineEdit_textEdited(const QString &arg5);
    void on_holeDetectHT6lineEdit_textEdited(const QString &arg6);
    void on_holeDetect1lineEdit_textEdited(const QString &arg1);
    void on_holeDetect2lineEdit_textEdited(const QString &arg2);
    void on_holeDetect3lineEdit_textEdited(const QString &arg3);
    void on_holeDetect1lineEdit_2_textEdited(const QString &arg4);
    void on_holeDetect2lineEdit_2_textEdited(const QString &arg5);
    void on_holeDetect3lineEdit_2_textEdited(const QString &arg6);
    void on_holeDetectSplit1lineEdit_textEdited(const QString &arg1);
    void on_holeDetectSplit2lineEdit_textEdited(const QString &arg1);
    void on_holeDetectSplit3lineEdit_textEdited(const QString &arg1);
    void on_hough1Slider_sliderMoved(int position);
    void on_hough2Slider_sliderMoved(int position);
    void on_hough3Slider_sliderMoved(int position);
    void on_holeDetectHT1Slider_sliderMoved(int position);
    void on_holeDetectHT2Slider_sliderMoved(int position);
    void on_holeDetectHT3Slider_sliderMoved(int position);
    void Mouse_current_pos();

    void on_getDepthDataPushButton_clicked();

    void on_fixPerspectivePushButton_clicked();

signals:
    void Mouse_Pos();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
