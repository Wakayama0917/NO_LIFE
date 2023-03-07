#pragma once

//スキルの項目
enum class SKILL_STATUSID
{
	UP_VALUE,		//上昇させる値
	LEVEL,			//現在レベル
	LEVELMAX,		//最大レベル
	RATE,			//当選確率
			
	END,			//最後の項目
};

//スキル画面で使うオブジェクトのリスト
enum class SKILLTYPEID
{
	BACKGROUND,		//背景
	FLAME,			//選択している項目を表すフレーム
	CHARACTER1,		//キャラクター１

	MAXHPUP_LITTLE,	//僅かに最大体力を上昇、全部引いた後の外れスキル

	ATKUP,			//攻撃力UP
	HEEL,			//回復
	MAXHPUP,		//最大体力UP
	SPEEDUP,		//移動速度UP
	RELOADUP,		//次の弾発射までの感覚をUP
	BULLETFRONT,	//前方向の弾を追加
	BULLETBACK,		//後ろ方向の弾を追加


	END,			//最後
};
