#ifndef FVECTORS_H
#define FVECTORS_H
#include <string>

class fVector2{
	private:
		float x;
		float y;
	public:
		fVector2(float initX = 0, float initY = 0);
		
		//Getters and Setters//
		float GetX();
		float GetY();
		void SetX(float newX);
		void SetY(float newY);
		//End Getters and Setters//
		
		
		//Operations//
		void Add(fVector2 addend);
		void Multiply(float factor);
		void Transform(fVector2 iHat, fVector2 jHat);
		//End Operations//
		
		
		//Quality of Life//
		std::string ToStr();
		//End Quality of Life//
	
};
#endif
