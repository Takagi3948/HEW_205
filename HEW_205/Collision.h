#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"


class Collision
{
public:
	//---  
	enum Type {
		eNone,
		eBox,
		eSphere,
		ePlane,
		eRay,
		eLine,
		ePoint,
		eTriangle,
	};

	//---  
	struct Box {  //  
		DirectX::XMFLOAT3  center;  //  
		DirectX::XMFLOAT3  size;  //  
	};
	struct Sphere { //  
		DirectX::XMFLOAT3  center;  //  
		float     radius;  //  
	};
	struct Plane { //  
		DirectX::XMFLOAT3 pos;   //  
		DirectX::XMFLOAT3 normal;  //  
	};
	struct Ray { // ( ) 
		DirectX::XMFLOAT3 origin;  //  
		DirectX::XMFLOAT3 dir;  //  
	};
	struct Line { //  
		DirectX::XMFLOAT3 start;  //  
		DirectX::XMFLOAT3 end;  //  
	};
	struct Point {  //  
		DirectX::XMFLOAT3 pos;  //  
	};
	struct Triangle { //  
		DirectX::XMFLOAT3 point[3]; //  
	};


	//---  
	struct Info {
		Type type;
		union {
			Box box;
			Sphere sphere;
			Plane plane;
			Ray  ray;
			Line line;
			Point  point;
			Triangle triangle;
		};
	};

	//---  Œ‹‰Ê
	struct Result {
		bool     isHit;  //  “–‚½‚Á‚½‚©
		DirectX::XMFLOAT3  point;  //  ƒqƒbƒgˆÊ’u
		DirectX::XMFLOAT3  normal;  //  ƒqƒbƒg•½–Ê
		Info     other;  //  “–‚½‚è”»’èƒIƒuƒWƒFƒNƒg
	};

public:
	// InfoŒ^‚Ì”»’è
	static Result Hit(Info a, Info b);
	//  ŽlŠp“¯Žm‚Ì”»’è
	static Result Hit(Box a, Box b);
	//‹…“¯Žm‚Ì”»’è
	static Result Hit(Sphere a, Sphere b);

	//‹…‚ÆŽlŠp
	static Result Hit(Sphere a, Box b);

	static Result Hit(Plane plane, Ray ray, float lenght);
	static Result Hit(Plane plane, Line line);

	static Result Hit(Point point, Triangle triangle);
};

