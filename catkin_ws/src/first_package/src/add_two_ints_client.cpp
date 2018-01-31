#include "ros/ros.h"
#include "first_package/AddTwoInts.h"
#include "first_package/Speak.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_two_ints_client");
  if (argc != 3)
  {
    ROS_INFO("usage: add_two_ints_client X Y");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<first_package::AddTwoInts>("add_two_ints");
  first_package::AddTwoInts srv;
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);
  if (client.call(srv))
  {
    ROS_INFO("Sum: %ld", (long int)srv.response.sum);
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }

  ros::ServiceClient clientSpeak = n.serviceClient<first_package::Speak>("speak");
  first_package::Speak spk;
  spk.request.msg = "hello";
  spk.request.repeat = 4;
  if (clientSpeak.call(spk))
  {
    ROS_INFO("Sum: %ld", (long int)spk.response.num);
  }
  else
  {
    ROS_ERROR("Failed to call service speak");
    return 1;
  }


  return 0;
}
