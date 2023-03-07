#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "BulletObject.h"
#include "EnumListPlayer.h"

//�O���錾
class CModel;
class EnemyObject;

class NormalBullet :public BulletObject
{
private:
	
	DirectX::XMFLOAT3	m_target;//�^�[�Q�b�g�̈ʒu
	float				m_radius;//����̔��a
public:

	bool Init();
	//void Draw();
	void Update();
	void Finalize();
	
	//�e�𐶐��������̈ʒu�Z�b�g
	void SetInitialPos(DirectX::XMFLOAT3 _pos){
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
	}

	//�ړ��̏����A�G�̈ʒu�A�Ώۂ̔��a���󂯎��
	bool MoveOLD(XMFLOAT3 _targetPosition,float _radius);
	void Move(const DirectX::XMFLOAT3 _targetPosition);

	//�^�[�Q�b�g�ʒu�Ɣ��a������A���t���[���m�F�������
	void SetTarget(DirectX::XMFLOAT3 _targetpos,float _radius) {
		m_target = _targetpos;
		m_radius = _radius;
	}

	void SetDirection(XMFLOAT4X4 _mtx, ENEMY_SHOT_DIRECTION _num);//�G�p
	void SetDirection(XMFLOAT4X4 _mtx, PLAYER_SHOT_DIRECTION _num);//�v���C���[�p

	void SetDirection(DirectX::XMFLOAT4X4 _mtx){
		m_mtx = _mtx;
		m_direction = DirectX::XMFLOAT3(_mtx._31, _mtx._32, _mtx._33);
		
		
	}


	bool isLive(){
		if (m_state == STATE::LIVE){
			return true;
		}
		else{
			return false;
		}
	}
	
};


