#include "DishWasher.h"
#include "first_package/Speak.h"

DishWasher::DishWasher()
  : node_(ros::NodeHandle())
{
  ROS_INFO("DishWasher constructor @ %s", node_.getNamespace().c_str());
}

void DishWasher::Start()
{
  service_ = node_.advertiseService<first_package::Speak::Request, first_package::Speak::Response>
    ("speak", 
        [this](first_package::Speak::Request &req, first_package::Speak::Response &res){
          res.num = Speak(req.msg, req.repeat);
          return true;
        }
    );
  ROS_INFO("DishWasher Start finished.");
}
void DishWasher::Stop()
{
  service_.shutdown();
}
int DishWasher::Speak(const std::string &msg, int repeat)
{
  for(int i=0; i<repeat; ++i)
  {
    ROS_INFO("speak %s", msg.c_str());
  }
  return msg.length() * repeat;
}  

