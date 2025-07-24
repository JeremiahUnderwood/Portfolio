#include "Triangle.h"

	//Constructor
	Triangle2D::Triangle2D(fVector2 vA, fVector2 vB, fVector2 vC){
		vertexA = vA;
		vertexB = vB;
		vertexC = vC;
		color[0] = 255;
		color[1] = 0;
		color[2] = 255;
		color[3] = 0;
		
	}
	Triangle2D::Triangle2D(fVector2 vA, fVector2 vB, fVector2 vC, int* col){
		vertexA = vA;
		vertexB = vB;
		vertexC = vC;
		for(int i = 0; i < sizeof(color) / sizeof(color[0]); i++){ //number of elements in array
			color[i] = col[i];
		}
	}
	//End Constructor
	
	//Getters/Setters
	fVector2 Triangle2D::GetA(){
		return vertexA;
	}
	fVector2 Triangle2D::GetB(){
		return vertexB;
	}
	fVector2 Triangle2D::GetC(){
		return vertexC;
	}
	int Triangle2D::GetColor(int ABGR){
		if (ABGR < 4){
			return color[ABGR];
		}
		else {
			return 0;
		}
	}
	void Triangle2D::SetA(fVector2 newA){
		vertexA = newA;
	}
	void Triangle2D::SetB(fVector2 newB){
		vertexA = newB;
	}
	void Triangle2D::SetC(fVector2 newC){
		vertexA = newC;
	}
	//End Getters/Setters
	
	//Quality of Life
	fVector2 Triangle2D::Center(){
		return fVector2((GetA().GetX() + GetB().GetX() + GetC().GetX()) / 3, (GetA().GetY() + GetB().GetY() + GetC().GetY()) / 3);
	}
	
	std::string Triangle2D::ToStr(){
		return vertexA.ToStr() + ", " + vertexB.ToStr() + ", " + vertexC.ToStr();
	}
	//End Quality of Life
	
	//Transform Functions
	void Triangle2D::Displace(fVector2 displacement){
		vertexA.Add(displacement);
		vertexB.Add(displacement);
		vertexC.Add(displacement);
	}
	
	void Triangle2D::Transform(fVector2 iHat, fVector2 jHat){
		vertexA.Transform(iHat, jHat);
		vertexB.Transform(iHat, jHat);
		vertexC.Transform(iHat, jHat);
	}
	
	void Triangle2D::Rotate(float rad){
		fVector2 matrixiHat = fVector2(std::cos(rad), std::sin(rad));
		fVector2 matrixjHat = fVector2(std::cos(rad + 3.1415926535897 / 2), std::sin(rad + 3.1415926535897 / 2));
		fVector2 center = Center();
		center.Multiply(-1);
		Displace(center);
		Transform(matrixiHat, matrixjHat);
		center.Multiply(-1);
		Displace(center);
		//std::cout << matrixiHat.ToStr() << matrixjHat.ToStr() << "\n";
	}
	
	void Triangle2D::Scale(fVector2 scale){
		fVector2 center = Center();
		center.Multiply(-1);
		Displace(center);
		Transform(fVector2(scale.GetX(), 0), fVector2(0, scale.GetY()));
		center.Multiply(-1);
		Displace(center);
		//std::cout << matrixiHat.ToStr() << matrixjHat.ToStr() << "\n";
	}
	//End Transform Functions
	

/*		For Reference
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
*/
