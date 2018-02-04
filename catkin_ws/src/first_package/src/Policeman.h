#pragma once
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <actionlib/server/simple_action_server.h>
#include <first_package/OrderAction.h>

class Policeman
{
public:
  Policeman();

  void GetOrder(int floor);
  // return number of thief in that floor
  // return -1 if preempted
  int FindThief();
private:
  static const int NUM_THREADS=1;
  ros::NodeHandle node_;
  ros::CallbackQueue callbackQueue_;
  ros::AsyncSpinner asyncSpinner_;
  actionlib::SimpleActionServer<first_package::OrderAction> as_;
  int currentFloor_, targetFloor_;
  void go_toward_floor(int floor);
  void executeCB(const first_package::OrderGoalConstPtr &goal);

};

