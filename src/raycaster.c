#include "../inc/header.h"
#include "../inc/Textures/BrickFireRed.ppm"
#include "../inc/Textures/floor_texture.ppm"

/**
*degToRad function converts degree to radian value
*@ a angle in degree
*/
float degToRad(float a) { 
    return a*M_PI/180.0;
    }

/**
*FixAng function normalize the angle to fall below 360 degree
*@ a angle 
*/
float FixAng(float a){ 
    if(a>359)
        { a-=360;} 
    if(a<0)
        { a+=360;} 
    return a;
    }

/**
*draw_player function draws player on 2D map
*@SDL_Renderer *renderer pointer to the renderer wich is the window
*@x,y cordinates of player
*/
void draw_player(SDL_Renderer *renderer, int x, int y) {
    SDL_Rect player;

    // Set the position and size of the player
    player.x = x;
    player.y = y;
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGHT;

    // Set the player color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the player
    SDL_RenderFillRect(renderer, &player);
}

/**
*Vertical_Raycast_Calculation calculates vertical intersection on map grid
*@ra ray angle
*ray pointer to struct
*/
void Vertical_Raycast_Calculation( float ra, Ray* ray){
    int dof = 0;
    ray->disV = 100000;
    float Tan = tan(degToRad(ra));

    if (cos(degToRad(ra)) > 0.001) {
        ray->x = (((int)px >> 6) << 6) + 64;
        ray->y = (px - ray->x) * Tan + py;
        ray->xo = 64;
        ray->yo = -ray->xo * Tan;
    }
    else if (cos(degToRad(ra)) < -0.001) {
        ray->x = (((int)px >> 6) << 6) - 0.0001;
        ray->y = (px - ray->x) * Tan + py;
        ray->xo = -64;
        ray->yo = -ray->xo * Tan;
    }
    else {
        ray->x = px;
        ray->y = py;
        dof = 8;
    }

    while (dof < 8) {
        if (mapHasWallAt(ray->x, ray->y)) {
            dof = 8;
            ray->disV = cos(degToRad(ra)) * (ray->x - px) - sin(degToRad(ra)) * (ray->y - py);
        }
        else {
            ray->x += ray->xo;
            ray->y += ray->yo;
            dof += 1;
        }
    }
    ray->vx = ray->x;
    ray->vy = ray->y;
}

/**
*Horizontal_Raycast_Calculation calculates Horizontal intersection on map grid
*@ra ray angle
*ray pointer to struct
*/
void Horizontal_Raycast_Calculation( float ra, Ray*ray){
    int dof = 0;
    ray->disH = 100000;
    float Tan = 1.0 / tan(degToRad(ra));

    if (sin(degToRad(ra)) > 0.001) {
        ray->y = (((int)py >> 6) << 6) - 0.0001;
        ray->x = (py - ray->y) * Tan + px;
        ray->yo = -64;
        ray->xo = -ray->yo * Tan;
    }
    else if (sin(degToRad(ra)) < -0.001) {
        ray->y = (((int)py >> 6) << 6) + 64;
        ray->x = (py - ray->y) * Tan + px;
        ray->yo = 64;
        ray->xo = -ray->yo * Tan;
    }
    else {
        ray->x = px;
        ray->y = py;
        dof = 8;
    }

    while (dof < 8) {
        if (mapHasWallAt(ray->x, ray->y)) {
            dof = 8;
            ray->disH = cos(degToRad(ra)) * (ray->x - px) - sin(degToRad(ra)) * (ray->y - py);
        }
        else {
            ray->x += ray->xo;
            ray->y += ray->yo;
            dof += 1;
        }
    }
    ray->hx = ray->x;
    ray->hy = ray->y;
}

/**
*Draw draws 3D projection of the ray cast
*@r number of rays
*ra ray angle
*ray pointer to struct
*/
void Draw(int r,float ra, Ray*ray){

	if (ray->disV<ray->disH) { ray->x=ray->vx; ray->y=ray->vy;  ray->disT = ray->disV; SDL_SetRenderDrawColor(renderer, 100, 0, 0, SDL_ALPHA_OPAQUE);}
	if (ray->disH<ray->disV) { ray->x=ray->hx; ray->y=ray->hy;  ray->disT = ray->disH; SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);} 
    if(map[(int)(ray->y/TILE_SIZE)][(int)(ray->x/TILE_SIZE)] == 2){SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);}
    if(map[(int)(ray->y/TILE_SIZE)][(int)(ray->x/TILE_SIZE)] == 2 && ray->disV<ray->disH ){SDL_SetRenderDrawColor(renderer, 0, 100, 0, SDL_ALPHA_OPAQUE);}


	SDL_RenderDrawLine(renderer, px, py, ray->x, ray->y);

    int ca=FixAng(pa-ra); ray->disT=ray->disT*cos(degToRad(ca));                            //fix fisheye 
    float lineH = (float)(TILE_SIZE*SCREEN_HEIGHT) / ray->disT;if(lineH>640){ lineH=640;} //line height
    int lineOff = SCREEN_HEIGHT/2 - (int)lineH/2;            //line offset

    
     SDL_RenderDrawLine(renderer, (r)+640, lineOff, (r)+640,  lineH+lineOff);
     

    float ty_step=32.0/(float)lineH;
    float ty_off=0;
    float ty=ty_off*ty_step;
    float tx;
    if (ray->disV<ray->disH) {tx=(int) (ray->y/2.0) %32; if (ra>90 && ra<270) { tx=31-tx; } }
    else {tx=(int) (ray->x/2.0) %32; if (ra>180) { tx=31-tx; }}

    for (int y=0; y<lineH; y++){

    int pixel=((int) ty* 32+ (int) tx) *3;
    int red =Red_Brick[pixel+0];
    int green = Red_Brick [pixel+1];
    int blue =Red_Brick [pixel+2];
    if (ray->disV<ray->disH){SDL_SetRenderDrawColor(renderer, red/2, green/2, blue/2, SDL_ALPHA_OPAQUE);}
    else {SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);}
    SDL_RenderDrawLine(renderer, (r)+640, y+lineOff, (r)+640,  y+lineH+lineOff);
    ty+=ty_step;
    }

   
    //Draw Floor
    for(int y = lineH+lineOff;y<SCREEN_HEIGHT;y++ ){

        float dy=y-(SCREEN_HEIGHT/2.0), deg=degToRad(ra), raFix=cos(degToRad(FixAng(pa-ra)));
        tx=px/2 + cos(deg)*158*2*32/dy/raFix;
        ty=py/2 - sin(deg)*158*2*32/dy/raFix;
        int pixel=(((int)(ty)&31)*32 + ((int)(tx)&31))*3;
        int red   =Floor[pixel+0];
        int green =Floor[pixel+1];
        int blue  =Floor[pixel+2];

        SDL_SetRenderDrawColor(renderer,red, green, blue, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, (r)+640, y, (r)+640, SCREEN_HEIGHT);
 
    

    //Draw Ceiling

    
        SDL_SetRenderDrawColor(renderer, 0, 0, 220, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, (r)+640, SCREEN_HEIGHT-y, (r)+640, 0);


	} 
	
}

/**
* Raycaster function implements a raycasting algorithm to render a 3D scene
*@ray pointer to struct
*/
void Raycaster(Ray*ray){
    float ra;
    ra=FixAng(pa+30); 
    int r;                                                             //ray set back 30 degrees	
	for(r=0;r<680;r++)
	{

        Horizontal_Raycast_Calculation( ra, ray);
        Vertical_Raycast_Calculation( ra, ray);
        Draw(r, ra,ray);

        ra=FixAng(ra-0.1);
           
    }

}

