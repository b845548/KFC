#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <assert.h>
#include <emscripten.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

enum { WIDTH = 16 };
enum { HEIGHT = 12 };
enum { ELMTSIZE = 34 };

enum { EMPTY, WALL, GEM, NENT };

int level1[HEIGHT][WIDTH] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
    SDL_Surface* img_enemy;
    Entity heroe;
    Entity enemy;
    Entity enemy2;
    SDL_Surface* img_enemy2;
Entity enemy3;
    SDL_Surface* img_enemy3;
Entity enemy4;
    SDL_Surface* img_enemy4;
Entity enemy5;
    SDL_Surface* img_enemy5;
	Entity enemy6;
    SDL_Surface* img_enemy6;
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
  data->img_gem = load("./objectif2.png"); // relative path

  data->img_heroe = load("./mario_droite.gif"); // relative path
  data->heroe.x = 0;
  data->heroe.y = 0;

  data->img_enemy = load("./3.jpg"); // relative path
  data->enemy.x = 10;
  data->enemy.y = 10;

  data->img_enemy2 = load("./objectif.jpg"); // relative path
  data->enemy2.x = 12;
  data->enemy2.y = 6;
  data->img_enemy3 = load("./objectif.jpg"); // relative path
  data->enemy3.x = 12;
  data->enemy3.y = 7;
  data->img_enemy4 = load("./objectif.jpg"); // relative path
  data->enemy4.x = 12;
  data->enemy4.y = 8;
  data->img_enemy5 = load("./objectif.jpg"); // relative path
  data->enemy5.x = 12;
  data->enemy5.y = 9;
  data->img_enemy6 = load("./objectif.jpg"); // relative path
  data->enemy6.x = 5;
  data->enemy6.y = 5;
  for(x = 0; x < WIDTH; x++) {
    for(y = 0; y < HEIGHT; y++) {
      data->board[y][x] = level1[y][x]; 
    }
  }
  for(x = 0; x < NENT; x++)
    data->ent_count[x] = 0;

  add_gems(data, 10);

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

int valid_moves(Data* data, Entity* e, int* vm) {
  int dir;
  int c = 0;
  for(dir = 0; dir < NDIR; dir++) {
     int x = e->x + coor[dir].x;
     int y = e->y + coor[dir].y;
     if(valid_pos(data, x, y)) {
       vm[c] = dir;
       c++;
     }
  }
  return c;
}

float euclid_dist(Entity* e, int x, int y) {
  return sqrtf(powf(x - e->x, 2) + powf(y - e->y, 2)); 
}

int euristic_move(Data* data, Entity* e) {
  int vm[4];
  int c = valid_moves(data, e, vm);
  if(c == 0)
    return -1;
  int i;
  int best = -1;
  float dist_mini = HEIGHT * WIDTH;
  for(i = 0; i < c; i++) {
    int dir = vm[i];
    int x = e->x + coor[dir].x;
    int y = e->y + coor[dir].y;
    float ed = euclid_dist(&data->heroe, x, y);
    if(ed < dist_mini) {
      dist_mini = ed;
      best = dir;
    }
  }
  return best;
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
  
  position.x = data->enemy.x * ELMTSIZE;
  position.y = data->enemy.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy, NULL, data->img_screen, &position);
  position.x = data->enemy2.x * ELMTSIZE;
  position.y = data->enemy2.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy2, NULL, data->img_screen, &position);

  position.x = data->enemy3.x * ELMTSIZE;
  position.y = data->enemy3.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy3, NULL, data->img_screen, &position);

  position.x = data->enemy4.x * ELMTSIZE;
  position.y = data->enemy4.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy4, NULL, data->img_screen, &position);

  position.x = data->enemy5.x * ELMTSIZE;
  position.y = data->enemy5.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy5, NULL, data->img_screen, &position);

  position.x = data->enemy6.x * ELMTSIZE;
  position.y = data->enemy6.y * ELMTSIZE;
  SDL_BlitSurface (data->img_enemy6, NULL, data->img_screen, &position);
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
       //move_entity(data, &data->enemy, random_move(data, &data->enemy));
       move_entity(data, &data->enemy, euristic_move(data, &data->enemy));
       move_entity(data, &data->enemy2, euristic_move(data, &data->enemy2));
       move_entity(data, &data->enemy3, euristic_move(data, &data->enemy3));
       move_entity(data, &data->enemy4, euristic_move(data, &data->enemy4));
       move_entity(data, &data->enemy5, euristic_move(data, &data->enemy5));
       move_entity(data, &data->enemy6, euristic_move(data, &data->enemy6));
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
  SDL_Flip(data->img_screen);

  printf("you should see an image.\n");

  emscripten_set_main_loop_arg(renderloop, data, 0, 0);

  return 0;
}

