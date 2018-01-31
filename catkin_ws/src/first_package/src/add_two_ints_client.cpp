#include "ros/ros.h"
#include "first_package/AddTwoInts.h"
#include "first_package/Speak.h"
#include <cstdlib>
#include <thread>

int execute_speak(int id, const std::string &msg, int repeat)
{
  ROS_INFO("execute_speak starts for %d", id);
  int retVal=0;
  ros::NodeHandle n;
  ros::ServiceClient clientSpeak = n.serviceClient<first_package::Speak>("speak");
  first_package::Speak spk;
  spk.request.msg = msg;
  spk.request.repeat = repeat;
  if (clientSpeak.call(spk))
  {
    ROS_INFO("speak result of %d is: %d", id, spk.response.num);
  }
  else
  {
    ROS_ERROR("Failed to call service speak");
    retVal = 1;
  }
  return retVal;
}

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

  std::thread th1([](){execute_speak(1, "hello", 4);});
  std::thread th2([](){execute_speak(2, "world", 5);});

  th1.join();
  th2.join();

  return 0;
}
