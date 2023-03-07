#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "BulletObject.h"
#include "dx11mathutil.h"

// 前方宣言
class CModel;

class HomingMissile :public BulletObject {
private:

	DirectX::XMFLOAT3		m_Velocity;		// ミサイルの現在速度
	DirectX::XMFLOAT4		m_Quaternion;	// 現在の姿勢を表すクォータニオン
	
	DirectX::XMFLOAT3		m_target;		// ホーミングするターゲット位置

	void Move(const DirectX::XMFLOAT3 _targetPosition);

public:

	bool Init();

	//void Draw();

	void Update();

	void Finalize();

	void SetDirection(DirectX::XMFLOAT4X4 _mtx, ENEMY_SHOT_DIRECTION _num) //初期位置セット
	{
		switch (_num)
		{
		case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT:
			m_direction = DirectX::XMFLOAT3(_mtx._31, 0.0f, _mtx._33);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_LEFT:
			m_direction = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_RIGHT:
			m_direction = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_BACK:
			m_direction = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT_LEFT:
			m_direction = DirectX::XMFLOAT3(-0.7f, 0.0f, 0.7f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT_RIGHT:
			m_direction = DirectX::XMFLOAT3(0.7f, 0.0f, 0.7f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_BACK_LEFT:
			m_direction = DirectX::XMFLOAT3(-0.7f, 0.0f, -0.7f);
			break;
		case ENEMY_SHOT_DIRECTION::DIRECTION_BACK_RIGHT:
			m_direction = DirectX::XMFLOAT3(0.7f, 0.0f, -0.7f);
			break;
		case ENEMY_SHOT_DIRECTION::END:
			break;
		default:
			break;
		}

	}

	void SetVelocity(DirectX::XMFLOAT3 _vel) {
		m_Velocity = _vel;
	}

	void SetInitailQuaternion(DirectX::XMFLOAT4X4 _mtx) {
		DX11GetQtfromMatrix(_mtx, m_Quaternion);		// 行列からクオータニオンを生成
	}

	void SetInitialPos(DirectX::XMFLOAT3 _pos)//弾を生成した時の位置セット
	{
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
	}

	bool isLive() {
		if (m_state == STATE::LIVE) return true;
		else return false;
	}

	void SetTarget(DirectX::XMFLOAT3 _targetpos) {
		m_target = _targetpos;
	}
};
