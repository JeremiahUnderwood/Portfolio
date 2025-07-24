#ifndef QUATERNION_H
#define QUATERNION_H
#include <string>
#include <cmath>

class Quaternion{
	private:
		float w;
		float x;
		float y;
		float z;
	public:
		Quaternion(float initW = 0, float initX = 1, float initY = 0, float initZ = 0);
		
		//Getters and Setters//
		float GetW();
		float GetX();
		float GetY();
		float GetZ();
		void SetW(float newW);
		void SetX(float newX);
		void SetY(float newY);
		void SetZ(float newZ);
		//End Getters and Setters//
		
		
		//Operations//
		void Hamilton(Quaternion prime);
		void Normalize();
		//End Operations//
		
		
		//Quality of Life//
		std::string ToStr();
		//End Quality of Life//
	
};
#endif
