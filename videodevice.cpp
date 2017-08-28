#include "videodevice.h"
#define	FORMAT_WIDTH 640
#define	FORMAT_HEIGHT 480
#define CAMERE_COM "/dev/video1"
#include <QDebug>
VideoDevice::VideoDevice(QObject *parent) : QObject(parent)
{

}


VideoDevice::VideoDevice(QString dev_name)
{


}
int VideoDevice::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}
int VideoDevice::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

int VideoDevice::OpenDevice()
{

    /*step 1: 打开设备*/
    fd = open(CAMERE_COM, O_RDWR | O_NONBLOCK);
    if (fd == -1)
    {
        perror("open error:");
        return -1;
    }


    /*step 2: 查询设备的功能*/
    struct v4l2_capability  cap;
   ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if (ret == -1)
    {
        perror("VIDIOC_QUERYCAP error:");
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))  //设备不支持capture
    {
        printf("don't support CAPTURE  interface\n");
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING))  //设备不支持streaming
    {
        printf("don't support STREAMING  interface\n");
        return -1;
    }



    /*step 3: 枚举设备支持的视频格式*/
    struct v4l2_fmtdesc fmt;

    //当前视频设备支持的视频格式
    memset(&fmt,0,sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while((ret = ioctl(fd,VIDIOC_ENUM_FMT,&fmt)) == 0)
    {
        fmt.index ++ ;

        printf("{pixelformat = %c%c%c%c},description = '%s'\n",
                fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
                (fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
                fmt.description);
    }


    /*step 4: 设置设备的视频格式。*/
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

   ret = ioctl(fd, VIDIOC_G_FMT, &format);
    if (ret  == 0)
    {

        printf("image resution: %d x %d  image size: %d  {pixelformat = %c%c%c%c}\n",
                format.fmt.pix.width, format.fmt.pix.height,
                format.fmt.pix.sizeimage,
                format.fmt.pix.pixelformat & 0xff ,(format.fmt.pix.pixelformat >> 8)&0xff,
                (format.fmt.pix.pixelformat >> 16) & 0xff,  (format.fmt.pix.pixelformat >> 24)&0xff
                );

    }


    struct v4l2_format  s_format;
    s_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s_format.fmt.pix.width = FORMAT_WIDTH;
    s_format.fmt.pix.height = FORMAT_HEIGHT;
    s_format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

   ret = ioctl(fd, VIDIOC_S_FMT, &s_format);
    if (ret == -1)
    {
        perror("VIDIOC_S_FMT error:");
        return -1;
    }


    /*
        step 5: 申请视频缓冲
    */
    struct v4l2_requestbuffers  reqbufs;
    reqbufs.count = 2; //申请缓冲区的数量
    reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; //缓冲类型
    reqbufs.memory = V4L2_MEMORY_MMAP;  //内存类型。

   ret = ioctl(fd, VIDIOC_REQBUFS, &reqbufs);
    if (ret == -1)
    {
        perror("VIDIOC_REQBUFS error:");
        return -1;
    }


    buffers = (buffer *)malloc(sizeof(*buffers) *  reqbufs.count);
    if (!buffers)
        {
            qDebug()<<"Out of memory";
            return false ;
        }

    /*
        step 6: 获取每个缓冲区的信息，并映射，再把它加入到
            采集队列中去。

    */
    int i;
    for (i = 0; i < 2; i++)
    {

        memset(&vbuf, 0, sizeof(vbuf));
        vbuf.index = i;
        vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vbuf.memory = V4L2_MEMORY_MMAP;

        /*查询缓冲区信息*/
       ret = ioctl(fd, VIDIOC_QUERYBUF, &vbuf);
        if (ret == -1)
        {
            perror("VIDIOC_QUERYBUF error:");
            continue;
        }

        /*映射缓冲区*/
        buffers[vbuf.index].start = mmap(
                        NULL,
                        vbuf.length,
                        PROT_WRITE,
                        MAP_SHARED,
                        fd,
                        vbuf.m.offset
                    );
        buffers[vbuf.index].len = vbuf.length;


        /*把申请到的缓冲区加入到采集队列上去*/
       ret = ioctl(fd, VIDIOC_QBUF, &vbuf);
        if (ret == -1)
        {
            perror("VIDIOC_QBUF error:");
            continue;
        }


    }


    /*
        step 7: 启动数据采集
    */
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret == -1)
    {
        perror("VIDIOC_STREAMON error:");
        return -1;
    }


    /*
        step 8:  读取数据，处理数据
    */
}

int VideoDevice::Getframe(unsigned char *imageBuf)
{

    /*取出缓冲区*/
   ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);
    if (ret == -1)
    {
       // perror("VIDIOC_DQBUF error:");
        //continue;
        return -1;
    }

    /*处理数据*/

  convert_yuv_to_rgb_buffer((unsigned char *)buffers[vbuf.index].start,imageBuf,FORMAT_WIDTH,FORMAT_HEIGHT);
    return 0;
}

int VideoDevice::Freeframe()
{
    /*重新把缓冲区加入到采集队列中去*/
   ret = ioctl(fd, VIDIOC_QBUF, &vbuf);
    if (ret == -1)
    {
        perror("VIDIOC_QBUF error:");
        return -1;

    }
    return ret;
}


