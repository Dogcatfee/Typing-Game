#include "gamelib.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_surface.h>


using namespace std;

int p_first_run = 1;

/* Basic shape drawing functions go here
 */
void draw_rectangle(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color) {
  SDL_SetRenderDrawColor(p_renderer, color.r, color.g, color.b, 255);

  if (SDL_RenderDrawRect(p_renderer, &rect) < 0) {
    std::cerr << "Error drawing: " << SDL_GetError() << std::endl;
  }
}
void draw_text(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color, char*text) {
  TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/OpenSans-Regular.ttf", 18);
  if(font==NULL) {
    cout << "Font not found: " << TTF_GetError() << endl;
  }
  SDL_Texture *texture;

  SDL_Surface *surface;
  SDL_Color textColor = {color.r, color.g, color.b};

  surface = TTF_RenderText_Solid(font, text, textColor);
  texture = SDL_CreateTextureFromSurface(p_renderer, surface);

  SDL_RenderCopy(p_renderer, texture, NULL, &rect);

  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
  SDL_DestroyTexture(texture);
}
/*
 */
int collide(coord_t *a, coord_t *b) {
  int left_side = a->x < b->x;
  int right_side = a->x > b->x;
  int top = a->y < b->y;
  if(top) {
    if(left_side) {
      if(a->x+a->w > b->x)
        return 1;
    }
    else if(right_side) {
      if(a->x < b->x+b->w)
        return 1;
    }
  }
  return 0;
}
/* Initialization goes here
 */
void game_main() {
  SDL_Surface* screenSurface = NULL;
  SDL_Window *p_window;
  SDL_GLContext p_context;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    //return 1;
    return;
  }
  p_window = SDL_CreateWindow(
                              "hello_sdl2",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN
                              );
  if (p_window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    //return 1;
    return;
  }

  p_context = SDL_GL_GetCurrentContext();
  SDL_GL_MakeCurrent(p_window, NULL);

  SDL_Renderer *p_renderer = NULL;
  if(p_first_run) {
    TTF_Init();
    SDL_GL_MakeCurrent(p_window, p_context);
    p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);
    p_first_run = 0;
  }

  game_t game;
  game.game_state = GAME_STATE_BEGIN;
  game.to_type = "this is a sentence";
  game.input = "";
  game.renderer = p_renderer;

  SDL_AddTimer(1, logic, &game);

  int should_run = 1;
  while(should_run) {
    SDL_Event event;
    //    while(SDL_PollEvent(&event)) {
    while(SDL_WaitEvent(&event)) {
      /* Handle keyboard input
       */
      switch(event.type){
      case SDL_TEXTINPUT:
        cout << "TEXT INPUT " << event.text.text << endl;
        game.input+=event.text.text;
        break;
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_BACKSPACE) {
          game.input.pop_back();
        }
        break;
      case SDL_KEYUP:
        if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT) {
        }
        break;
        /* Handle calls to render
         */
      case SDL_USEREVENT:
        {
          // if(event.user.code == GAME_END) {
          //   should_run = 0;
          // } else {
            unsigned int(*func)(void*) = (unsigned int(*)(void*))(event.user.data1);
            func(event.user.data2);
            //}
          break;
        }
      default:
        break;
      }
      /* If escape key released, end the program
       */
      if(event.key.type == SDL_KEYUP){
        if(event.key.keysym.sym == SDLK_ESCAPE) {
          should_run = 0;
        }
        break;
      }
    }
  }
  TTF_Quit();
  SDL_DestroyWindow(p_window);
  SDL_Quit();
}
