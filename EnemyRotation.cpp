#include "EnemyRotation.h"
#include "drawaxis.h"
#include "imgui.h"


//初期化
bool EnemyRotation::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player) {
	//行列初期化
	DX11MtxIdentity(m_mtx);

	//当たり判定初期化
	CollisionCreateInit();

	//生きている設定
	m_state = STATE::LIVE;

	m_enemyNum = ENEMY_TYPE::ROTATION;//何の敵か設定

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

void EnemyRotation::EnemyMove()
{
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//AutoPilot();//プレイヤーで言うキー操作

	//以下飛ばす操作と同じ文
	//X軸を取り出す
	axisX.x = m_mtx._11;
	axisX.y = m_mtx._12;
	axisX.z = m_mtx._13;
	axisX.w = 0.0f;

	//Y軸を取り出す
	axisY.x = m_mtx._21;
	axisY.y = m_mtx._22;
	axisY.z = m_mtx._23;
	axisY.w = 0.0f;

	//Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;


	XMFLOAT4 qt;//クォータニオン

	//行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, qt);

	XMFLOAT4 qtx, qty, qtz;//クオータニオン

	//指定軸回転のクオータニオン生成
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	//クオータニオン合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	//クオータニオンをノーマライズ
	DX11QtNormalize(tempqt3, tempqt3);

	//クオータニオンから行列を作成
	DX11MtxFromQt(m_mtx, tempqt3);


	//Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	//常に移動させるために加算
	m_pos.x += axisZ.x * m_moveSpeed;
	m_pos.y += axisZ.y * m_moveSpeed;
	m_pos.z += axisZ.z * m_moveSpeed;

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

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

//void EnemyRotation::Draw(){
//	//モデル描画
//	m_model->Draw(m_mtx);
//	
//}

void EnemyRotation::DrawWithAxis(){
	//モデル描画
	drawaxis(m_mtx, 200, m_pos);
	//m_model->Draw(m_mtx);
}

void EnemyRotation::Update() {
	
	EnemyMove();
	BulletReload();

	
}

void EnemyRotation::Finalize(){
	//m_model->Uninit();
}