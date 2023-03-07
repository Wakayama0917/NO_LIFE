#pragma once
#include "gameobject.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "EnumListPlayer.h"



class Player :public GameObject {
private:
	/*************************************************************
   * CSV用変数
   *************************************************************/
	float m_moveSpeed = 0;          //移動速度
	float m_maxhp = 0;				//最大体力
	float m_hp = 0;                 //体力
	float m_atk = 0;              //攻撃力
	//bool  m_rotation = false;       //回転するか
	//float m_maxRot = 0;             //１フレーム内の最大回転角度
	int   m_bulletType = 0;         //弾の種類
	float m_reload;                 //次の弾を撃つまでの時間
	//弾を撃つ方向
	int   m_shotDirectionFront = 0;         //前
	int   m_shotDirectionLeft = 0;          //左
	int   m_shotDirectionRight = 0;         //右
	int   m_shotDirectionBack = 0;          //後ろ
	int   m_shotDirectionFrontLeft = 0;     //左前
	int   m_shotDirectionFrontRight = 0;    //右前
	int   m_shotDirectionBackLeft = 0;      //左後ろ
	int   m_shotDirectionBackRight = 0;     //右後ろ


	/*************************************************************
   * 変数
   *************************************************************/
private:
	PHitEffectFlg m_pHitEffectFlg = PHitEffectFlg::NONE;

	// 現在の姿勢を表すクォータニオン
	DirectX::XMFLOAT4 m_Quaternion;
	//弾を打つか判定する用のフラグ
	bool m_fireBulletFlg;
	//bool m_fireBulletFlg[8];//各方向ごとにリロード時間を変える場合
	// 
	//リロード時間用、デルタタイムを入れる
	float m_reloadTime = 0.0f;

	//ステージの幅を記憶する用の変数
	float m_stageX = 0;
	float m_stageZ = 0;
public:
	/*************************************************************
   * 基本的な関数
   *************************************************************/
	
	bool Init();//初期化CSV設定なしの場合
	bool Init(std::unique_ptr<CSVFile>& _csvFile);
	void Reset();//次のステージに行く際の初期化

	void Draw();

	void DrawWithAxis();

	void Update();

	void Finalize();
	/*************************************************************
   * 対戦時に使用する関数
   *************************************************************/
	void FireBulletFlgFalse() {
		//フラグを折る、撃った後など
		m_fireBulletFlg = false;
	}
	//デルタタイムを加算して発射するかを決める関数
	void EnemyBulletReload();
	//敵や弾に当たった時の関数、引数分体力が減る
	//ヒット時の演出フラグも立てている
	void HPDecrease(float _damage);

	/*************************************************************
   * スキル獲得時に使用する関数
   *************************************************************/
	//回復する関数
	void HPIncrease(float _value);
	//最大HPUP
	void UPMAXHP(float _value);
	//移動速度UP
	void UPSpeed(float _value);
	// 攻撃力UP
	void UPAttack(float _value);
	//リロード速度アップ
	void UPReload(float _value);
	//前方向弾追加、レベルを受け取る
	void UPFrontBullet(float _level);
	//後ろ方向弾追加、レベルを受け取る
	void UPBackBullet(float _level);
	
	

	 /*************************************************************
	*　GET SET関数
	*************************************************************/
	//SET
	void SetPos(float _x, float _z);	//ポジションセット
	void SetStartPos(float _x, float _z);	//ゲーム開始時、ステージの縦横幅を受け取りセット
	void SetAxis(float _x,float _y);	//向きをセット
	void SetAllStatus(float _playerChangeData[static_cast<int>(PLAYER_STATUSID::END)]);
	void SetPHitEffectFlg(PHitEffectFlg _pHitEffectFlg);

	//GET
	//各方向の真偽を取る
	bool GetShotDirection(int _num);
	//ゲット系
	int GetBulletType() {
		//弾の種類
		return m_bulletType;
	}
	bool GetFireBulletFlg()
	{
		//発射するか
		return m_fireBulletFlg;
	}
	//CSVで読み取る数値を取得する
	float GetCSVStatus(PLAYER_STATUSID _statusid);
	//ヒット時の演出フラグを取得
	PHitEffectFlg GetPHitEffectFlg() { return m_pHitEffectFlg; };

private:
   /*************************************************************
   * 関数類プライベート
   *************************************************************/
   //m_enemyNumが初期化されていないとメッセージ表記して止まる
	bool PlayerCsvSet(std::unique_ptr<CSVFile>& _csvFile);//CSV受け取り

	//void Advance(bool _keyInput,XMFLOAT3 _angle);//進む処理の部分
	void UpdatePlayerMove(float _LX, float _LY);//進む処理の部分
	void UpdatePlayerDirection(float _RX, float _RY);
	//デルタタイムを加算して発射するかを決める関数
	void BulletReload();//次の球を発射するかの関数

};