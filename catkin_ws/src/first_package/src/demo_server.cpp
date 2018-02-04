#include "ros/ros.h"
#include "Victim.h"
#include "Policeman.h"
#include "PoliceOfficer.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo_server");

  Victim victim;
  victim.Start();

  Policeman policeman;
  PoliceOfficer policeofficer;

  ros::spin();

  return 0;
}

