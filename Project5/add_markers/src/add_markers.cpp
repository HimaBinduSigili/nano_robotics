#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <math.h>

ros::Publisher marker_pub;
visualization_msgs::Marker marker;
float pickat[2] = {-1.3,3.8};
float dropat[2] = {1.3,-3.8};
float threshold_dist = 0;
std::string action = "pick";


void odom_callback(const nav_msgs::Odometry::ConstPtr& msg){

	if(action=="pick"){
	  ROS_INFO("Robot is at position x= %f,  y= %f", msg->pose.pose.position.x, msg->pose.pose.position.y);
	  threshold_dist = sqrt(pow(pickat[0]-msg->pose.pose.position.x,2)+pow(pickat[1]-msg->pose.pose.position.y,2));
	  ROS_INFO("Pick up object is at a distance of %f meters",threshold_dist);
	  if(threshold_dist>0.4){
		marker_pub.publish(marker);
	  }
	  else{
		marker.color.a = 0.0;
		marker_pub.publish(marker);
		action = "drop";
	  }
	}
	else{
	  ROS_INFO("Robot is at position x= %f,  y= %f", msg->pose.pose.position.x, msg->pose.pose.position.y);
	  threshold_dist = sqrt(pow(dropat[0]-msg->pose.pose.position.x,2)+pow(dropat[1]-msg->pose.pose.position.y,2));
	  ROS_INFO("drop off object is at a distance of %f meters",threshold_dist);
	  if(threshold_dist>0.3){
		marker.color.a = 0.0;
		marker_pub.publish(marker);
	  }
	  else{
		marker.pose.position.x = 1.3;
       		marker.pose.position.y = -3.8;
       		marker.pose.position.z = 0;
		marker.color.a = 1.0;
		marker_pub.publish(marker);
	  }

	}
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  
  while (ros::ok())
  {
       
       // Set the frame ID and timestamp.  See the TF tutorials for information on these.
       marker.header.frame_id = "/map";
       marker.header.stamp = ros::Time::now();
   
       // Set the namespace and id for this marker.  This serves to create a unique ID
       // Any marker sent with the same namespace and id will overwrite the old one
       marker.ns = "add_markers";
       marker.id = 0;
   
       // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
       marker.type = shape;
   
       // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
       marker.action = visualization_msgs::Marker::ADD;
   
       // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
       marker.pose.position.x = -1.3;
       marker.pose.position.y = 3.8;
       marker.pose.position.z = 0;
       marker.pose.orientation.x = 0.0;
       marker.pose.orientation.y = 0.0;
       marker.pose.orientation.z = 0.0;
       marker.pose.orientation.w = 1.0;
   
       // Set the scale of the marker -- 1x1x1 here means 1m on a side
       marker.scale.x = 0.3;
       marker.scale.y = 0.3;
       marker.scale.z = 0.3;
   
       // Set the color -- be sure to set alpha to something non-zero!
       marker.color.r = 0.0f;
       marker.color.g = 0.0f;
       marker.color.b = 1.0f;
       marker.color.a = 1.0;
   
       marker.lifetime = ros::Duration();
   
       // Publish the marker
       while (marker_pub.getNumSubscribers() < 1)
       {
         if (!ros::ok())
         {
           return 0;
         }
         ROS_WARN_ONCE("Please create a subscriber to the marker");
         sleep(1);
       }
       ros::Subscriber sub = n.subscribe("/odom",10,odom_callback);
       ros::spin();
       

    }
 }
