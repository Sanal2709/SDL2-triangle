#include "sdl2_util.h"
#include "cmath"
#include <vector>

static SDL_Window * g_screen;
static SDL_Renderer * g_renderer;

extern volatile bool gShutDown;

std::vector<PixelInfo> border1;
std::vector<PixelInfo> border2;
std::vector<PixelInfo> border3;

int monitor_sdl_open(void) {
  /*Initialize the SDL*/
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
    return -1;
  }

  g_screen = SDL_CreateWindow("triangle",
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

int monitor_sdl_close(void) {
  SDL_DestroyWindow(g_screen);
  SDL_Quit();

  return 0;
}

int sdl_event_poll(){
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

void Draw_Pixel(PixelInfo pixel){
  SDL_SetRenderDrawColor(g_renderer, pixel.r, pixel.g, pixel.b, 255);
  SDL_RenderDrawPoint(g_renderer, pixel.x, pixel.y);
  SDL_RenderPresent(g_renderer);
}

void Draw_Line(PixelInfo pixel1, PixelInfo pixel2, std::vector<PixelInfo>& border){

  double t1 = ((float)(pixel1.y - pixel2.y));
  double t2 = ((float)(pixel1.x - pixel2.x));
  double k = t1 / t2;
  double b = (double)pixel1.y - k * pixel1.x;

  int x_step = (pixel1.x - pixel2.x > 0)? -1 : 1;

  double x_delta = pixel1.x - pixel2.x;
  double y_delta = pixel1.y - pixel2.y;

  double all_distance = sqrt( x_delta*x_delta + y_delta*y_delta );
  double coeff = 0;

  int y = 0;
  for(double i = pixel1.x; i != pixel2.x; i += (double)x_step/2.0){
    y = k * (double)i + b;

    //interpolate colors
    double curr_x_delta = pixel1.x - i;
    double curr_y_delta = pixel1.y - y;
    double curr_distance = sqrt( curr_x_delta*curr_x_delta + curr_y_delta*curr_y_delta );
    coeff = 1 - curr_distance / all_distance;

    uint32_t r = coeff * pixel1.r + (1 - coeff) * pixel2.r;
    uint32_t g = coeff * pixel1.g + (1 - coeff) * pixel2.g;
    uint32_t b = coeff * pixel1.b + (1 - coeff) * pixel2.b;

    border.push_back(PixelInfo(i, y, r, g, b));

    SDL_SetRenderDrawColor(g_renderer, r, g, b, 255);
    SDL_RenderDrawPoint(g_renderer, i, y);
  }
  SDL_RenderPresent(g_renderer);
}

void Draw_Triangle(){
  PixelInfo up{320, 10, 255, 0, 0};
  PixelInfo left{10, 470, 0, 255, 0};
  PixelInfo right{630, 470, 0, 0, 255};

  Draw_Pixel(up);
  Draw_Pixel(left);
  Draw_Pixel(right);

  Draw_Line(up, left, border1);
  Draw_Line(up, right, border2);
  Draw_Line(right, left, border3);

  int vertical_size = std::min(border1.size(), border2.size());

  std::vector<PixelInfo> stub;
  for(int i = 0; i < vertical_size; i++){
    Draw_Line(border1[i], border2[i], stub);
  }
}