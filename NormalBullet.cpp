#include <memory>
#include "NormalBullet.h"
#include "dx11mathutil.h"
#include "CModel.h"
#include "CDirectInput.h"
#include "ModelMgr.h"
#include "Collider.h"
#include "EnemyObject.h"
#include "dx11mathutil.h"

constexpr float BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;

bool NormalBullet::Init(){
	bool sts = true;

	//行列初期化
	DX11MtxIdentity(m_mtx);
	//当たり判定の実体化
	CollisionCreateInit();

	//m_life = BULLETLIFE;//生き続ける時間を与える

	return sts;
}

//void NormalBullet::Draw(){
//	//モデル描画
//	if (m_state == STATE::LIVE){
//		m_model->Draw(m_mtx);
//	}
//}

void NormalBullet::Update(){
	m_liveTime--;//生存時間を減らす
	if (m_liveTime > 0){
		//移動させる
		//Move();
	}
	else{
		m_state = STATE::DEAD;//死亡扱いにする
	}
}

void NormalBullet::Finalize(){
	//m_model->Uninit();
}

bool NormalBullet::MoveOLD(XMFLOAT3 _targetPosition,float _radius)
{
	float lng1, lng2;
	lng1 = Distance(_targetPosition);
	lng2 = m_collision->GetHitdata().r + _radius;//半径を足す
	if (lng1 <= lng2)
	{
		return true;//当たっている
	}

	//移動量を加算
	m_pos.x = m_pos.x + m_attackSpeed * m_direction.x;
	m_pos.y = m_pos.y + m_attackSpeed * m_direction.y;
	m_pos.z = m_pos.z + m_attackSpeed * m_direction.z;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	return false;
}

void NormalBullet::Move(const DirectX::XMFLOAT3 _targetPosition)
{
	//移動量を加算
	m_pos.x = m_pos.x + m_attackSpeed * m_direction.x;
	m_pos.y = m_pos.y + m_attackSpeed * m_direction.y;
	m_pos.z = m_pos.z + m_attackSpeed * m_direction.z;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_collision->UpdatePosition(m_mtx);//球表示
}

void NormalBullet::SetDirection(XMFLOAT4X4 _mtx, ENEMY_SHOT_DIRECTION _num)
{
	m_mtx = _mtx;
	//各方向、引数の確度分変更してセット
	switch (_num)
	{
	case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT: {
		m_direction = DirectX::XMFLOAT3(_mtx._31, _mtx._32, _mtx._33);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_LEFT: {
		DirectionSet(_mtx, -90.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_RIGHT: {
		DirectionSet(_mtx, 90.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_BACK: {
		m_direction = DirectX::XMFLOAT3(-_mtx._31, _mtx._32, -_mtx._33);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT_LEFT: {
		DirectionSet(_mtx, -45.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_FRONT_RIGHT: {
		DirectionSet(_mtx, 45.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_BACK_LEFT: {
		DirectionSet(_mtx, -135.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::DIRECTION_BACK_RIGHT: {
		DirectionSet(_mtx, 135.0f);
	}
		break;
	case ENEMY_SHOT_DIRECTION::END: {
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "弾の方向が正しく設定されていない箇所があります",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
		break;
	default:
		break;
	}
	
}

void NormalBullet::SetDirection(XMFLOAT4X4 _mtx, PLAYER_SHOT_DIRECTION _num)
{
	m_mtx = _mtx;

	switch (_num)
	{
	case PLAYER_SHOT_DIRECTION::DIRECTION_FRONT: {
		m_direction = DirectX::XMFLOAT3(_mtx._31, _mtx._32, _mtx._33);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_LEFT: {
		DirectionSet(_mtx, -90.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_RIGHT: {
		DirectionSet(_mtx, 90.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_BACK: {
		m_direction = DirectX::XMFLOAT3(-_mtx._31, _mtx._32, -_mtx._33);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_FRONT_LEFT: {
		DirectionSet(_mtx, -45.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_FRONT_RIGHT: {
		DirectionSet(_mtx, 45.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_BACK_LEFT: {
		DirectionSet(_mtx, -135.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::DIRECTION_BACK_RIGHT: {
		DirectionSet(_mtx, 135.0f);
	}
	break;
	case PLAYER_SHOT_DIRECTION::END: {
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "弾の方向が正しく設定されていない箇所があります",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	break;
	default:
		break;
	}
}
