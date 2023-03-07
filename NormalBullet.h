#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "BulletObject.h"
#include "EnumListPlayer.h"

//前方宣言
class CModel;
class EnemyObject;

class NormalBullet :public BulletObject
{
private:
	
	DirectX::XMFLOAT3	m_target;//ターゲットの位置
	float				m_radius;//相手の半径
public:

	bool Init();
	//void Draw();
	void Update();
	void Finalize();
	
	//弾を生成した時の位置セット
	void SetInitialPos(DirectX::XMFLOAT3 _pos){
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
	}

	//移動の処理、敵の位置、対象の半径を受け取る
	bool MoveOLD(XMFLOAT3 _targetPosition,float _radius);
	void Move(const DirectX::XMFLOAT3 _targetPosition);

	//ターゲット位置と半径を入れる、枚フレーム確認するもの
	void SetTarget(DirectX::XMFLOAT3 _targetpos,float _radius) {
		m_target = _targetpos;
		m_radius = _radius;
	}

	void SetDirection(XMFLOAT4X4 _mtx, ENEMY_SHOT_DIRECTION _num);//敵用
	void SetDirection(XMFLOAT4X4 _mtx, PLAYER_SHOT_DIRECTION _num);//プレイヤー用

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


