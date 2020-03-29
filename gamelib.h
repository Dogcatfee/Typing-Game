#ifndef GAMELIB_H
#define GAMELIB_H
#include <SDL2/SDL.h>
#include <string>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define N_MISSILES 32
#define GAME_END 1

using std::string;

enum GAME_STATE {
                 GAME_STATE_BEGIN,
                 GAME_STATE_DONE,
                 GAME_STATE_OVER,
};

typedef SDL_Rect coord_t;

typedef struct color_s {
  uint8_t r,g,b;
} color_t;

typedef struct control_s {
  uint8_t direction;
} control_t;

typedef struct game_s {
  SDL_Renderer *renderer;
  string input;
  string to_type;
  int game_state;
  int time;
  int tick_counter = 0;
} game_t;

enum CONTROL_DIRECTIONS {
                         CONTROL_DIR_NONE,
                         CONTROL_DIR_LEFT,
                         CONTROL_DIR_RIGHT,
                         CONTROL_DIR_UP,
                         CONTROL_DIR_DOWN,
};


void draw_rectangle(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color);
void render(void *data);
void game_main();
unsigned int logic(unsigned int t, void *data);
void draw_text(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color, char*text);
int collide(coord_t *a, coord_t *b);
#endif
