#ifndef FVECTOR3_H
#define FVECTOR3_H
#include <string>
#include "Quaternion.h"
#include <cmath>

class fVector3{
	private:
		float x;
		float y;
		float z;
	public:
		fVector3(float initX = 0, float initY = 0, float initZ = 0);
		
		//Getters and Setters//
		float GetX();
		float GetY();
		float GetZ();
		void SetX(float newX);
		void SetY(float newY);
		void SetZ(float newZ);
		//End Getters and Setters//
		
		
		//Operations//
		void Add(fVector3 addend);
		void Multiply(float factor);
		void Transform(fVector3 iHat, fVector3 jHat, fVector3 kHat); //multiply by a matrix
		void Rotate(float rad, fVector3 axis);
		void qRotate(Quaternion quat);
		//End Operations//
		
		
		//Quality of Life//
		std::string ToStr();
		//End Quality of Life//
	
};
#endif
