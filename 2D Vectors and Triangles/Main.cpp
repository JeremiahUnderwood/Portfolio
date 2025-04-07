#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_render.h>
#include "fVectors.h"
#include "Triangle.h"

//global variables
int renderBuffer[256][256]; //std::cout << renderBuffer[x][y];
uint8_t* textureBytes;
int pitch; //variable to store image width in bytes, not pixels (important for editing image)
void fRenderVector(fVector2);
void iRenderTriangle(Triangle2D);
bool InTriangle(fVector2, Triangle2D);

int main(int argc, char **argv)
{
	//inits
	int color[4] = {255, 0, 255, 0};
	int* colRef = color;
	Triangle2D myTri = Triangle2D(fVector2(0, 20), fVector2(20, 0), fVector2(-20, 0), colRef);
	
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(900, 900, 0, &window, &renderer);
    
    ///// Create Texture /////
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 256);
	SDL_LockTexture(texture, NULL, (void**)&textureBytes, &pitch);
	
	for(int i = 0; i < (pitch / 4) * 256; i++) { //Translate buffer into texture
		int pixelX = i % 256;
		int pixelY = 255 - (i / 256);
		textureBytes[4 * i] = 255;
		textureBytes[4 * i + 1] = 255;
		textureBytes[4 * i + 2] = 255;
		textureBytes[4 * i + 3] = 255;
	}		
	SDL_UnlockTexture(texture);
	
    
    ///// Get Preexisting Image As Texture /////
    /*
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_Surface* image = SDL_LoadBMP("mecat.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    image = NULL;
	
    
    if (!texture){
    	std::cout << "image load failure\n";
	}
    */
    
    SDL_Vertex QuadVerts[] =
    {
    	{
    		{50, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
		},
		{
    		{850, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 0.0f}/*texture position*/
		},
		{
    		{850, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
		},
		{
    		{50, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
		},
		{
    		{850, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
		},
		{
    		{50, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 1.0f}/*texture position*/
		}
	};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_RenderGeometry(renderer, texture, QuadVerts, 6, NULL, 0);
    SDL_RenderPresent(renderer); 
    
	bool breakloop = false;
    while (!breakloop){
    	SDL_LockTexture(texture, NULL, (void**)&textureBytes, &pitch);
	
		for(int i = 0; i < (pitch / 4) * 256; i++) {
			int pixelX = i % 256;
			int pixelY = 255 - (i / 256);
			textureBytes[4 * i] = 255;
			textureBytes[4 * i + 1] = 255;
			textureBytes[4 * i + 2] = 255;
			textureBytes[4 * i + 3] = 255;

		}		
		
		//Render Triangle
		iRenderTriangle(myTri);
		myTri.Displace(fVector2(1, 1));
		//myTri.Rotate(0.01);
		//End Render Triangle
		
		
		SDL_UnlockTexture(texture);
	    
	    SDL_Vertex QuadVerts[] =
	    {
	    	{
	    		{50, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
			},
			{
	    		{850, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 0.0f}/*texture position*/
			},
			{
	    		{850, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
			},
			{
	    		{50, 50},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
			},
			{
	    		{850, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
			},
			{
	    		{50, 850},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 1.0f}/*texture position*/
			}
		};
	
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    SDL_RenderClear(renderer);
	    
	    SDL_RenderGeometry(renderer, texture, QuadVerts, 6, NULL, 0);
	    SDL_RenderPresent(renderer); 
    	
	}
    
    //On program exit
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;    
}



//Render Vector Functions

void fRenderVector(fVector2 rVector){ //rasterises vectors
	float absX = std::fabs(rVector.GetX());
	float absY = std::fabs(rVector.GetY());
	int xDir = rVector.GetX() / absX;
	int yDir = rVector.GetY() / absY;
	int j = 0;
	
	if (absX > absY){
		float slope = rVector.GetY()/rVector.GetX();
		for(int i = 0; i < (int)absX; i++){

			j = slope * i;
			if (std::abs(i) < 127 && std::abs(j) < 127){
				renderBuffer[127 + (i * xDir)][127 + (j * xDir)] = 1; // multiply j by the x direction to cancel out its influence on the slope addition.
			}
		}
	}
	else{
		float slope = rVector.GetX()/rVector.GetY();
		for(int i = 0; i < (int)absY; i++){
			j = slope * i;
			if (std::abs(i) < 127 && std::abs(j) < 127){
				renderBuffer[127 + (j * yDir)][127 + (i * yDir)] = 1; //see above but coordinates reversed
			}
		}
	}
}

void iRenderTriangle(Triangle2D tri){ //rasterises triangles

	for(int i = 0; i < (pitch / 4) * 256; i++) {
		int pixelX = i % 256;
		int pixelY = 255 - (i / 256);
		int xCoord = pixelX - 127;
		int yCoord = pixelY - 127;
		if(InTriangle(fVector2(xCoord, yCoord), tri)){ //check if in bounds of triangle
			textureBytes[4 * i] = tri.GetColor(0);
			textureBytes[4 * i + 1] = tri.GetColor(1);
			textureBytes[4 * i + 2] = tri.GetColor(2);
			textureBytes[4 * i + 3] = tri.GetColor(3);
		}
	}		
	/*
	for(int i = 0; i < 256; i++) { //check i coordinates of array
		for(int j = 0; j < 256; j++) { //check j coordinates of array
			int xCoord = i - 127;
			int yCoord = j - 127;
			if(InTriangle(fVector2(xCoord, yCoord), tri)){ //check if in bounds of triangle
				renderBuffer[i][j] = 1;
			}
			else{
				renderBuffer[i][j] = 0;
			}
			
		} 
	}
	*/
}

//Quality of life
bool InTriangle(fVector2 P, Triangle2D tri){ //Returns true if P is inside the triangle tri
	
	//Define Vectors
	fVector2 AB = tri.GetB();
	AB.Add(fVector2(tri.GetA().GetX() * -1, tri.GetA().GetY() * -1));
	
	fVector2 BC = tri.GetC();
	BC.Add(fVector2(tri.GetB().GetX() * -1, tri.GetB().GetY() * -1));
	
	fVector2 CA = tri.GetA();
	CA.Add(fVector2(tri.GetC().GetX() * -1, tri.GetC().GetY() * -1));
	
	fVector2 AP = P;
	AP.Add(fVector2(tri.GetA().GetX() * -1, tri.GetA().GetY() * -1));
	
	fVector2 BP = P;
	BP.Add(fVector2(tri.GetB().GetX() * -1, tri.GetB().GetY() * -1));
	
	fVector2 CP = P;
	CP.Add(fVector2(tri.GetC().GetX() * -1, tri.GetC().GetY() * -1));
	
	//Get Cross Product 3rd values
	float crossA = (AB.GetX() * AP.GetY()) - (AP.GetX() * AB.GetY());
	float crossB = (BC.GetX() * BP.GetY()) - (BP.GetX() * BC.GetY());
	float crossC = (CA.GetX() * CP.GetY()) - (CP.GetX() * CA.GetY());
	
	//return
	if((crossA > 0 && crossB > 0 && crossC > 0) || (crossA < 0 && crossB < 0 && crossC < 0)){
		return true;
	}
	else{
		return false;
	}
}
