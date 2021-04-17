#include "sdl2_util.h"
#include <iostream>
#include <unistd.h> //usleep

volatile bool gShutDown = false;


int main(int argc, char * argv[])
{
  (void)argc;
  (void)argv;
  if(monitor_sdl_open() < 0){
    std::cout << "SDL couldn't open monitor!" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "SDL opened monitor!" << std::endl;

  Draw_Triangle();

  while (!gShutDown){
    /* check buttons and send event to the uart*/
    sdl_event_poll();

    usleep(1000 * 5);
  }
  
  return EXIT_SUCCESS;
}