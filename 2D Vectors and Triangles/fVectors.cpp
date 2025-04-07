#include "fVectors.h"
		
	fVector2::fVector2(float initX, float initY){
		x = initX;
		y = initY;
	}
	
	//Getters and Setters//
	float fVector2::GetX(){
		return x;
	}
	float fVector2::GetY(){
		return y;
	}
	float fVector2::SetX(float newX){
		x = newX;
	}
	float fVector2::SetY(float newY){
		y = newY;
	}
	//End Getters and Setters//
	
	
	//Operations//
	void fVector2::Add(fVector2 addend){
		x = x + addend.GetX();
		y = y + addend.GetY();
	}
	
	void fVector2::Multiply(float numerator, float denominator){
		x = x * numerator / denominator;
		y = y * numerator / denominator;
	}
	
	void fVector2::Transform(fVector2 iHat, fVector2 jHat){
		float xBuffer = x;
		float yBuffer = y;
		x = (xBuffer * iHat.GetX()) + (yBuffer * jHat.GetX());
		y = (xBuffer * iHat.GetY()) + (yBuffer * jHat.GetY());
	}
	//End Operations//
	
	
	//Quality of Life//
	std::string fVector2::ToStr(){
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
	//End Quality of Life//
	
