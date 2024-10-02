#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include <map>

using namespace std;

class Equipment
{ private:
	
	int xLocation;
	int yLocation;
	int size;
	int health;
	string name;
	
  public:
  	
  	Equipment(string initName, int initX, int initY, int initSize, int initHealth){
  		name = initName;
  		xLocation = initX;
  		yLocation = initY;
  		size = initSize;
  		health = initHealth;
  	}
  	
  	int GetX(){
  		return xLocation;
  	}
  	int GetY(){
  		return yLocation;
  	}
  	int GetSize(){
  		return size;
  	}
  	int GetRad(){ //Get radius
  		return (size - 1) / 2;
  	}
  	int GetHealth(){
  		return health;
  	}
  	string GetName(){
  		return name;
  	}
  	
  	void Hit(){
  		health--;
  	}
	bool isHit(int x, int y){
		int r = (size - 1)/2;                           //radius
		bool returnBool = true;
		if (x > xLocation + r) {returnBool = false;}           //may be more efficient to subract x from xLocation and check if the absolute value is above r
		else if (x < xLocation - r) {returnBool = false;}
		else if (y > yLocation + r) {returnBool = false;}
		else if (y < yLocation - r) {returnBool = false;}
		return returnBool;
	}
	bool isAlive(){
		if (health <= 0) { return false;}
		return true;
	}
};
