#ifndef IVECTORS_H
#define IVECTORS_H
#include <string>

class iVector2{
	private:
		int x;
		int y;
	public:
		iVector2(int initX = 0, int initY = 0);
		
		//Getters and Setters//
		int GetX();
		int GetY();
		int SetX(int newX);
		int SetY(int newY);
		//End Getters and Setters//
		
		
		//Operations//
		void Add(iVector2 addend);
		void Multiply(int numerator, int denominator = 1);
		void Transform(iVector2 iHat, iVector2 jHat);
		//End Operations//
		
		
		//Quality of Life//
		std::string ToStr();
		//End Quality of Life//
	
};
#endif
