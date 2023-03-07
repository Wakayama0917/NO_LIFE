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
#include "SkillManager.h"
#include "EnumListGameState.h"
#include "EnumListSkill.h"

class Editor : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();

private:
	//エディターで使用するCSVファイルのリスト
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
	//弾の情報を入れるCSV
	std::unique_ptr<CSVFile> m_csvBulletData;

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

	enum class IMGUI_STATE
	{
		PLAYER,		//プレイヤーエディット
		SKILL,		//スキルエディット
		ENEMY,		//エネミーエディット
		BULLET,		//弾エディット
		STAGE,		//ステージエディット
		END,		//最後の項目
	};
	//現在どこのエディターシーンを操作しているか。
	IMGUI_STATE m_guiState = IMGUI_STATE::PLAYER;

	
	//基準として持っておく、カメラなどに使用する
	XMFLOAT4X4 m_mtx;
	bool m_ResetFlg[static_cast<int>(IMGUI_STATE::END)];//初期化処理を行うかどうかのフラグ
	bool m_saveFlg = false;//セーブするかどうかのフラグ

	//プレイヤーステータス変更時に使用
	float m_playerChangeData[static_cast<int>(PLAYER_STATUSID::END)] = { 0 };
	//スキルステータス変更時に使用
	std::vector<std::vector<float>> m_skillChangeData;
	int m_choiceSkill = 0;//何のスキルを選択しているか
	
	//敵のステータス変更時に使用する
	std::vector<std::vector<float>> m_enemyChangeData;
	int m_choiceEnemy = 0;//何の敵を選択しているか
	int m_choiceEnemyOld = 0;//何の敵を選択しているか
	//現在のフレームのm_choiceEnemyとm_choiceEnemyPreviousを比べ同じならそのまま
	//違えば、変更したことになるので初期化処理を行う為の変数
	int m_choiceEnemyPrevious = 0;
	std::vector<ENEMY_TYPE> m_enemyArray;//敵の配列、実体化させる用
	
	//弾のステータス変更時に使用する
	std::vector<std::vector<float>> m_bulletChangeData;
	int m_choiceBullet = 0;//何の弾を選択しているか

	//ステージのステータス変更時に使用する
	std::vector<std::vector<float>> m_stageChangeData;
	int m_stageNumber = 0;//現在のステージ数
	bool m_resetFlg = 0;
	
	
	

};


