#include <memory>
#include "HommingMissile.h"
#include "dx11mathutil.h"
#include "CModel.h"
#include "CDirectInput.h"
#include "ModelMgr.h"

constexpr float BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;


bool HomingMissile::Init() {

	bool sts = true;

	// 行列初期化
	DX11MtxIdentity(m_mtx);
	//当たり判定の実体化
	CollisionCreateInit();

	return sts;
}

//void HomingMissile::Draw() {
//
//	// モデル描画
//	if (m_state == STATE::LIVE) {
//		m_model->Draw(m_mtx);
//	}
//}

void HomingMissile::Update() {

	m_liveTime--;
	if (m_liveTime > 0) {

		// ターゲットにむけて位置を更新
		//Move(m_target);

	}
	else {
		m_state = STATE::DEAD;
	}

}

void HomingMissile::Finalize() {

}

/*----------------------------------------
	ミサイルのデータを更新する
	ターゲット位置と数値を入れている

	(ターゲットとの距離がlngより小さくなったらtrueを返す)
-----------------------------------------*/
void HomingMissile::Move(const DirectX::XMFLOAT3 _targetPosition) {

	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;					// ターゲット方向の姿勢

	DirectX::XMFLOAT3 TargetVector;

	// 現在位置からターゲット方向へのベクトルを求める
	TargetVector.x = _targetPosition.x - m_pos.x;
	TargetVector.y = _targetPosition.y - m_pos.y;
	TargetVector.z = _targetPosition.z - m_pos.z;

	//DX11Vec3Length(TargetVector, lng);

	//lng = Distance(_targetPosition);
	//if (lng <= _l) {
	//	//あたっているので真を返す
	//	return true;
	//}

	DirectX::XMFLOAT3 ZDir = m_Velocity;			// ミサイルの速度ベクトル（ミサイルZ方向の向き）

	DX11Vec3Normalize(TargetVector, TargetVector);	// 正規化
	DX11Vec3Normalize(ZDir, ZDir);					// 正規化

	float	Dot;										// ２本のベクトルの内積値（現在の進行方向とターゲット方向）
	TargetQt = RotationArc(ZDir, TargetVector, Dot);	// ２本のベクトルから為す角度とクオータニオンを求める
	float angleDiff = acosf(Dot);						// ラジアン角度

	
	// ミサイルの姿勢を決定する
	if (m_maxRot >= angleDiff) {
		// 1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が小さい場合
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が大きい場合
		float t = m_maxRot / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// クォータニオンを行列にする
	// 現在の姿勢をクオータニオンにする
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// ミサイルの進行方向にスピードを掛けて移動量（速度ベクトルを求める）
	m_Velocity.x = m_mtx._31*m_attackSpeed;
	m_Velocity.y = m_mtx._32*m_attackSpeed;
	m_Velocity.z = m_mtx._33*m_attackSpeed;

	// 位置を更新（速度ベクトルで更新）
	m_pos.x += m_Velocity.x;
	m_pos.y += m_Velocity.y;
	m_pos.z += m_Velocity.z;

	// 行列に位置座標をセット
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	//return sts;
}
