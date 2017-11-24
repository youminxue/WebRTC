#include "WebRTCSignal.h"

#include <functional>
#include <iostream>
#include <string>
#include <syscall.h>
#include <unistd.h>

int main()
{
  std::cout << "tid:" << syscall(SYS_gettid) << std::endl;
  WebRTCSignal rtcSignal;

  rtcSignal.connect("http://localhost:8080", [&](){
    std::string room = "foo";
    rtcSignal.create_join_room(room);
  });

  rtcSignal.set_message_listener([&](message::ptr const& msg){
  });

  while(1);
  return 0;
}
