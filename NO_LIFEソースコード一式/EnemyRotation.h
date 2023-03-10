#pragma once
#include <random>
#include "gameobject.h"
#include "CModel.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "EnemyObject.h"

//敵のAI
class EnemyRotation : public EnemyObject
{
private:
	XMFLOAT3 m_angle;//回転角度

	
public:
	/*************************************************************
	* 関数類
	*************************************************************/
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //初期化
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	//void AutoPilot();
private:
	void EnemyMove();
	
};

