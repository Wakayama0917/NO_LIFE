#pragma once

//エネミーのナンバリング
enum class ENEMY_TYPE
{
    ROTATION,     //回転する敵
    LOOP,       //周回する敵
    HOMMING,    //追尾する敵
    BOSS1,       //ボス１

    END,        //最後、エラー確認用
};

//エネミーデータの項目
enum class ENEMY_STATUSID
{
    //MODEL_NAME,         //モデルファイルパス
    //TEX_FOLDER,         //テクスチャパス
    MODEL_NUM,              //モデルナンバー
    MOVE_SPEED,             //移動速度
    HP,                     //体力
    MAX_HP,                 //最大体力
    ATTACK,                 //攻撃力
    ROTATION,               //回転するか
    MAX_ROT,                //１フレーム内の最大回転角度
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
    
    END,                //最後の数値
};

//エネミーの弾方向の時にだけ使うリスト
enum class ENEMY_SHOT_DIRECTION
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
