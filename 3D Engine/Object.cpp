#include "Object.h"

	Object::Object(fVector3 initLoc, fVector3 initScale, Quaternion initRot, std::list<Triangle3D> initMesh){
		location = initLoc;
		scale = initScale;
		rotation = initRot;
		mesh = initMesh;
	}
	
	//Getters
	fVector3 Object::GetLocation(){
		return location;
	}
	fVector3 Object::GetScale(){
		return scale;
	}
	Quaternion Object::GetRotation(){
		return rotation;
	}
	std::list<Triangle3D> Object::GetMesh(){
		return mesh;
	}
	
	//Setters
	void Object::SetLocation(fVector3 newLocation){
		location = newLocation;
	}
	void Object::SetScale(fVector3 newScale){
		scale = newScale;
	}
	void Object::SetRotation(Quaternion newRotation){
		rotation = newRotation;
		rotation.Normalize();
	}
	
	
