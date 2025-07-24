#ifndef Object_H
#define Object_H
#include "fVector3.h"
#include "Triangle3D.h"
#include <string>
#include <list>

class Object{
	private:
		fVector3 location = fVector3(0, 0, 0);
		fVector3 scale = fVector3(1, 1, 1);
		Quaternion rotation = Quaternion(1, 0, 0, 0);
		std::list<Triangle3D> mesh;
		
	public:
		Object(fVector3 initLoc, fVector3 initScale, Quaternion initRot, std::list<Triangle3D> initMesh);
		
		void SetLocation(fVector3 newLocation);
		void SetScale(fVector3 newScale);
		void SetRotation(Quaternion newRotation);
		
		fVector3 GetLocation();
		fVector3 GetScale();
		Quaternion GetRotation();
		std::list<Triangle3D> GetMesh();
};
#endif
