#pragma once

//エネミーのナンバリング
enum class BULLET_TYPE
{
    NORMAL,     //敵通常弾
    HARD,       //破壊できない通常弾
    HOMMING,    //追尾弾
    NONE,       //何も無し
    P_NORMAL,   //プレイヤーのノーマル弾

    END,        //最後、エラー確認用
};

//エネミーデータの項目
enum class BULLET_STATUSID
{
    MODEL_NUM,          //モデルナンバー
    ATTACK_SPEED,       //攻撃速度
    ATTACK_POWER,       //攻撃力
    LIVE_TIME,          //生存時間
    MAX_ROT,            //１フレーム内の最大回転角度
    SIZE,               //拡大率
    RADIUS,             //半径、当たり判定用

    END,                //最後の数値
};


