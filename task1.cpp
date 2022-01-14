#include "task1.hpp"

namespace ImageTrans
{
  ImageTrans::ImageTrans()
  {
    ROS_INFO("====== IMAGE TRANS NODE STARTED ======");
    countImg_src = 3;
    countImg_dst = 3;
    img_sub_src = nh.subscribe("/usb_cam/image_raw", 1, &ImageTrans::img_callback_src, this);
    img_pub = nh.advertise<sensor_msgs::Image>("/processed_image", 1);
    img_sub_dst = nh.subscribe("/processed_image", 1, &ImageTrans::img_callback_dst, this);
  }
  //原图回调函数
  void ImageTrans::img_callback_src(const sensor_msgs::ImageConstPtr &img)
  {

    cv::Mat src;
    sensor_msgs::ImagePtr image;
    src = cv_bridge::toCvCopy(img, "bgr8")->image;
    char path[70];
    ros::Rate loop_rate(0.2);
    if(countImg_src != 0)
    {
    //信息接收
    ROS_INFO("====TEST RATE 1====");
    sprintf(path, "/home/smile/ROS/10.10_10.17_ws/results/src_%d.jpg",countImg_src--);
    cv::imwrite(path, src);

    {//图片处理
    //尺度变换
    //cv::resize(src, src, cv::Size(src.rows / 4, src.cols / 4));

    /*旋转变化
    cv::Mat dst;
    cv::Size src_sz = src.size();
    cv::Size dst_sz(src_sz.height,src_sz.width);
    cv::Point2f center(src.cols/2.,src.rows/2.);
    cv::Mat rot_mat = cv::getRotationMatrix2D(center,-50.0,1.0);
    cv::warpAffine(src,src,rot_mat,dst_sz);*/ 

    //翻转变换 + 0 -
    cv::flip(src,src,-1);

    //高斯滤波
    //cv::GaussianBlur(src,src,cv::Size(5,5),3,3);
    }

    //信息发布
    image = cv_bridge::CvImage(std_msgs::Header(), "bgr8", src).toImageMsg();
    img_pub.publish(image);

    loop_rate.sleep();
    }
  }
  //处理后图片回调函数
  void ImageTrans::img_callback_dst(const sensor_msgs::ImageConstPtr &img)
  {
    //cv型图片
    cv::Mat dst;
    //ROS型图片
    sensor_msgs::ImagePtr image;
    //ROS型变CV型
    dst = cv_bridge::toCvCopy(img, "bgr8")->image;

    char path[70];
    ros::Rate loop_rate(0.2);
    if(countImg_dst != 0)
    {
    //信息接收
    ROS_INFO("====TEST RATE 2====");
    sprintf(path, "/home/smile/ROS/10.10_10.17_ws/results/dst_%d.jpg",countImg_dst--);
    cv::imwrite(path, dst);
    cv::imshow("dst",dst);
    //图片显示1秒
    cv::waitKey(1000);
    loop_rate.sleep();
    }
  }
}


int main(int argc, char **argv)
{

  // Initiate ROS
  ros::init(argc, argv, "image_trans");

  // Create an object of class SubscribeAndPublish that will take care of everything
  ImageTrans::ImageTrans letsRoll;
  ros::AsyncSpinner spinner(4); //非阻塞式的spinner, 可以使用start和stop进行启停
  spinner.start();              //启动线程
  ros::waitForShutdown();       //等待退出
  return 0;
}
