#pragma once
#include "gameobject.h"
#include "EnumListBullet.h"

class BulletObject : public GameObject
{
protected:
	/*************************************************************
	* CSVで初期化する変数
	*************************************************************/
	float m_attackSpeed = 1.0f;       //攻撃速度
	//float m_reload;            //次の弾を撃つまでの時間
	float m_attackPower;       //攻撃力
	float m_liveTime = 300;          //生存時間
	float m_maxRot = 5.0f;            //１フレーム内の最大回転角度
	//float m_expand;              //拡大率、GameObjectの中に入ってる
	float m_radius;            //半径、当たり判定用

	/*************************************************************
	* その他の変数
	*************************************************************/
	BULLET_TYPE m_bulletType;//自分がなんの弾かを設定
	DirectX::XMFLOAT3	m_direction;	// 発射方向(移動時に加算する値)

	

public:
	/*************************************************************
	* 関数類
	*************************************************************/
	//m_enemyNumが初期化されていないとメッセージ表記して止まる
	bool BulletCsvSet(CSVFile _csvFile);//CSV受け取り
	void SetAllStatus(float _value[static_cast<int>(BULLET_STATUSID::END)]);
	void SetAllStatus(std::vector<float> _value);
	void DirectionSet(XMFLOAT4X4 _mtx, float _angle);//弾の発射方向セット

	/*************************************************************
	* SET、GET系関数類
	*************************************************************/
	
	
	//CSVで読み取る数値を取得する
	float GetCSVStatus(BULLET_STATUSID _statusid);
	


    /*************************************************************
    * バーチャル関数類
    *************************************************************/
	//凡庸
	virtual bool Init() = 0;
	virtual void Draw();
	virtual void Update() = 0;
	//virtual void Update(unique_ptr<EnemyObject>& _enemyObject) = 0;
	virtual void Finalize() { /*m_model->Uninit();*/ };

	//特殊
	virtual void SetTarget(DirectX::XMFLOAT3 _targetpos) {};
	virtual void SetDirection(DirectX::XMFLOAT4X4 _mtx) {};
	virtual bool isLive() = 0;
	virtual void Move(const DirectX::XMFLOAT3 _targetPosition) = 0;

private:

};

