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
#include "Defines.h"
#include "Ground.h"
#include "Enemy.h"
#include <vector>	// 1.準備
using namespace std;

//読み込んだオブジェクトの情報格納構造体
typedef struct StageInfo{
	int World;		//面
	int Stage;		//ステージ
	int type;		//物の種類
	int Class;		//種族クラス
	float Pos[3];	//位置
	float Size[3];	//サイズ
	float Angle;	//角度
};

class CStageManager{
private:
	StageInfo StageInfo;	//オブジェクト情報
	vector<Ground> v_Ground;	//ステージごとの足場格納コンテナ(形きもすぎ)
	vector<CEnemy> v_Enemy;//ステージごとの障害物格納コンテナ(形きもすぎ)

public:
	CStageManager(int World = 1, int Stage = 1);
	~CStageManager();

	void Update();

	void Draw();

	Collision::Info GetGroundCollision();
	Collision::Info GetEnemyCollision();


	// Groundの数を取得
	int GetGroundCount();
	// Groundの当たり判定を取得
	Collision::Info GetGroundCollision(int num);
	// Groundの三角形情報を取得
	Collision::Triangle* GetGroundTriangle(int num);

	// Groundの当たり判定を取得　横
	Collision::Info* GetGroundSideCollision(int num);
	// Groundの三角形情報を取得　横
	Collision::Triangle* GetGroundSideTriangle(int num);


	//Enemyの数を取得
	int GetEnemyCount();
	//Enemyの当たり判定の取得
	Collision::Info GetEnemyCollision(int num);


};

