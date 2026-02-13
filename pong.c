#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
// #include <stdio.h>
#include <unistd.h>

typedef struct {
  int x, y;
} Position;

typedef struct {
  int center_x, center_y;
  int radius;
  Position initial;
  Position final;
} Circle;

Circle circle = {500, 400, 10};

typedef struct {
  int x, y;
  int w, h;
  bool hit; // player's hit value decides the players chance to move, it the
            // value is true, the player has hit the ball and its time for other
            // player to move
} Player;

Player player_one = {0, 350, 25, 150, false};
Player player_two = {975, 350, 25, 150, true};

int exchange = 240;

void swap_values(Circle *circle, Position *position) 
{

}

void change_player(Player *player_one, Player *player_two, int *exchange) 
{

  (*(exchange))--;
  if (*(exchange) == 0) 
  {
    player_one->hit = !player_one->hit;
    player_two->hit = !player_two->hit;
    *(exchange) = 240;
  }
}

void move_player(Player *player, int direction) 
{
  if (player->hit == false) 
  {
    if (direction == 1) 
    {
      player->y -= 1 * 10;
      if (player->y < 0) 
      {
        player->y = 0;
      }
    }

    if (direction == 0) 
    {
      player->y += 1 * 10;
      if (player->y > 650) 
      {
        player->y = 650;
      }
    }
  }
}

void draw_player(SDL_Surface *surface, Player *player, Uint32 color) 
{
  SDL_Rect rect;
  for (int x = player->x; x <= player->x + player->w; x++) 
  {
    for (int y = player->y; y <= player->y + player->h; y++) 
    {
      rect = (SDL_Rect){x, y, 1, 1};
      SDL_FillSurfaceRect(surface, &rect, color);
    }
  }
}

bool is_inside_circle(Circle *circle, int x, int y) 
{
  float dx = x - circle->center_x;
  float dy = y - circle->center_y;
  return (dx * dx + dy * dy <= circle->radius * circle->radius);
}

void draw_circle(SDL_Surface *surface, Circle *circle, Uint32 color) 
{
  SDL_Rect rect;

  for (int x = circle->center_x - circle->radius;
       x <= circle->center_x + circle->radius; x++) {
    for (int y = circle->center_y - circle->radius;
         y <= circle->center_y + circle->radius; y++) {
      if (is_inside_circle(circle, x, y)) 
      {
        rect = (SDL_Rect){x, y, 1, 1};
        SDL_FillSurfaceRect(surface, &rect, color);
      }
    }
  }
}

int main(int argc, char *argv[]) 
{

  circle.initial = (Position){circle.center_x, circle.center_y};

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("Pong", 1000, 800, 0);
  SDL_Surface *surface = SDL_GetWindowSurface(window);

  const SDL_PixelFormatDetails *fmt =
      SDL_GetPixelFormatDetails(surface->format);
  Uint32 white = SDL_MapRGBA(fmt, NULL, 255, 255, 255, 255);

  int running = 1;

  while (running) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {

      if (event.type == SDL_EVENT_QUIT) 
      {
        running = 0;
      }
      if (event.type == SDL_EVENT_KEY_DOWN) 
      {
        int direction;
        if (event.key.scancode == SDL_SCANCODE_UP) 
        {
          direction = 1;
          move_player(&player_two, direction);
          move_player(&player_one, direction);
        }
        if (event.key.scancode == SDL_SCANCODE_DOWN) 
        {
          direction = 0;
          move_player(&player_two, direction);
          move_player(&player_one, direction);
        }
      }
    }

    SDL_FillSurfaceRect(surface, NULL, 0);
    draw_player(surface, &player_one, white);
    draw_player(surface, &player_two, white);
    draw_circle(surface, &circle, white);
    SDL_UpdateWindowSurface(window);
    change_player(&player_one, &player_two, &exchange);
    SDL_Delay(17);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
