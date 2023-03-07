#include "Boss1.h"
#include "drawaxis.h"


bool Boss1::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player)
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

	m_enemyNum = ENEMY_TYPE::BOSS1;//何の敵か設定

	EnemyCsvSet(*_csvFile);//ステータス初期化

	m_modelNum = ALLMODEL::BULLET_HARD;//モデルを設定

	//大きさを３倍にする
	m_expand = 3.0f;
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

void Boss1::DrawWithAxis() {
	//モデル描画
	drawaxis(m_mtx, 200, m_pos);//棒を出すやつ
	//m_model->Draw(m_mtx);
}

/*----------------------------------------
	エネミーのデータを更新する
	ターゲット位置と数値を入れている

	(ターゲットとの距離がlngより小さくなったらtrueを返す)
-----------------------------------------*/
void Boss1::Update() {
	EnemyMove();
	BulletReload();
}

void Boss1::Finalize() {
	//m_model->Uninit();
}

void Boss1::EnemyMove()
{
	//ここから改造
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;
	XMFLOAT4X4 mtx = m_mtx;
	XMFLOAT4 qt, qty;//クォータニオン

	axisZ.x = mtx._21;
	axisZ.y = mtx._22;
	axisZ.z = mtx._23;
	axisZ.w = 0.0f;

	//行列からクオータニオンを生成
	DX11GetQtfromMatrix(mtx, qt);

	DX11QtRotationAxis(qty, axisZ, -0.5);//指定確度分変える

	//クオータニオン合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qty);

	//クオータニオンをノーマライズ
	DX11QtNormalize(tempqt1, tempqt1);

	//クオータニオンから行列を作成
	DX11MtxFromQt(mtx, tempqt1);

	m_mtx._11 = mtx._11;
	m_mtx._12 = mtx._12;
	m_mtx._13 = mtx._13;

	m_mtx._21 = mtx._21;
	m_mtx._22 = mtx._22;
	m_mtx._23 = mtx._23;

	m_mtx._31 = mtx._31;
	m_mtx._32 = mtx._32;
	m_mtx._33 = mtx._33;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_mtx._14 = 0;
	m_mtx._24 = 0;
	m_mtx._34 = 0;
	m_mtx._44 = 1;
}
