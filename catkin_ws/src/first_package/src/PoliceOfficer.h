#pragma once
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <actionlib/server/simple_action_server.h>
#include <first_package/OrderAction.h>
#include <boost/thread.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/thread/condition_variable.hpp>

class PoliceOfficer
{
public:
  PoliceOfficer();
  ~PoliceOfficer();

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
  boost::condition_variable cv_;
  boost::scoped_thread<boost::join_if_joinable> workingTh_;
  int currentFloor_, targetFloor_;
  void go_toward_floor(int floor);

  void goalCB();
  void preemptCB();
  void working_thread_body();

};

