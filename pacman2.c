#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <assert.h>
#include <emscripten.h>
#include <unistd.h>
#include <stdlib.h>

enum { WIDTH = 16 };
enum { HEIGHT = 12 };
enum { ELMTSIZE = 34 };

enum { EMPTY, WALL, GEM, NENT };

int level1[HEIGHT][WIDTH] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

typedef struct {
  int x;
  int y;
} Entity;


enum {LEVEL1, GAMEOVER, NSCREEN};

typedef struct {
    int screen;
    SDL_Surface* img_screen;
    SDL_Surface* img_wall;
    SDL_Surface* img_heroe;
    SDL_Surface* img_gem;
    Entity heroe;
    int board[HEIGHT][WIDTH];
    int ent_count[NENT];
} Data;

enum { NORTH, SOUTH, EAST, WEST, NDIR };
typedef struct {
  int x;
  int y;
} Point;
Point coor[NDIR] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0} };

SDL_Surface* load(const char* fileName) {
  SDL_Surface *image = IMG_Load(fileName);
  if (!image)
  {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  return image;
}


int end_of_game(Data* data) {
  return data->ent_count[GEM] == 0;
}

void add_gems(Data* data, int nb) {
  int i;
  for(i = 0; i < nb; i++) {
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    if(data->board[y][x] == EMPTY) {
      data->board[y][x] = GEM;
    } else {
      i--;
    }
  }
  data->ent_count[GEM] = nb;
}

void init_data(Data* data) {
  int x, y;
  data->img_screen = SDL_SetVideoMode(WIDTH * ELMTSIZE, HEIGHT * ELMTSIZE, 32, SDL_ANYFORMAT);
  data->img_wall = load("./mur.jpg"); // relative path
  data->img_heroe = load("./mario_droite.png"); // relative path
  data->img_gem = load("./objectif.png"); // relative path
  data->heroe.x = 0;
  data->heroe.y = 0;
  for(x = 0; x < WIDTH; x++) {
    for(y = 0; y < HEIGHT; y++) {
      data->board[y][x] = level1[y][x]; 
    }
  }
  for(x = 0; x < NENT; x++)
    data->ent_count[x] = 0;

  add_gems(data, 6);

  data->screen = LEVEL1;
}

int valid_pos(Data* data, int x, int y) {
  return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] != WALL;
}

void move_entity(Data* data, Entity* e, int dir) {
  int x = e->x + coor[dir].x;
  int y = e->y + coor[dir].y;
  if(valid_pos(data, x, y)) {
    e->x = x;
    e->y = y;
  }
}

void board(Data* data) {
  int x, y;
  for(x = 0; x < WIDTH; x++) {
    for(y = 0; y < HEIGHT; y++) {
      SDL_Rect position;
      position.x = x * ELMTSIZE;
      position.y = y * ELMTSIZE;
      if(data->board[y][x] == WALL)
        SDL_BlitSurface (data->img_wall, NULL, data->img_screen, &position);
      else if(data->board[y][x] == GEM)
        SDL_BlitSurface (data->img_gem, NULL, data->img_screen, &position);
    }
  }
  SDL_Rect position;
  position.x = data->heroe.x * ELMTSIZE;
  position.y = data->heroe.y * ELMTSIZE;
  SDL_BlitSurface (data->img_heroe, NULL, data->img_screen, &position);
}

void get_gem(Data* data) {
  if(data->board[data->heroe.y][data->heroe.x] == GEM) {
    data->board[data->heroe.y][data->heroe.x] = EMPTY;
    data->ent_count[GEM]--;
  }
}

void loop_game_over(Data* data) {
  SDL_FillRect(data->img_screen, NULL, SDL_MapRGB(data->img_screen->format, 0, 0, 0));
}

void loop_level1(Data* data) {
  SDL_FillRect(data->img_screen, NULL, SDL_MapRGB(data->img_screen->format, 0, 0, 0));
  
  board(data);
  SDL_Flip(data->img_screen);
  

  SDL_Event event;
  if(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_RIGHT:
          SDL_LockSurface(data->img_screen);
          move_entity(data, &data->heroe, EAST);
          SDL_UnlockSurface(data->img_screen);
          break;
          case SDLK_LEFT:
          SDL_LockSurface(data->img_screen);
          move_entity(data, &data->heroe, WEST);
          SDL_UnlockSurface(data->img_screen);
          break;
          case SDLK_UP:
          SDL_LockSurface(data->img_screen);
          move_entity(data, &data->heroe, NORTH);
          SDL_UnlockSurface(data->img_screen);
          break;
          case SDLK_DOWN:
          SDL_LockSurface(data->img_screen);
          move_entity(data, &data->heroe, SOUTH);
          SDL_UnlockSurface(data->img_screen);
          break;
          default: break;
       }
       get_gem(data);
       if(end_of_game(data))
         data->screen = GAMEOVER;
       break;
       default: break;
    }
  }

  //SDL_Quit();
}

void renderloop(void* arg) {
  Data* data = arg;
  switch(data->screen) {
    case LEVEL1: 
      loop_level1(data);
      break;
    case GAMEOVER:
      loop_game_over(data);
      break;
    default: break;
  }
}
int main() {
  Data* data = malloc(sizeof *data);
  SDL_Init(SDL_INIT_VIDEO);

  init_data(data);
 // SDL_Flip(data->img_screen);

  printf("you should see an image.\n");

  emscripten_set_main_loop_arg(renderloop, data, 0, 0);

  return 0;
}

