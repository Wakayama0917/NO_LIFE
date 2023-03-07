#pragma once
#include "CBillBoard.h"

class BillBoard
{
public:
	//シングルトン
	static BillBoard* GetInstance() {
		static BillBoard instance;
		return &instance;
	}

private:
	//敵エフェクト
	std::vector<std::unique_ptr<CBillBoard>> m_bEnemyEffect;
	//敵UI(HPバーなど)
	std::vector<std::unique_ptr<CBillBoard>> m_bEnemyUI;
	//角度を指定するためのマトリックス
	XMFLOAT4X4 m_mtxEffect;

public:
	
	//敵のエフェクトに使う画像
	enum class BEnemyEffectID
	{
		RING,		    //地面に出るリング
		RINGHALF,		//斜めリング
		DEATH,			//死亡時のエフェクト
	
		END,			//最後
	};

	//敵に使うエフェクト以外の画像
	enum class BEnemyUIID
	{
		HPBAR,			//HPバー

		END,			//最後
	};
	
public:
	/*************************************************************
	* 全てに共通して使う関数
	*************************************************************/
	//全てを初期読み込み、一度のみ
	void Init();
	//削除
	void Finalize();

public:
	/*************************************************************
	* 敵のエフェクトに使う関数、EEはエネミーエフェクトの略
	*************************************************************/
	//リセット、ヒット時に行う。
	void EEReset();
	//ヒット時のエフェクト
	void EEHit(XMFLOAT3 _pos);
	//死亡時のエフェクト
	void EEDeath(int& _imageNum, int _count, XMFLOAT3 _pos);
	
	/*************************************************************
	* 敵のUI系
	*************************************************************/
	//敵のHPバーの表示
	void EHPBer(int _hp, int _maxhp, XMFLOAT3 _pos);

};

