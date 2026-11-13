#pragma once
#include "directX.h"
/* 説明文-Collison.h
* ここに使いそうな判定処理を作成しました。
* なんとなく呼び出すだけで使えるようにすれば、今後必要になったときにこれだけを呼び出すだけで済むと思ったためです。
* 使い方はGMと同じです。(Float2型を作るのが面倒だったので、float型のみですが...)
*/


// 矩形と矩形の当たり判定
bool CheckCollisionRtoR(float Pos1X,float Pos1Y,float Size1X,float Size1Y,float Pos2X,float Pos2Y,float Size2X,float Size2Y);
// 円形と円形の当たり判定
bool CheckCollisionCtoC(float Pos1X,float Pos1Y,float Rad1,float Pos2X,float Pos2Y,float Rad2);
// 円形と矩形の当たり判定 (未実装)
bool CheckCollisionRtoC(float Pos1X,float Pos1Y,float Size1X,float Size1Y,float Pos2X,float Pos2Y,float Rad2);

//typedef struct Float2
//{
//	float x;
//	float y;
//};
//
//Float2 MakeFloat2(float x, float y);
