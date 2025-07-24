#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include "fVector3.h"
#include "fVectors.h"
#include "Triangle.h"
#include "Quaternion.h"
#include "Triangle3D.h"
#include <list>
#include "Object.h"

//global variables
const float PI = 3.1415926535897;
const float DEGTORAD = 360 / (2 * PI);
int renderBuffer[256][256]; //std::cout << renderBuffer[x][y];
float zBuffer[256*256];
uint8_t* textureBytes;
int pitch; //variable to store image width in bytes, not pixels (important for editing image)
void fRenderVector(fVector2);
void iRenderTriangle(Triangle2D);
void RenderTriangle3D(Triangle3D);
void RenderObj(Object object);
bool InTriangle(fVector2, Triangle2D);
float AngleOf(fVector3);
float cameraRad = 1; //camera radius (camera is square, so this is half of one side length)
float focalLength = 1;
float degrees = 0;

int main(int argc, char **argv)
{
	//inits
	int color[4] = {255, 0, 255, 0};
	int* colRef = color;
	Triangle3D myTri1 = Triangle3D(fVector3(1, 1, 1), fVector3(-1, -1, 1), fVector3(-1, 1, 1), colRef);
	Triangle3D myTri2 = Triangle3D(fVector3(1, 1, 1), fVector3(-1, -1, 1), fVector3(1, -1, 1), colRef);
	Triangle3D myTri3 = Triangle3D(fVector3(1, 1, -1), fVector3(-1, -1, -1), fVector3(-1, 1, -1), colRef);
	Triangle3D myTri4 = Triangle3D(fVector3(1, 1, -1), fVector3(-1, -1, -1), fVector3(1, -1, -1), colRef);
	int color2[4] = {255, 0, 200, 0};
	colRef = color2;
	Triangle3D myTri5 = Triangle3D(fVector3(-1, 1, 1), fVector3(-1, 1, -1), fVector3(-1, -1, -1), colRef);
	Triangle3D myTri6 = Triangle3D(fVector3(-1, 1, 1), fVector3(-1, -1, 1), fVector3(-1, -1, -1), colRef);
	Triangle3D myTri7 = Triangle3D(fVector3(1, 1, 1), fVector3(1, 1, -1), fVector3(1, -1, -1), colRef);
	Triangle3D myTri8 = Triangle3D(fVector3(1, 1, 1), fVector3(1, -1, 1), fVector3(1, -1, -1), colRef);
	int color3[4] = {255, 0, 0150, 0};
	colRef = color3;
	Triangle3D myTri9 = Triangle3D(fVector3(1, -1, 1), fVector3(-1, -1, 1), fVector3(-1, -1, -1), colRef);
	Triangle3D myTri10 = Triangle3D(fVector3(1, -1, 1), fVector3(1, -1, -1), fVector3(-1, -1, -1), colRef);
	Triangle3D myTri11 = Triangle3D(fVector3(1, 1, 1), fVector3(-1, 1, 1), fVector3(-1, 1, -1), colRef);
	Triangle3D myTri12 = Triangle3D(fVector3(1, 1, 1), fVector3(1, 1, -1), fVector3(-1, 1, -1), colRef);
	
	std::list<Triangle3D> myList = {};
	myList.push_front(myTri1);myList.push_front(myTri2);myList.push_front(myTri3);
	myList.push_front(myTri4);myList.push_front(myTri5);myList.push_front(myTri6);
	myList.push_front(myTri7);myList.push_front(myTri8);myList.push_front(myTri9);
	myList.push_front(myTri10);myList.push_front(myTri11);myList.push_front(myTri12);
	
	Object obj = Object(fVector3(0, 0, 7), fVector3(1, 1, 1), Quaternion(1, 0, 0, 0), myList);
	
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(720, 720, 0, &window, &renderer);
    
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
    		{25, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
		},
		{
    		{695, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 0.0f}/*texture position*/
		},
		{
    		{695, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
		},
		{
    		{25, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
		},
		{
    		{695, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
		},
		{
    		{25, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 1.0f}/*texture position*/
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
		
		for(int i = 0; i < 256 * 256; i++){
    		zBuffer[i] = INFINITY;
		}	
		
		//Render Triangle
		//myTri.Rotate(PI / 64, fVector3(0, 1, 0));
		//RenderTriangle3D(myTri);
		Quaternion qRotation = Quaternion(std::cos(degrees/2), std::sin(degrees/2) * .707, std::sin(degrees/2) * .707, std::sin(degrees/2) * 0);
		degrees += 0.1;
		obj.SetRotation(qRotation);
		RenderObj(obj);
		//myTri.Displace(fVector3(1, 1));
		//myTri.Rotate(0.01);
		//End Render Triangle
		
		
		SDL_UnlockTexture(texture);
	    
	    SDL_Vertex QuadVerts[] =
		{
			{
				{25, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
			},
			{
				{695, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 0.0f}/*texture position*/
			},
			{
				{695, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
			},
			{
				{25, 25},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 0.0f}/*texture position*/
			},
			{
				{695, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {1.0f, 1.0f}/*texture position*/
			},
			{
				{25, 695},/*location*/ {255, 255, 255, 255}, /*color*/ {0.0f, 1.0f}/*texture position*/
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
		float xCoord = pixelX - 127;
		float yCoord = pixelY - 127;
		if(InTriangle(fVector2(xCoord * (2 * cameraRad / 256), yCoord * (2 * cameraRad / 256)), tri)){ //check if in bounds of triangle
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

void RenderTriangle3D(Triangle3D tri){ //I don't know how to z buffer with this :(
	
	Triangle3D transformedTri = tri;
	
	//rotation based on camera TBI
	
	int color[4] = {tri.GetColor(0), tri.GetColor(1), tri.GetColor(2), tri.GetColor(3)};
	int* colRef = color;
	
	fVector2 newA = fVector2((focalLength * transformedTri.GetA().GetX()) / transformedTri.GetA().GetZ(), (focalLength * transformedTri.GetA().GetY()) / transformedTri.GetA().GetZ());
	fVector2 newB = fVector2((focalLength * transformedTri.GetB().GetX()) / transformedTri.GetB().GetZ(), (focalLength * transformedTri.GetB().GetY()) / transformedTri.GetB().GetZ());
	fVector2 newC = fVector2((focalLength * transformedTri.GetC().GetX()) / transformedTri.GetC().GetZ(), (focalLength * transformedTri.GetC().GetY()) / transformedTri.GetC().GetZ());
	
	Triangle2D tri2D = Triangle2D(newA, newB, newC, colRef);
	//tri2D.Scale(fVector2(100, 100));
	
	fVector3 vN = transformedTri.GetNormal();
	fVector3 vP = transformedTri.GetA();
	
	//std::cout << tri.ToStr() << "\n";
	for(int i = 0; i < (pitch / 4) * 256; i++) {
		int pixelX = i % 256;
		int pixelY = 255 - (i / 256);
		float xCoord = pixelX - 127;
		float yCoord = pixelY - 127;
		if(InTriangle(fVector2(xCoord * (2 * cameraRad / 256), yCoord * (2 * cameraRad / 256)), tri2D)){ //check if in bounds of triangle
			fVector3 vD = fVector3(xCoord * (2.0 * cameraRad / 256.0), yCoord * (2.0 * cameraRad / 256.0), focalLength);
			//std::cout << vN.ToStr() << "\n";
			float PdotN = (vP.GetX() * vN.GetX()) + (vP.GetY() * vN.GetY()) + (vP.GetZ() * vN.GetZ());
			float DdotN = (vD.GetX() * vN.GetX()) + (vD.GetY() * vN.GetY()) + (vD.GetZ() * vN.GetZ());
			float zValue = std::abs(PdotN / DdotN);
			//std::cout << zValue << "\n";
			if (zValue < zBuffer[i]){
				zBuffer[i] = zValue;
				//std::cout << i << ": " << zBuffer[37484] << ",\n";
				//std::cout << vN.ToStr() << "\n";
				textureBytes[4 * i] = tri2D.GetColor(0);
				textureBytes[4 * i + 1] = tri2D.GetColor(1);
				textureBytes[4 * i + 2] = tri2D.GetColor(2);
				textureBytes[4 * i + 3] = tri2D.GetColor(3);
			}
		}
	}		
	
	
}

void RenderObj(Object object){
	
	for (Triangle3D renderTri : object.GetMesh()){
		renderTri.Displace(object.GetLocation());
		renderTri.Scale(object.GetScale());
		renderTri.RotateQuat(object.GetRotation(), object.GetLocation());
		//std::cout << renderTri.ToStr() << "\n";
		RenderTriangle3D(renderTri);
	}
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

float AngleOf(fVector3 angleVector){ //returns the angle between x axis and the given vector
	fVector3 xVector = fVector3(1, 0, 0);
	float dotProduct = xVector.GetX() * angleVector.GetX(); //no point in doing the other parts since the 
	float magnitude = std::sqrt(angleVector.GetX() * angleVector.GetX() + angleVector.GetY() * angleVector.GetY() + angleVector.GetZ() * angleVector.GetZ()); //only need angle magnitude since xVector magnitude is 1
	return std::acos(dotProduct / magnitude);
}
