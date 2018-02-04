#include "ros/ros.h"
#include <cstdlib>
#include <thread>
#include "first_package/Speak.h"
#include <actionlib/client/simple_action_client.h>
#include "first_package/OrderAction.h"

//speak
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
//use policeman, which has a main function body
int execute_policeman_simple(int floor)
{
  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<first_package::OrderAction> ac("PolicemanActionServer", true);

  // wait for the action server to start
  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  first_package::OrderGoal goal;
  goal.floor = floor;
  ac.sendGoal(goal);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    first_package::OrderResultConstPtr pResult = ac.getResult();

    ROS_INFO("Action finished: %s: %d thiefs in floor %d",state.toString().c_str(), pResult->numThief, pResult->floor);
  }
  else
  {
    ROS_INFO("Action did not finish before the time out.");
  }
  return 0;
}
// use policeofficer, also use other callbacks
// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}
// Called once when the goal completes
void doneCb(const actionlib::SimpleClientGoalState& state,
            const first_package::OrderResultConstPtr& pResult)
{
  ROS_INFO("done callback: %s: %d thiefs in floor %d",state.toString().c_str(), pResult->numThief, pResult->floor);
}
// Called every time feedback is received for the goal
void feedbackCb(const first_package::OrderFeedbackConstPtr& pFeedback)
{
  ROS_INFO("Got Feedback at floor %d", pFeedback->currentFloor);
}
int execute_policofficer_with_callback(int floor)
{
  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<first_package::OrderAction> ac("PoliceOfficerActionServer", true);

  // wait for the action server to start
  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action with callbacks
  first_package::OrderGoal goal;
  goal.floor = floor;
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    first_package::OrderResultConstPtr pResult = ac.getResult();

    ROS_INFO("Action finished: %s: %d thiefs in floor %d",state.toString().c_str(), pResult->numThief, pResult->floor);
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}

//what a mass, re-organize them
int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo_client");
  if (argc != 4)
  {
    ROS_INFO("usage for victim: demo_client 1 helloCount worldCount");
    ROS_INFO("usage for policeman: demo_client 2 floor1 floor2");
    ROS_INFO("usage for policeofficer: demo_client 3 floor1 floor2");
    return 1;
  }
  int mode = std::atoi(argv[1]);
  if (mode == 1)
  {
    std::thread th1([argv](){
      execute_speak(1, "hello", std::atoi(argv[2]));
    });
    std::thread th2([argv](){
      execute_speak(2, "world", std::atoi(argv[3]));
    });
    th1.join();
    th2.join();
  }
  else if (mode == 2)
  {
    std::thread th1([argv](){
      execute_policeman_simple(std::atoi(argv[2]));
    });
    std::thread th2([argv](){
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      execute_policeman_simple(std::atoi(argv[3]));
    });
    th1.join();
    th2.join();
  }
  else if (mode == 3)
  {
    std::thread th1([&argv](){
      execute_policofficer_with_callback(std::atoi(argv[2]));
    });
    std::thread th2([&argv](){
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      execute_policofficer_with_callback(std::atoi(argv[3]));
    });
    th1.join();
    th2.join();
  }
  return 0;
}
