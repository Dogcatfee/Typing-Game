#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h>
#include <time.h>

#include "gamelib.h"
#include <string.h>

using namespace std;

void render(void *data) {
  game_t *game = (game_t*)data;
  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  SDL_RenderClear(game->renderer);

  char buffer[256] = {0};
  if(game->to_type == game->input) {
    sprintf(buffer, "Done, took %d seconds.", game->time);
    draw_text(
              game->renderer,
              {.x=10,.y=10,.w=strlen(buffer)*18,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);
  }
  else {
    sprintf(buffer, "type this: %s", game->to_type.c_str());
    draw_text(
              game->renderer,
              {.x=10,.y=10,.w=strlen(buffer)*18,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);
    sprintf(buffer, "input: %s", game->input.c_str());
    draw_text(
              game->renderer,
              {.x=10,.y=80,.w=strlen(buffer)*18,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);

  }
  SDL_RenderPresent(game->renderer);
}

unsigned int logic(unsigned int t, void *data) {
  game_t *game = (game_t*)data;
  if(game->to_type == game->input && game->game_state == GAME_STATE_BEGIN) {
    game->time = SDL_GetTicks()/1000;
    game->game_state = GAME_STATE_DONE;
  }
  // queue render event
  if(game->tick_counter%15==0) {
    SDL_UserEvent p_userevent;
    p_userevent.type = SDL_USEREVENT;
    p_userevent.code = 0;
    p_userevent.data1 = (void*)render;
    p_userevent.data2 = data;

    SDL_Event p_event;
    p_event.type = SDL_USEREVENT;
    p_event.user = p_userevent;

    SDL_PushEvent(&p_event);
  }
  game->tick_counter ++;
  return t;
}
int main(int argc, char* args[]) {
  game_main();
  return 0;
}
