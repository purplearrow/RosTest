#pragma once

#include "ros/ros.h"
#include <string>

class DishWasher
{
public:
  DishWasher();
  void Start();
  void Stop();
  int Speak(const std::string &msg, int repeat);
private:
  ros::NodeHandle node_;
  ros::ServiceServer service_;
};

