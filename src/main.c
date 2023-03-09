#define SDL_MAIN_HANDLED
#include "header.h"

/**
*Global pointer variables declarion 
*/
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;
Ray ray;

/**
*Global declarion of map
*/
const int map[][10]=
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,1,0,0,0,0,0,1},
  {1,0,0,1,1,1,1,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,1},
  {1,0,0,0,0,0,2,0,0,1},
  {1,0,0,0,0,0,2,0,0,1},
  {1,0,2,2,2,2,2,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
  
};
float px=150;
float py=400;
float pa=90;

/**
*the main function of the maze project
*it also handles input from user
*/

int main(void)
{
	atexit(Shutdown);
  	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );

	if(!Initialize()){
		exit(1);
	}

	SDL_Event event;
	bool quit = false;
	bool is_left_down = false;
	bool is_right_down = false;
	bool is_up_down = false;
	bool is_down_down = false;
	float pdx,pdy;
	pdx=cos(degToRad(pa)); 
    pdy=-sin(degToRad(pa)); 
    float last_Tick;
    float Curt_Time;
    float Elapsed_Time;
    
    last_Tick = SDL_GetTicks();
    

while (!quit) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        is_left_down = true;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        is_right_down = true;
                        break;
                    case SDL_SCANCODE_UP:
                        is_up_down = true;
                        break;
                    case SDL_SCANCODE_DOWN:
                        is_down_down = true;
                        break;
                    default:
                        // Handle all other cases
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        is_left_down = false;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        is_right_down = false;
                        break;
                    case SDL_SCANCODE_UP:
                        is_up_down = false;
                        break;
                    case SDL_SCANCODE_DOWN:
                        is_down_down = false;
                        break;
                    default:
                        // Handle all other cases
                        break;
                }
                break;
        }
    }

    if (is_left_down ) {       
         pa+=0.1*Elapsed_Time; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));       
    }

    if (is_right_down ) {
        pa-=0.1*Elapsed_Time; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
    }

    if (is_up_down && !mapHasWallAt(px,py)) {
        px+=pdx*5; py+=pdy*5;
        if(mapHasWallAt(px,py)){px-=pdx*5; py-=pdy*5;}
    }

    if (is_down_down && !mapHasWallAt(px,py)) {
       px-=pdx*5; py-=pdy*5;
       if(mapHasWallAt(px,py)){px+=pdx*5; py+=pdy*5;}
    }
    Curt_Time = SDL_GetTicks();
    Elapsed_Time =(Curt_Time-last_Tick);
    last_Tick = Curt_Time;
    Update(&ray);
   
}
	
	
	//Quit SDL subsystems
	SDL_Quit();

	return 0;



}

/**
*Initialize function initializes SDL window
*/
bool Initialize(void){

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	//Create Window
	window = SDL_CreateWindow("the_maze",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if (!window){
		return false;
		}
	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!window){
		return false;
		}


	return true;

 	}

/**
*Update function updates the rendering
*@ray pointer to Struct which has info about the ray
*/    
void Update(Ray*ray){

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);	
	DrawMap();
	draw_player(renderer,px,py);
    Raycaster(ray);
 	SDL_RenderPresent(renderer);
	
		
  	}

/**
* Shutdown handles shuting down of SDL window
*/
void Shutdown(void){

	if(renderer){
		SDL_DestroyRenderer(renderer);
	}
	if(window){
		SDL_DestroyWindow(window);
	}	
	
	SDL_Quit();
	}

/**
* DrawMap function Draws 2D map 
*/
void DrawMap(){
    
	for (int x = 0; x < ROWS; x++) {
        for (int y = 0; y < COLUMNS; y++) {
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, (TILE_SIZE), (TILE_SIZE)};
            if (map[y][x] != 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
}
	SDL_SetRenderDrawColor(renderer, 90, 90, 90, SDL_ALPHA_OPAQUE);
	 // Vertical lines
    for (int i = 0; i <= COLUMNS; i++) {
        SDL_RenderDrawLine(renderer, i * TILE_SIZE, 0, i * TILE_SIZE, SCREEN_HEIGHT);
		
    }
    
    // Horizontal lines
    for (int i = 0; i <= ROWS; i++) {
        SDL_RenderDrawLine(renderer, 0, i * TILE_SIZE, COLUMNS*TILE_SIZE, i * TILE_SIZE);
    }

}

/**
*mapHasWallAt function checkes if there is a wall
*      at the passed location x,y cordinate
*/
int mapHasWallAt(float x, float y)
{
    int mapGridIndexX, mapGridIndexY;

    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        return (false);
    }
    mapGridIndexX = floor(x / TILE_SIZE);
    mapGridIndexY = floor(y / TILE_SIZE);
    return (map[mapGridIndexY][mapGridIndexX] != 0);
}

