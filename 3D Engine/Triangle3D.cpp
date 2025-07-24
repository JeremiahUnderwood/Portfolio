#include "Triangle3D.h"

	//Constructor
	Triangle3D::Triangle3D(fVector3 vA, fVector3 vB, fVector3 vC){
		vertexA = vA;
		vertexB = vB;
		vertexC = vC;
		for(int i = 0; i < sizeof(color) / sizeof(color[0]); i++){ //number of elements in array
			color[i] = 255;
		}
	}
	Triangle3D::Triangle3D(fVector3 vA, fVector3 vB, fVector3 vC, int* col){
		vertexA = vA;
		vertexB = vB;
		vertexC = vC;
		for(int i = 0; i < sizeof(color) / sizeof(color[0]); i++){ //number of elements in array
			color[i] = col[i];
		}
	}
	//End Constructor
	
	//Getters/Setters
	fVector3 Triangle3D::GetA(){
		return vertexA;
	}
	fVector3 Triangle3D::GetB(){
		return vertexB;
	}
	fVector3 Triangle3D::GetC(){
		return vertexC;
	}
	fVector3 Triangle3D::GetNormal(){
		//fVector3 returnVector = fVector3(0, 0, 0);
		fVector3 aBuffer = vertexA;
		aBuffer.Multiply(-1);
		fVector3 ab = vertexB;
		fVector3 ac = vertexC;
		ab.Add(aBuffer);
		ac.Add(aBuffer);
		float retX = (ab.GetY() * ac.GetZ()) - (ab.GetZ() * ac.GetY());
		float retY = -(ab.GetX() * ac.GetZ()) + (ab.GetZ() * ac.GetX());
		float retZ = (ab.GetX() * ac.GetY()) - (ab.GetY() * ac.GetX());
		return fVector3(retX, retY, retZ);
	}
	int Triangle3D::GetColor(int ABGR){
		if (ABGR < 4){
			return color[ABGR];
		}
		else {
			return 0;
		}
	}
	void Triangle3D::SetA(fVector3 newA){
		vertexA = newA;
	}
	void Triangle3D::SetB(fVector3 newB){
		vertexA = newB;
	}
	void Triangle3D::SetC(fVector3 newC){
		vertexA = newC;
	}
	//End Getters/Setters
	
	//Quality of Life
	fVector3 Triangle3D::Center(){
		return fVector3((GetA().GetX() + GetB().GetX() + GetC().GetX()) / 3, (GetA().GetY() + GetB().GetY() + GetC().GetY()) / 3, (GetA().GetZ() + GetB().GetZ() + GetC().GetZ()) / 3);
	}
	std::string Triangle3D::ToStr(){
		return vertexA.ToStr() + ", " + vertexB.ToStr() + ", " + vertexC.ToStr();
	}
	//End Quality of Life
	
	//Transform Functions
	void Triangle3D::Displace(fVector3 displacement){
		vertexA.Add(displacement);
		vertexB.Add(displacement);
		vertexC.Add(displacement);
	}
	
	void Triangle3D::Transform(fVector3 iHat, fVector3 jHat, fVector3 kHat){
		vertexA.Transform(iHat, jHat, kHat);
		vertexB.Transform(iHat, jHat, kHat);
		vertexC.Transform(iHat, jHat, kHat);
	}
	
	void Triangle3D::Rotate(float rad, fVector3 axis){ //rotate relative to triangle center
		fVector3 center = Center();
		center.Multiply(-1);
		Displace(center);
		vertexA.Rotate(rad, axis);
		vertexB.Rotate(rad, axis);
		vertexC.Rotate(rad, axis);
		center.Multiply(-1);
		Displace(center);
	}
	
	void Triangle3D::RotateRelative(float rad, fVector3 axis, fVector3 center){ //rotate relative to specified center
		center.Multiply(-1);
		Displace(center);
		vertexA.Rotate(rad, axis);
		vertexB.Rotate(rad, axis);
		vertexC.Rotate(rad, axis);
		center.Multiply(-1);
		Displace(center);
	}
	
	void Triangle3D::RotateQuat(Quaternion quat, fVector3 center){ //rotate relative to specified center
		center.Multiply(-1);
		Displace(center);
		vertexA.qRotate(quat);
		vertexB.qRotate(quat);
		vertexC.qRotate(quat);
		center.Multiply(-1);
		Displace(center);
	}
	
	void Triangle3D::Scale(fVector3 scale){
		fVector3 center = Center();
		center.Multiply(-1);
		Displace(center);
		Transform(fVector3(scale.GetX(), 0, 0), fVector3(0, scale.GetY(), 0), fVector3(0, 0, scale.GetZ()));
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
