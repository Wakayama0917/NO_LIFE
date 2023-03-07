#pragma once
//#include "dx11mathutil.h"
#include "gameobject.h"
#include "Player.h"
#include "EnumListEnemy.h"
#include "CBillBoard.h"

//class CBillBoard;

class EnemyObject : public GameObject
{

protected:

    /*************************************************************
   * 変数
   *************************************************************/
    //自分が何の敵かを設定
    ENEMY_TYPE m_enemyNum = ENEMY_TYPE::END;

    //プレイヤーのポインタを格納するよう
    std::shared_ptr<Player> m_sPlayer;

    //弾を打つか判定する用のフラグ
    bool m_fireBulletFlg = false;
    //bool m_fireBulletFlg[8];//各方向ごとにリロード時間を変える場合
   
    //リロード時間用、デルタタイムを入れる
    float m_reloadTime = 0;

    //ステージの幅を記憶する用の変数
    float m_stageX = 0;
    float m_stageZ = 0;

    /*************************************************************
     * エフェクト類に使う変数
    *************************************************************/
    //使用するUIのリスト
    //enum class GAMEUIID
    //{
    //    RING,		    //地面に出るリング
    //    RINGHALF,		//斜めリング
    //    Delete,			//死亡時のエフェクト

    //    END,			//最後
    //};
    ////ビルボード
    //std::vector<std::unique_ptr<CBillBoard>> m_billBoard;
    //エフェクトの進行カウント
    int m_effectCount = 0;
    //エフェクト処理に入るかのフラグ
    bool m_effectFlg = 0;
    //基準として持っておく、UIの描画向きに利用
    XMFLOAT4X4 m_mtxEffect;
    //アニメーションの際に何枚目を使うかの変数
    int m_imageNum = 0;
    

    /*************************************************************
    * CSVから受け取る変数
    *************************************************************/
    float m_moveSpeed = 0;          //移動速度
    float m_hp = 0;                 //体力
    float m_maxHP;                  //敵の最大HP
    float m_atk = 0;                //攻撃力
    //float m_atkSpeed = 0;         //攻撃速度
    //float m_atk = 0;              //攻撃力
    bool  m_rotation = false;       //回転するか
    float m_maxRot = 0;             //１フレーム内の最大回転角度
    int   m_bulletType = 0;         //弾の種類
    float m_reload = 0;             //次の弾を撃つまでの時間
    //弾を撃つ方向
    int   m_shotDirectionFront = 0;         //前
    int   m_shotDirectionLeft = 0;          //左
    int   m_shotDirectionRight = 0;         //右
    int   m_shotDirectionBack = 0;          //後ろ
    int   m_shotDirectionFrontLeft = 0;     //左前
    int   m_shotDirectionFrontRight = 0;    //右前
    int   m_shotDirectionBackLeft = 0;      //左後ろ
    int   m_shotDirectionBackRight = 0;     //右後ろ

public:
    /*************************************************************
    * 関数類
    *************************************************************/
    //m_enemyNumが初期化されていないとメッセージ表記して止まる
    bool EnemyCsvSet(CSVFile _csvFile);//CSV受け取り
    void FireBulletFlgFalse() {
        //フラグを折る、撃った後など
        m_fireBulletFlg = false;
    }
   
    //リロード時間をセット、リロード時間がCSVからセットされた後じゃないと０になる。
    void ReloadInit();

    //ダメージを受ける関数
    void HPDecrease(float _damage);
    //回復する関数
    //void HPIncrease(float _damage);

    /*************************************************************
    * エフェクト系関数
    *************************************************************/
    //リセット、ヒット時に行う。
    void EffectReset();
    //描画
    void EffectDraw();

    /*************************************************************
    * SET、GET関数
    *************************************************************/
    //セット系
    //ポジションセット
    void SetPos(float _x, float _z);	
    //ゲーム開始時、ステージの縦横幅を受け取りセット
    void SetStageSize(float _x, float _z);	
    //CSV系ステータス一括変更ベクター型
    void SetCsvStatus(std::vector<float> _value);
   
    //ゲット系
    //弾の種類
    int GetBulletType() {
        return m_bulletType;
    }
    //発射するか
    bool GetFireBulletFlg()
    {
        return m_fireBulletFlg;
    }
    //CSVで読み取る数値を取得する
    float GetCSVStatus(ENEMY_STATUSID _statusid);

    //各方向の真偽を取る
    bool GetShotDirection(int _num);

    /*************************************************************
    * バーチャル関数
    *************************************************************/
    virtual bool Update(const DirectX::XMFLOAT3 _TargetPosition, float _l) { return true; };
    virtual bool Init(std::unique_ptr<CSVFile>& _csvFile, std::unique_ptr<Player>& _player) { return true; };   //初期化
    virtual void Update() = 0;                                  //更新
    virtual void Draw();                                    //描画
    virtual void DrawWithAxis() {};                             //XYZ軸含めて描画
    virtual void Finalize() = 0;                                //終了処理

protected:
    /*************************************************************
   * プライベート関数
   *************************************************************/
    virtual void EnemyMove() = 0;//各キャラの移動させるときの関数
    //デルタタイムを加算して発射するかを決める関数
    void BulletReload();//次の球を発射するかの関数
};


