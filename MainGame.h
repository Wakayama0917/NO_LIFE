#pragma once
#include "SceneBase.h"
#include "CBillBoard.h"
#include "Quad2d.h"
#include "CSV.h"
#include "EnemyManager.h"
#include "Stand.h"
#include "Skydome.h"
#include "Player.h"
#include "bulletmanager.h"
#include "EnumListGameState.h"
#include "EnumListSkill.h"
#include "SkillManager.h"



class Game : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();

private:
	//メインで使用するCSVファイルのリスト
	enum class CSVID
	{
		//START,		//最初の数値
		PLAYER,			//プレイヤー
		ENEMY,			//敵
		BULLET,			//弾
		STAGE,			//ステージ（大きさ、位置など）
		MODEL_SS,		//ステージ、スカイドームのモデルデータ
		SKILL,			//スキルデータ
		END,			//最後
	};
	std::vector<std::unique_ptr<CSVFile>> m_csvData;

	//台
	std::unique_ptr<Stand> m_stand;
	// スカイドームオブジェクト生成
	std::unique_ptr<Skydome> m_skydome;
	//プレイヤー、ポインタを各敵に持たせる
	std::shared_ptr<Player> m_player;
	//エネミーマネージャー生成
	std::unique_ptr<EnemyManager> m_enemyManager;
	//バレットマネージャー生成
	std::unique_ptr<BulletManager> m_bulletManager;
	//スキルマネージャー生成
	std::unique_ptr<SkillManager> m_skillManager;
	//各ステージの敵の種類を格納する配列
	std::vector<ENEMY_TYPE> m_enemyList;
	

	int m_stageNumber = 0;//現在のステージ数
	GAMESTATE m_gameState = GAMESTATE::INIT;//現在のメインゲーム中の状態がどこか
	bool m_fadeFlg = false;

	//使用するUIのリスト
	enum class GAMEUIID
	{
		HPFLAME,		//HPフレーム
		HPBAR,			//増えたり減ったりするバー
		HPLOGO,			//HPロゴマーク

		CLEAR,			//クリアロゴ
		GAMEOVER,		//ゲームオーバーロゴ

		NOISE,			//ノイズ

		NUMBER,			//数字

		END,			//最後
	};
	//UI生成
	std::vector<std::unique_ptr<Quad2D>> m_ui;
	

};




