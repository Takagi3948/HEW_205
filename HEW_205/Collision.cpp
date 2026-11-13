#include "Collision.h"

bool CheckCollisionRtoR(float Pos1X, float Pos1Y, float Size1X, float Size1Y, float Pos2X, float Pos2Y, float Size2X, float Size2Y)
{
	//Float2型の代わりにfloat型で宣言。また、ここでしか使わないのでconst化。
	const float halfSize1X = Size1X / 2.0f;
	const float halfSize1Y = Size1Y / 2.0f;
	const float halfSize2X = Size2X / 2.0f;
	const float halfSize2Y = Size2Y / 2.0f;

	if ((-halfSize2X + Pos2X < halfSize1X + Pos1X) &&
		(-halfSize1X + Pos1X < halfSize2X + Pos2X))
	{
		if((-halfSize2Y + Pos2Y < halfSize1Y + Pos1Y) &&
			(-halfSize1Y + Pos1Y < halfSize2Y + Pos2Y))
			{
				return true;
			}
	}
	return false;
}

bool CheckCollisionCtoC(float Pos1X, float Pos1Y, float Rad1, float Pos2X, float Pos2Y, float Rad2)
{
	return false;
}

bool CheckCollisionRtoC(float Pos1X, float Pos1Y, float Size1X, float Size1Y, float Pos2X, float Pos2Y, float Rad2)
{
	return false;
}

//Float2 MakeFloat2(float x, float y)
//{
//	Float2 MF2;
//	MF2.x = x;
//	MF2.y = y;
//	return MF2;
//}
