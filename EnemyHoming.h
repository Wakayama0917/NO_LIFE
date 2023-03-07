#pragma once
#include <random>
//#include "gameobject.h"
#include "EnemyObject.h"
#include "CModel.h"
//#include "dx11mathutil.h"
#include "CDirectInput.h"

//敵のAI
class EnemyHoming : public EnemyObject
{
private:
	XMFLOAT3 m_angle;//回転角度
	float m_maxRot = DirectX::XM_PI * 10.0f / 180.0f;// 最大回転角度
	// 現在の姿勢を表すクォータニオン
	DirectX::XMFLOAT4 m_Quaternion;
public:
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //初期化
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	void EnemyMove();

};