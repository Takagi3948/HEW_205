#include "StageManager.h"

CStageManager::CStageManager(int World, int Stage)
	: StageInfo()
	, v_Ground()
	, v_Enemy()
{
	//CSVファイルの読み込み
	ifstream file("Stage/Stage.csv"); //開きたいファイル

	//ファイルが開けた場合
	if (file)
	{
		//読み込んだファイルを格納する変数
		string line;	//一行

		//現在読み込んでいる行のカウンター
		int y = 0;
		
		//csvを一行ずつ読み込む
		while (getline(file, line) && y < 100)
		{
			int Objnum = 0; //何の情報かのカウント
			int x = 0;	//現在読み込んでいる列のカウンター
			bool IsEmptyRow = false; //空のデータか確認

			// UTF-8 BOMを取り除く
			if (line.compare(0, 3, "\xEF\xBB\xBF") == 0)
			{
				line.erase(0, 3);
			}

			//読み込んだ一行を文字ストリームに格納する
			stringstream ss(line);

			string value;

			//「,」で区切って１つずつ読み込む
			while (getline(ss, value, ',') && x < 12)
			{
				//最初の番号が0なら何もない＝breakして次の行へ
				if (stoi(value) == 0 && Objnum == 0)
				{
					IsEmptyRow = true;
					break;
				}

				//何番目かによって入れ方を変える
				switch (Objnum)
				{
				case 1:	StageInfo.World = stoi(value);	break;	//面
				case 2: StageInfo.Stage = stoi(value);	break;  //ステージ
				case 3: StageInfo.type = stoi(value);	break;	//種類
				case 4: StageInfo.Class = stoi(value);	break;	//種族クラス
				case 5: StageInfo.Pos[0] = stof(value);	break;	//位置ｘ
				case 6:	StageInfo.Pos[1] = stof(value);	break;	//位置ｙ
				case 7: StageInfo.Pos[2] = stof(value);	break;	//位置ｚ
				case 8: StageInfo.Size[0] = stof(value);break;	//サイズｘ
				case 9: StageInfo.Size[1] = stof(value);break;	//サイズｙ
				case 10: StageInfo.Size[2] = stof(value);break;	//サイズｚ
				case 11: StageInfo.Angle = stof(value);	break;	//角度
				}

				Objnum++;
				x++;
			}

			// 表示するステージと一致しないか空データの場合次の処理へcontinue
			if ((StageInfo.World != World ||
				StageInfo.Stage != Stage) || IsEmptyRow)
			{
				y++;
				continue;
			}


			//足場か障害物かの仕分け
			switch (StageInfo.Class)
			{
			case 0: //足場

				// 対応するステージのコンテナにGroundを生成
				v_Ground.emplace_back(
					StageInfo.Pos[0], StageInfo.Pos[1], StageInfo.Pos[2],
					StageInfo.Size[0], StageInfo.Size[1], StageInfo.Size[2],
					StageInfo.Angle, StageInfo.type);
				break;

			case 1://障害物
				// 対応するステージのコンテナにEnemyを生成
				v_Enemy.emplace_back(
					StageInfo.Pos[0], StageInfo.Pos[1], StageInfo.Pos[2],
					StageInfo.Size[0], StageInfo.Size[1], StageInfo.Size[2],
					StageInfo.type);
				break;

			}
			y++;
		}
		//ファイルを閉じる
		file.close();
	}
	else
	{
		MessageBox(NULL, "CSV読み込み失敗", "Load Error", MB_OK);
	}

}

CStageManager::~CStageManager()
{
	//pushじゃなくてemplaceにするとdeleteの必要もない模様
}

void CStageManager::Update()
{
	//範囲for文。条件で処理を分けるとかしなければ、イテレーターより楽
	for (Ground& ground : v_Ground)
	{
		ground.Update();
	}

	for (CEnemy& enemy : v_Enemy)
	{
		enemy.Update();
	}
}

void CStageManager::Draw()
{
	for (Ground& ground : v_Ground)
	{
		ground.Draw();
	}

	for (CEnemy& enemy : v_Enemy)
	{
		enemy.Draw();
	}
}

Collision::Info CStageManager::GetGroundCollision()
{
	for (Ground& ground : v_Ground)
	{
		return ground.GetCollision();
	}
}

Collision::Info CStageManager::GetEnemyCollision()
{
	for (CEnemy& enemy : v_Enemy)
	{
		return enemy.GetCollision();
	}
}


int CStageManager::GetGroundCount()
{
	return (int)v_Ground.size();
}

Collision::Info CStageManager::GetGroundCollision(int num)
{
	if (num < 0 || num >= v_Ground.size())
	{
		return Collision::Info();
	}

	return v_Ground[num].GetCollision();
}

Collision::Triangle* CStageManager::GetGroundTriangle(int num)
{
	if (num < 0 || num >= v_Ground.size())
	{
		return nullptr;
	}

	return v_Ground[num].GetTriangle();
}
