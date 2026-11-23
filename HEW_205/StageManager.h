#pragma once
//CSVファイル読み込みプログラム
//マップチップのCSVファイルを読み込むときに使う
#include<iostream>
#include<iomanip>
#include<fstream>	//ファイルの読み書き
#include<sstream>	//文字列ストリーム
#include<string>	//文字列
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"
#include "Ground.h"
#include "Enemy.h"
#include <vector>	// 1.準備
using namespace std;

class CStageManager{
private:
	int World;		//面
	int Stage;		//ステージ
	int type;		//物の種類
	float Pos[3];	//位置
	float Size[3];	//サイズ
	float Angle;	//角度
	vector<Ground> v_Ground; //足場のコンテナ
	vector<CEnemy> v_Enemy; //障害物のコンテナ

public:
	CStageManager();
	~CStageManager();

	void Update(int World, int Stage);

	void Draw(int World,int Stage);

	Collision::Info GetGroundCollision(int num);
	Collision::Info GetEnemyCollision(int num);

};

