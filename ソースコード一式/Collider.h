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
	float m_hitDistance;//当たり判定計算の際に使う

	//２つのオブジェクトの距離を取る
	float ColliderNum(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2);
	float ColliderNum(const DirectX::XMFLOAT3 _pos1, const DirectX::XMFLOAT3 _pos2);

public:
	//シングルトン
	static Collider* GetInstance() {
		static Collider instance;
		return &instance;
	}
	
	/*************************************************************
	* 当たり判定を取る関数
	*************************************************************/
	//当たり判定を取る凡庸
	bool OnCollision(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2, float _Radius);

	//全ての弾と敵とプレイヤーの当たり判定を行う関数
	void AllOnCollision(std::shared_ptr<Player>& _player, std::vector<std::unique_ptr<EnemyObject>>& _enemy,
		std::vector<std::unique_ptr<BulletObject>>& _Pbullet, std::vector<std::unique_ptr<BulletObject>>& _Ebullet);

	//プレイヤーと敵オブジェクト
	bool OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<EnemyObject>& _enemy);
	//プレイヤーと敵オブジェクト
	bool OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<BulletObject>& _bullet);
	//敵と弾オブジェクト
	bool OnCollision(std::unique_ptr<EnemyObject>& _enemy, std::unique_ptr<BulletObject>& _bullet, std::shared_ptr<Player>& _player);
	//弾と弾オブジェクト
	bool OnCollision(std::unique_ptr<BulletObject>& _bullet1, std::unique_ptr<BulletObject>& _bullet2);

	//プレイヤーと敵オブジェクト
	void OnCollision(Player& _player, EnemyLoop& _enemy);

};


