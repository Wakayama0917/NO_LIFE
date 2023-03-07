#pragma once

//ステージデータの項目
enum class STAGE_STATUSID
{
    STAGE_MODEL_NUM,        //ステージモデルナンバー
    SKY_MODEL_NUM,          //スカイモデルナンバー
    STAGE_X,                //ステージX幅
    STAGE_Z,                //ステージZ幅
    ENEMY_00,               //ENEMY_TYPE０番目
    ENEMY_01,               //ENEMY_TYPE1番目
    ENEMY_02,               //ENEMY_TYPE2番目
    ENEMY_BOSS1,               //ENEMY_TYPE2番目
    
    END,                    //最後の数値
};

//台とスカイドームのナンバー
enum class MODEL_SS
{
	STAGE_NORMAL1,
	STAGE_BOSS1,
	SKY_NORMAL1,
	SKY_BOSS1,
};



//ステージの最大数、０からのカウントになっている
#define MAXSTAGE 9
