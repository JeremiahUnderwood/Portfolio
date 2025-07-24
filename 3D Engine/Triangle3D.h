#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H
#include <string>
#include "fVector3.h"
#include <cmath>
#include <iostream>
#include "Quaternion.h"


class Triangle3D{
	private:
		//Fundamental Variables
		fVector3 vertexA;
		fVector3 vertexB;
		fVector3 vertexC;
		int color[4]{};
		
	public:
		
		//Constructors
		Triangle3D(fVector3 vA, fVector3 vB, fVector3 vC);
		Triangle3D(fVector3 vA, fVector3 vB, fVector3 vC, int* col);
		
		//Getters/Setters
		fVector3 GetA();
		fVector3 GetB();
		fVector3 GetC();
		fVector3 GetNormal();
		int GetColor(int ABGR);
		void SetA(fVector3 newA);
		void SetB(fVector3 newB);
		void SetC(fVector3 newC);
		
		//Quality of Life
		fVector3 Center();
		std::string ToStr();
		
		//Transform Functions
		void Displace(fVector3 displacement);
		void Transform(fVector3 iHat, fVector3 jHat, fVector3 kHat);
		void Rotate(float rad, fVector3 axis);
		void RotateRelative(float rad, fVector3 axis, fVector3 center);
		void RotateQuat(Quaternion quat, fVector3 center = fVector3(0, 0, 0));
		void Scale(fVector3 scale);
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
