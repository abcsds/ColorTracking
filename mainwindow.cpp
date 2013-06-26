#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    capture = 0;
    frame = 0;
    capturing = false;
    showThreshold = false;
    showOriginal = false;
    Color = 50;

    imgScribble = NULL;
}

MainWindow::~MainWindow()
{
    cvReleaseCapture(&capture);
    cvDestroyWindow("threshold");
    cvDestroyWindow("video");
    delete ui;
}

IplImage* MainWindow::GetThresholdedImage(IplImage *img) {
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    cvInRangeS(imgHSV, cvScalar((Color-5), 50, 50), cvScalar((Color+5), 255, 255), imgThreshed);

    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

void MainWindow::paintEvent(QPaintEvent *e) {

          QPainter painter(this);

          if(capturing) {
                printf("position (%d,%d)\n", playerPos.x(), playerPos.y());
          }



        this->update();
        e->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    if(e->button()==Qt::LeftButton) {
        QPoint pf = e->pos();
    }
    e->accept();
}

void MainWindow::on_camaraButton_clicked()
{

        if(capturing)
        {
        while(true)
            {
                // Will hold a frame captured from the camera
                frame = 0;
                frame = cvQueryFrame(capture);
                // If we couldn't grab a frame... quit
                if(!frame) break;
                // If this is the first frame, we need to initialize it
                if(imgScribble == NULL)
                {
                    imgScribble = cvCreateImage(cvGetSize(frame), 8, 3);
                }
//                // Holds the yellow thresholded image (yellow = white, rest = black)
                IplImage* imgGreenThresh = GetThresholdedImage(frame);
                // Calculate the moments to estimate the position of the ball
                CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
                cvMoments(imgGreenThresh, moments, 1);

                // The actual moment values
                double moment10 = cvGetSpatialMoment(moments, 1, 0);
                double moment01 = cvGetSpatialMoment(moments, 0, 1);
                double area = cvGetCentralMoment(moments, 0, 0);

                // Holding the last and current ball positions
                static int posX = 0;
                static int posY = 0;

                int lastX = posX;
                int lastY = posY;

                posX = moment10/area;
                posY = moment01/area;

                // Print it out for debugging purposes
                printf("position (%d,%d)\n", posX, posY);
                playerPos = QPoint(posX, posY);
                QString str = "( " + QString::number(posX) + " , " + QString::number(posY) + " ) ";
                ui->positionLabel->setText(str);

                // We want to draw a line only if its a valid position
                if(lastX>0 && lastY>0 && posX>0 && posY>0)
                {
                    // Draw a yellow line from the previous point to the current point
                    cvLine(imgScribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,255,255), 5);
                }

//                // Add the scribbling image and the frame...
                        cvAdd(frame, imgScribble, frame);
                        cvShowImage("threshold", imgGreenThresh);
                        cvShowImage("video", frame);

                // Wait for a keypress
                        int c = cvWaitKey(10);
                        if(c!=-1)
                        {
                            // If pressed, break out of the loop
                            cvDestroyWindow("threshold");
                            cvDestroyWindow("video");
                            break;
                        }

                        // Release the thresholded image+moments... we need no memory leaks.. please
                                cvReleaseImage(&imgGreenThresh);
                                delete moments;
            }
        // We're done using the camera. Other applications can now use it
            //cvReleaseCapture(&capture);
            //cvReleaseWindow?();
        }
}

void MainWindow::on_capturarButton_clicked()
{
    capturing = !capturing;
    if (capturing) {
        ui->capturarButton->setText("Detener captura");
        capture = cvCaptureFromCAM(0);
    }
    else {
        cvDestroyWindow("video");
        cvDestroyWindow("threshold");
        ui->capturarButton->setText("Capturar");
        cvReleaseCapture(&capture);
    }
}

void MainWindow::on_imagenButton_clicked()
{
    showOriginal = !showOriginal;
    cvDestroyWindow("threshold");
    if(showOriginal) {
        //ui->camaraButton->setText("");
        if(capturing)
        {
        while(true)
            {
                frame = 0;
                frame = cvQueryFrame(capture);


                        cvShowImage("video", frame);

                // Wait for a keypress
                        int c = cvWaitKey(10);
                        if(c!=-1)
                        {
                            // If pressed, break out of the loop
                            cvDestroyWindow("video");
                            break;
                        }


            }
        }
    }
    else {
        cvDestroyWindow("video");
        //Release window
    }
}

void MainWindow::on_objetoButton_clicked()
{
    showThreshold = !showThreshold;
    cvDestroyWindow("video");
    if(showThreshold) {
        if(capturing)
        {

        while(true)
            {
                frameT = cvQueryFrame(capture);
                // If we couldn't grab a frame... quit
                if(!frameT) break;

                IplImage* imgGreenThresh = GetThresholdedImage(frameT);

                        cvShowImage("threshold", imgGreenThresh);

                // Wait for a keypress
                        int c = cvWaitKey(10);
                        if(c!=-1)
                        {
                            // If pressed, break out of the loop
                            cvDestroyWindow("threshold");
                            break;
                        }

                                cvReleaseImage(&imgGreenThresh);

            }
        }

    }
    else {
        cvDestroyWindow("threshold");
    }
}

void MainWindow::on_colorSlider_valueChanged(int value)
{
    printf("H = %i\n", value);
    Color = value;
    ui->label->setText(QString::number(value));
}
