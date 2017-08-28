/*
 *
 *  已实现图片的人脸检测，摄像头的人脸检测
 *
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#define	FORMAT_WIDTH 640 //图片显示宽度
#define	FORMAT_HEIGHT 480 //图片显示高度
#define imgSizeScaleSmall 0.5    //图像放缩比例
#define imgSizeScaleBig 2    //图像放缩比例
 unsigned char frame_buffer[FORMAT_WIDTH*FORMAT_HEIGHT*3];
#define image_Format QImage::Format_RGB888 //图片显示格式
#define cameraDevice "/dev/video1"  //摄像头设备
#define haarXML "./date/haarcascade_frontalface_alt2.xml"   //人脸模型级联分类器文件

int v2r[256] = {  //(1.370705 * (v-128))
    -175, -174, -172, -171, -169, -168, -167, -165, -164, -163, -161, -160, -159, -157, -156, -154,
    -153, -152, -150, -149, -148, -146, -145, -143, -142, -141, -139, -138, -137, -135, -134, -132,
    -131, -130, -128, -127, -126, -124, -123, -121, -120, -119, -117, -116, -115, -113, -112, -111,
    -109, -108, -106, -105, -104, -102, -101, -100, -98, -97, -95, -94, -93, -91, -90, -89,
    -87, -86, -84, -83, -82, -80, -79, -78, -76, -75, -74, -72, -71, -69, -68, -67,
    -65, -64, -63, -61, -60, -58, -57, -56, -54, -53, -52, -50, -49, -47, -46, -45,
    -43, -42, -41, -39, -38, -37, -35, -34, -32, -31, -30, -28, -27, -26, -24, -23,
    -21, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -4, -2, -1,
    0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 15, 16, 17, 19, 20,
    21, 23, 24, 26, 27, 28, 30, 31, 32, 34, 35, 37, 38, 39, 41, 42,
    43, 45, 46, 47, 49, 50, 52, 53, 54, 56, 57, 58, 60, 61, 63, 64,
    65, 67, 68, 69, 71, 72, 74, 75, 76, 78, 79, 80, 82, 83, 84, 86,
    87, 89, 90, 91, 93, 94, 95, 97, 98, 100, 101, 102, 104, 105, 106, 108,
    109, 111, 112, 113, 115, 116, 117, 119, 120, 121, 123, 124, 126, 127, 128, 130,
    131, 132, 134, 135, 137, 138, 139, 141, 142, 143, 145, 146, 148, 149, 150, 152,
    153, 154, 156, 157, 159, 160, 161, 163, 164, 165, 167, 168, 169, 171, 172, 174

};
int v2g[256]={ //(0.698001 * (v-128))
    -89, -88, -87, -87, -86, -85, -85, -84, -83, -83, -82, -81, -80, -80, -79, -78,
    -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -69, -68, -67,
    -67, -66, -65, -64, -64, -63, -62, -62, -61, -60, -60, -59, -58, -57, -57, -56,
    -55, -55, -54, -53, -53, -52, -51, -50, -50, -49, -48, -48, -47, -46, -46, -45,
    -44, -43, -43, -42, -41, -41, -40, -39, -39, -38, -37, -36, -36, -35, -34, -34,
    -33, -32, -32, -31, -30, -30, -29, -28, -27, -27, -26, -25, -25, -24, -23, -23,
    -22, -21, -20, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11,
    -11, -10, -9, -9, -8, -7, -6, -6, -5, -4, -4, -3, -2, -2, -1, 0,
    0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10,
    11, 11, 12, 13, 13, 14, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21,
    22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32,
    33, 34, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 41, 42, 43, 43,
    44, 45, 46, 46, 47, 48, 48, 49, 50, 50, 51, 52, 53, 53, 54, 55,
    55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 64, 65, 66,
    67, 67, 68, 69, 69, 70, 71, 71, 72, 73, 73, 74, 75, 76, 76, 77,
    78, 78, 79, 80, 80, 81, 82, 83, 83, 84, 85, 85, 86, 87, 87, 88

};
int u2g[256]={ //(0.337633 * (u-128))

    -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38,
    -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -34, -33, -33, -33, -32,
    -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27,
    -27, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -23, -22, -22, -22, -21,
    -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16,
    -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -12, -11, -11, -11,
    -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5,
    -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, -1, 0, 0,
    0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
    5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10,
    10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15,
    16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21,
    21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26,
    27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32,
    32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37,
    37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42

};
int u2b[256]={ //(1.732446 * (u-128))
    -221, -220, -218, -216, -214, -213, -211, -209, -207, -206, -204, -202, -200, -199, -197, -195,
    -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -168,
    -166, -164, -162, -161, -159, -157, -155, -154, -152, -150, -148, -147, -145, -143, -142, -140,
    -138, -136, -135, -133, -131, -129, -128, -126, -124, -123, -121, -119, -117, -116, -114, -112,
    -110, -109, -107, -105, -103, -102, -100, -98, -97, -95, -93, -91, -90, -88, -86, -84,
    -83, -81, -79, -77, -76, -74, -72, -71, -69, -67, -65, -64, -62, -60, -58, -57,
    -55, -53, -51, -50, -48, -46, -45, -43, -41, -39, -38, -36, -34, -32, -31, -29,
    -27, -25, -24, -22, -20, -19, -17, -15, -13, -12, -10, -8, -6, -5, -3, -1,
    0, 1, 3, 5, 6, 8, 10, 12, 13, 15, 17, 19, 20, 22, 24, 25,
    27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 45, 46, 48, 50, 51, 53,
    55, 57, 58, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81,
    83, 84, 86, 88, 90, 91, 93, 95, 97, 98, 100, 102, 103, 105, 107, 109,
    110, 112, 114, 116, 117, 119, 121, 123, 124, 126, 128, 129, 131, 133, 135, 136,
    138, 140, 142, 143, 145, 147, 148, 150, 152, 154, 155, 157, 159, 161, 162, 164,
    166, 168, 169, 171, 173, 174, 176, 178, 180, 181, 183, 185, 187, 188, 190, 192,
    194, 195, 197, 199, 200, 202, 204, 206, 207, 209, 211, 213, 214, 216, 218, 220


};
String cascadeName = "./date/haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "./date/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
#define imgSizeScaleSmall 0.5    //图像放缩比例
#define imgSizeScaleBig 2    //图像放缩比例
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 //   frame_buffer = (unsigned char *)malloc(FORMAT_HEIGHT * FORMAT_WIDTH* 3 * sizeof(char));
// 摄像头操作
    frame = new QImage(frame_buffer,FORMAT_WIDTH,FORMAT_HEIGHT,image_Format);   //根据存储空间开辟QImage图像
    m_camera = new VideoDevice(tr(cameraDevice));  //获取摄像头设备

    m_camera->OpenDevice();

    /*
     * 图片检测
     */
    //图片打开
//   fram = imread("./1.jpg",1);
//    if(fram.empty())
//    {
//        cout << "open the photo error" << endl;

//    }else
//    {
//        cout << "open the photo ok" << endl;
//    }
    //设定时间间隔，对图像界面进行刷新
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));   //update将调用paintEvent
    timer->start(200);

    // Storage for the rectangles detected
    cascadeFile = haarXML;
    cascade = (CvHaarClassifierCascade *) cvLoad(cascadeFile.toUtf8());
    if(cascade)
    {
        printf("cascade\n");
    }
    m_FaceCount = 0;
    storage = cvCreateMemStorage(0);
    cascade1.load(cascadeName);
    nestedCascade.load(nestedCascadeName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{

    camReturn = m_camera->Getframe(frame_buffer);
    QImage image(frame_buffer,640,480,QImage :: Format_RGB888);
    if(camReturn < 0)return; //没有获取到原数据
 double sizeScale = imgSizeScaleSmall;
     // QImage转IplImage
    IplImage* ipl_img = QImageToIplImage(&image);
    //压缩图片进行人脸检测  ，提高检测的速度    （测试证明提高10倍）
    CvSize img_cvsize;
    img_cvsize.width = ipl_img->width * sizeScale;
    img_cvsize.height = ipl_img->height * sizeScale;
    IplImage* dst = cvCreateImage(img_cvsize, ipl_img->depth, ipl_img->nChannels);
    cvResize(ipl_img, dst, CV_INTER_LINEAR);
    //实现人脸检测
    detect_and_draw(dst);
    //还原图片的大小
    sizeScale = imgSizeScaleBig;
    img_cvsize.width = dst->width * sizeScale;
    img_cvsize.height = dst->height * sizeScale;
    IplImage* img = cvCreateImage(img_cvsize, dst->depth, dst->nChannels);
    cvResize(dst, img, CV_INTER_LINEAR);

    //IplImage 转 QImage 模式 BUG:图片会倾斜
    // QImage qimage = QImage((uchar *)ipl_img->imageData, ipl_img->width,ipl_img->height,QImage::Format_RGB888 );
   QImage qimage = QImage((uchar *)img->imageData, img->width,img->height,QImage::Format_RGB888 );
     //IplImage为BGR格式，QImage为RGB格式，所以要交换B和R通道显示才正常
    //可以用OpenCV的cvConcertImage函数交换，也可以用QImage的rgbSwapped函数交换；
     qimage = qimage.rgbSwapped();

     //Mat*/ 转换位 IplImage 模式
     //   IplImage *ipl_img;
  /*
     Mat rgb;
        QImage img;
        if(fram.channels()==3)
        {
            //cvt Mat BGR 2 QImage RGB
            cvtColor(fram,rgb,CV_BGR2RGB);
            img =QImage((const unsigned char*)(rgb.data),
                        rgb.cols,rgb.rows,
                        rgb.cols*rgb.channels(),
                        QImage::Format_RGB888);
        }
        else
        {
            img =QImage((const unsigned char*)(fram.data),
                        fram.cols,fram.rows,
                        fram.cols*fram.channels(),
                        QImage::Format_RGB888);
        }
*/

        // 在lable中显示图片
        ui->m_imgLabel->setPixmap(QPixmap::fromImage(qimage));
        ui->m_imgLabel->resize(ui->m_imgLabel->pixmap()->size());
         //释放图片内存
        m_camera->Freeframe();
        cvReleaseImage(&ipl_img);
}

void MainWindow::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}


//QImage 转 IplImage
IplImage* MainWindow::QImageToIplImage(const QImage * qImage)
{
    int width = qImage->width();
    int height = qImage->height();
    CvSize Size;
    Size.height = height;
    Size.width = width;
    IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QRgb rgb = qImage->pixel(x, y);
            cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
        }
    }
    return IplImageBuffer;
}

//人脸检测函数
void MainWindow::detect_and_draw(IplImage *img)
{

        const static Scalar colors[] =
        {
            Scalar(255,0,0),
            Scalar(255,128,0),
            Scalar(255,255,0),
            Scalar(0,255,0),
            Scalar(0,128,255),
            Scalar(0,255,255),
            Scalar(0,0,255),
            Scalar(255,0,255)
        };

    double scale = 2.0;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                cvRound (img->height/scale)), 8, 1 );
    int i;

    cvCvtColor( img, gray, CV_BGR2GRAY );
   cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );
    if( cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                        1.2, 2, 0, cvSize(30, 30) );
        t = (double)cvGetTickCount() - t;
        printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
        printf("======================detect ok =========\n");
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

                        CvPoint rectP1, rectP2;
                        rectP1.x = cvRound(r->x * scale);
                        rectP1.y = cvRound(r->y * scale);
                        rectP2.x = cvRound((r->x + r->width) * scale);
                        rectP2.y = cvRound((r->y + r->height) * scale);
                        cvRectangle(img, rectP1, rectP2, colors[i%8], 3, 8, 0);

        }
                m_FaceCount = faces->total;
    }

    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}
