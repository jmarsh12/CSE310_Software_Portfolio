#include "ros/ros.h"
#include "my_id_robot/FindObjectOpenCV.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void findContours(Mat image, int &x, int &y)
{

  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  int i;
  
  findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
 
  if (contours.size() == 0)
    {
      ROS_INFO("No objects found");
      return;
    }
  else
    {
      for (i = 0; i < contours.size(); i++)
	{
	  Point2f center;
	  float radius;
	  minEnclosingCircle(contours[i], center, radius);
	  circle(image, center, radius, (0, 255, 0), 2, 8);
	  //	  imshow("Image", image);
	  //waitKey(0);
	  //destroyWindow("Image");
	  x = int(center.x);
	  y = int(center.y);
	  ROS_INFO("Object found %d, %d", x, y);
	  break;
	}
    }
  return;
}
bool find_object(my_id_robot::FindObjectOpenCV::Request  &req,
         my_id_robot::FindObjectOpenCV::Response &res)
{
  ROS_INFO("Got a call to find an Object");
  VideoCapture capture(0);

  if (!capture.isOpened())
    ROS_INFO("Camera not opened");
  else
    {
      Mat image;
      Mat grey_image;
      Mat img_noise;
      Mat image_threshold;
      int x = 0;
      int y = 0;
      capture >> image;
      namedWindow("Display Image", WINDOW_AUTOSIZE);
      imshow("Display Image", image);
      waitKey(0);
      destroyWindow("Display Image");
      cvtColor(image, grey_image, COLOR_BGR2GRAY);
      medianBlur(grey_image,img_noise, 3);
      threshold(img_noise, image_threshold, 0, 255, THRESH_OTSU);
      findContours(image_threshold, x, y);
      res.x = x;
      res.y = y;
      ROS_INFO("request: s=%s", req.id_object.c_str() );
      ROS_INFO("sending back response: x=%d, y=%d", res.x, res.y);
    }
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "find_object");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("my_id_robot", find_object);
  ROS_INFO("Ready to find Object");
  ros::spin();

  return 0;
}