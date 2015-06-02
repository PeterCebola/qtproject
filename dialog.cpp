#include "dialog.h"
#include "ui_dialog.h"
#include "mouse_qlabel.h"

#include <armadillo>


//---------------------------------------------------------------------------------------
//------------------------ Variable declaration & initialization ------------------------
//---------------------------------------------------------------------------------------

// Export function variables
QString img_filename;
QString program_filepath = "/home/pi/Dropbox/Algoritmo/OpenCV/OpenCvTest1-build-desktop-Qt_4_8_1_in_PATH__System__Release";
        //"/home/default/Dropbox/[TESE]/Algoritmo/OpenCV/OpenCvTest1-build-desktop-Qt_4_8_1_in_PATH__System__Release";
QString image_filepath = "/home/pi/Dropbox/Imagens";
        //"/home/default/Desktop/ImagensTeste/";
QFile file("temp");

// Photo function variables
int camIndex = 1;
VideoCapture cap(camIndex);
Mat cameraFrame;

// Program variable
int functionChoiceState = 0;
int targetType = -1;
bool keepDataFlag = false;

// Hough transform variables
double accumResolutionTarget = 1;
double accumResolutionHole = 1;
int minDistTarget = 20;
int cannyThreshTarget = 100;
int accumThreshTarget = 100;
int minRadiusTarget = 100;
int maxRadiusTarget = 300;
int minDistHole = 4;
int cannyThreshHole = 10;
int accumThreshHole = 1;
int minRadiusHole = 20;
int maxRadiusHole = 30;

// Contour extraction variables
double targetRadius;
double minAreaTarget = 90792;
double maxAreaTarget = 125663;
size_t contourPointsTargetMin = 2500;
size_t contourPointsTargetMax = 2500;
vector<vector<Point> > contoursTarget;
vector<Vec4i> hierarchyTarget;
Point targetCentroid(0,0), targetCenter;

// Hole contour detection variables
vector<double> holeRadius;
vector<Point> holeCenter;
size_t contourPointsHole = 100;
vector<vector<Point> > contoursHole;
vector<Vec4i> hierarchyHole;
vector<Point> holeCentroid;
vector<Point> holeDefects;
double minAreaHole = 1250;
double maxAreaHole = 2265;
int minConvexityDefect = 4;
int HContourMax = 180;
int SContourMax = 255;
int VContourMax = 100;
int HContourMin = 0;
int SContourMin = 0;
int VContourMin = 0;

// Hole detection variables
int maxIts = 1000;
float tol = 0.00001; //1e-5;
int R_H_Max = 28;
int G_S_Max = 255;
int B_V_Max = 255;
int R_H_Min = 17;
int G_S_Min = 0;
int B_V_Min = 0;
bool HSV_flag = 0;

// Text messages
QString noImageErrorMessage = "Image doesn't exist or wasn't loaded.\n";
QString noImageErrorMessage2 = "Image wasn't loaded.\n";
QString noCameraErrorMessage = "Couldn't detect camera.\n";
QString noCentroidErrorMessage = "No centroid detected.\n";
QString noContoursErrorMessage = "No contours detected.\n";
QString noCenterErrorMessage = "No center detected.\n";
QString ioErrorMessage = "Couldn't read/write file.\n";
QString snapshotErrorMessage = "Snapshot wasn't taken.\n";
QString systemErrorMessage = "Could not write command.\n";
QString cmdErrorMessage = "Error running command.\n";
QString cancelButtonErrorMessage = "Operation canceled.\n";
QString convergenceLSQErrorMessage = "Gauss Newton iteration failed to converge.\n";
QString noOverlappedContoursErrorMessage = "No overlapped holes detected.\n";
QString valuesResetInfoMessage = "Values reset.\n";
QString fileWrittenInfoMessage = "File written.\n";
QString imageLoadedInfoMessagePart1 = "Image ";
QString imageLoadedInfoMessagePart2 = " loaded.\n";
QString getParamsInfoMessage1 = "300m Rifle target selected.\n";
QString getParamsInfoMessage2 = "50m Rifle target selected.\n";
QString getParamsInfoMessage3 = "10m Air Rifle target selected.\n";
QString getParamsInfoMessage4 = "50m Pistol or 25m Precision Pistol target selected.\n";
QString getParamsInfoMessage5 = "25m Center Fire Pistol target selected.\n";
QString getParamsInfoMessage6 = "10m Air Pistol target selected.\n";
QString getParamsInfoMessage7 = "SCAT target selected.\n";

bool ellipsisCompare(cv::RotatedRect const &a, cv::RotatedRect const &b) {
    return (a.size.width+a.size.height)<(b.size.width+b.size.height);
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //setWindowFlags(Qt::Dialog);
    ui->setupUi(this);

    // Connect slot for mouse pointer pixel location
    //connect(ui->imageContainerLabel,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_current_pos()));
    connect(ui->testLabel,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_current_pos()));

    // Line edit validators initialization
    int180Validator = new QIntValidator(0, 180, this);
    int255Validator = new QIntValidator(0, 255, this);
    int2048Validator = new QIntValidator(0, 2048, this);
    int65536Validator = new QIntValidator(0, 65536, this);
    int2147483647Validator = new QIntValidator(0, 2147483647, this);
    doubleValidator = new QDoubleValidator(0.0001, 1000.0000, 4, this);

    //doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    // Text edit initializations
    ui->infoPlainTextEdit->setReadOnly(true);
    ui->info2PlainTextEdit->setReadOnly(true);

    // Set up initial window appearance
    ui->contours1lineEdit->hide();
    ui->contours1Label->hide();
    ui->hough1Slider->hide();
    ui->hough1lineEdit->hide();
    ui->hough1Label->hide();
    ui->hough2Slider->hide();
    ui->hough2lineEdit->hide();
    ui->hough2Label->hide();
    ui->hough3Slider->hide();
    ui->hough3lineEdit->hide();
    ui->hough3Label->hide();
    ui->hough4lineEdit->hide();
    ui->hough4Label->hide();
    ui->hough5lineEdit->hide();
    ui->hough5Label->hide();
    ui->hough6lineEdit->hide();
    ui->hough6Label->hide();
    ui->holeDetectHT1Slider->hide();
    ui->holeDetectHT1lineEdit->hide();
    ui->holeDetectHT2Slider->hide();
    ui->holeDetectHT2lineEdit->hide();
    ui->holeDetectHT3Slider->hide();
    ui->holeDetectHT3lineEdit->hide();
    ui->holeDetectHT4lineEdit->hide();
    ui->holeDetectHT5lineEdit->hide();
    ui->holeDetectHT6lineEdit->hide();
    ui->holeDetect1lineEdit->hide();
    ui->holeDetect1lineEdit_2->hide();
    ui->holeDetect1Label->hide();
    ui->holeDetect2lineEdit->hide();
    ui->holeDetect2lineEdit_2->hide();
    ui->holeDetect2Label->hide();
    ui->holeDetect3lineEdit->hide();
    ui->holeDetect3lineEdit_2->hide();
    ui->holeDetect3Label->hide();
    ui->holeDetect4Label->hide();
    ui->holeDetect5Label->hide();
    ui->holeDetectSplit1lineEdit->hide();
    ui->holeDetect6Label->hide();
    ui->holeDetectSplit2lineEdit->hide();
    ui->holeDetect7Label->hide();
    ui->holeDetectSplit3lineEdit->hide();
    ui->holeDetect8Label->hide();
    ui->keepDataCheckBox->show();
    ui->holeDetectHTPushButton->hide();
    ui->holeDetectCPushButton->hide();
    ui->holeDetectSplitPushButton->hide();
    ui->exportPushButton->hide();
    ui->confirmPushButton->hide();
    ui->savePushButton->hide();
    ui->printValuesPushButton->hide();
    ui->centerCalcPushButton->hide();
    ui->radiusCalcPushButton->hide();
    ui->leastSquareFitPushButton->hide();
    ui->pixelCoordinateLabel->hide();
    ui->infoPlainTextEdit->hide();
    ui->info2PlainTextEdit->show();
    ui->targetChoice->hide();
    ui->cameraPushButton->hide();
    ui->setupCameraPushButton->hide();
    ui->resetValuesPushButton->hide();
    ui->getDepthDataPushButton->hide();
    ui->fixPerspectivePushButton->hide();
}

Dialog::~Dialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
//--------------------------------- Class function code ---------------------------------
//---------------------------------------------------------------------------------------

void Dialog::on_loadImage_clicked() // Load an image from PC
{
    // Choose image and store its path
    img_filename = QFileDialog::getOpenFileName(this,
                                                tr("Open Image"),
                                                image_filepath,
                                                tr("Files (*.png *.jpg *.tiff *.bmp)"));

    // Do nothing if filename is empty
    if ( img_filename.isEmpty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Get image from specified path
    img = imread(img_filename.toStdString(), CV_LOAD_IMAGE_COLOR);

    // Set up window appearance
    if( img.empty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    QFileInfo file(img_filename);
    QString imageLoadedInfoMessage = imageLoadedInfoMessagePart1 + file.baseName() +
                                     imageLoadedInfoMessagePart2;

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText(imageLoadedInfoMessage);
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    if( !keepDataFlag )
    {
        // Contour extraction variables
        //targetRadius = 0;
        contoursTarget.clear();
        hierarchyTarget.clear();
        targetCentroid.x = 0;
        targetCentroid.y = 0;

        // Hole contour detection variables
        holeRadius.clear();
        holeCenter.clear();
        contourPointsHole = 0;//10;
        contoursHole.clear();
        hierarchyHole.clear();
        holeCentroid.clear();
        holeDefects.clear();
    }

    // Convert image format from OpenCV to Qt and display it on label
    cvtColor(img, img, CV_BGR2RGB);
    QImage _img((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    ui->testLabel->setPixmap(QPixmap::fromImage(_img));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_functionChoice_activated(int index) // Function choice
{
    switch(index)
    {

    case 0: // Take photo from webcam
    {
        functionChoiceState = 0;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->show();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->show();
        ui->savePushButton->show();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->show();
        ui->setupCameraPushButton->show();
        ui->resetValuesPushButton->hide();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        break;
    }

    case 1: // Show original image
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 1;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->hide();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->hide();
        ui->savePushButton->hide();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->hide();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Display image on label
        QImage _img((uchar*)img.data, img.cols, img.rows,
                    img.step, QImage::Format_RGB888);
        ui->testLabel->setPixmap(QPixmap::fromImage(_img));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img));

        break;
    }

    case 2: // Convert image to grayscale
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 2;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->hide();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->hide();
        ui->savePushButton->hide();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->hide();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Convert image to gray
        Mat img_gray(img.size(), CV_8UC1);
        cvtColor(img, img_gray, CV_RGB2GRAY);

        // Convert OpenCV image to RGB888 for Qt
        QImage _img((const unsigned char*)img_gray.data,
                    img_gray.cols,img_gray.rows,
                    img_gray.step,QImage::Format_Indexed8);
        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img1,Qt::AutoColor));

        break;
    }

    case 3: // Detect image edges with Canny filter
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 3;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->hide();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->hide();
        ui->savePushButton->hide();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->hide();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Convert image to gray
        Mat img_gray(img.size(), CV_8UC1);
        cvtColor(img, img_gray, CV_RGB2GRAY);

        // Apply gaussian filter to smooth gray image (remove some noise)
        GaussianBlur(img_gray, img_gray, Size(5, 5), 2, 2);

        // Apply Canny filter to get image's edges
        Mat img_canny(img.size(), CV_8UC1);
        Canny(img_gray, img_canny, 30, 60, 3);

        //-----------------------------------------------------------------

        /*Mat img_hsv(img.size(), CV_8UC1);
        cvtColor(img, img_hsv, CV_BGR2HSV);

        Mat img_mod(img.rows, img.cols, CV_8UC1);

        inRange(img_hsv, Scalar(R_H_Min, B_V_Min, G_S_Min), Scalar(R_H_Max, B_V_Max, G_S_Max), img_mod);

        // Get contours with an erosion minus original image
        Mat contourExtractionElement = getStructuringElement(MORPH_RECT, Size(2,2), Point(-1,-1));
        Mat img_erode(img_mod.size(), CV_8UC1);
        erode(img_mod, img_erode, contourExtractionElement, Point(-1,-1), 1, BORDER_CONSTANT);
        Mat img_contour(img_mod.size(), CV_8UC1);
        img_contour = img_mod-img_erode;
        img_canny = img_canny+img_contour;
        img_canny = img_canny-img_mod;

        // Create circular structuring element
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3,3), Point(-1,-1));
        // Remove noise with binary image open function
        morphologyEx(img_canny, img_canny, MORPH_CLOSE,
                     element, Point(-1,-1), 1, BORDER_CONSTANT);

        dilate(img_canny, img_canny, contourExtractionElement, Point(-1,-1), 1, BORDER_CONSTANT);*/

        //-----------------------------------------------------------------

        // Convert OpenCV image to Indexed8 for Qt
        QImage _img((const unsigned char*)img_canny.data,
                    img_canny.cols,img_canny.rows,
                    img_canny.step,QImage::Format_Indexed8);
        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));

        break;
    }

    case 4: // Detect target information (radius & center) with Hough Transform
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 4;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->show();
        ui->hough1lineEdit->show();
        ui->hough1Label->show();
        ui->hough2Slider->show();
        ui->hough2lineEdit->show();
        ui->hough2Label->show();
        ui->hough3Slider->show();
        ui->hough3lineEdit->show();
        ui->hough3Label->show();
        ui->hough4lineEdit->show();
        ui->hough4Label->show();
        ui->hough5lineEdit->show();
        ui->hough5Label->show();
        ui->hough6lineEdit->show();
        ui->hough6Label->show();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->hide();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->show();
        ui->savePushButton->hide();
        ui->printValuesPushButton->show();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->show();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->show();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        // Set validators for user input fields
        ui->hough1lineEdit->setValidator(doubleValidator);
        ui->hough2lineEdit->setValidator(int255Validator);
        ui->hough3lineEdit->setValidator(int255Validator);
        ui->hough4lineEdit->setValidator(int2048Validator);
        ui->hough5lineEdit->setValidator(int2048Validator);
        ui->hough6lineEdit->setValidator(int2048Validator);

        break;
    }

    case 5: // Detect target information (radius & center) with contour functions
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 5;

        // Set up window appearance
        ui->contours1lineEdit->show();
        ui->contours1Label->show();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->hide();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->show();
        ui->confirmPushButton->show();
        ui->savePushButton->hide();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->show();
        ui->radiusCalcPushButton->show();
        ui->leastSquareFitPushButton->show();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->show();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->hide();

        // Set validators for user input fields
        ui->contours1lineEdit->setValidator(int65536Validator);

        break;
    }

    case 6: // Detect hole information (radius & center) with Hough Transform & contour functions
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        functionChoiceState = 6;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->show();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->show();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->show();
        ui->hough4lineEdit->hide();
        ui->hough4Label->show();
        ui->hough5lineEdit->hide();
        ui->hough5Label->show();
        ui->hough6lineEdit->hide();
        ui->hough6Label->show();
        ui->holeDetectHT1Slider->show();
        ui->holeDetectHT1lineEdit->show();
        ui->holeDetectHT2Slider->show();
        ui->holeDetectHT2lineEdit->show();
        ui->holeDetectHT3Slider->show();
        ui->holeDetectHT3lineEdit->show();
        ui->holeDetectHT4lineEdit->show();
        ui->holeDetectHT5lineEdit->show();
        ui->holeDetectHT6lineEdit->show();
        ui->holeDetect1lineEdit->show();
        ui->holeDetect1lineEdit_2->show();
        ui->holeDetect1Label->show();
        ui->holeDetect2lineEdit->show();
        ui->holeDetect2lineEdit_2->show();
        ui->holeDetect2Label->show();
        ui->holeDetect3lineEdit->show();
        ui->holeDetect3lineEdit_2->show();
        ui->holeDetect3Label->show();
        ui->holeDetect4Label->show();
        ui->holeDetect5Label->show();
        ui->holeDetectSplit1lineEdit->show();
        ui->holeDetect6Label->show();
        ui->holeDetectSplit2lineEdit->show();
        ui->holeDetect7Label->show();
        ui->holeDetectSplit3lineEdit->show();
        ui->holeDetect8Label->show();
        ui->keepDataCheckBox->show();
        ui->holeDetectHTPushButton->show();
        ui->holeDetectCPushButton->show();
        ui->holeDetectSplitPushButton->show();
        ui->exportPushButton->hide();
        ui->confirmPushButton->show();
        ui->savePushButton->hide();
        ui->printValuesPushButton->show();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->show();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->hide();
        ui->info2PlainTextEdit->show();
        ui->targetChoice->hide();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->show();
        ui->getDepthDataPushButton->show();
        ui->fixPerspectivePushButton->hide();

        // Set validators for user input fields
        ui->holeDetect1lineEdit->setValidator(int180Validator);
        ui->holeDetect1lineEdit_2->setValidator(int180Validator);
        ui->holeDetect2lineEdit->setValidator(int255Validator);
        ui->holeDetect2lineEdit_2->setValidator(int255Validator);
        ui->holeDetect3lineEdit->setValidator(int255Validator);
        ui->holeDetect3lineEdit_2->setValidator(int255Validator);
        ui->holeDetectHT1lineEdit->setValidator(doubleValidator);
        ui->holeDetectHT2lineEdit->setValidator(int255Validator);
        ui->holeDetectHT3lineEdit->setValidator(int255Validator);
        ui->holeDetectHT4lineEdit->setValidator(int2048Validator);
        ui->holeDetectHT5lineEdit->setValidator(int2048Validator);
        ui->holeDetectHT6lineEdit->setValidator(int2048Validator);
        ui->holeDetectSplit1lineEdit->setValidator(int2147483647Validator);
        ui->holeDetectSplit2lineEdit->setValidator(int2147483647Validator);
        ui->holeDetectSplit3lineEdit->setValidator(int65536Validator);

        break;
    }

    case 7: // Run full algorithm
    {
        functionChoiceState = 7;

        // Set up window appearance
        ui->contours1lineEdit->hide();
        ui->contours1Label->hide();
        ui->hough1Slider->hide();
        ui->hough1lineEdit->hide();
        ui->hough1Label->hide();
        ui->hough2Slider->hide();
        ui->hough2lineEdit->hide();
        ui->hough2Label->hide();
        ui->hough3Slider->hide();
        ui->hough3lineEdit->hide();
        ui->hough3Label->hide();
        ui->hough4lineEdit->hide();
        ui->hough4Label->hide();
        ui->hough5lineEdit->hide();
        ui->hough5Label->hide();
        ui->hough6lineEdit->hide();
        ui->hough6Label->hide();
        ui->holeDetectHT1Slider->hide();
        ui->holeDetectHT1lineEdit->hide();
        ui->holeDetectHT2Slider->hide();
        ui->holeDetectHT2lineEdit->hide();
        ui->holeDetectHT3Slider->hide();
        ui->holeDetectHT3lineEdit->hide();
        ui->holeDetectHT4lineEdit->hide();
        ui->holeDetectHT5lineEdit->hide();
        ui->holeDetectHT6lineEdit->hide();
        ui->holeDetect1lineEdit->hide();
        ui->holeDetect1lineEdit_2->hide();
        ui->holeDetect1Label->hide();
        ui->holeDetect2lineEdit->hide();
        ui->holeDetect2lineEdit_2->hide();
        ui->holeDetect2Label->hide();
        ui->holeDetect3lineEdit->hide();
        ui->holeDetect3lineEdit_2->hide();
        ui->holeDetect3Label->hide();
        ui->holeDetect4Label->hide();
        ui->holeDetect5Label->hide();
        ui->holeDetectSplit1lineEdit->hide();
        ui->holeDetect6Label->hide();
        ui->holeDetectSplit2lineEdit->hide();
        ui->holeDetect7Label->hide();
        ui->holeDetectSplit3lineEdit->hide();
        ui->holeDetect8Label->hide();
        ui->keepDataCheckBox->show();
        ui->holeDetectHTPushButton->hide();
        ui->holeDetectCPushButton->hide();
        ui->holeDetectSplitPushButton->hide();
        ui->exportPushButton->hide();
        ui->confirmPushButton->show();
        ui->savePushButton->hide();
        ui->printValuesPushButton->hide();
        ui->centerCalcPushButton->hide();
        ui->radiusCalcPushButton->hide();
        ui->leastSquareFitPushButton->hide();
        ui->pixelCoordinateLabel->hide();
        ui->infoPlainTextEdit->show();
        ui->info2PlainTextEdit->hide();
        ui->targetChoice->show();
        ui->cameraPushButton->hide();
        ui->setupCameraPushButton->hide();
        ui->resetValuesPushButton->show();
        ui->getDepthDataPushButton->hide();
        ui->fixPerspectivePushButton->show();

        break;
    }

    default: // Failsafe check
        break;
    }

}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_targetChoice_activated(int index)
{

    switch(index)
    {

    case 0: // 300m Rifle
    {
       minAreaHole = 1250;
       maxAreaHole = 2165;
       minAreaTarget = 5000;
       maxAreaTarget = 10000;
       minRadiusHole = 10;
       maxRadiusHole = 50;
       contourPointsTargetMin = 2500;
       contourPointsTargetMax = 2500;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage1);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 1: // 50m Rifle
    {
       minAreaHole = 1250;
       maxAreaHole = 2665;
       minAreaTarget = 5000;
       maxAreaTarget = 10000;
       minConvexityDefect = 4;
       minRadiusHole = 10;
       maxRadiusHole = 50;
       contourPointsTargetMin = 2500;
       contourPointsTargetMax = 2500;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage2);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 2: // 10m Air Rifle
    {
       minAreaHole = 1250;
       maxAreaHole = 2665;
       minAreaTarget = 91608;
       maxAreaTarget = 127234;
       minConvexityDefect = 4;
       minRadiusHole = 20;
       maxRadiusHole = 30;
       contourPointsTargetMin = 500;
       contourPointsTargetMax = 650;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage3);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 3: // 50m Pistol / 25m Precision
    {
       minAreaHole = 1250;
       maxAreaHole = 2665;
       minAreaTarget = 100531;
       maxAreaTarget = 157080;
       minConvexityDefect = 4;
       minRadiusHole = 20;
       maxRadiusHole = 30;
       contourPointsTargetMin = 500;
       contourPointsTargetMax = 650;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage4);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 4: // 25m Center Fire Pistol
    {
       minAreaHole = 1250;
       maxAreaHole = 2665;
       minAreaTarget = 50;
       maxAreaTarget = 100000;
       minConvexityDefect = 4;
       minRadiusHole = 10;
       maxRadiusHole = 50;
       contourPointsTargetMin = 25;
       contourPointsTargetMax = 50000;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage5);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 5: // 10m Air Pistol
    {
       minAreaHole = 1250;
       maxAreaHole = 2665;
       minAreaTarget = 307876;
       maxAreaTarget = 481056;
       minConvexityDefect = 4;
       minRadiusHole = 20;
       maxRadiusHole = 30;
       contourPointsTargetMin = 900;
       contourPointsTargetMax = 1300;
       targetType = index;
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage6);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       break;
    }

    case 6: // SCAT
    {
        minAreaHole = 1250;
        maxAreaHole = 2665;
        minAreaTarget = 100531;
        maxAreaTarget = 157080;
        minConvexityDefect = 4;
        minRadiusHole = 20;
        maxRadiusHole = 30;
        contourPointsTargetMin = 500;
        contourPointsTargetMax = 650;
        targetType = index;
        ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
        ui->infoPlainTextEdit->insertPlainText(getParamsInfoMessage7);
        ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
        break;
    }

    default: // Failsafe check
        break;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_confirmPushButton_clicked()
{
    switch(functionChoiceState) // Different actions depending on function choice
    {

    case 0: // Take photo from webcam
    {
        // If no camera detected, do nothing
        if( !cap.isOpened() )
        {
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noCameraErrorMessage);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Get a new frame from camera (only the 10th one)
        for( int i = 0; i < 10; i++ )
            cap >> cameraFrame;

        // Convert image format from OpenCV to Qt
        cvtColor(cameraFrame, cameraFrame, CV_BGR2RGB);

        // Convert OpenCV image to RGB888 for Qt
        QImage _img((uchar*)cameraFrame.data,
                    cameraFrame.cols,cameraFrame.rows,
                    cameraFrame.step,QImage::Format_RGB888);

        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img));

        break;
    }

    case 4: // Apply Hough Transform
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Convert image to gray
        Mat img_gray(img.size(), CV_8UC1);
        cvtColor(img, img_gray, CV_RGB2GRAY);

        // Apply gaussian filter to smooth gray image (remove some noise)
        GaussianBlur(img_gray, img_gray, Size(11, 11), 2, 2);

        // Apply Canny filter to get image's edges
        Mat img_canny(img.size(), CV_8UC1);
        Canny(img_gray, img_canny, 25, 50, 3);

        // Apply Hough Transform to find the circles
        vector<Vec3f> circles;
        HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT,
                     accumResolutionTarget, minDistTarget,
                     cannyThreshTarget, accumThreshTarget,
                     minRadiusTarget, maxRadiusTarget);

        cvtColor(img_canny, img_canny, CV_GRAY2BGR);

        // Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            // Draw circle center
            circle(img_canny, center, 3, Scalar(0,255,0), -1, 8, 0);
            // Draw circle outline
            circle(img_canny, center, radius, Scalar(0,0,255), 1, 8, 0);
        }

        // Convert OpenCV image to RGB888 for Qt
        QImage _img((uchar*)img_canny.data,img_canny.cols,img_canny.rows,
                    img_canny.step,QImage::Format_RGB888);
        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img));

        break;
    }

    case 5: // Choose applicable contours
    {
        // Do nothing if image is empty
        if( img.empty() ){
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Convert image to gray
        Mat img_gray(img.size(), CV_8UC1 );
        cvtColor(img, img_gray, CV_RGB2GRAY);

        // Apply gaussian filter to smooth gray image (remove some noise)
        GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);

        // Apply Canny filter to get image's edges
        Mat img_canny(img.size(), CV_8UC1);
        Canny(img_gray, img_canny, 30, 60, 3);

        // Find contours
        findContours(img_canny, contoursTarget, hierarchyTarget,
                     CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        // Draw in grayscale image
        Mat drawing = Mat::zeros(img.size(), CV_8UC3);
        cvtColor(img, drawing, CV_RGB2GRAY);
        cvtColor(drawing, drawing, CV_GRAY2RGB);

        int sumX = 0, sumY = 0, eraseCount = 0;
        Point centroidCheck;
        size_t contoursSize = contoursTarget.size();

        for( size_t i = 0; i < contoursSize; i++ )
        {
            // Keep only contours that are larger than defined threshold
            if( (contoursTarget[i-eraseCount].size() > contourPointsTargetMin) &&
                (contoursTarget[i-eraseCount].size() < contourPointsTargetMax) &&
                (contourArea(contoursTarget[i-eraseCount]) > minAreaTarget) &&
                (contourArea(contoursTarget[i-eraseCount]) < maxAreaTarget) )
            {
                // Contour centroid calculation
                for( size_t j = 0; j < contoursTarget[i-eraseCount].size(); j++ )
                {
                    sumX += contoursTarget[i-eraseCount][j].x;
                    sumY += contoursTarget[i-eraseCount][j].y;
                }
                centroidCheck.x = sumX/contoursTarget[i-eraseCount].size();
                centroidCheck.y = sumY/contoursTarget[i-eraseCount].size();

                // Keep only contours that have a centroid located on the center of the image
                if( (centroidCheck.x < img_canny.cols/3) || (centroidCheck.x > img_canny.cols*2/3) ||
                    (centroidCheck.y < img_canny.rows/3) || (centroidCheck.y > img_canny.rows*2/3) )
                {
                    contoursTarget.erase(contoursTarget.begin()+i-eraseCount);
                    eraseCount++;
                }
                else
                    drawContours(drawing, contoursTarget, i-eraseCount,
                                 Scalar(0,0,255), 3, 8, hierarchyTarget, 0, Point());

                sumX = 0;
                sumY = 0;
            }

            else
            {
                contoursTarget.erase(contoursTarget.begin()+i-eraseCount);
                eraseCount++;
            }
        }

        qDebug() << "contoursTarget size:" << contoursTarget.size() << endl;

        // In case of no contour detected, try again taking in black area of target
        if( contoursTarget.size() < 1 )
        {
            ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
            ui->infoPlainTextEdit->insertPlainText("Using HSV algorithm.\n");
            ui->infoPlainTextEdit->moveCursor(QTextCursor::End);

            // Convert image representaion from RGB to HSV
            Mat img_hsv(img.size(), CV_8UC1);
            cvtColor(img, img_hsv, CV_BGR2HSV);

            // Filter target's hole with Hue information
            Mat img_mod(img.rows, img.cols, CV_8UC1);
            inRange(img_hsv, Scalar(HContourMin, SContourMin, VContourMin),
            Scalar(HContourMax, SContourMax, VContourMax), img_mod);

            // Create circular structuring element
            Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5), Point(-1,-1));
            // Remove noise with binary image open function
            morphologyEx(img_mod, img_mod, MORPH_CLOSE,
                         element, Point(-1,-1), 1, BORDER_CONSTANT);

            vector<vector<Point> > contoursTargetHSV;
            vector<Vec4i> hierarchyTargetHSV;

            // Find contours
            findContours(img_mod, contoursTargetHSV, hierarchyTargetHSV,
                         CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

            // Do nothing if contour vector is empty
            if( contoursTargetHSV.size() < 1 )
            {
               ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
               ui->infoPlainTextEdit->insertPlainText(noContoursErrorMessage);
               ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
               return;
            }

            eraseCount = 0;
            size_t contoursTargetHSVSize = contoursTargetHSV.size();

            for( size_t i = 0; i < contoursTargetHSVSize; i++ )
            {

                // Keep only contours that are larger than defined threshold
                if( (contoursTargetHSV[i-eraseCount].size() > contourPointsTargetMin/2) &&
                    (contoursTargetHSV[i-eraseCount].size() < contourPointsTargetMax*2) &&
                    (contourArea(contoursTargetHSV[i-eraseCount]) > minAreaTarget/2) &&
                    (contourArea(contoursTargetHSV[i-eraseCount]) < maxAreaTarget*2) )
                {

                   // Contour centroid calculation
                    for( size_t j = 0; j < contoursTargetHSV[i-eraseCount].size(); j++ )
                    {
                        sumX += contoursTargetHSV[i-eraseCount][j].x;
                        sumY += contoursTargetHSV[i-eraseCount][j].y;
                    }
                    centroidCheck.x = sumX/contoursTargetHSV[i-eraseCount].size();
                    centroidCheck.y = sumY/contoursTargetHSV[i-eraseCount].size();

                    // Keep only contours that have a centroid located on the center of the image
                    if( (centroidCheck.x < img_mod.cols/3) || (centroidCheck.x > img_mod.cols*2/3) ||
                        (centroidCheck.y < img_mod.rows/3) || (centroidCheck.y > img_mod.rows*2/3) )
                    {
                        contoursTargetHSV.erase(contoursTargetHSV.begin()+i-eraseCount);
                        eraseCount++;
                    }

                    else
                        drawContours(drawing, contoursTargetHSV, i-eraseCount,
                                     Scalar(0,0,255), 3, 8, hierarchyTargetHSV, 0, Point());

                    sumX = 0;
                    sumY = 0;
                }

                else
                {
                    contoursTargetHSV.erase(contoursTargetHSV.begin()+i-eraseCount);
                    eraseCount++;
                }

            }

            contoursTarget = contoursTargetHSV;

        }

        // Convert OpenCV image to RGB888 for Qt
        QImage _img((uchar*)drawing.data,
                    drawing.cols,drawing.rows,
                    drawing.step,QImage::Format_RGB888);

        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
        //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));

        break;
    }

    case 6: //Choose target's holes with HSV information
    {
        // Convert image representaion from RGB to HSV
        Mat img_hsv(img.size(), CV_8UC1);
        cvtColor(img, img_hsv, CV_BGR2HSV);

        // Filter target's hole with Hue information
        Mat img_mod(img.rows, img.cols, CV_8UC1);
        inRange(img_hsv, Scalar(R_H_Min, G_S_Min, B_V_Min),
                Scalar(R_H_Max, G_S_Max, B_V_Max), img_mod);

        // Create circular structuring element
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7,7), Point(-1,-1));
        // Remove noise with binary image open function
        morphologyEx(img_mod, img_mod, MORPH_OPEN,
                     element, Point(-1,-1), 1, BORDER_CONSTANT);

        // Convert OpenCV image to RGB888 for Qt
        QImage _img((uchar*)img_mod.data,img_mod.cols,img_mod.rows,
                    img_mod.step,QImage::Format_Indexed8);
        // Display image on label
        ui->testLabel->setPixmap(QPixmap::fromImage(_img));

        break;
    }

    case 7: //Test
    {
        vector<vector<Point> > contoursSplit;
        holeRadius.clear();
        holeCenter.clear();
        contoursSplit = contoursHole;
        size_t contoursSize = contoursSplit.size();
        int eraseCount = 0;

        for( size_t i = 0; i < contoursSize; i++ )
        {
            if( contourArea(contoursSplit[i-eraseCount]) < maxAreaHole*1.3 )
            {
                contoursSplit.erase(contoursSplit.begin()+i-eraseCount);
                eraseCount++;
            }
        }

        if( (contoursSplit.size() < 1) )
        {
           ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
           ui->infoPlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
           ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
           return;
        }

        // Draw in grayscale image
        Mat drawing = Mat::zeros(img.size(), CV_8UC3);
        cvtColor(img, drawing, CV_RGB2GRAY);
        cvtColor(drawing, drawing, CV_GRAY2RGB);

        // Calculate line to divide hole area and draw in image
        Vec4f lineHole;
        fitLine(Mat(contoursSplit[0]), lineHole, CV_DIST_FAIR, 0, 0.01, 0.01);

        int lefty = (-lineHole[2]*lineHole[1]/lineHole[0])+lineHole[3];
        int righty = ((img.cols-lineHole[2])*lineHole[1]/lineHole[0])+lineHole[3];

        line(drawing,Point(img.cols-1,righty),Point(0,lefty),Scalar(255,0,0),2);

        int ny = -(lineHole[0]/lineHole[1]);
        int mag = sqrt(1 + pow(ny, 2));
        lineHole[1] = (-lineHole[0]/lineHole[1])/mag;
        lineHole[0] = 1/mag;
        lefty = (-lineHole[2]*lineHole[1]/lineHole[0])+lineHole[3];
        righty = ((img.cols-lineHole[2])*lineHole[1]/lineHole[0])+lineHole[3];

        line(drawing,Point(img.cols-1,righty),Point(0,lefty),Scalar(255,0,0),2);

        // Divider algorithm
        arma::fvec b1, s;
        arma::fmat m, B, U, V;
        Point center(0,0), centerTemp;
        double radius = 0, radiusTemp;
        int position = 0;
        bool equalCountFlag = false, filterPhase = false;

        for( size_t i = 0; i < contoursSplit.size(); i++)
        {
            Vec4f lineHole;
            fitLine(Mat(contoursSplit[i]), lineHole, CV_DIST_FAIR, 0, 0.01, 0.01);

            int normalY = -( lineHole[0]/lineHole[1] );
            int magnitude = sqrt( 1 + pow(normalY, 2) );
            lineHole[1] = ( -lineHole[0]/lineHole[1] )/magnitude;
            lineHole[0] = 1/magnitude;
            lefty = ( -lineHole[2]*lineHole[1]/lineHole[0] ) + lineHole[3];
            righty = ( (img.cols-lineHole[2])*lineHole[1]/lineHole[0] ) + lineHole[3];

            Point dividerStart(img.cols-1,righty);
            Point dividerEnd(0,lefty);

            int CONTOUR_SIZE = contoursSplit[i].size();
            arma::fvec x1, x1Left, x1Right;
            arma::fvec x2, x2Left, x2Right;

            x1Left.set_size(CONTOUR_SIZE);
            x2Left.set_size(CONTOUR_SIZE);
            x1Right.set_size(CONTOUR_SIZE);
            x2Right.set_size(CONTOUR_SIZE);

            size_t countLeft = 0, countRight = 0;

            for( size_t j = 0; j < contoursSplit[i].size(); j++ )
            {

                position = ( (dividerEnd.x-dividerStart.x)*(contoursSplit[i][j].y-dividerStart.y) -
                             (dividerEnd.y-dividerStart.y)*(contoursSplit[i][j].x-dividerStart.x) );

                if( position > 0 )
                {
                    x1Left(countLeft) = contoursSplit[i][j].x;
                    x2Left(countLeft) = contoursSplit[i][j].y;
                    countLeft++;
                    //qDebug() << "countLeft:" << countLeft << "Winky!" << j;
                }

                else if( position < 0 )
                {
                    x1Right(countRight) = contoursSplit[i][j].x;
                    x2Right(countRight) = contoursSplit[i][j].y;
                    countRight++;
                    //qDebug() << "countRight:" << countRight << "Winky!" << j;
                }

                else
                {
                    x1Left(countLeft) = contoursSplit[i][j].x;
                    x2Left(countLeft) = contoursSplit[i][j].y;
                    x1Right(countRight) = contoursSplit[i][j].x;
                    x2Right(countRight) = contoursSplit[i][j].y;
                    countLeft++;
                    countRight++;
                    //qDebug() << "countLeft:" << countLeft << "countRight:" << countRight
                    //         << "Winky!" << j << endl;
                }
            }

            qDebug() << "Contour Size" << contoursSplit[i].size();

            for( int k = 0; k < 2; k++)
            {
                int SIZE = CONTOUR_SIZE;

                if( k == 0 )
                {
                    if( countLeft > countRight )
                    {
                        x1.set_size(countLeft);
                        x2.set_size(countLeft);
                        x1Left.resize(countLeft);
                        x2Left.resize(countLeft);
                        x1 = x1Left;
                        x2 = x2Left;
                        SIZE = countLeft;
                        qDebug() << "SIZE (left 1): " << SIZE << endl;
                    }

                    else if( countRight > countLeft )
                    {
                        x1.set_size(countRight);
                        x2.set_size(countRight);
                        x1Right.resize(countRight);
                        x2Right.resize(countRight);
                        x1 = x1Right;
                        x2 = x2Right;
                        SIZE = countRight;
                        qDebug() << "SIZE (right 1): " << SIZE << endl;
                    }

                    else
                    {
                        x1.set_size(countLeft);
                        x2.set_size(countLeft);
                        x1 = x1Left;
                        x2 = x2Left;
                        SIZE = countLeft;
                        qDebug() << "SIZE (equal 1): " << SIZE << endl;
                        equalCountFlag = true;
                    }
                }

                else if( countLeft < countRight )
                {
                    x1.set_size(countLeft);
                    x2.set_size(countLeft);
                    x1Left.resize(countLeft);
                    x2Left.resize(countLeft);
                    x1 = x1Left;
                    x2 = x2Left;
                    SIZE = countLeft;
                    qDebug() << "SIZE (left 2): " << SIZE << endl;
                }

                else if( countRight < countLeft )
                {
                    x1.set_size(countRight);
                    x2.set_size(countRight);
                    x1Right.resize(countRight);
                    x2Right.resize(countRight);
                    x1 = x1Right;
                    x2 = x2Right;
                    SIZE = countRight;
                    qDebug() << "SIZE (right 2): " << SIZE << endl;
                }

                else
                {
                    x1.set_size(countRight);
                    x2.set_size(countRight);
                    x1 = x1Right;
                    x2 = x2Right;
                    SIZE = countRight;
                    qDebug() << "SIZE (equal 2): " << SIZE << endl;
                }

                qDebug() << "Test 1";

                // Get contour points coordinates in Armadillo vectors
                // Run algorithm twice by removing outliers

                m.reset();
                m.ones(SIZE);
                B.reset();
                U.reset();
                s.reset();
                V.reset();

                int ms = 100;
                struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };

                nanosleep(&ts, NULL);
                qDebug() << "Test 2";

                arma::fmat x = join_rows(x1, x2);

                nanosleep(&ts, NULL);
                qDebug() << "Test 3";

                b1 = x1%x1 + x2%x2;

                nanosleep(&ts, NULL);
                qDebug() << "Test 4";

                cout << "x1:" << x1.n_rows << "|" << x1.n_cols;
                cout <<" x2:" << x2.n_rows << "|" << x2.n_cols << endl;
                cout << "b1:" << b1.n_rows << "|" << b1.n_cols;
                cout << " m:" << m.n_rows << "|" << m.n_cols << endl;
                B = join_rows(x1, x2);
                cout << "B:" << B.n_rows << "|" << B.n_cols;
                B = join_rows(b1, B);
                cout << " B:" << B.n_rows << "|" << B.n_cols;
                B = join_rows(B, m);
                cout << " B:" << B.n_rows << "|" << B.n_cols << endl;

                nanosleep(&ts, NULL);
                qDebug() << "Test 5";

                // Singular value decomposition
                arma::svd(U,s,V,B);

                arma::fmat u = V.col(3);
                float a = u(0);
                arma::fmat b = u.rows(1,2);
                float c = u(3);

                arma::fmat z = -b/(2*a);
                arma::fmat r(1,1);
                r = sqrt(pow(arma::norm(b)/(2*a),2) - c/a);

                arma::fmat ufit = join_vert(z,r);

                // Gauss-Newton iterative method
                float delta = INFINITY;
                bool converged = false;

                for( int j = 0; j < maxIts; j++ )
                {
                    //Objective function
                    //f = (sqrt(sum((repmat(u(1:2), 1, m) - x).^2)) - u(3))';
                    arma::fmat u_part = ufit.head_rows(2);
                    arma::fmat f_part = arma::repmat(u_part, 1, SIZE) - x.t();
                    arma::fmat f = arma::trans(arma::sqrt( arma::sum(f_part%f_part)) - ufit(2));

                    //Jacobian
                    //denom = sqrt( (u(1) - x1).^2 + (u(2) - x2).^2 );
                    //J = [(u(1) - x1) ./ denom, (u(2) - x2) ./ denom, repmat(-1, m, 1)];
                    arma::fmat denom  = arma::sqrt( (ufit(0)-x1)%(ufit(0)-x1) + (ufit(1)-x2)%(ufit(1)-x2) );
                    arma::fmat j1 = (ufit(0)-x1) / denom;
                    arma::fmat j2 = (ufit(1)-x2) / denom;
                    arma::fmat temp(1,1);
                    temp = -1;
                    arma::fmat j3 = arma::repmat(temp, SIZE, 1);
                    arma::fmat J  = arma::join_rows(j1, j2);
                    J = arma::join_rows(J, j3);

                    arma::fmat h = solve(-J,f);
                    ufit = ufit + h;

                    //Check for convergence
                    delta = arma::max(arma::max(arma::abs(h)))/arma::max(arma::max(arma::abs(ufit)));
                    if( delta < tol)
                    {
                        converged = true;
                        break;
                    }
                }

                // Terminate if convergence test failed
                if( !converged )
                {
                    ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
                    ui->infoPlainTextEdit->insertPlainText(convergenceLSQErrorMessage);
                    ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
                    return;
                }

                // Save found radius & center
                z = ufit.rows(0,1);
                r = ufit(2);
                centerTemp.x = (int) round(arma::as_scalar(z(0)));
                centerTemp.y = (int) round(arma::as_scalar(z(1)));
                radiusTemp = round(arma::as_scalar(r));

                if( (radiusTemp >= minRadiusHole*0.7) && (radiusTemp <= maxRadiusHole*1.3) )
                {
                    holeCenter.push_back(centerTemp);
                    holeRadius.push_back(radiusTemp);

                    qDebug() << "Center:" << centerTemp.x << "|" << centerTemp.y;
                    qDebug() << "Radius:" << radiusTemp << endl;

                    // Print values to user
                    cout << "z: " << z << endl;
                    cout << "r: " << r << endl;
                }

                countLeft = 0;
                countRight = 0;
            }
        }

        for( size_t j = 0; j < holeRadius.size(); j++ )
        {
            circle(drawing, holeCenter[j], 20, Scalar(255,0,0), 3, 8, 0);
            circle(drawing, holeCenter[j], 3, Scalar(0,255,0), -1, 8, 0);
        }

        QImage _img((uchar*)drawing.data,drawing.cols,
                    drawing.rows,drawing.step,QImage::Format_RGB888);
        ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
        break;
    }

    default: // Failsafe check
        break;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_centerCalcPushButton_clicked()
{
    // Do nothing if contour vector is empty
    if( contoursTarget.size() < 1 )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    int sumX=0, sumY=0, contourCount=0;
    Point sumCenter, centroid;

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    for( size_t i = 0; i < contoursTarget.size(); i++ )
    {
        // Contour centroid calculation
        for( size_t j = 0; j < contoursTarget[i].size(); j++ )
        {
            sumX += contoursTarget[i][j].x;
            sumY += contoursTarget[i][j].y;
        }

        centroid.x = sumX/contoursTarget[i].size();
        centroid.y = sumY/contoursTarget[i].size();

        // Consider only contours' centers that have a centroid
        // located on the center of the image
        if( (centroid.x > img.cols/3) && (centroid.x < img.cols*2/3) &&
            (centroid.y > img.rows/3) && (centroid.y < img.rows*2/3) )
        {
            sumCenter += centroid;
            contourCount += 1;
            drawContours(drawing, contoursTarget, i,
                         Scalar(0,0,255), 3, 8, hierarchyTarget, 0, Point());
            qDebug() << "Contour" << i << "Centroid - x: "
                     << centroid.x << "| y: " << centroid.y << endl;
        }

        sumX = 0;
        sumY = 0;
    }

    if( contourCount > 0 )
    {
        targetCentroid.x = sumCenter.x/contourCount;
        targetCentroid.y = sumCenter.y/contourCount;

        // Draw circle center
        circle(drawing, targetCentroid, 3, Scalar(0,255,0), -1, 8, 0);
        qDebug() << "Center - X: " << targetCentroid.x
                 << "| y: " << targetCentroid.y << endl;
    }

    else
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noCentroidErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)drawing.data,drawing.cols,drawing.rows,
                drawing.step,QImage::Format_RGB888);

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_radiusCalcPushButton_clicked()
{
    // Do nothing if contour vector is empty
    if( contoursTarget.size() < 1 )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Do nothing if no center was detected
    if( (targetCentroid.x < 1) || (targetCentroid.y < 1))
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noCenterErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Get radius by calculating average of distance of contour points with centroid
    double sumRadius = 0;

    for( size_t i = 0; i < contoursTarget.size(); i++ )
    {
        for( size_t j = 0; j < contoursTarget[i].size(); j++ )
            sumRadius += sqrt(contoursTarget[i][j].x*targetCentroid.x +
                              contoursTarget[i][j].y*targetCentroid.y);

        targetRadius = sumRadius/contoursTarget[i].size();
        qDebug() << "Radius: " << targetRadius << endl;
    }

    // Get radius by fitting contours points in a ellipsis
    vector<RotatedRect> minEllipse( contoursTarget.size() );

    for( size_t i = 0; i < contoursTarget.size(); i++ )
    {
        minEllipse[i] = fitEllipse(Mat(contoursTarget[i]));
        qDebug() << "Radius1: " << minEllipse[i].size.height/2;
        qDebug() << "Radius2: " << minEllipse[i].size.width/2 << endl;
    }

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    // Draw circles
    for( size_t i = 0; i < contoursTarget.size(); i++ )
        ellipse(drawing, minEllipse[i], Scalar(0,250,0), 2, 8);

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)drawing.data,drawing.cols,drawing.rows,
                drawing.step,QImage::Format_RGB888);

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_leastSquareFitPushButton_clicked()
{
    // Get contours vector depending on function chosen
    vector<vector<Point> > contoursLSF;

    if( functionChoiceState == 5 )
    {
        contoursLSF = contoursTarget;
        size_t contourSize = contoursLSF.size();
        int eraseCount = 0;

        qDebug() << "contourSize: " << contourSize << endl;

        for( size_t i = 0; i < contourSize; i++ )
        {
            // Keep only the first contour
            if( i != 0 )
            {
                contoursLSF.erase(contoursLSF.begin()+i-eraseCount);
                eraseCount++;
            }

            qDebug() << "contoursLSF Size: " << contoursLSF.size() << endl;
        }
    }

    else
    {
        holeRadius.clear();
        holeCenter.clear();
        contoursLSF = contoursHole;
        size_t contourSize = contoursLSF.size();
        int eraseCount = 0;

        qDebug() << "contourSize: " << contourSize << endl;

        for( size_t i = 0; i < contourSize; i++ )
        {
            // Keep contours with area between specified areas
            qDebug() << "contour Area" << i << ":"
                     << contourArea(contoursLSF[i-eraseCount]) << endl;
            if( (contourArea(contoursLSF[i-eraseCount]) < minAreaHole) ||
                (contourArea(contoursLSF[i-eraseCount]) > maxAreaHole) )
            {
                contoursLSF.erase(contoursLSF.begin()+i-eraseCount);
                eraseCount++;
            }
            qDebug() << "contoursLSF Size: " << contoursLSF.size() << endl;
        }
    }

    // Do nothing if contour vector is empty
    if( contoursLSF.size() < 1 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    bool filterPhase = false;
    Point center(0,0), holeCenterTemp;
    double radius = 0, holeRadiusTemp;

    // Least square fitting with contour points routine
    for( size_t i = 0; i < contoursLSF.size(); )
    {
        int SIZE = contoursLSF[i].size();
        int eraseVectorCount = 0;
        center.x = 0;
        center.y = 0;
        radius = 0;
        arma::fvec b1, s;
        arma::fmat m, B, U, V;
        arma::fvec x1, x2;

        x1.set_size(SIZE);
        x2.set_size(SIZE);
        m.ones(SIZE);
        B.reset();
        U.reset();
        s.reset();
        V.reset();

        // Get contour points coordinates in Armadillo vectors
        // Run algorithm twice by removing outliers
        if( filterPhase ) //Filter outliers
        {
            Point centerPoint;
            double circleRadius;
            if ( functionChoiceState == 5 )
            {
                centerPoint = targetCenter;
                circleRadius = targetRadius;
            }
            else
            {
                centerPoint = holeCenterTemp;
                circleRadius = holeRadiusTemp;
            }

            qDebug() << "circle radius:" << circleRadius;

            // Outliers removed with initial guess (using all points)
            for( int j = 0; j < SIZE; j++ )
            {
                double distance = sqrt(pow( contoursLSF[i][j].x - centerPoint.x, 2 ) +
                                       pow( contoursLSF[i][j].y - centerPoint.y, 2 ));

                if( (distance/circleRadius <= 1.03) &&
                    (distance/circleRadius >= 0.97) )
                {
                    x1(j-eraseVectorCount) = contoursLSF[i][j].x;
                    x2(j-eraseVectorCount) = contoursLSF[i][j].y;
                }
                else
                    eraseVectorCount++;
            }

            SIZE = SIZE - eraseVectorCount;

            x1.resize(SIZE);
            x2.resize(SIZE);
            m.resize(SIZE);
        }

        // First attempt
        else
        {
            for( int j = 0; j < SIZE; j++ )
            {
                x1(j) = contoursLSF[i][j].x;
                x2(j) = contoursLSF[i][j].y;
            }
        }

        arma::fmat x = join_rows(x1, x2);

        b1 = x1%x1 + x2%x2;
        B = join_rows(x1, x2);
        B = join_rows(b1, B);
        B = join_rows(B, m);

        // Singular value decomposition
        arma::svd(U,s,V,B);

        arma::fmat u = V.col(3);
        float a = u(0);
        arma::fmat b = u.rows(1,2);
        float c = u(3);

        arma::fmat z = -b/(2*a);
        arma::fmat r(1,1);
        r = sqrt(pow(arma::norm(b)/(2*a),2) - c/a);

        arma::fmat ufit = join_vert(z,r);

        // Gauss-Newton iterative method
        float delta = INFINITY;
        bool converged = false;

        // Find values until max value of iterations
        for( int j = 0; j < maxIts; j++ )
        {
            // Objective function
            //f = (sqrt(sum((repmat(u(1:2), 1, m) - x).^2)) - u(3))';
            arma::fmat u_part = ufit.head_rows(2);
            arma::fmat f_part = arma::repmat(u_part, 1, SIZE) - x.t();
            arma::fmat f = arma::trans(arma::sqrt( arma::sum(f_part%f_part)) - ufit(2));

            // Jacobian
            //denom = sqrt( (u(1) - x1).^2 + (u(2) - x2).^2 );
            //J = [(u(1) - x1) ./ denom, (u(2) - x2) ./ denom, repmat(-1, m, 1)];
            arma::fmat denom  = arma::sqrt( (ufit(0)-x1)%(ufit(0)-x1) + (ufit(1)-x2)%(ufit(1)-x2) );
            arma::fmat j1 = (ufit(0)-x1) / denom;
            arma::fmat j2 = (ufit(1)-x2) / denom;
            arma::fmat temp(1,1);
            temp = -1;
            arma::fmat j3 = arma::repmat(temp, SIZE, 1);
            arma::fmat J  = arma::join_rows(j1, j2);
            J = arma::join_rows(J, j3);

            arma::fmat h = solve(-J,f);
            ufit = ufit + h;

            //  Check for convergence
            delta = arma::max(arma::max(arma::abs(h)))/arma::max(arma::max(arma::abs(ufit)));
            if( delta < tol )
            {
                converged = true;
                break;
            }
        }

        // Terminate if convergence test failed
        if( !converged )
        {
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(convergenceLSQErrorMessage);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        // Radius & center
        z = ufit.rows(0,1);
        r = ufit(2);
        center.x = (int) round(arma::as_scalar(z(0)));
        center.y = (int) round(arma::as_scalar(z(1)));
        radius = round(arma::as_scalar(r));

        // Print values to user
        cout << "z: " << z << endl;
        cout << "r: " << r << endl;

        // Save found radius & center
        if( functionChoiceState == 5 )
        {
             targetRadius = radius;
             targetCenter.x = center.x;
             targetCenter.y = center.y;
        }

        else
        {
            if( filterPhase )
            {
                holeRadius.push_back(radius);
                holeCenter.push_back(center);
            }
            else
            {
                holeRadiusTemp = radius;
                holeCenterTemp = center;
            }
        }

        // Phase choice (when true, remove outliers)
        if( !filterPhase )
            filterPhase = true;
        else
        {
            filterPhase = false;
            i++;

            // Draw circles and circles' centers
            circle(drawing, center, radius, Scalar(255,0,0), 3, 8, 0);
            circle(drawing, center, 3, Scalar(0,255,0), -1, 8, 0);
        }

    }    

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)drawing.data,drawing.cols,drawing.rows,
                drawing.step,QImage::Format_RGB888);

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHTPushButton_clicked()
{
    // Do nothing if image is empty
    if( img.empty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Convert image representaion from RGB to HSV
    Mat img_hsv(img.size(), CV_8UC1);
    cvtColor(img, img_hsv, CV_BGR2HSV);

    // Filter target's hole with Hue information
    Mat img_mod(img.rows, img.cols, CV_8UC1);
    inRange(img_hsv, Scalar(R_H_Min, B_V_Min, G_S_Min),
            Scalar(R_H_Max, B_V_Max, G_S_Max), img_mod);

    // Create circular structuring element
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7,7), Point(-1,-1));
    // Remove noise with binary image open function
    morphologyEx(img_mod, img_mod, MORPH_OPEN,
                 element, Point(-1,-1), 1, BORDER_CONSTANT);

    // Get contours with an erosion minus original image
    Mat contourExtractionElement = getStructuringElement(MORPH_RECT, Size(3,3), Point(-1,-1));
    Mat img_erode(img_mod.size(), CV_8UC1);
    erode(img_mod, img_erode, contourExtractionElement, Point(-1,-1), 1, BORDER_CONSTANT);
    Mat img_contour = img_mod-img_erode;

    // Apply Hough Transform to find the circles
    vector<Vec3f> circles;
    HoughCircles(img_contour, circles, CV_HOUGH_GRADIENT,
                 accumResolutionHole, minDistHole,
                 cannyThreshHole, accumThreshHole,
                 minRadiusHole, maxRadiusHole);

    // Draw the circles detected
    cvtColor(img_contour, img_contour, CV_GRAY2BGR);
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        // Draw circle center
        circle(img_contour, center, 3, Scalar(0,255,0), -1, 8, 0);
        // Draw circle outline
        circle(img_contour, center, radius, Scalar(0,0,255), 1, 8, 0);
    }

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)img_contour.data,img_contour.cols,img_contour.rows,
                img_contour.step,QImage::Format_RGB888);

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectCPushButton_clicked()
{
    // Do nothing if image is empty
    if( img.empty() )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Convert image representation from RGB to HSV
    Mat img_hsv(img.size(), CV_8UC1);
    cvtColor(img, img_hsv, CV_BGR2HSV);

    // Filter target's hole with Hue information
    Mat img_mod(img.rows, img.cols, CV_8UC1);
    inRange(img_hsv, Scalar(R_H_Min, B_V_Min, G_S_Min),
            Scalar(R_H_Max, B_V_Max, G_S_Max), img_mod);

    // Create circular structuring element
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5), Point(-1,-1));
    // Remove noise with binary image open function
    morphologyEx(img_mod, img_mod, MORPH_CLOSE,
                 element, Point(-1,-1), 3, BORDER_CONSTANT);

    vector<vector<Point> > contoursHoleTemp;

    // Find contours
    findContours(img_mod, contoursHoleTemp, hierarchyHole,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Do nothing if contour vector is empty
    if( contoursHoleTemp.size() < 1 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    // Draw contours
    Mat drawing = Mat::zeros(img_mod.size(), CV_8UC3);

    for( size_t i = 0; i < contoursHoleTemp.size(); i++ )
    {
        if( contoursHoleTemp[i].size() > contourPointsHole )
            drawContours(drawing, contoursHoleTemp, i,
                         Scalar(0,0,255), 3, 8, hierarchyHole, 0, Point());
    }

    contoursHole = contoursHoleTemp;

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)drawing.data,drawing.cols,drawing.rows,
                drawing.step,QImage::Format_RGB888);

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectSplitPushButton_clicked()
{

    holeDefects.clear();
    vector<vector<Point> > contoursSplit;
    contoursSplit = contoursHole;
    size_t contoursSize = contoursSplit.size();
    int eraseCount = 0;

    for( size_t i = 0; i < contoursSize; i++ )
    {
        if( contourArea(contoursSplit[i-eraseCount]) < maxAreaHole*1.3 )
        {
            contoursSplit.erase(contoursSplit.begin()+i-eraseCount);
            eraseCount++;
        }
    }


    bool ok;
    int numHoles = QInputDialog::getInteger( this, tr("Information"),
                                             tr("Insert number of shots:"),
                                             1, 0, 5, 1, &ok);
    if( !ok )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(cancelButtonErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }
    //qDebug() << "Value:" << numHoles << "| ok:" << ok << endl;

    if( (contoursSplit.size() < 1) || (numHoles == 0) )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    vector<vector<int> > hullI(contoursSplit.size());
    vector<vector<Point> > hullP(contoursSplit.size());
    vector<vector<Vec4i> > defects(contoursSplit.size());

    for( size_t i = 0; i < contoursSplit.size(); i++ )
    {
        convexHull(contoursSplit[i], hullI[i], false );
        convexHull(contoursSplit[i], hullP[i], false );
        convexityDefects(contoursSplit[i], hullI[i], defects[i]);
    }

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    vector<Point> holeDefectsTemp;
    vector<int> depth;
    //float std;

    //qDebug() << "contours Size" << contoursSplit.size() << "| HoleDefectsTemp Size" << holeDefectsTemp.size() << endl;

    for( size_t i = 0; i < contoursSplit.size(); i++ )
    {
        drawContours(drawing, hullP, i, Scalar(50,255,100), 1, 8, vector<Vec4i>(), 0, Point());

        vector<Vec4i>::iterator d = defects[i].begin();
        while( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            depth.push_back(v[3]/256);
            d++;
        }

        vector<int> indices(5);
        fill_n(indices.begin(), 5, -1);//-1 used as invalid index for cases where Vec.size()<5

        /*for( size_t l = 0; l < indices.size(); l++ )
            qDebug() << "indices before" << l << ":" << indices[l];*/

        for( size_t j = 0; j < depth.size(); ++j )
        {
            for( int k = 0; k < 5; ++k )
            {
                if( (indices[k] == - 1) || (depth[j] > depth[indices[k]]) )
                {
                    copy_backward(indices.begin() + k, indices.end() - 1, indices.end());
                    indices[k] = j;
                    break;
                }
            }
        }

        /*for( size_t l = 0; l < indices.size(); l++ )
            qDebug() << "indices after" << l << ":" << indices[l];*/

        for( size_t l = 0; l < indices.size(); l++ )
        {
            int position = indices[l];
            Vec4i vectrix = defects[i][position];
            int faridx = vectrix.val[2];
            Point ptFar(contoursSplit[i][faridx]);
            //qDebug() << "ptFar" << indices[l] << ":" << ptFar.x << ptFar.y;
        }

        for( int j = 0;  j < numHoles; j++ )
        {
            int position = indices[j];
            Vec4i vectrix = defects[i][position];
            int faridx = vectrix.val[2];
            Point ptFar(contoursSplit[i][faridx]);
            holeDefectsTemp.push_back(ptFar);
            //qDebug() << "inserting point:" << indices[j] << ":" << ptFar.x << ptFar.y;
        }

        /*d = defects[i].begin();
        while( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            int faridx = v[2]; Point ptFar(contoursSplit[i][faridx]);
            int startidx=v[0]; Point ptStart( contoursSplit[i][startidx] );
            int endidx=v[1]; Point ptEnd( contoursSplit[i][endidx] );

            qDebug() << "Start point: " << ptStart.x << ptStart.y;
            qDebug() << "End point: " << ptEnd.x << ptEnd.y;
            qDebug() << "Depth value: " << v[3]/256;

            if ( ( (v[3]/256) >= mean ) && ( (v[3]/256) > minConvexityDefect) )
            {
                depth = v[3] / 256;
                holeDefectsTemp.push_back(ptFar);

                circle(drawing, holeDefectsTemp[count], 3, Scalar(255,0,100), -1, 8, 0);
                //QString test = QString("holeDefectsTemp[%1]: %2, %3\n").arg(count)
                //        .arg(holeDefectsTemp[count].x).arg(holeDefectsTemp[count].y);
                //ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
                //ui->info2PlainTextEdit->appendPlainText(test);
                //ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
                QImage _img2((uchar*)drawing.data,
                             drawing.cols,drawing.rows,
                             drawing.step,QImage::Format_RGB888);
                ui->testLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));

                count++;

                qDebug() << "New depth :" << depth;
            }*/

            /*line( drawing, ptStart, ptEnd, Scalar(0,255,0), 1 );
            line( drawing, ptStart, ptFar, Scalar(0,255,0), 1 );
            line( drawing, ptEnd, ptFar, Scalar(0,255,0), 1 );
            circle( drawing, ptFar, 2, Scalar(0,255,0), 2 );
            d++;
        }*/

        for( size_t p = 0; p < holeDefectsTemp.size(); p++ )
            qDebug() << "Hole defect temp" << p << ":" << holeDefectsTemp[p].x << holeDefectsTemp[p].y;


        if( holeDefectsTemp.size() < 1 )
        {
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            ui->info2PlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
            ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
            return;
        }

        Point ptFirst = holeDefectsTemp[0];
        holeDefectsTemp.push_back(ptFirst);
    }

    holeDefects.clear();
    holeDefects = holeDefectsTemp;

    if( holeDefects.size() < 3 )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    for( size_t p = 0; p < holeDefects.size(); p++ )
        qDebug() << "Hole defect" << p << ":" << holeDefects[p].x << holeDefects[p].y;

    holeCenter.clear();
    holeRadius.clear();

    Point centerTemp;
    int radiusTemp;
    arma::fvec b1, s;
    arma::fmat m, B, U, V;
    int position = 0;

    for( size_t i = 0; i < contoursSplit.size(); i++ )
    {
        int CONTOUR_SIZE = contoursSplit[i].size();
        int SIZE = CONTOUR_SIZE;

        arma::fvec x1, x1Left, x1Right;
        arma::fvec x2, x2Left, x2Right;

        for( size_t k = 0; k < holeDefects.size()-1; k++)
        {
            x1Left.set_size(CONTOUR_SIZE);
            x2Left.set_size(CONTOUR_SIZE);
            x1Right.set_size(CONTOUR_SIZE);
            x2Right.set_size(CONTOUR_SIZE);

            size_t countLeft=0, countRight=0;

            for( size_t j = 0; j < contoursSplit[i].size(); j++ )
            {
                if( contoursSplit[i][j] == holeDefects[k] )
                {
                    position = j;
                    break;
                }
            }

            size_t current_position = position;

            while( contoursSplit[i][current_position] != holeDefects[k+1] )
            {
                x1Left(countLeft) = contoursSplit[i][current_position].x;
                x2Left(countLeft) = contoursSplit[i][current_position].y;
                current_position++;
                if( current_position == contoursSplit[i].size() )
                    current_position = 0;
                countLeft++;
            }

            x1Left.resize(countLeft);
            x2Left.resize(countLeft);
            x1.set_size(countLeft);
            x2.set_size(countLeft);
            x1 = x1Left;
            x2 = x2Left;
            SIZE = countLeft;

            if( countLeft > contoursSplit[i].size() )
            {
                while( contoursSplit[i][current_position] != holeDefects[k] )
                {
                    x1Right(countRight) = contoursSplit[i][current_position].x;
                    x2Right(countRight) = contoursSplit[i][current_position].y;
                    current_position++;
                    if( current_position > contoursSplit[i].size() )
                        current_position = 0;
                    countRight++;
                }

                x1Right.resize(countRight);
                x2Right.resize(countRight);
                x1.set_size(countRight);
                x2.set_size(countRight);
                x1 = x1Right;
                x2 = x2Right;
                SIZE = countRight;
            }

            m.ones(SIZE);
            B.reset();
            U.reset();
            s.reset();
            V.reset();

            arma::fmat x = join_rows(x1, x2);

            b1 = x1%x1 + x2%x2;
            B = join_rows(x1, x2);
            B = join_rows(b1, B);
            B = join_rows(B, m);

            // Singular value decomposition
            arma::svd(U,s,V,B);

            arma::fmat u = V.col(3);
            float a = u(0);
            arma::fmat b = u.rows(1,2);
            float c = u(3);

            arma::fmat z = -b/(2*a);
            arma::fmat r(1,1);
            r = sqrt(pow(arma::norm(b)/(2*a),2) - c/a);

            arma::fmat ufit = join_vert(z,r);

            // Gauss-Newton iterative method
            float delta = INFINITY;
            bool converged = false;

            for( int j = 0; j < maxIts; j++ )
            {
                //Objective function
                //f = (sqrt(sum((repmat(u(1:2), 1, m) - x).^2)) - u(3))';
                arma::fmat u_part = ufit.head_rows(2);
                arma::fmat f_part = arma::repmat(u_part, 1, SIZE) - x.t();
                arma::fmat f = arma::trans(arma::sqrt( arma::sum(f_part%f_part)) - ufit(2));

                //Jacobian
                //denom = sqrt( (u(1) - x1).^2 + (u(2) - x2).^2 );
                //J = [(u(1) - x1) ./ denom, (u(2) - x2) ./ denom, repmat(-1, m, 1)];
                arma::fmat denom  = arma::sqrt( (ufit(0)-x1)%(ufit(0)-x1) + (ufit(1)-x2)%(ufit(1)-x2) );
                arma::fmat j1 = (ufit(0)-x1) / denom;
                arma::fmat j2 = (ufit(1)-x2) / denom;
                arma::fmat temp(1,1);
                temp = -1;
                arma::fmat j3 = arma::repmat(temp, SIZE, 1);
                arma::fmat J  = arma::join_rows(j1, j2);
                J = arma::join_rows(J, j3);

                arma::fmat h = solve(-J,f);
                ufit = ufit + h;

                //Check for convergence
                delta = arma::max(arma::max(arma::abs(h)))/arma::max(arma::max(arma::abs(ufit)));
                if( delta < tol)
                {
                    converged = true;
                    break;
                }
            }

            // Terminate if convergence test failed
            if( !converged )
            {
                ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
                ui->info2PlainTextEdit->insertPlainText(convergenceLSQErrorMessage);
                ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
                return;
            }

            // Save found radius & center
            z = ufit.rows(0,1);
            r = ufit(2);
            centerTemp.x = (int) round(arma::as_scalar(z(0)));
            centerTemp.y = (int) round(arma::as_scalar(z(1)));
            radiusTemp = round(arma::as_scalar(r));

            if( (radiusTemp >= minRadiusHole*0.7) && (radiusTemp <= maxRadiusHole*1.3) )
            {
                holeCenter.push_back(centerTemp);
                holeRadius.push_back(radiusTemp);

                qDebug() << "Center:" << centerTemp.x << "|" << centerTemp.y << endl;
                qDebug() << "Radius:" << radiusTemp;

                // Print values to user
                cout << "z: " << z << endl;
                cout << "r: " << r << endl;
            }

            holeDefectsTemp.clear();
        }

        for( size_t j = 0; j < holeRadius.size(); j++ )
        {
            circle(drawing, holeCenter[j], 20, Scalar(255,0,0), 3, 8, 0);
            circle(drawing, holeCenter[j], 3, Scalar(0,255,0), -1, 8, 0);
        }
    }

    QImage _img((uchar*)drawing.data,drawing.cols,
                drawing.rows,drawing.step,QImage::Format_RGB888);
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_fixPerspectivePushButton_clicked()
{
    // Do nothing if image is empty
    if( img.empty() )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Convert image to gray
    Mat img_gray(img.size(), CV_8UC1);
    cvtColor(img, img_gray, CV_RGB2GRAY);

    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);

    // Convert image representaion from RGB to HSV
    Mat img_hsv(img.size(), CV_8UC1);
    cvtColor(img, img_hsv, CV_BGR2HSV);

    // Separate the image in 3 places (H, S and V)
    vector<Mat> img_hsv_planes;
    split( img_hsv, img_hsv_planes );
    double VThresh = threshold(img_hsv_planes[2], img_hsv_planes[1], 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    // Filter target's hole with Value information
    Mat img_mod(img.rows, img.cols, CV_8UC1);
    inRange(img_hsv, Scalar(HContourMin, SContourMin, VContourMin),
    Scalar(HContourMax, SContourMax, VThresh), img_mod);

    // Create circular structuring element
    Mat elementOpen = getStructuringElement(MORPH_ELLIPSE, Size(3,3), Point(-1,-1));
    // Remove noise with binary image open function
    morphologyEx(img_mod, img_mod, MORPH_OPEN,
                 elementOpen, Point(-1,-1), 1, BORDER_CONSTANT);

    // Create circular structuring element
    Mat elementClose = getStructuringElement(MORPH_ELLIPSE, Size(3,3), Point(-1,-1));
    // Remove noise with binary image open function
    morphologyEx(img_mod, img_mod, MORPH_CLOSE,
                 elementClose, Point(-1,-1), 3, BORDER_CONSTANT);

    vector<vector<Point> > contoursTestHSV;
    vector<Vec4i> hierarchyTestHSV;
    int eraseCount = 0;
    long minAreaTest = ((img.rows/3)*(img.cols/3))/10;
    long maxAreaTest = (img.rows*img.cols)/2;

    qDebug() << "minAreaTest" << minAreaTest << "maxAreaTest" << maxAreaTest << endl;

    // Find contours
    findContours(img_mod, contoursTestHSV, hierarchyTestHSV,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    size_t contoursTestHSVSize = contoursTestHSV.size();

    // Do nothing if contour vector is empty
    if( contoursTestHSV.size() < 1 )
    {
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       ui->infoPlainTextEdit->insertPlainText(noContoursErrorMessage);
       ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    //----------------------------------------------------------------------------------

    /*// Establish the number of bins
    int histSize = 256;

    // Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    // Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    // Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    // Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
    }

    // Display
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );*/

    //----------------------------------------------------------------------------------

    vector<double> circleRadius;
    vector<RotatedRect> ellipsis;
    for( size_t i = 0; i < contoursTestHSVSize; i++ )
    {
        // Keep only contours that have area between defined thresholds
        if( (contourArea(contoursTestHSV[i-eraseCount]) > minAreaTest) &&
            (contourArea(contoursTestHSV[i-eraseCount]) < maxAreaTest) )
        {
            qDebug() << "Contour Area: " << contourArea(contoursTestHSV[i-eraseCount]);
            RotatedRect ellipse = fitEllipse(contoursTestHSV[i-eraseCount]);
            double ellipseArea = M_PI*(ellipse.size.height/2)*(ellipse.size.width/2);
            qDebug() << "ellipseArea:" << ellipseArea;

            if( (ellipseArea < contourArea(contoursTestHSV[i-eraseCount])*1.005) &&
                (ellipseArea > contourArea(contoursTestHSV[i-eraseCount])*0.995) )
            {
                qDebug() << "Considered ellipse." << endl;

                circleRadius.push_back((ellipse.size.height+ellipse.size.width)/4);
                ellipsis.push_back(ellipse);

                drawContours(drawing, contoursTestHSV, i-eraseCount,
                             Scalar(255,0,0), 5, 8, hierarchyTestHSV, 0, Point());
            }

            else
            {
                contoursTestHSV.erase(contoursTestHSV.begin()+i-eraseCount);
                eraseCount++;
            }

        }

        else
        {
            contoursTestHSV.erase(contoursTestHSV.begin()+i-eraseCount);
            eraseCount++;
        }

    }

    int sumX=0, sumY=0, contourCount=0;
    Point sumCenter, centroid;

    for( size_t i = 0; i < contoursTestHSV.size(); i++ )
    {
        // Contour centroid calculation
        for( size_t j = 0; j < contoursTestHSV[i].size(); j++ )
        {
            sumX += contoursTestHSV[i][j].x;
            sumY += contoursTestHSV[i][j].y;
        }

        centroid.x = sumX/contoursTestHSV[i].size();
        centroid.y = sumY/contoursTestHSV[i].size();

        qDebug() << "Contour" << i << "Centroid - x: "
                 << centroid.x << "| y: " << centroid.y << endl;

        sumCenter += centroid;
        contourCount += 1;
        sumX = 0;
        sumY = 0;
    }

    if( contourCount > 0 )
    {
        targetCentroid.x = sumCenter.x/contourCount;
        targetCentroid.y = sumCenter.y/contourCount;

        // Draw circle center
        circle(drawing, targetCentroid, 4, Scalar(0,255,0), -1, 8, 0);
        qDebug() << "Center - x: " << targetCentroid.x
                 << "| y: " << targetCentroid.y << endl;
    }

    else
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noCentroidErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }

    //vector<double>::iterator largestRadiusIndex = max_element(circleRadius.begin(), circleRadius.end());
    double marginCorrector, largestRadius = *max_element(circleRadius.begin(), circleRadius.end());

    switch( targetType )
    {
        case 0: // 300m Rifle
        {
            marginCorrector = largestRadius*3; // Placeholder
            break;
        }

        case 1:  // 50m Rifle
        {
            marginCorrector = largestRadius*3; // Placeholder
            break;
        }

        case 2: // 10m Air Rifle
        {
            marginCorrector = largestRadius*80/30;
            break;
        }

        case 3: // 50m Pistol / 25m Precision
        {
            marginCorrector = largestRadius*550/200;
            break;
        }

        case 4: // 25m Center Fire Pistol
        {
            marginCorrector = largestRadius*3; // Placeholder
            break;
        }

        case 5: // 10m Air Pistol
        {
            marginCorrector = largestRadius*170/60; // Placeholder
            break;
        }

        case 6: // SCAT
        {
            marginCorrector = largestRadius*8/2;
            break;
        }

        default:
            break;
    }

    qDebug() << "marginCorrector:" << marginCorrector << endl;

    Rect ROI(targetCentroid.x-marginCorrector, targetCentroid.y-marginCorrector,
             marginCorrector*2, marginCorrector*2);

    qDebug() << "x:" << ROI.x << "y:" << ROI.y << "Height:" << ROI.height << "Width:" << ROI.width << endl;

    Rect img_bounds(0,0,img.cols,img.rows);
    ROI = ROI & img_bounds;

    qDebug() << "x:" << ROI.x << "y:" << ROI.y << "Height:" << ROI.height << "Width:" << ROI.width << endl;

    Mat img_cropped = Mat(img, ROI);
    drawing = Mat(drawing, ROI);

    for( int i = 0; i < ellipsis.size(); i++ )
    {

    }

    /*RotatedRect largestEllipse;
    max_element(ellipsis.begin(), ellipsis.end(),
                static_cast<bool(*)(cv::RotatedRect(const Point2f &center, const Size2f &size, float angle),
                cv::RotatedRect(const Point2f &center, const Size2f &size, float angle))>(ellipsisCompare));*/

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)drawing.data,
                drawing.cols,drawing.rows,
                drawing.step,QImage::Format_RGB888);

    // Convert OpenCV image to RGB888 for Qt
    /*QImage _img((uchar*)img_cropped.data,
                img_cropped.cols,img_cropped.rows,
                img_cropped.step,QImage::Format_RGB888);*/

    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img2,Qt::AutoColor));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_printValuesPushButton_clicked()
{
    switch(functionChoiceState)
    {
        case 4:
            qDebug() << "\n - Hough Transform Values - \n";
            qDebug() << qSetRealNumberPrecision(4) << " accumResolutionTarget: " << accumResolutionTarget;
            qDebug() << " minDistTarget:         " << minDistTarget
                     << "\n cannyThreshTarget:     " << cannyThreshTarget
                     << "\n accumThreshTarget:     " << accumThreshTarget;
            qDebug() << " minRadiusTarget:       " << minRadiusTarget
                     << "\n maxRadiusTarget:       " << maxRadiusTarget;
            break;

        case 6:
            qDebug() << "\n - Hough Transform Values - \n";
            qDebug() << qSetRealNumberPrecision(4) << " accumResolutionHole: " << accumResolutionHole;
            qDebug() << " minDistHole:         " << minDistHole
                     << "\n cannyThreshHole:     " << cannyThreshHole
                     << "\n accumThreshHole:     " << accumThreshHole;
            qDebug() << " minRadiusHole:       " << minRadiusHole
                     << "\n maxRadiusHole:       " << maxRadiusHole;
            break;

        default:
            break;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_exportPushButton_clicked()
{
    // Do nothing if contour vector is empty
    if( contoursTarget.size() == 0 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    // Path name initialization
    QString filenamePart1 = "/home/default/Dropbox/[TESE]/Algoritmo/OpenCV/OpenCvTest1/Export/Data";
    QString filenamePart2 = ".txt";
    QString filename = filenamePart1 + filenamePart2;
    //qDebug() << filename;
    file.setFileName(filename);

    // If path name already exists
    if( file.exists() )
    {
        int num = 1;
        QString stringNum;

        // Find file name until it doesn't exists
        while( num < 65536 )
        {
            stringNum = QString::number(num);
            filename = filenamePart1 + stringNum + filenamePart2;
            //qDebug() << filename;
            file.setFileName(filename);

            if(!file.exists())
                break;
            num++;
        }
    }

    // If file can be opened, save contour data in file
    if( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream << "-" << img_filename << endl;

        for(size_t i= 0; i < contoursTarget.size(); i++)
        {
            stream << "-Contour" << i << endl;

            for(size_t j= 0; j < contoursTarget[i].size(); j++)
                stream << contoursTarget[i][j].x << " " << contoursTarget[i][j].y << endl;
        }

        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(fileWrittenInfoMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }

    else
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(ioErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_getDepthDataPushButton_clicked()
{
    // Do nothing if image is empty
    if( img.empty() )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Convert image representaion from RGB to HSV
    Mat img_hsv(img.size(), CV_8UC1);
    cvtColor(img, img_hsv, CV_BGR2HSV);

    // Filter target's hole with Hue information
    Mat img_mod(img.rows, img.cols, CV_8UC1);
    inRange(img_hsv, Scalar(R_H_Min, B_V_Min, G_S_Min),
            Scalar(R_H_Max, B_V_Max, G_S_Max), img_mod);

    // Create circular structuring element
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5), Point(-1,-1));
    // Remove noise with binary image open function
    morphologyEx(img_mod, img_mod, MORPH_CLOSE,
                 element, Point(-1,-1), 3, BORDER_CONSTANT);

    vector<vector<Point> > contoursHoleTemp;

    // Find contours
    findContours(img_mod, contoursHoleTemp, hierarchyHole,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Do nothing if contour vector is empty
    if( contoursHoleTemp.size() < 1 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    // Draw in grayscale image
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    cvtColor(img, drawing, CV_RGB2GRAY);
    cvtColor(drawing, drawing, CV_GRAY2RGB);

    for( size_t i = 0; i < contoursHoleTemp.size(); i++ )
    {
        if( contoursHoleTemp[i].size() > contourPointsHole )
            drawContours(drawing, contoursHoleTemp, i,
                         Scalar(0,0,255), 3, 8, hierarchyHole, 0, Point());
    }

    size_t contoursSize = contoursHoleTemp.size();
    int eraseCount = 0;

    for( size_t i = 0; i < contoursSize; i++ )
    {
        if( contourArea(contoursHoleTemp[i-eraseCount]) < maxAreaHole*1.3 )
        {
            contoursHoleTemp.erase(contoursHoleTemp.begin()+i-eraseCount);
            eraseCount++;
        }
    }

    if( contoursHoleTemp.size() < 1 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    vector<vector<int> > hullI(contoursHoleTemp.size());
    vector<vector<Point> > hullP(contoursHoleTemp.size());
    vector<vector<Vec4i> > defects(contoursHoleTemp.size());

    for( size_t i = 0; i < contoursHoleTemp.size(); i++ )
    {
        convexHull(contoursHoleTemp[i], hullI[i], false);
        convexHull(contoursHoleTemp[i], hullP[i], false);
        convexityDefects(contoursHoleTemp[i], hullI[i], defects[i]);
    }

    vector<vector<double> > depthFileData;
    vector<Point> holeDefectsTemp;

    for( size_t i = 0; i < contoursHoleTemp.size(); i++ )
    {
        float thresh = 0, depth = 0;
        int count = 0, mean;
        //float std;

        vector<Vec4i>::iterator d = defects[i].begin();
        vector<double> tempDepth;
        /*while( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            if( v[3]/256 > thresh )
                thresh = v[3]/256;
            d++;
        }*/

        d = defects[i].begin();
        while( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            depth = depth + v[3]/256;
            count++;
            d++;
        }
        mean = floor(depth/count);

        qDebug() << "Mean: " << mean;

        /*depth = 0;
        count = 0;
        d=defects[i].begin();
        while( d!=defects[i].end() )
        {
            Vec4i& v=(*d);
            depth = pow(v[3]/256 - mean, 2) + depth;
            count++;
            d++;
        }
        std = sqrt(depth/count);

        qDebug() << "Standard deviation: " << std;*/

        count = 0;

        d = defects[i].begin();
        while( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            int faridx = v[2]; Point ptFar(contoursHoleTemp[i][faridx]);
            /*int startidx=v[0]; Point ptStart( contoursHoleTemp[i][startidx] );
            int endidx=v[1]; Point ptEnd( contoursHoleTemp[i][endidx] );

            qDebug() << "Start point: " << ptStart.x << ptStart.y;
            qDebug() << "End point: " << ptEnd.x << ptEnd.y;*/
            qDebug() << "Depth value: " << v[3]/256;
            tempDepth.push_back(v[3]/256);

            if ( ( (v[3]/256) >= mean ) && ( (v[3]/256) > minConvexityDefect ) )
            {
                depth = v[3] / 256;
                holeDefectsTemp.push_back(ptFar);
                /*QString test = QString("holeDefectsTemp[%1]: %2, %3\n").arg(count)
                        .arg(holeDefectsTemp[count].x).arg(holeDefectsTemp[count].y);
                ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
                ui->info2PlainTextEdit->appendPlainText(test);
                ui->info2PlainTextEdit->moveCursor(QTextCursor::End);*/
                count++;
            }

            /*line( drawing, ptStart, ptEnd, Scalar(0,255,0), 1 );
            line( drawing, ptStart, ptFar, Scalar(0,255,0), 1 );
            line( drawing, ptEnd, ptFar, Scalar(0,255,0), 1 );
            circle( drawing, ptFar, 2, Scalar(0,255,0), 2 );*/
            d++;
        }

        depthFileData.push_back(tempDepth);
    }

    if( holeDefectsTemp.size() < 1 )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noOverlappedContoursErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Do nothing if contour vector is empty
    if( depthFileData.size() == 0 )
    {
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       ui->info2PlainTextEdit->insertPlainText("No depth data available.");
       ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
       return;
    }

    // Path name initialization
    QString filenamePart1 = "/home/default/Dropbox/[TESE]/Algoritmo/OpenCV/OpenCvTest1/Export/DataDepth";
    QString filenamePart2 = ".txt";
    QString filename = filenamePart1 + filenamePart2;
    file.setFileName(filename);

    // If path name already exists
    if( file.exists() )
    {
        int num = 1;
        QString stringNum;

        // Find file name until it doesn't exists
        while( num < 65536 )
        {
            stringNum = QString::number(num);
            filename = filenamePart1 + stringNum + filenamePart2;
            file.setFileName(filename);

            if(!file.exists())
                break;
            num++;
        }
    }

    // If file can be opened, save depth data in file
    if( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream << "-" << img_filename << endl;

        for(size_t i= 0; i < depthFileData.size(); i++)
        {
            stream << "-Depth" << i << endl;

            for(size_t j = 0; j < depthFileData[i].size(); j++)
                stream << depthFileData[i][j] << endl;
        }

        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(fileWrittenInfoMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }

    else
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(ioErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_resetValuesPushButton_clicked()
{
    // Contour extraction variables
    //targetRadius = 0;
    contoursTarget.clear();
    hierarchyTarget.clear();
    targetCentroid.x = 0;
    targetCentroid.y = 0;

    // Hole contour detection variables
    holeRadius.clear();
    holeCenter.clear();
    contourPointsHole = 0;//10;
    contoursHole.clear();
    hierarchyHole.clear();
    holeCentroid.clear();
    holeDefects.clear();

    ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
    ui->infoPlainTextEdit->insertPlainText(valuesResetInfoMessage);
    ui->infoPlainTextEdit->moveCursor(QTextCursor::End);
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_savePushButton_clicked()
{
    // Do nothing if snapshot image is empty
    if( cameraFrame.empty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(snapshotErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Convert OpenCV image to RGB888 for Qt
    QImage _img((uchar*)cameraFrame.data,
                cameraFrame.cols,cameraFrame.rows,
                cameraFrame.step,QImage::Format_RGB888);
    // Display image on label
    ui->testLabel->setPixmap(QPixmap::fromImage(_img,Qt::AutoColor));

    // Ask user to keep photo
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save image", "Do you want to use this snapshot?",
                                  QMessageBox::Yes|QMessageBox::No);

    // If yes, save photo for use
    if( reply == QMessageBox::Yes )
      cameraFrame.copyTo(img);
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_cameraPushButton_clicked()
{
    // Checking if processor is available...
    if( !system(NULL) )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(systemErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Take picture and save on root folder
    int sysValue = system("gphoto2 --capture-image-and-download");
    if( sysValue != 0 ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(cmdErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    //system("cd /home/default/Dropbox/[TESE]/Algoritmo/OpenCV/OpenCvTest1/CameraImages");
    //system("ptpcam --capture");
    qDebug() << "System return value:" << sysValue;

    QStringList nameFilter("*.jpg");
    QDir directory(program_filepath);
    QStringList jpgFilesAndDirectories = directory.entryList(nameFilter);
    QString image_name = jpgFilesAndDirectories.takeLast();
    QString image_filepath = program_filepath + "/" + image_name;

    // Do nothing if filename is empty
    if ( image_filepath.isEmpty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Get image from specified path
    img = imread(image_filepath.toStdString(), CV_LOAD_IMAGE_COLOR);

    // Set up window appearance
    if( img.empty() ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(noImageErrorMessage2);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    QFileInfo file(image_name);
    QString imageLoadedInfoMessage = imageLoadedInfoMessagePart1 + file.baseName() +
                                     imageLoadedInfoMessagePart2;

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText(imageLoadedInfoMessage);
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    // Convert image format from OpenCV to Qt and display it on label
    cvtColor(img, img, CV_BGR2RGB);
    QImage _img((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    ui->testLabel->setPixmap(QPixmap::fromImage(_img));
    //ui->imageContainerLabel->setPixmap(QPixmap::fromImage(_img));
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_setupCameraPushButton_clicked()
{
    // Checking if processor is available...
    if( !system(NULL) )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(systemErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    FILE *systemOutput;
    char buffer[1024];
    char *substr;

    // Find processes that use usb connection of camera and test if it worked
    if( !(systemOutput = popen("lsof -c gvfsd-gph | grep usb", "r")) )
    {
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText("Failed.");
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    // Get the terminal output
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText("Finding processes...\n");
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    while( fgets(buffer,sizeof(buffer),systemOutput) != NULL );

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText("Processes found.\n");
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    pclose(systemOutput);

    substr = strndup(buffer + 10, 6);

    cout << substr;

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText("Terminating processes...\n");
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    char killString[15] = "kill ";

    int sysValue = system( strcat(killString, substr) );
    if( sysValue != 0 ){
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        ui->info2PlainTextEdit->insertPlainText(cmdErrorMessage);
        ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
        return;
    }

    qDebug() << "System return value:" << sysValue;

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText("Processes terminated.\n");
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);

    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
    ui->info2PlainTextEdit->insertPlainText("Done.\n");
    ui->info2PlainTextEdit->moveCursor(QTextCursor::End);
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_keepDataCheckBox_toggled(bool checked)
{
    keepDataFlag = checked;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough1Slider_sliderMoved(int position)
{
    double factor = 1;
    accumResolutionTarget = double(factor/position);
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough1lineEdit_textEdited(const QString &arg1)
{
    accumResolutionTarget = arg1.toDouble();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough2Slider_sliderMoved(int position)
{
    cannyThreshTarget = position;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough2lineEdit_textEdited(const QString &arg2)
{
    cannyThreshTarget = arg2.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough3Slider_sliderMoved(int position)
{
    accumThreshTarget = position;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough3lineEdit_textEdited(const QString &arg3)
{
    accumThreshTarget = arg3.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough4lineEdit_textEdited(const QString &arg4)
{
    minRadiusTarget = arg4.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough5lineEdit_textEdited(const QString &arg5)
{
    maxRadiusTarget = arg5.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_hough6lineEdit_textEdited(const QString &arg6)
{
    minDistTarget = arg6.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT1Slider_sliderMoved(int position)
{
    double factor = 1;
    accumResolutionHole = double(factor/position);
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT1lineEdit_textEdited(const QString &arg1)
{
    accumResolutionHole = arg1.toDouble();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT2Slider_sliderMoved(int position)
{
    cannyThreshHole = position;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT2lineEdit_textEdited(const QString &arg2)
{
    cannyThreshHole = arg2.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT3Slider_sliderMoved(int position)
{
    accumThreshHole = position;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT3lineEdit_textEdited(const QString &arg3)
{
    accumThreshHole = arg3.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT4lineEdit_textEdited(const QString &arg4)
{
    minRadiusHole = arg4.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT5lineEdit_textEdited(const QString &arg5)
{
    maxRadiusHole = arg5.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectHT6lineEdit_textEdited(const QString &arg6)
{
    minDistHole = arg6.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_contours1lineEdit_textEdited(const QString &arg1)
{
    contourPointsTargetMax = arg1.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect1lineEdit_textEdited(const QString &arg1)
{
    R_H_Max = arg1.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect2lineEdit_textEdited(const QString &arg2)
{
    G_S_Max = arg2.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect3lineEdit_textEdited(const QString &arg3)
{
    B_V_Max = arg3.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect1lineEdit_2_textEdited(const QString &arg4)
{
    R_H_Min = arg4.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect2lineEdit_2_textEdited(const QString &arg5)
{
    G_S_Min = arg5.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetect3lineEdit_2_textEdited(const QString &arg6)
{
    B_V_Min = arg6.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectSplit1lineEdit_textEdited(const QString &arg1)
{
    minAreaHole = arg1.toDouble();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectSplit2lineEdit_textEdited(const QString &arg2)
{
    maxAreaHole = arg2.toDouble();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::on_holeDetectSplit3lineEdit_textEdited(const QString &arg3)
{
    minConvexityDefect = arg3.toInt();
}

//--------------------------------------------------------------------------------------------------------------------------------

void Dialog::Mouse_current_pos()
{
    ui->pixelCoordinateLabel->setText(QString("X= %1 Y= %2").arg(ui->testLabel->x).arg(ui->testLabel->y));
    //ui->pixelCoordinateLabel->setText(QString("X= %1 Y= %2").arg(ui->imageContainerLabel->x).arg(ui->imageContainerLabel->y));
    //ui->lblMouse_Current_Event->setText("Mouse Moving!!!");
}
