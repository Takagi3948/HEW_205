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