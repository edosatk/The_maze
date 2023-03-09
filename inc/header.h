#ifndef _HEADER_
#define _HEADER_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/*Marcos*/
#define SCREEN_WIDTH (COLUMNS*TILE_SIZE)*2
#define SCREEN_HEIGHT (ROWS*TILE_SIZE)
#define TILE_SIZE 64
#define ROWS 10
#define COLUMNS 10
#define PI 3.14159
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10
#define PLAYER_SPEED 50
#define PLAYER_TURN_SPEED 0.1
#define FOV_ANGLE 60
#define DR 0.0174533
#define PLAYER_SIZE 100

/*ray struct properties of the ray*/
typedef struct Ray{
    float x;
    float y;
    float xo;
    float yo;
    float vx;
    float vy;
    float hx;
    float hy;
    float disV;
    float disH;
    float disT;
}Ray;

extern int Floor[];
extern int Red_Brick[];
extern const int map[ROWS][COLUMNS];
extern float px;
extern float py;
extern float pa;
extern Ray ray;
extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern SDL_Renderer* renderer;
float degToRad(float a); 
float FixAng(float a);

/* Functions*/
bool Initialize(void);
void Update(Ray*ray);
void Shutdown(void);
void DrawMap();
void draw_player(SDL_Renderer *renderer, int x, int y);
int mapHasWallAt(float x, float y);
void Raycaster(Ray*ray);
void Horizontal_Raycast_Calculation( float ra, Ray*ray);
void Vertical_Raycast_Calculation( float ra, Ray*ray);
void Draw(int r,float ra, Ray*ray);

#endif /* _HEADER_ */

