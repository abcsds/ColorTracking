#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>

#include <cv2qt.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   IplImage* GetThresholdedImage(IplImage* img);

    
private slots:
   void on_camaraButton_clicked();

   void on_capturarButton_clicked();

   void on_imagenButton_clicked();

   void on_objetoButton_clicked();

   void on_colorSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    bool capturing;
    bool showThreshold;
    bool showOriginal;
    bool showPoint;
    IplImage* imgScribble;
    IplImage* frame;
    IplImage* frameT;
    QPoint playerPos;
    unsigned int Color;


    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    CvCapture *capture;
    //virtual void mouseGrabber();
};

#endif // MAINWINDOW_H
