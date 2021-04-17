#include <SDL2/SDL.h> /* Video feature */
#include <iostream>
#include <unistd.h> //usleep

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

static volatile bool gShutDown = false;
static SDL_Window * g_screen;
static SDL_Renderer * g_renderer;

static int monitor_sdl_open(void) {
  /*Initialize the SDL*/
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
    return -1;
  }

  g_screen = SDL_CreateWindow("Indication module simulator",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            0);
  
  if(!g_screen) {
    fprintf(stderr, "Could not create window\n");
    return -1;
  }

  /* all operation work over renderer*/
  g_renderer = SDL_CreateRenderer(g_screen, -1, SDL_RENDERER_SOFTWARE);
  if(!g_renderer) {
    fprintf(stderr, "Could not create renderer\n");
    return 1;
  }

  //SDL_RenderSetScale(g_renderer, 2.0f, 2.0f);

  /* clear screen at start to the black*/
  SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
  SDL_RenderClear(g_renderer);
  SDL_RenderPresent(g_renderer);

  return 0;
}

static int monitor_sdl_close(void) {
  SDL_DestroyWindow(g_screen);
  SDL_Quit();

  return 0;
}

static int sdl_event_poll(){
  SDL_Event event;

  if(SDL_PollEvent(&event)){
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
          monitor_sdl_close();
          gShutDown = true;
        break;
        default:
        break;
      }
    }
  }
  
  return 0;
}

int main(int argc, char * argv[])
{
  (void)argc;
  (void)argv;
  if(monitor_sdl_open() < 0){
    std::cout << "SDL couldn't open monitor!" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "SDL opened monitor!" << std::endl;

  while (!gShutDown){
    /* check buttons and send event to the uart*/
    sdl_event_poll();

    usleep(1000 * 5);
  }
  
  return EXIT_SUCCESS;
}