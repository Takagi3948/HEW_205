#include "Collision.h"

Collision::Result Collision::Hit(Info a, Info b)
{
	Result out = {};
	if (a.type == b.type) {
		switch (a.type) {
		case eBox:  out = Hit(a.box, b.box);   break;
		case eSphere: out = Hit(a.sphere, b.sphere); break;
		}
	}
	else
		out = Hit(a.sphere, b.box);

	return out;
}

Collision::Result Collision::Hit(Box a, Box b)
{
	Result out = {};
	// VECTOR 
	DirectX::XMVECTOR vPosA = DirectX::XMLoadFloat3(&a.center);
	DirectX::XMVECTOR vPosB = DirectX::XMLoadFloat3(&b.center);
	DirectX::XMVECTOR vSizeA = DirectX::XMLoadFloat3(&a.size);
	DirectX::XMVECTOR vSizeB = DirectX::XMLoadFloat3(&b.size);
	//  
	vSizeA = DirectX::XMVectorScale(vSizeA, 0.5f);
	vSizeB = DirectX::XMVectorScale(vSizeB, 0.5f);
	//  
	DirectX::XMVECTOR vMaxA = DirectX::XMVectorAdd(vPosA, vSizeA);
	DirectX::XMVECTOR vMinA = DirectX::XMVectorSubtract(vPosA, vSizeA);
	DirectX::XMVECTOR vMaxB = DirectX::XMVectorAdd(vPosB, vSizeB);
	DirectX::XMVECTOR vMinB = DirectX::XMVectorSubtract(vPosB, vSizeB);
	DirectX::XMFLOAT3 maxA, minA, maxB, minB;
	DirectX::XMStoreFloat3(&maxA, vMaxA);
	DirectX::XMStoreFloat3(&minA, vMinA);
	DirectX::XMStoreFloat3(&maxB, vMaxB);
	DirectX::XMStoreFloat3(&minB, vMinB);

	//  
	out.isHit = false;
	if (maxA.x >= minB.x && minA.x <= maxB.x) {
		if (maxA.y >= minB.y && minA.y <= maxB.y) {
			if (maxA.z >= minB.z && minA.z <= maxB.z) {
				out.isHit = true;
				//  
				out.normal.x = min(maxA.x - minB.x, maxB.x - minA.x);
				out.normal.y = min(maxA.y - minB.y, maxB.y - minA.y) * 0.5f;
				out.normal.z = min(maxA.z - minB.z, maxB.z - minA.z);
				//  
				if (out.normal.x < out.normal.y) {
					if (out.normal.x < out.normal.z)
						out.normal = DirectX::XMFLOAT3(
							a.center.x >= b.center.x ? 1.0 : -1.0f, 0.0f, 0.0f);
					else
						out.normal = DirectX::XMFLOAT3(0.0f, 0.0f,
							a.center.z >= b.center.z ? 1.0 : -1.0f);
				}
				else {
					if (out.normal.y < out.normal.z)
						out.normal = DirectX::XMFLOAT3(0.0f,
							a.center.y >= b.center.y ? 1.0 : -1.0f, 0.0f);
					else
						out.normal = DirectX::XMFLOAT3(0.0f, 0.0f,
							a.center.z >= b.center.z ? 1.0 : -1.0f);
				}
			}
		}
	}
	return out;
}

Collision::Result Collision::Hit(Sphere a, Sphere b)
{
	Result out = {};
	//  
	DirectX::XMVECTOR vPosA = DirectX::XMLoadFloat3(&a.center);
	DirectX::XMVECTOR vPosB = DirectX::XMLoadFloat3(&b.center);
	DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vPosA, vPosB);
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vDist);
	float length;
	DirectX::XMStoreFloat(&length, vLen);

	out.isHit = length <= (a.radius + b.radius);
	return out;
}

Collision::Result Collision::Hit(Sphere a, Box b)
{
	Result out = {};

	// 円の中心と四角の中心・サイズを取得
	DirectX::XMVECTOR vSpherePos = DirectX::XMLoadFloat3(&a.center);
	DirectX::XMVECTOR vBoxPos = DirectX::XMLoadFloat3(&b.center);
	DirectX::XMVECTOR vBoxSize = DirectX::XMLoadFloat3(&b.size);

	// 四角の半分のサイズ
	vBoxSize = DirectX::XMVectorScale(vBoxSize, 0.5f);

	// 四角の最小座標と最大座標
	DirectX::XMVECTOR vMin = DirectX::XMVectorSubtract(vBoxPos, vBoxSize);
	DirectX::XMVECTOR vMax = DirectX::XMVectorAdd(vBoxPos, vBoxSize);

	// 円の中心を四角の範囲内に収める（最近接点）
	DirectX::XMVECTOR vClosest = DirectX::XMVectorClamp(
		vSpherePos, vMin, vMax
	);

	// 円の中心から最近接点までの距離
	DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(
		vSpherePos, vClosest
	);

	DirectX::XMVECTOR vLengthSq = DirectX::XMVector3LengthSq(vDist);

	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, vLengthSq);

	// 半径の2乗と比較
	out.isHit = lengthSq <= (a.radius * a.radius);

	return out;
}


Collision::Result Collision::Hit(Plane plane, Ray ray, float lenght)
{
	//  
	DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&ray.origin);
	DirectX::XMVECTOR vRay = DirectX::XMLoadFloat3(&ray.dir);
	//  
	vRay = DirectX::XMVector3Normalize(vRay);
	vRay = DirectX::XMVectorScale(vRay, lenght);
	//  
	Line line = {};
	line.start = ray.origin;
	DirectX::XMStoreFloat3(&line.end, DirectX::XMVectorAdd(vPos, vRay));
	//  
	return Hit(plane, line);
}


Collision::Result Collision::Hit(Plane plane, Line line)
{
	Result out = {};
	//  
	DirectX::XMVECTOR vPlanePos = DirectX::XMLoadFloat3(&plane.pos);
	DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&plane.normal);
	DirectX::XMVECTOR vLineStart = DirectX::XMLoadFloat3(&line.start);
	DirectX::XMVECTOR vLineEnd = DirectX::XMLoadFloat3(&line.end);
	vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

	//  
	DirectX::XMVECTOR vToStart = DirectX::XMVectorSubtract(vLineStart, vPlanePos);
	DirectX::XMVECTOR vToEnd = DirectX::XMVectorSubtract(vLineEnd, vPlanePos);
	DirectX::XMVECTOR vDotStart = DirectX::XMVector3Dot(vToStart, vPlaneN);
	DirectX::XMVECTOR vDotEnd = DirectX::XMVector3Dot(vToEnd, vPlaneN);

	//  
	float dotStart, dotEnd;
	DirectX::XMStoreFloat(&dotStart, vDotStart);
	DirectX::XMStoreFloat(&dotEnd, vDotEnd);

	if (dotStart > 0.0f && dotEnd < 0.0f) {
		//  
		dotStart = fabsf(dotStart);
		dotEnd = fabsf(dotEnd);
		float rate = dotStart / (dotStart + dotEnd); //  
		DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vLineEnd, vLineStart);
		vDist = DirectX::XMVectorScale(vDist, rate); //  
		vDist = DirectX::XMVectorAdd(vLineStart, vDist); //  

		//  
		out.isHit = true;
		out.normal = plane.normal;
		DirectX::XMStoreFloat3(&out.point, vDist);
	}
	return out;
}


Collision::Result Collision::Hit(Point point, Triangle triangle)
{
	Result out = {};
	//  
	DirectX::XMVECTOR vTriPoint[3] = {
	 DirectX::XMLoadFloat3(&triangle.point[0]),
	 DirectX::XMLoadFloat3(&triangle.point[1]),
	 DirectX::XMLoadFloat3(&triangle.point[2]),
	};
	DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&point.pos);

	//  
	DirectX::XMVECTOR vEdge[3];
	DirectX::XMVECTOR vToPoint[3];
	for (int i = 0; i < 3; ++i) {
		//  
		vEdge[i] = DirectX::XMVectorSubtract(vTriPoint[(i + 1) % 3], vTriPoint[i]);
		vEdge[i] = DirectX::XMVector3Normalize(vEdge[i]);
		//  
		vToPoint[i] = DirectX::XMVectorSubtract(vPoint, vTriPoint[i]);
		vToPoint[i] = DirectX::XMVector3Normalize(vToPoint[i]);
	}

	//  
	DirectX::XMVECTOR vNormal;
	vNormal = DirectX::XMVector3Cross(vEdge[0], vEdge[1]);
	vNormal = DirectX::XMVector3Normalize(vNormal);

	//  
	float dot[3];
	DirectX::XMStoreFloat(&dot[0], DirectX::XMVector3Dot(vNormal, vToPoint[0]));
	if (fabsf(dot[0]) > FLT_EPSILON) { // FLT_EPSILON ≠ 0 0 float 
		return out; //  
	}

	//  
	DirectX::XMVECTOR vCross[3];
	for (int i = 0; i < 3; ++i) {
		vCross[i] = DirectX::XMVector3Cross(vEdge[i], vToPoint[i]);
	}

	//  
	for (int i = 0; i < 3; ++i) {
		DirectX::XMStoreFloat(&dot[i], DirectX::XMVector3Dot(vNormal, vCross[i]));
	}

	//  
	if (dot[0] >= 0.0f &&
		dot[1] >= 0.0f &&
		dot[2] >= 0.0f) 
	{
		out.isHit = true;
		out.point = point.pos;
		DirectX::XMStoreFloat3(&out.normal, vNormal);
	}

	return out;
}
