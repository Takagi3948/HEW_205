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

	//---  
	struct Info {
		Type type;
		union {
			Box box;
			Sphere sphere;
		};
	};

	//---  結果
	struct Result {
		bool     isHit;  //  当たったか
		DirectX::XMFLOAT3  point;  //  ヒット位置
		DirectX::XMFLOAT3  normal;  //  ヒット平面
		Info     other;  //  当たり判定オブジェクト
	};

public:
	// Info型の判定
	static Result Hit(Info a, Info b);
	//  四角同士の判定
	static Result Hit(Box a, Box b);
	//球同士の判定
	static Result Hit(Sphere a, Sphere b);

	static Result Hit(Sphere a, Box b);

};
