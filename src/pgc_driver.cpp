#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include "../include/pgc_driver/Driver.h"


int main(int argc, char **argv)
{
	ros::init(argc, argv, "pgcDriver", ros::init_options::AnonymousName);
	ros::NodeHandle nh;

	image_transport::ImageTransport it(nh);

	Driver driver = Driver();

	//set up driver's publishers according to ros standards
	driver.cameraPublisher = it.advertise(driver.topic + "/cameraInfo", 1);
	driver.distortMonoPublisher = it.advertise(driver.topic + "/image", 1);
	driver.distortColorPublisher = it.advertise(driver.topic + "/image_color", 1);
	driver.rectColorPublisher = it.advertise(driver.topic + "/image_rect_color", 1);
	driver.rectMonoPublisher = it.advertise(driver.topic + "/image_rect", 1);


	// try to connect the camera
	while(!driver.connectCamera(driver.getSerialNumber()) && nh.ok())
	{
		ros::Duration wait(1);
		wait.sleep(); // wait 1 second
	}

	ros::Rate loop_rate(driver.frame_rate);

	while(nh.ok())
	{
		ROS_DEBUG_ONCE("capturing image");
		driver.captureImage();

		//driver.viewImage(driver.getImage());

		loop_rate.sleep();
	}


	return 0;
}