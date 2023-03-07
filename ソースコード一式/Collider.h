#pragma once
#include <DirectXMath.h>
//#include	"Sphere.h"
//#include	"DX11util.h"
//#include	"dx11mathutil.h"
//#include	"DX11Settransform.h"
#include "Player.h"
#include "EnemyManager.h"
//#include "bulletmanager.h"
#include "BulletObject.h"

class Collider
{
	Collider() {
	}

private:
	float m_hitDistance;//�����蔻��v�Z�̍ۂɎg��

	//�Q�̃I�u�W�F�N�g�̋��������
	float ColliderNum(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2);
	float ColliderNum(const DirectX::XMFLOAT3 _pos1, const DirectX::XMFLOAT3 _pos2);

public:
	//�V���O���g��
	static Collider* GetInstance() {
		static Collider instance;
		return &instance;
	}
	
	/*************************************************************
	* �����蔻������֐�
	*************************************************************/
	//�����蔻������}�f
	bool OnCollision(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2, float _Radius);

	//�S�Ă̒e�ƓG�ƃv���C���[�̓����蔻����s���֐�
	void AllOnCollision(std::shared_ptr<Player>& _player, std::vector<std::unique_ptr<EnemyObject>>& _enemy,
		std::vector<std::unique_ptr<BulletObject>>& _Pbullet, std::vector<std::unique_ptr<BulletObject>>& _Ebullet);

	//�v���C���[�ƓG�I�u�W�F�N�g
	bool OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<EnemyObject>& _enemy);
	//�v���C���[�ƓG�I�u�W�F�N�g
	bool OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<BulletObject>& _bullet);
	//�G�ƒe�I�u�W�F�N�g
	bool OnCollision(std::unique_ptr<EnemyObject>& _enemy, std::unique_ptr<BulletObject>& _bullet, std::shared_ptr<Player>& _player);
	//�e�ƒe�I�u�W�F�N�g
	bool OnCollision(std::unique_ptr<BulletObject>& _bullet1, std::unique_ptr<BulletObject>& _bullet2);

	//�v���C���[�ƓG�I�u�W�F�N�g
	void OnCollision(Player& _player, EnemyLoop& _enemy);

};


