#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <QObject>
#include <stdio.h>
#include <errno.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/ioctl.h>


#include <QString>
#define CLEAR(x) memset(&(x), 0, sizeof(x))
typedef struct vBuf
{
    void *start;  //指向视频缓冲的首地址
    unsigned int len; //长度
}buffer;


class VideoDevice : public QObject
{
    Q_OBJECT
public:
    explicit VideoDevice(QObject *parent = 0);
    VideoDevice(QString dev_name);

    int OpenDevice();
    int Getframe(unsigned char *imageBuf);
    int Freeframe();
  int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
public slots:
private:
    buffer *buffers;

    QString dev_name;
    int fd,ret;
    struct v4l2_buffer  vbuf;
    unsigned int n_buffers;
    int index;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);


signals:
    void display_error(QString);

};

#endif // VIDEODEVICE_H
