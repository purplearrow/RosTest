#include "PoliceOfficer.h"
#include <thread>

PoliceOfficer::PoliceOfficer()
  : node_(), callbackQueue_(true), asyncSpinner_(NUM_THREADS, &callbackQueue_),
    as_(node_, "PoliceOfficerActionServer", false), //auto start is false
    cv_(), workingTh_([this](){working_thread_body();}),
    currentFloor_(1), targetFloor_(1)
{
  ROS_INFO("Policeman constructor, namespace = %s", node_.getNamespace().c_str());
  node_.setCallbackQueue(&callbackQueue_);
  asyncSpinner_.start();

  as_.registerGoalCallback([this](){goalCB();});
  as_.registerPreemptCallback([this](){preemptCB();});
  as_.start();
}
PoliceOfficer::~PoliceOfficer()
{
  cv_.notify_all();
}
void PoliceOfficer::goalCB()
{
  // accept the new goal
  int targetFloor = as_.acceptNewGoal()->floor;
  GetOrder(targetFloor);
  ROS_INFO("PoliceOfficer goalCB:- accept new goal = %d", targetFloor);

  //wake up the working thread
  cv_.notify_all();
}
void PoliceOfficer::preemptCB()
{
  ROS_INFO("PoliceOfficer preemptCB");
  // set the action state to preempted
  as_.setPreempted();
}
void PoliceOfficer::working_thread_body()
{
  boost::mutex mtx;
  boost::unique_lock<boost::mutex> lock(mtx);
  while(true)
  {
    cv_.wait(lock);
    ROS_INFO("PoliceOfficer working_thread_body wake up");
    if(!ros::ok())
    {
      ROS_INFO("PoliceOfficer working_thread_body leave");
      break;
    }

    int numThief = FindThief();

    first_package::OrderResult orderResult;
    orderResult.floor = currentFloor_;
    orderResult.numThief = numThief;
    if (numThief >= 0)
    {
      ROS_INFO("Policeman setSucceeded");
      as_.setSucceeded(orderResult);
    }
    else
    {
      ROS_INFO("Policeman setAborted");
      as_.setAborted(orderResult);
    }
  }
}
void PoliceOfficer::GetOrder(int floor)
{
  targetFloor_ = floor;
}
int PoliceOfficer::FindThief()
{
  int numThief = -1;
  while (currentFloor_ != targetFloor_)
  {
    if (!as_.isActive())
    {
      ROS_INFO("FindThief found action server not active, break");
      break;
    }
    go_toward_floor(targetFloor_);
  }
  if (currentFloor_ == targetFloor_)
  {
    //In this demo, 2 thief in 4-th floor
    numThief = currentFloor_ == 4 ? 2 : 0;
  }
  return numThief;
}
void PoliceOfficer::go_toward_floor(int floor)
{
  if (floor != currentFloor_)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //update floor
    if (floor > currentFloor_)
      ++currentFloor_;
    else if (floor < currentFloor_)
      --currentFloor_;

    //send feedback
    ROS_INFO("PoliceOfficer current floor: %d", currentFloor_);
    first_package::OrderFeedback feedback;
    feedback.currentFloor = currentFloor_;
    as_.publishFeedback(feedback);
  }
}

