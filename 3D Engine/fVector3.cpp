 #include "fVector3.h"
		
	fVector3::fVector3(float initX, float initY, float initZ){
		x = initX;
		y = initY;
		z = initZ;
	}
	
	//Getters and Setters//
	float fVector3::GetX(){
		return x;
	}
	float fVector3::GetY(){
		return y;
	}
	float fVector3::GetZ(){
		return z;
	}
	void fVector3::SetX(float newX){
		x = newX;
	}
	void fVector3::SetY(float newY){
		y = newY;
	}
	void fVector3::SetZ(float newZ){
		z = newZ;
	}
	//End Getters and Setters//
	
	
	//Operations//
	void fVector3::Add(fVector3 addend){
		x += addend.GetX();
		y += addend.GetY();
		z += addend.GetZ();
	}
	
	void fVector3::Multiply(float factor){
		x *= factor;
		y *= factor;
		z *= factor;
	}
	
	void fVector3::Transform(fVector3 iHat, fVector3 jHat, fVector3 kHat){
		float xBuffer = x;
		float yBuffer = y;
		float zBuffer = z;
		x = (xBuffer * iHat.GetX()) + (yBuffer * jHat.GetX()) + (zBuffer * kHat.GetX());
		y = (xBuffer * iHat.GetY()) + (yBuffer * jHat.GetY()) + (zBuffer * kHat.GetY());
		z = (xBuffer * iHat.GetZ()) + (yBuffer * jHat.GetZ()) + (zBuffer * kHat.GetZ());
	}
	
	void fVector3::Rotate(float rad, fVector3 axis){
		Quaternion buffer = Quaternion(0, x, y, z);
		Quaternion modifier = Quaternion(std::cos(rad/2), std::sin(rad/2) * axis.x, std::sin(rad/2) * axis.y, std::sin(rad/2) * axis.z);
		Quaternion modifier2 = Quaternion(modifier.GetW(), -modifier.GetX(), -modifier.GetY(), -modifier.GetZ());
		modifier.Hamilton(buffer);
		modifier.Hamilton(modifier2);
		x = modifier.GetX();
		y = modifier.GetY();
		z = modifier.GetZ();
	}
	
	void fVector3::qRotate(Quaternion quat){
	
		Quaternion buffer = Quaternion(0, x, y, z);
		Quaternion modifier = quat;
		modifier.Normalize();
		Quaternion modifier2 = Quaternion(modifier.GetW(), -modifier.GetX(), -modifier.GetY(), -modifier.GetZ());
		modifier.Hamilton(buffer);
		modifier.Hamilton(modifier2);
		x = modifier.GetX();
		y = modifier.GetY();
		z = modifier.GetZ();
		
	}
	//End Operations//
	
	
	//Quality of Life//
	std::string fVector3::ToStr(){
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}
	//End Quality of Life//
	
