#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <string>
#include "fVectors.h"
#include <cmath>
#include <iostream>


class Triangle2D{
	private:
		//Fundamental Variables
		fVector2 vertexA;
		fVector2 vertexB;
		fVector2 vertexC;
		int color[4]{};
		
	public:
		
		//Constructors
		Triangle2D(fVector2 vA, fVector2 vB, fVector2 vC);
		Triangle2D(fVector2 vA, fVector2 vB, fVector2 vC, int* col);
		
		//Getters/Setters
		fVector2 GetA();
		fVector2 GetB();
		fVector2 GetC();
		int GetColor(int ABGR);
		void SetA(fVector2 newA);
		void SetB(fVector2 newB);
		void SetC(fVector2 newC);
		
		//Quality of Life
		fVector2 Center();
		std::string ToStr();
		
		//Transform Functions
		void Displace(fVector2 displacement);
		void Transform(fVector2 iHat, fVector2 jHat);
		void Rotate(float rad);
		void Scale(fVector2 scale);
};
#endif
/* For reference
class fVector2{
	private:
		float x;
		float y;
	public:
		fVector2(float initX = 0, float initY = 0);
		
		//Getters and Setters//
		float GetX();
		float GetY();
		float SetX(float newX);
		float SetY(float newY);
		//End Getters and Setters//
		
		
		//Operations//
		void Add(fVector2 addend);
		void Multiply(float numerator, float denominator = 1);
		void Transform(fVector2 iHat, fVector2 jHat);
		//End Operations//
		
		
		//Quality of Life//
		std::string ToStr();
		//End Quality of Life//
	
};
#endif
*/
