#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

//おすすめ
//プレイヤー⇔地面	Line(Ray)+Plane+Triangle
//プレイヤー⇔坂道	Line(Ray) + Plane + Triangle
// ↑プレイヤー側に実装
// 
//果物⇔台			Ray + Plane + Triangle
//果物⇔障害物		Sphere + Box
// ↑果物側に実装


class Collision
{
public:
	//---  当たり判定の形状の種類
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

	//---  当たり判定の形状
	struct Box {  //  四角
		DirectX::XMFLOAT3  center;  //  中心座標
		DirectX::XMFLOAT3  size;  //  サイズ
	};
	struct Sphere { //  球
		DirectX::XMFLOAT3  center;  //  中心座標
		float     radius;  //  半径
	};
	struct Plane { //  平面
		DirectX::XMFLOAT3 pos;   //  平面上の1点
		DirectX::XMFLOAT3 normal;  //  平面の法線
	};
	struct Ray { //レイ (無限の直線 ) 
		DirectX::XMFLOAT3 origin;  //  始点
		DirectX::XMFLOAT3 dir;  //  レイの向き
	};
	struct Line { //  線分
		DirectX::XMFLOAT3 start;  //  線分の始点
		DirectX::XMFLOAT3 end;  //  線分の終点
	};
	struct Point {  //  点
		DirectX::XMFLOAT3 pos;  //  座標
	};
	struct Triangle { //  三角形
		DirectX::XMFLOAT3 point[3]; //  各頂点
	};


	//---  当たり判定の統合情報
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

	//---  当たり判定の結果
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
	//球と四角
	static Result Hit(Sphere a, Box b);
	//平面とレイの当たり判定
	static Result Hit(Plane plane, Ray ray, float lenght);

	//平面と線分の当たり判定
	static Result Hit(Plane plane, Line line);
	//点と三角形の当たり判定
	static Result Hit(Point point, Triangle triangle);
};

