#pragma once

#include "ros/ros.h"
#include <ros/callback_queue.h>
#include <string>

class DishWasher
{
public:
  DishWasher();
  void Start();
  void Stop();
  int Speak(const std::string &msg, int repeat);
private:
  static const int NUM_THREADS=4;
  ros::NodeHandle node_;
  ros::CallbackQueue callbackQueue_;
  ros::AsyncSpinner asyncSpinner_;
  ros::ServiceServer service_;
};

