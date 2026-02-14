//#define _USE_MATH_DEFINES
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define PI 3.14159265358979323846

typedef struct {
    int center_x, center_y;
    int radius;
    double vx, vy;
} Circle;

typedef struct {
    int x, y;
    int w, h;
    bool hit;
} Player;

Circle ball = {500, 400, 10, 0, 0};
Player player_one = {0, 325, 25, 150, false};   
Player player_two = {975, 325, 25, 150, true};  

double speed = 5.0;

int init_ball_direction(Circle *circle)
{
    double min_angle = -30.0;
    double max_angle = 30.0;
    double angle_deg = min_angle + (double)rand() / RAND_MAX * (max_angle - min_angle);
    double angle_rad = angle_deg * PI / 180.0;
    int direction = (rand() % 2 == 0) ? -1 : 1; 

    circle->vx = direction * speed * cos(angle_rad);
    circle->vy = speed * sin(angle_rad);

    return direction;
}

bool circle_rect_collision(Circle *circle, Player *player)
{
    int closestX = circle->center_x;
    int closestY = circle->center_y;

    if (circle->center_x < player->x) closestX = player->x;
    else if (circle->center_x > player->x + player->w) closestX = player->x + player->w;

    if (circle->center_y < player->y) closestY = player->y;
    else if (circle->center_y > player->y + player->h) closestY = player->y + player->h;

    int dx = circle->center_x - closestX;
    int dy = circle->center_y - closestY;

    return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

bool is_inside_circle(Circle *circle, int x, int y)
{
    int dx = x - circle->center_x;
    int dy = y - circle->center_y;
    return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

void draw_circle(SDL_Surface *surface, Circle *circle, Uint32 color)
{
    SDL_Rect rect;
    for (int x = circle->center_x - circle->radius; x <= circle->center_x + circle->radius; x++)
    {
        for (int y = circle->center_y - circle->radius; y <= circle->center_y + circle->radius; y++)
        {
            if (is_inside_circle(circle, x, y))
            {
                rect = (SDL_Rect){x, y, 1, 1};
                SDL_FillSurfaceRect(surface, &rect, color);
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

void move_player(Player *player, int direction)
{
    if (player->hit){
        return;
    } 

    if (direction == 1) 
    {
        player->y -= 10;
        if (player->y < 0) player->y = 0;
    }

    else if (direction == 0)
    {
        player->y += 10;
        if (player->y + player->h > WINDOW_HEIGHT){
            player->y = WINDOW_HEIGHT - player->h;
        }
    }
}

void update_ball(Circle *circle, Player *p1, Player *p2, bool *running)
{
    circle->center_x += circle->vx;
    circle->center_y += circle->vy;

    if (circle->center_y - circle->radius <= 0 || circle->center_y + circle->radius >= WINDOW_HEIGHT)
        circle->vy = -circle->vy;

    if (circle_rect_collision(circle, p1) && !p1->hit)
    {
        circle->vx = fabs(circle->vx);
        circle->center_x = p1->x + p1->w + circle->radius;
        p1->hit = true;  
        p2->hit = false;  
    }

    if (circle_rect_collision(circle, p2) && !p2->hit)
    {
        circle->vx = -fabs(circle->vx);
        circle->center_x = p2->x - circle->radius;
        p2->hit = true;  
        p1->hit = false; 
    }

    if (circle->center_x - circle->radius <= 0 || circle->center_x + circle->radius >= WINDOW_WIDTH)
    {
        printf("Game Over\n");
        *running = false;
    }
}

int main(void)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    const SDL_PixelFormatDetails *fmt = SDL_GetPixelFormatDetails(surface->format);
    Uint32 white = SDL_MapRGBA(fmt, NULL, 255, 255, 255, 255);

    int dir = init_ball_direction(&ball);
    if (dir < 0) 
    { 
        player_one.hit = false;
         player_two.hit = true; 
    }else { 
        player_one.hit = true; 
        player_two.hit = false;
    }

    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_UP)
                {
                    move_player(&player_one, 1);
                    move_player(&player_two, 1);
                }
                if (event.key.scancode == SDL_SCANCODE_DOWN)
                {
                    move_player(&player_one, 0);
                    move_player(&player_two, 0);
                }
            }
        }

        SDL_FillSurfaceRect(surface, NULL, 0); 
        draw_player(surface, &player_one, white);
        draw_player(surface, &player_two, white);
        update_ball(&ball, &player_one, &player_two, &running);
        draw_circle(surface, &ball, white);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}