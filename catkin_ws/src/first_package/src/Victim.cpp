#include "Victim.h"
#include "first_package/Speak.h"
#include <thread>

Victim::Victim()
  : node_(), callbackQueue_(true), asyncSpinner_(NUM_THREADS, &callbackQueue_)
{
  ROS_INFO("Victim constructor, namespace = %s", node_.getNamespace().c_str());
  node_.setCallbackQueue(&callbackQueue_);
  asyncSpinner_.start();
}
void Victim::Start()
{
  service_ = node_.advertiseService<first_package::Speak::Request, first_package::Speak::Response>
    ("speak", 
        [this](first_package::Speak::Request &req, first_package::Speak::Response &res){
          res.num = Speak(req.msg, req.repeat);
          return true;
        }
    );
}
void Victim::Stop()
{
  service_.shutdown();
}
int Victim::Speak(const std::string &msg, int repeat)
{
  for(int i=0; i<repeat; ++i)
  {
    ROS_INFO("speak %s, %d", msg.c_str(), i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return msg.length() * repeat;
}  

