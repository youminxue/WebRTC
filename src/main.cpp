#include "WebRTCSignal.h"

#include <functional>
#include <iostream>
#include <string>
#include <syscall.h>
#include <unistd.h>

#include "SDL.h"

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

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_Window *window = SDL_CreateWindow("WebRTC", 10, 10, 640, 480, SDL_WINDOW_OPENGL);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
  SDL_UpdateWindowSurface(window);

  SDL_Event event;
  bool runing = true;
  while(runing){
    SDL_WaitEvent(&event);
    switch(event.type) {
      case SDL_KEYDOWN:
        printf("keyboard:%s\n", SDL_GetKeyName(event.key.keysym.sym));
        break;
      case SDL_QUIT:
        runing = false;
        break;
      default:
        //printf("event %x\n", event.type);
        break;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
