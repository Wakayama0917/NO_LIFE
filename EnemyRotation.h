#pragma once
#include <random>
#include "gameobject.h"
#include "CModel.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "EnemyObject.h"

//“G‚ÌAI
class EnemyRotation : public EnemyObject
{
private:
	XMFLOAT3 m_angle;//‰ñ“]Šp“x

	
public:
	/*************************************************************
	* ŠÖ”—Ş
	*************************************************************/
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //‰Šú‰»
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	//void AutoPilot();
private:
	void EnemyMove();
	
};

