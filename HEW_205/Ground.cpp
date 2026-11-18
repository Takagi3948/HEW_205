#include "Ground.h"
#include "Geometory.h"
#include <DirectXMath.h>

Ground::Ground()
	:m_pos{0.0f,0.0f,0.0f}
	,m_scale{1.0f,1.0f,1.0f}
    ,m_angle(10.0f)
    ,m_triangle()
    ,m_collision()
{
    // 当たり判定の種類を線分にする
    m_collision.type = Collision::ePlane;
}

Ground::~Ground()
{
}

void Ground::Update()
{
    /*
    float angle = DirectX::XMConvertToRadians(m_angle);

    //平面
    m_collision.plane.pos = {
    m_pos[0],
    m_pos[1] + m_scale[1] * 0.5f,
    m_pos[2]
    };

    
    m_collision.plane.normal = {
        -sinf(angle),
        cosf(angle),
        0.0f
    };


    //三角形
    // ボックスの四隅の座標
    float halfX = m_scale[0] * 0.5f;
    float halfY = m_scale[1] * 0.5f;
    float halfZ = m_scale[2] * 0.5f;


    DirectX::XMFLOAT3 p0 = {
        m_pos[0] - halfX,
        m_pos[1] + halfY,
        m_pos[2] - halfZ
    };

    DirectX::XMFLOAT3 p1 = {
        m_pos[0] - halfX,
        m_pos[1] + halfY,
        m_pos[2] + halfZ
    };

    DirectX::XMFLOAT3 p2 = {
        m_pos[0] + halfX,
        m_pos[1] + halfY,
        m_pos[2] + halfZ
    };

    DirectX::XMFLOAT3 p3 = {
        m_pos[0] + halfX,
        m_pos[1] + halfY,
        m_pos[2] - halfZ
    };


    // 1つ目の三角形
    m_triangle[0].point[0] = p0;
    m_triangle[0].point[1] = p1;
    m_triangle[0].point[2] = p2;

    // 2つ目の三角形
    m_triangle[1].point[0] = p2;
    m_triangle[1].point[1] = p3;
    m_triangle[1].point[2] = p0;

    */

    float angle = DirectX::XMConvertToRadians(m_angle);

    float c = cosf(angle);
    float s = sinf(angle);

    float halfX = m_scale[0] * 0.5f;
    float halfY = m_scale[1] * 0.5f;
    float halfZ = m_scale[2] * 0.5f;

    // ローカル座標をZ軸回転してワールド座標に変換する
    auto RotatePoint = [&](float x, float y, float z)
        {
            DirectX::XMFLOAT3 point;

            point.x = m_pos[0] + x * c - y * s;
            point.y = m_pos[1] + x * s + y * c;
            point.z = m_pos[2] + z;

            return point;
        };

    // 平面の位置（上面の中心）
    m_collision.plane.pos = RotatePoint(0.0f, halfY, 0.0f);

    // 平面の法線
    m_collision.plane.normal = {
        -s,
        c,
        0.0f
    };

    // 三角形の頂点
    DirectX::XMFLOAT3 p0 = RotatePoint(-halfX, halfY, -halfZ);
    DirectX::XMFLOAT3 p1 = RotatePoint(-halfX, halfY, halfZ);
    DirectX::XMFLOAT3 p2 = RotatePoint(halfX, halfY, halfZ);
    DirectX::XMFLOAT3 p3 = RotatePoint(halfX, halfY, -halfZ);

    // 1つ目の三角形
    m_triangle[0].point[0] = p0;
    m_triangle[0].point[1] = p1;
    m_triangle[0].point[2] = p2;

    // 2つ目の三角形
    m_triangle[1].point[0] = p2;
    m_triangle[1].point[1] = p3;
    m_triangle[1].point[2] = p0;
}

void Ground::Draw()
{
    float angle = DirectX::XMConvertToRadians(m_angle);

	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_scale[0], m_scale[1], m_scale[2]);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationZ(angle);
	DirectX::XMMATRIX mat = S * R * T;

	//変換行列をGPUに渡すための前処理
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//matをfMatに格納
	DirectX::XMStoreFloat4x4(&fMat, mat);


	//描画処理
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();

}

void Ground::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

void Ground::SetScale(float x, float y, float z)
{
	m_scale[0] = x;
	m_scale[1] = y;
	m_scale[2] = z;
}

Collision::Box Ground::GetBox()
{
	return Collision::Box{ {m_pos[0],m_pos[1],m_pos[2]},{m_scale[0],m_scale[1],m_scale[2] } };
}

Collision::Triangle* Ground::GetTriangle()
{
    return m_triangle;
}
