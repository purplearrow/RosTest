#include "DishWasher.h"
#include "first_package/Speak.h"
#include <thread>

DishWasher::DishWasher()
  : node_(), callbackQueue_(true), asyncSpinner_(NUM_THREADS, &callbackQueue_)
{
  ROS_INFO("DishWasher constructor, namespace = %s", node_.getNamespace().c_str());
  node_.setCallbackQueue(&callbackQueue_);
  asyncSpinner_.start();
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
}
void DishWasher::Stop()
{
  service_.shutdown();
}
int DishWasher::Speak(const std::string &msg, int repeat)
{
  for(int i=0; i<repeat; ++i)
  {
    ROS_INFO("speak %s, %d", msg.c_str(), i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return msg.length() * repeat;
}  

