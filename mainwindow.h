#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videodevice.h"
#include <QMessageBox>
#include <QTimer>
#include <stdio.h>
#include "/usr/local/opencv-imx6/include/opencv2/opencv.hpp"

using namespace std;
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    VideoDevice *m_camera;  //摄像头
    uchar *imgBuf;  //QImage图像空间
    int camReturn;  //摄像头使用过程返回的结果，用于状态判断
    QImage *frame;  //QImage图像
    QTimer *timer;
 Mat fram;
    CvMemStorage *storage;
    QString cascadeFile;
    CvHaarClassifierCascade *cascade;
    int m_FaceCount;
  CascadeClassifier cascade1, nestedCascade;
   VideoCapture *cap;

    //以下两个函数用于YUV格式转换成RGB格式
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    IplImage *QImageToIplImage(const QImage * qImage);
    void detect_and_draw(IplImage *img);
    void detectAndDraw( Mat& img );
protected:
    void changeEvent(QEvent *e);

private slots:
    void paintEvent(QPaintEvent *);
    void display_error(QString err);};

#endif // MAINWINDOW_H
