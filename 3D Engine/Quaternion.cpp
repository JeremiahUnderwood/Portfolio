 #include "Quaternion.h"
		
	Quaternion::Quaternion(float initW, float initX, float initY, float initZ){
		w = initW;
		x = initX;
		y = initY;
		z = initZ;
	}
	
	//Getters and Setters//
	float Quaternion::GetW(){
		return w;
	}
	float Quaternion::GetX(){
		return x;
	}
	float Quaternion::GetY(){
		return y;
	}
	float Quaternion::GetZ(){
		return z;
	}
	void Quaternion::SetW(float newW){
		w = newW;
	}
	void Quaternion::SetX(float newX){
		x = newX;
	}
	void Quaternion::SetY(float newY){
		y = newY;
	}
	void Quaternion::SetZ(float newZ){
		z = newZ;
	}
	//End Getters and Setters//
	
	
	
	//Operations//
	
	/*
	Given:
	(w + xi + yj +zk) * (w` + x`i + y`j + z`k) you get
	
	ww` + wx`i + wy`j + wz`k +
	xw`i + xx`ii + xy`ij + xz`ik + 
	yw`j + yx`ji + yy`jj + yz`jk +
	zw`k + zx`ki + zy`kj + zz`kk, which is equivalent to
	
	ww` + wx`i +wy`j + wz`k +
	xw`i + -xx` + xy`k + -xz`j +
	yw`j + -yx`k + -yy` + yz`i +
	zw`k + zx`j + -zy`i + -zz`, which simplifies to
	
	ww` -xx` - yy` - zz` +
	(xw` + wx` + yz` - zy`)i +
	(yw` + wy` - xz` + zx`)j +
	(zw` + wz` + xy` - yx`)k,
	
	*/
	void Quaternion::Hamilton(Quaternion prime){
		float newW = (w * prime.GetW()) - (x * prime.GetX()) - (y * prime.GetY()) - (z * prime.GetZ());
		float newX = (x * prime.GetW()) + (w * prime.GetX()) + (y * prime.GetZ()) - (z * prime.GetY());
		float newY = (y * prime.GetW()) + (w * prime.GetY()) - (x * prime.GetZ()) + (z * prime.GetX());
		float newZ = (z * prime.GetW()) + (w * prime.GetZ()) + (x * prime.GetY()) - (y * prime.GetX());
		w = newW;
		x = newX;
		y = newY;
		z = newZ;
	}
	
	void Quaternion::Normalize(){ // Sets Quaternion Values to scale a length of 1;
		float magnitude = std::sqrt(w * w + x * x + y * y + z * z);
		w = w / magnitude;
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}
	//End Operations//
	
	
	//Quality of Life//
	std::string Quaternion::ToStr(){
		return "(" + std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}
	//End Quality of Life//
	
