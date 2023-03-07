#pragma once
#include "gameobject.h"
#include "EnumListBullet.h"

class BulletObject : public GameObject
{
protected:
	/*************************************************************
	* CSV�ŏ���������ϐ�
	*************************************************************/
	float m_attackSpeed = 1.0f;       //�U�����x
	//float m_reload;            //���̒e�����܂ł̎���
	float m_attackPower;       //�U����
	float m_liveTime = 300;          //��������
	float m_maxRot = 5.0f;            //�P�t���[�����̍ő��]�p�x
	//float m_expand;              //�g�嗦�AGameObject�̒��ɓ����Ă�
	float m_radius;            //���a�A�����蔻��p

	/*************************************************************
	* ���̑��̕ϐ�
	*************************************************************/
	BULLET_TYPE m_bulletType;//�������Ȃ�̒e����ݒ�
	DirectX::XMFLOAT3	m_direction;	// ���˕���(�ړ����ɉ��Z����l)

	

public:
	/*************************************************************
	* �֐���
	*************************************************************/
	//m_enemyNum������������Ă��Ȃ��ƃ��b�Z�[�W�\�L���Ď~�܂�
	bool BulletCsvSet(CSVFile _csvFile);//CSV�󂯎��
	void SetAllStatus(float _value[static_cast<int>(BULLET_STATUSID::END)]);
	void SetAllStatus(std::vector<float> _value);
	void DirectionSet(XMFLOAT4X4 _mtx, float _angle);//�e�̔��˕����Z�b�g

	/*************************************************************
	* SET�AGET�n�֐���
	*************************************************************/
	
	
	//CSV�œǂݎ�鐔�l���擾����
	float GetCSVStatus(BULLET_STATUSID _statusid);
	


    /*************************************************************
    * �o�[�`�����֐���
    *************************************************************/
	//�}�f
	virtual bool Init() = 0;
	virtual void Draw();
	virtual void Update() = 0;
	//virtual void Update(unique_ptr<EnemyObject>& _enemyObject) = 0;
	virtual void Finalize() { /*m_model->Uninit();*/ };

	//����
	virtual void SetTarget(DirectX::XMFLOAT3 _targetpos) {};
	virtual void SetDirection(DirectX::XMFLOAT4X4 _mtx) {};
	virtual bool isLive() = 0;
	virtual void Move(const DirectX::XMFLOAT3 _targetPosition) = 0;

private:

};

