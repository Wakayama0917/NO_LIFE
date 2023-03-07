#pragma once
#include <random>
//#include "gameobject.h"
#include "EnemyObject.h"
#include "CModel.h"
//#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "BoundingSphere.h"

//�G��AI
class EnemyLoop : public EnemyObject
{
private:

	XMFLOAT3 m_angle = {0,0,0};//��]�p�x

	//�i�ޒl
	float m_moveValue = 0.0f;
public:

	/**************************
	//�G�l�~�[�֘A�֐�
	***************************/
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //������
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	//void AutoPilot();
	void EnemyMove();

};


