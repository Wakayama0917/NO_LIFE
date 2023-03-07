#pragma once
#include <random>
#include "EnemyObject.h"
#include "CModel.h"
#include "CDirectInput.h"

//�G��AI
class Boss1 : public EnemyObject
{
private:
	XMFLOAT3 m_angle;//��]�p�x
	float m_maxRot = DirectX::XM_PI * 10.0f / 180.0f;// �ő��]�p�x
	// ���݂̎p����\���N�H�[�^�j�I��
	DirectX::XMFLOAT4 m_Quaternion;
public:
	bool Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player);   //������
	//void Draw();
	void DrawWithAxis();
	void Update();
	void Finalize();
	void EnemyMove();

};