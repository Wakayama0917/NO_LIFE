#pragma once

//プレイヤーデータの項目
enum class PLAYER_STATUSID
{
    MODEL_NUM,              //モデルナンバー
    MOVE_SPEED,             //移動速度
    MAXHP,                  //最大体力
    HP,                     //体力
    ATTACK,                 //攻撃力
    BULLET_TYPE,            //弾の種類
    RELOAD,                 //次の弾を撃つまでの時間
    DIRECTION_FRONT,        //弾を撃つ方向
    DIRECTION_LEFT,         //弾を撃つ方向
    DIRECTION_RIGHT,        //弾を撃つ方向
    DIRECTION_BACK,         //弾を撃つ方向
    DIRECTION_FRONT_LEFT,   //左前
    DIRECTION_FRONT_RIGHT,  //右前
    DIRECTION_BACK_LEFT,    //左後ろ
    DIRECTION_BACK_RIGHT,   //右後ろ

    END,                    //最後の数値
};

//プレイヤーの弾方向の時にだけ使うリスト
enum class PLAYER_SHOT_DIRECTION
{
    DIRECTION_FRONT,     //弾を撃つ方向
    DIRECTION_LEFT,     //弾を撃つ方向
    DIRECTION_RIGHT,     //弾を撃つ方向
    DIRECTION_BACK,     //弾を撃つ方向
    DIRECTION_FRONT_LEFT,   //左前
    DIRECTION_FRONT_RIGHT,  //右前
    DIRECTION_BACK_LEFT,    //左後ろ
    DIRECTION_BACK_RIGHT,   //右後ろ

    END,                //最後の数値
};

//ヒット時の演出用フラグ
enum class PHitEffectFlg
{
    NONE,		    //何も無し
    HIT,			//敵の攻撃が当たった時
    DEATH,			//死亡したとき

    END,			//初期化用
};