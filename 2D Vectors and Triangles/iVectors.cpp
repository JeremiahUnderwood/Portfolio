#include "iVectors.h"
		
	iVector2::iVector2(int initX, int initY){
		x = initX;
		y = initY;
	}
	
	//Getters and Setters//
	int iVector2::GetX(){
		return x;
	}
	int iVector2::GetY(){
		return y;
	}
	int iVector2::SetX(int newX){
		x = newX;
	}
	int iVector2::SetY(int newY){
		y = newY;
	}
	//End Getters and Setters//
	
	
	//Operations//
	void iVector2::Add(iVector2 addend){
		x = x + addend.GetX();
		y = y + addend.GetY();
	}
	
	void iVector2::Multiply(int numerator, int denominator){
		x = x * numerator / denominator;
		y = y * numerator / denominator;
	}
	
	void iVector2::Transform(iVector2 iHat, iVector2 jHat){
		int xBuffer = x;
		int yBuffer = y;
		x = (xBuffer * iHat.GetX()) + (yBuffer * jHat.GetX());
		y = (xBuffer * iHat.GetY()) + (yBuffer * jHat.GetY());
	}
	//End Operations//
	
	
	//Quality of Life//
	std::string iVector2::ToStr(){
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
	//End Quality of Life//
	
