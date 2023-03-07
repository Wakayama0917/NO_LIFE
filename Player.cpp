#include "player.h"
#include "drawaxis.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "EnumListPlayer.h"
#include "imgui.h"
#include "Input.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Player::Init()
{
	// 行列初期化
	DX11MtxIdentity(m_mtx);
	m_pos = { 0, 0, 0 };

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);
	//当たり判定の実体化
	CollisionCreateInit();

	//拡大率をセット
	m_expand = 2.0f;

	//生きている設定
	m_state = STATE::LIVE;

	return true;
}

bool Player::Init(std::unique_ptr<CSVFile>& _csvFile) {
	// 行列初期化
	DX11MtxIdentity(m_mtx);
	m_pos = { 0, 0, 0 };

	// 行列初期化
	//DX11MtxIdentity(m_Quaternion);

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);
	//当たり判定の実体化
	CollisionCreateInit();

	//拡大率をセット
	m_expand = 2.0f;
	//m_collision->GetHitdata().r* m_expand;
	//生きている設定
	m_state = STATE::LIVE;

	PlayerCsvSet(_csvFile);

	//CSVセットしたのでセット後の初期化も行う
	SetModel();

	return true;
}

void Player::Reset()
{
	//ヒット演出の処理を初期化
	m_pHitEffectFlg = PHitEffectFlg::NONE;
}

void Player::Draw() {
	if (m_state == STATE::LIVE)
	{
		//大きさを変えるための処理
		/*static int Scalenum = 0;
		if (Scalenum == 0)
		{*/
		DirectX::XMFLOAT4X4 big;
		//DX11MtxScale(m_expand, m_expand, m_expand, big);//大きくするための4x4を作製
		DX11MtxMultiply(big, m_bigMtx, m_mtx);//先頭に後ろ2つをかけたものを入れる、現在位置
		//g_player.change4x4(big);//行列を書き換え

		/*Scalenum++;
	}*/
	// モデル描画
		ModelMgr::GetInstance().Draw(m_modelNum,big);
		//GetSphere().Draw();
	}

}

void Player::DrawWithAxis() {

	// モデル描画
	drawaxis(m_mtx, 200, m_pos);
	Draw();

}

void Player::Update()
{
	if (m_state == STATE::LIVE)
	{
		XMFLOAT2 L = Input::GetInstance().GetLStickPlayer();
		XMFLOAT2 R = Input::GetInstance().GetRStickPlayer();

		//キー入力左スティックの代わり
		if ((Input::GetInstance().CheckKeyPress(DIK_A)))  L.x = -1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_D)))  L.x = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_W)))  L.y = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_S)))  L.y = -1.0f;
		
		//キー入力右スティックの代わり
		if ((Input::GetInstance().CheckKeyPress(DIK_LEFT)))  R.x = -1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_RIGHT))) R.x = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_UP)))    R.y = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_DOWN)))  R.y = -1.0f;



		if ((L.x != 0) || (L.y != 0))//入力があれば
		{
			//angle.y += X / 5;
			//Keyinput = true;
			UpdatePlayerMove(L.x, L.y);
		}
		if ((R.x != 0) || (R.y != 0))//入力があれば
		{
			//Keyinput = true;
			UpdatePlayerDirection(R.x, R.y);
		}

		m_collision->UpdatePosition(m_mtx);//球表示
		BulletReload();//弾発射のカウント
	}
}

void Player::UpdatePlayerDirection(float _RX, float _RY)
{
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);// 行列からクオータニオンを生成

	//スティックの向き。YがZ
	DirectX::XMFLOAT3 RPosition;
	RPosition.x = _RX;
	RPosition.y = 0.0f;
	RPosition.z = _RY;

	//プレイヤーのベクトルZ。YがZ
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;

	DirectX::XMFLOAT4 TargetQt;					// ターゲット方向の姿勢


	DirectX::XMFLOAT3 ZDir = PPosition;			// プレイヤーの速度ベクトル（ミサイルZ方向の向き）

	DX11Vec3Normalize(RPosition, RPosition);	// 正規化
	DX11Vec3Normalize(ZDir, ZDir);					// 正規化

	float	Dot;										// ２本のベクトルの内積値（現在の進行方向とターゲット方向）
	TargetQt = RotationArc(ZDir, RPosition, Dot);	// ２本のベクトルから為す角度とクオータニオンを求める
	float angleDiff = acosf(Dot);						// ラジアン角度

	float m_AddRotMax = 360.0f;

	// プレイヤーの姿勢を決定する
	if (m_AddRotMax >= angleDiff) {
		 //1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が小さい場合
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が大きい場合
		float t = m_AddRotMax / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// クォータニオンを行列にする
	// 現在の姿勢をクオータニオンにする
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// 行列に位置座標をセット
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

void Player::BulletReload()
{
	if (m_reloadTime >= m_reload) {//リロード時間以上なら発射

		m_fireBulletFlg = true;//発射に変更
		m_reloadTime = 0;//リセット
	}
	else// if (m_HommingBulletWaittime < m_bulletReloadTime)//リロード時間
	{
		m_reloadTime += ImGui::GetIO().DeltaTime;
	}
}

void Player::Finalize() {
	//m_model->Uninit();
}

void Player::EnemyBulletReload()
{
	if (m_reloadTime >= m_reload) {//リロード時間以上なら発射

		m_fireBulletFlg = true;//発射に変更
		m_reloadTime = 0;//リセット
	}
	else// if (m_HommingBulletWaittime < m_bulletReloadTime)//リロード時間
	{
		m_reloadTime += ImGui::GetIO().DeltaTime;
	}
}

void Player::HPDecrease(float _damage)
{
	m_hp = m_hp - _damage;
	//0以下なら死亡
	if (m_hp <= 0)
	{
		//死亡演出フラグを立てる
		m_pHitEffectFlg = PHitEffectFlg::DEATH;
		//死亡
		m_state = STATE::DEAD;
		m_hp = 0;
	}
	else
	{
		//死亡演出フラグを立てる
		m_pHitEffectFlg = PHitEffectFlg::HIT;
	}
}

void Player::HPIncrease(float _value)
{
	m_hp = m_hp + _value;
	//最大値を超えていたら止める
	if (m_hp > m_maxhp)
	{
		m_hp = m_maxhp;
	}
}

void Player::UPMAXHP(float _value)
{
	m_maxhp += _value;//最大体力上昇
	m_hp += _value;//上昇した分回復
	//最大値を超えていたら止める
	if (m_hp > m_maxhp)
	{
		m_hp = m_maxhp;
	}
}

void Player::UPSpeed(float _value)
{
	m_moveSpeed += _value;
}

void Player::UPAttack(float _value)
{
	m_atk += _value;
}

void Player::UPFrontBullet(float _level)
{
	if (_level == 0)
	{
		m_shotDirectionFrontLeft = 1;	//左斜め前
		m_shotDirectionFrontRight = 1;	//右斜め前
	}
	else if(_level == 1)
	{
		m_shotDirectionLeft = 1;        //左
		m_shotDirectionRight = 1;       //右
	}
	
}

void Player::UPBackBullet(float _level)
{
	if (_level == 0)
	{
		m_shotDirectionBack = 1;          //後ろ
	}
	else if (_level == 1)
	{
		m_shotDirectionBackLeft = 1;      //左後ろ
		m_shotDirectionBackRight = 1;     //右後ろ
	}
}

void Player::UPReload(float _value)
{
	m_reload = (m_reload - _value);
}

void Player::SetPos(float _x, float _z)
{
	//pos更新
	m_pos.x = _x;
	m_pos.z = _z;

	//行列更新
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}

void Player::SetStartPos(float _x, float _z)
{
	//ステージの縦横幅を保存
	m_stageX = _x;
	m_stageZ = _z;

	//pos更新
	m_pos.x = 0;
	m_pos.z = -(_z / 2) + 10;

	//行列更新
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}

void Player::SetAxis(float _x, float _y)
{

	DX11GetQtfromMatrix(m_mtx, m_Quaternion);// 行列からクオータニオンを生成

	//スティックの向き。YがZ
	DirectX::XMFLOAT3 RPosition;
	RPosition.x = _x;
	RPosition.y = 0.0f;
	RPosition.z = _y;

	//プレイヤーのベクトルZ。YがZ
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;

	/*bool sts = false;
	float lng;*/

	DirectX::XMFLOAT4 TargetQt;					// ターゲット方向の姿勢


	DirectX::XMFLOAT3 ZDir = PPosition;			// ミサイルの速度ベクトル（ミサイルZ方向の向き）

	DX11Vec3Normalize(RPosition, RPosition);	// 正規化
	DX11Vec3Normalize(ZDir, ZDir);					// 正規化

	float	Dot;										// ２本のベクトルの内積値（現在の進行方向とターゲット方向）
	TargetQt = RotationArc(ZDir, RPosition, Dot);	// ２本のベクトルから為す角度とクオータニオンを求める
	float angleDiff = acosf(Dot);						// ラジアン角度

	float m_AddRotMax = 360.0f;

	//セットなので、１フレームの最大角度上限を考えない
	DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	
	// クォータニオンを行列にする
	// 現在の姿勢をクオータニオンにする
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// 行列に位置座標をセット
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

void Player::SetAllStatus(float _playerChangeData[static_cast<int>(PLAYER_STATUSID::END)])
{

	m_moveSpeed = _playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)];          //移動速度
	m_maxhp = _playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)];				//最大体力
	m_hp = _playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)];                 //体力
	m_atk = _playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)];              //攻撃力
	//int   m_bulletType = 0;         //弾の種類
	m_reload = _playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)];                 //次の弾を撃つまでの時間
}

void Player::SetPHitEffectFlg(PHitEffectFlg _pHitEffectFlg)
{
	//フラグ状態を変更
	m_pHitEffectFlg = _pHitEffectFlg;
}

bool Player::GetShotDirection(int _num)
{
	//各方向の真偽確認
	switch (_num)
	{
	case 0: {
		return m_shotDirectionFront;
		break;
	}
	case 1: {
		return m_shotDirectionLeft;
		break;
	}
	case 2: {
		return m_shotDirectionRight;
		break;
	}
	case 3: {
		return m_shotDirectionBack;
		break;
	}
	case 4: {
		return m_shotDirectionFrontLeft;
		break;
	}
	case 5: {
		return m_shotDirectionFrontRight;
		break;
	}
	case 6: {
		return m_shotDirectionBackLeft;
		break;
	}
	case 7: {
		return m_shotDirectionBackRight;
		break;
	}
	default:
		break;
	}

	return false;
}

float Player::GetCSVStatus(PLAYER_STATUSID _statusid)
{
	switch (_statusid)
	{
	case PLAYER_STATUSID::MOVE_SPEED: {
		return m_moveSpeed;
	}
		break;
	case PLAYER_STATUSID::MAXHP: {
		return m_maxhp;
	}
		break;
	case PLAYER_STATUSID::HP: {
		return m_hp;
	}
		break;
	case PLAYER_STATUSID::ATTACK: {
		return m_atk;
	}
		break;
	case PLAYER_STATUSID::BULLET_TYPE: {
		return m_bulletType;
	}
		break;
	case PLAYER_STATUSID::RELOAD: {
		return m_reload;
	}
		break;
	case PLAYER_STATUSID::DIRECTION_FRONT: {
		return m_shotDirectionFront;
	}
										break;
	case PLAYER_STATUSID::DIRECTION_LEFT: {
		return m_shotDirectionLeft;
	}
									   break;
	case PLAYER_STATUSID::DIRECTION_RIGHT: {
		return m_shotDirectionRight;
	}
										break;
	case PLAYER_STATUSID::DIRECTION_BACK: {
		return m_shotDirectionBack;
	}
									   break;
	case PLAYER_STATUSID::DIRECTION_FRONT_LEFT: {
		return m_shotDirectionFrontLeft;
	}
											 break;
	case PLAYER_STATUSID::DIRECTION_FRONT_RIGHT: {
		return m_shotDirectionFrontRight;
	}
											  break;
	case PLAYER_STATUSID::DIRECTION_BACK_LEFT: {
		return m_shotDirectionBackLeft;
	}
											break;
	case PLAYER_STATUSID::DIRECTION_BACK_RIGHT: {
		return m_shotDirectionBackRight;
	}
											 break;
	case PLAYER_STATUSID::END: {
		// 保険
		Texture tex;
		MessageBox(nullptr, "プレイヤーオブジェクト：無効な数値が入っています",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
		return 0.0f;
	}
	break;
	default:
		break;
	}
}

bool Player::PlayerCsvSet(std::unique_ptr<CSVFile>& _csvFile)
{
		//共通変数初期化
		//GetCell().at(この敵の種類列).at(値)
		m_modelNum = static_cast<ALLMODEL>(_csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MODEL_NUM)));//モデルナンバー
		m_moveSpeed = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MOVE_SPEED));//移動速度
		m_maxhp = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MAXHP));//体力
		m_hp = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::HP));//体力
		//m_atkSpeed = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ATK_SPEED));//攻撃速度
		m_atk = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::ATTACK));//攻撃力
		//m_rotation = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ROTATION));//回転するか
		//m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT));//１フレーム内の最大回転角度
		m_bulletType = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::BULLET_TYPE));//弾の種類
		m_reload = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::RELOAD));//弾の種類;
		m_shotDirectionFront = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT));//弾を撃つ方向
		m_shotDirectionLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_LEFT));//弾を撃つ方向
		m_shotDirectionRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_RIGHT));//弾を撃つ方向
		m_shotDirectionBack = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK));//弾を撃つ方向
		m_shotDirectionFrontLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT_LEFT));//弾を撃つ方向
		m_shotDirectionFrontRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT_RIGHT));//弾を撃つ方向
		m_shotDirectionBackLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK_LEFT));//弾を撃つ方向
		m_shotDirectionBackRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK_RIGHT));//弾を撃つ方向
	
	return true;
}

void Player::UpdatePlayerMove(float _LX, float _LY){
	
	//移動を加算
	m_pos.x += m_moveSpeed * _LX;
	m_pos.z += m_moveSpeed * _LY;

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
	
	//位置を更新
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}




