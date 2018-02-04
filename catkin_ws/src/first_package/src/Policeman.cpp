#include "Policeman.h"
#include <thread>

Policeman::Policeman()
  : node_(), callbackQueue_(true), asyncSpinner_(NUM_THREADS, &callbackQueue_),
    as_(node_, "PolicemanActionServer", 
        [this](const first_package::OrderGoalConstPtr &goal)
        {
          executeCB(goal);
        },
        false), //auto start is false
    currentFloor_(1), targetFloor_(1)
{
  ROS_INFO("Policeman constructor, namespace = %s", node_.getNamespace().c_str());
  node_.setCallbackQueue(&callbackQueue_);
  asyncSpinner_.start();

//  the following two lines will not work. At run time, it will show error because executeCB is registered
//  as_.registerGoalCallback([this](){goalCB();});
//  as_.registerPreemptCallback([this](){preemptCB();});
  as_.start();
}
void Policeman::executeCB(const first_package::OrderGoalConstPtr &goal)
{
  ROS_INFO("Policeman executeCB");
  GetOrder(goal->floor);
  int numThief = FindThief();
  if (numThief >= 0)
  {
    first_package::OrderResult orderResult;
    orderResult.floor = goal->floor;
    orderResult.numThief = numThief;
    as_.setSucceeded(orderResult);
  }
}
void Policeman::GetOrder(int floor)
{
  targetFloor_ = floor;
}
int Policeman::FindThief()
{
  int numThief = -1;
  while (currentFloor_ != targetFloor_)
  {
    if (as_.isPreemptRequested() || !ros::ok())
    {
      ROS_INFO("FindThief found action get preempt request, break");
      // set the action state to preempted
      as_.setPreempted();
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
void Policeman::go_toward_floor(int floor)
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
    ROS_INFO("Policeman current floor: %d", currentFloor_);
    first_package::OrderFeedback feedback;
    feedback.currentFloor = currentFloor_;
    as_.publishFeedback(feedback);
  }
}

