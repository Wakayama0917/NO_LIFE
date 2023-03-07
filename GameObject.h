#pragma once
#include <directxmath.h>
#include <memory>
#include "dx11mathutil.h"
#include "BoundingSphere.h"
#include "CModel.h"
#include "CSV.h"
#include "EnumAllModel.h"

using namespace std;

//生きてるかどうか
enum  class STATE
{
	LIVE,
	DEAD
};

class GameObject {
	uint64_t m_id = 0;						//識別子
	
protected:
	/*************************************************************
	* ステータス関連の変数、関数
	*************************************************************/
	STATE		m_state;//ステータス
public:
	//現在ステータス（生きてる、死んでる）を返す
	STATE GetState() {
		return m_state;
	}
	//ステータス状態をセット
	void SetState(STATE _status) {
		m_state = _status;
	}
	//ステータスを死亡に変える
	void SetDead() { m_state = STATE::DEAD; };
protected:
	/*************************************************************
	* 位置や姿勢系変数
	*************************************************************/
	DirectX::XMFLOAT3 m_pos{};				//位置
	DirectX::XMFLOAT4X4 m_mtx{};			//姿勢
	float m_expand = 1.0f;					//拡大率
	DirectX::XMFLOAT4X4 m_bigMtx;			//拡大率をかけた行列
	
public:
	/*************************************************************
	* 位置や姿勢系関数
	*************************************************************/
	//＝０はエラーを出してくれる
	GameObject() {};
	virtual ~GameObject() {}

	//２つのベクトルからクオータニオンを作成する
	DirectX::XMFLOAT4 RotationArc(DirectX::XMFLOAT3 _v0, DirectX::XMFLOAT3 _v1, float& _d);

	// 姿勢を取得
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// 位置を取得
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// ＩＤを取得
	uint64_t GetID() {
		return m_id;
	}

	/*************************************************************
	* 当たり判定系
	*************************************************************/
protected:
	// 境界球プレイヤー、スマートポインタ―
	std::unique_ptr<BoundingSphere> m_collision;
	//所有権自体を放棄する場合は、release関数を使う
	//この場合、メモリの解放自体は自分で行う必要がある
	//pint = ptr.release();
	//delete pint;
public:
	//実体化、各自のInit時に呼び出す
	void CollisionCreateInit() {
		if (m_collision == nullptr)
		{
			//std::unique_ptr<BoundingSphere> ptr;
			m_collision = std::make_unique<BoundingSphere>();
		}
		else
		{
			// 正常に行われなかった場合
			Texture tex;
			MessageBox(nullptr, "スフィアオブジェクトが生成できません",
				"Error!", MB_ICONERROR | MB_OK);
			tex.texture = nullptr;
		}
		
	};

	//当たり判定初期化処理、モデル読み込み後に使用
	void CollisionInitOld();
	void CollisionInit();

	//当たり判定返すよう
	BoundingSphere GetSphere() {
		return *m_collision;
		//return *m_collision2;
	}

	//当たり判定の際の半径を返すよう関数
	float GetRadius() {
		//拡大していた場合、その分かける
		return m_collision->GetHitdata().r * m_expand;
	}

	//自身と相手の当たり判定を取る
	float Distance(const DirectX::XMFLOAT3 _targetPosition);

	/*************************************************************
	* モデル系
	*************************************************************/
protected:
	//全てのオブジェクトに必要な情報
	//この番号のモデルの当たり判定を生成し、描画する。
	//ENDで初期化、ENDが入っているとエラーが起きるように制作
	ALLMODEL m_modelNum = ALLMODEL::END;//このモデルの番号

	//CModel* m_model;//モデル
public:
	ALLMODEL GetModelNum() {
		return m_modelNum;
	}

	//void SetModel(CModel* _p) {
	//	m_model = _p;

	//	//以下当たり判定の初期化、拡大率設定済みなので処理
	//	//当たり判定初期化
	//	CollisionInitOld();
	//	//描画時のモデルの大きさを決定
	//	DX11MtxScale(m_expand, m_expand, m_expand, m_bigMtx);//大きくするための4x4を作製

	//}
	void SetModel();

	/*const CModel* GetModel() {
		return m_model;
	}*/

};