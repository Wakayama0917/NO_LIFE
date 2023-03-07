#pragma once

//モデルのナンバー
enum class ALLMODEL
{
	//ステージ系のモデル
	STAGE_NORMAL1,				//通常
	STAGE_BOSS1,				//ボス
	SKY_NORMAL1,				//通常
	SKY_BOSS1,					//ボス
	//プレイヤーモデル
	PLAYER,						//プレイヤーモデル
	//敵のモデル
	ENEMY_CYLINDER,				//円柱型
	ENEMY_TRIANGULAR_PYRAMID,	//円錐型
	//弾のモデル
	BULLET_PLAYER,				//プレイヤーの弾
	BULLET_NORMAL,				//球（赤）
	BULLET_HARD,				//球（紫）
	BULLET_TRIANGULAR_PRISM,    //三角柱
	//最後の項目
	END,
};
