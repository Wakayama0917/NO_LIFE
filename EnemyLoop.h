#pragma once
#include <random>
//#include "gameobject.h"
#include "EnemyObject.h"
#include "CModel.h"
//#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "BoundingSphere.h"

//敵のAI
class EnemyLoop : public EnemyObject
{
private:

	XMFLOAT3 m_angle = {0,0,0};//回転角度

	//進む値
	float m_moveValue = 0.0f;
public:

	/**************************
	//エネミー関連関数
	***************************/
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //初期化
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	//void AutoPilot();
	void EnemyMove();

};


