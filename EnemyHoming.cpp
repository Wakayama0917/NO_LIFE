#include "EnemyHoming.h"
#include "drawaxis.h"


bool EnemyHoming::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player)
{
	//行列初期化
	DX11MtxIdentity(m_mtx);

	//m_actionKeepTime = 10;

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);

	//当たり判定初期化
	CollisionCreateInit();

	//生きている設定
	m_state = STATE::LIVE;

	m_enemyNum = ENEMY_TYPE::HOMMING;//何の敵か設定

	EnemyCsvSet(*_csvFile);//ステータス初期化

	
	//CSVセットしたのでセット後の初期化も行う
	SetModel();
	

	//プレイヤーオブジェクトのポインタを格納
	if (_player != nullptr)
	{
		m_sPlayer = _player;
	}
	else
	{
		//初期化されていないのでメッセージを表示
		Texture tex;
		MessageBox(nullptr, "プレイヤーの中身がNULLです",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;

		return false;
	}

	return true;
}

//void EnemyHoming::Draw() {
//	//モデル描画
//	m_model->Draw(m_mtx);
//}

void EnemyHoming::DrawWithAxis() {
	//モデル描画
	drawaxis(m_mtx, 200, m_pos);//棒を出すやつ
	//m_model->Draw(m_mtx);
}

/*----------------------------------------
	エネミーのデータを更新する
	ターゲット位置と数値を入れている

	(ターゲットとの距離がlngより小さくなったらtrueを返す)
-----------------------------------------*/
void EnemyHoming::Update() {
	EnemyMove();
	BulletReload();
}

void EnemyHoming::Finalize() {
	//m_model->Uninit();
}

void EnemyHoming::EnemyMove()
{
	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;					// ターゲット方向の姿勢
	DirectX::XMFLOAT3 TargetVector;
	//ホーミングエネミーのベクトルZ軸。YがZ
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;


	TargetVector.x = m_sPlayer->GetMtx()._41 - m_pos.x;
	TargetVector.y = m_sPlayer->GetMtx()._42 - m_pos.y;
	TargetVector.z = m_sPlayer->GetMtx()._43 - m_pos.z;
	

	DX11Vec3Length(TargetVector, lng);
	//if (lng <= m_sPlayer->GetRadius()) {
	//	//あたっているので真を返す
	//	//return true;
	//}

	DirectX::XMFLOAT3 ZDir = PPosition;			// ミサイルの速度ベクトル（ミサイルZ方向の向き）

	DX11Vec3Normalize(TargetVector, TargetVector);	// 正規化
	DX11Vec3Normalize(ZDir, ZDir);					// 正規化

	float	Dot;										// ２本のベクトルの内積値（現在の進行方向とターゲット方向）
	TargetQt = RotationArc(ZDir, TargetVector, Dot);	// ２本のベクトルから為す角度とクオータニオンを求める
	float angleDiff = acosf(Dot);						// ラジアン角度


	// エネミーの姿勢を決定する
	if (m_maxRot >= angleDiff) {
		// 1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が小さい場合
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が大きい場合
		float t = m_maxRot / angleDiff;
		//float t = m_maxRot;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// クォータニオンを行列にする
	// 現在の姿勢をクオータニオンにする
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// ミサイルの進行方向にスピードを掛けて移動量（速度ベクトルを求める）
	PPosition.x = (m_mtx._31 * m_moveSpeed);
	PPosition.y = (m_mtx._32 * m_moveSpeed);
	PPosition.z = (m_mtx._33 * m_moveSpeed);

	// 位置を更新（速度ベクトルで更新）
	m_pos.x += PPosition.x;
	m_pos.y += PPosition.y;
	m_pos.z += PPosition.z;

	//エリア外に出ないように調整
	if (m_pos.x > (m_stageX / 2))
	{
		m_pos.x = (m_stageX / 2);
	}
	if (m_pos.x < -(m_stageX / 2))
	{
		m_pos.x = -(m_stageX / 2);
	}
	if (m_pos.z > (m_stageZ / 2))
	{
		m_pos.z = (m_stageZ / 2);
	}
	if (m_pos.z < -(m_stageZ / 2))
	{
		m_pos.z = -(m_stageZ / 2);
	}

	// 行列に位置座標をセット
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	//return sts;


	
}

