#include "EnemyObject.h"
#include "imgui.h"
#include "XAudio2.h"
#include "Random.h"
#include "CCamera.h"
#include "ModelMgr.h"
#include "BillBoard.h"

bool EnemyObject::EnemyCsvSet(CSVFile _csvFile)
{

    if (m_enemyNum == ENEMY_TYPE::END)
    {
        //初期化されていないのでメッセージを表示
        Texture tex;
        MessageBox(nullptr, "何の敵か設定されていません",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //共通変数初期化
        //GetCell().at(この敵の種類列).at(値)
        m_modelNum = static_cast<ALLMODEL>(_csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MODEL_NUM)));//モデルナンバー
        m_moveSpeed = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED));//移動速度
        m_hp = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::HP));//体力
        m_maxHP = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_HP));//最大体力
        m_atk = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ATTACK));//攻撃力
        m_rotation = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ROTATION));//回転するか
        m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT));//１フレーム内の最大回転角度
        m_bulletType = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE));//弾の種類
        m_reload = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::RELOAD));//弾の種類;
        m_shotDirectionFront = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT));//弾を撃つ方向
        m_shotDirectionLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_LEFT));//弾を撃つ方向
        m_shotDirectionRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_RIGHT));//弾を撃つ方向
        m_shotDirectionBack = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK));//弾を撃つ方向
        m_shotDirectionFrontLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_LEFT));//弾を撃つ方向
        m_shotDirectionFrontRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_RIGHT));//弾を撃つ方向
        m_shotDirectionBackLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_LEFT));//弾を撃つ方向
        m_shotDirectionBackRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_RIGHT));//弾を撃つ方向
    }
 
	return true;
}

void EnemyObject::SetPos(float _x, float _z)
{
    //pos更新
    m_pos.x = _x;
    m_pos.z = _z;

    //行列更新
    m_mtx._41 = m_pos.x;
    m_mtx._43 = m_pos.z;
}

void EnemyObject::SetStageSize(float _x, float _z)
{
    m_stageX = _x;
    m_stageZ = _z;
}

//void EnemyObject::SetAllStatus(float _value[static_cast<int>(ENEMY_STATUSID::END)])
//{
//    m_moveSpeed = _value[static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)];
//    m_hp = _value[static_cast<int>(ENEMY_STATUSID::HP)];
//    m_atk = _value[static_cast<int>(ENEMY_STATUSID::ATTACK)];
//    m_rotation = _value[static_cast<int>(ENEMY_STATUSID::ROTATION)];
//    m_maxRot = _value[static_cast<int>(ENEMY_STATUSID::MAX_ROT)];
//    m_bulletType = _value[static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)];
//    m_reload = _value[static_cast<int>(ENEMY_STATUSID::RELOAD)];
//    m_shotDirectionFront = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT)];
//    m_shotDirectionLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_LEFT)];
//    m_shotDirectionRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_RIGHT)];
//    m_shotDirectionBack = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK)];
//    m_shotDirectionFrontLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_LEFT)];
//    m_shotDirectionFrontRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_RIGHT)];
//    m_shotDirectionBackLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_LEFT)];
//    m_shotDirectionBackRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_RIGHT)];
//}

void EnemyObject::SetCsvStatus(std::vector<float> _value)
{
    m_moveSpeed = _value[static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)];
    m_hp = _value[static_cast<int>(ENEMY_STATUSID::HP)];
    m_maxHP = _value[static_cast<int>(ENEMY_STATUSID::MAX_HP)];
    m_atk = _value[static_cast<int>(ENEMY_STATUSID::ATTACK)];
    m_rotation = _value[static_cast<int>(ENEMY_STATUSID::ROTATION)];
    m_maxRot = _value[static_cast<int>(ENEMY_STATUSID::MAX_ROT)];
    m_bulletType = _value[static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)];
    m_reload = _value[static_cast<int>(ENEMY_STATUSID::RELOAD)];
    m_shotDirectionFront = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT)];
    m_shotDirectionLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_LEFT)];
    m_shotDirectionRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_RIGHT)];
    m_shotDirectionBack = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK)];
    m_shotDirectionFrontLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_LEFT)];
    m_shotDirectionFrontRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_RIGHT)];
    m_shotDirectionBackLeft = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_LEFT)];
    m_shotDirectionBackRight = _value[static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_RIGHT)];
}

float EnemyObject::GetCSVStatus(ENEMY_STATUSID _statusid)
{
    switch (_statusid)
    {
    case ENEMY_STATUSID::MOVE_SPEED: {
        return m_moveSpeed;
    }
        break;
    case ENEMY_STATUSID::HP: {
        return m_hp;
    }
        break;
    case ENEMY_STATUSID::MAX_HP: {
        return m_maxHP;
    }
        break;
    case ENEMY_STATUSID::ATTACK: {
        return m_atk;
    }
        break;
    case ENEMY_STATUSID::ROTATION: {
        return m_rotation;
    }
        break;
    case ENEMY_STATUSID::MAX_ROT: {
        return m_maxRot;
    }
        break;
    case ENEMY_STATUSID::BULLET_TYPE: {
        return m_maxRot;
    }
        break;
    case ENEMY_STATUSID::RELOAD: {
        return m_reload;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_FRONT: {
        return m_shotDirectionFront;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_LEFT: {
        return m_shotDirectionLeft;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_RIGHT: {
        return m_shotDirectionRight;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_BACK: {
        return m_shotDirectionBack;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_FRONT_LEFT: {
        return m_shotDirectionFrontLeft;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_FRONT_RIGHT: {
        return m_shotDirectionFrontRight;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_BACK_LEFT: {
        return m_shotDirectionBackLeft;
    }
        break;
    case ENEMY_STATUSID::DIRECTION_BACK_RIGHT: {
        return m_shotDirectionBackRight;
    }
        break;
    case ENEMY_STATUSID::END: {
        // 保険
        Texture tex;
        MessageBox(nullptr, "エネミーオブジェクト：無効な数値が入っています",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;
        return 0.0f;
    }
        break;
    default:
        break;
    }
    //保険
    Texture tex;
    MessageBox(nullptr, "エネミーオブジェクト：無効な数値が入っています",
        "Error!", MB_ICONERROR | MB_OK);
    tex.texture = nullptr;
    return 0.0f;
}

bool EnemyObject::GetShotDirection(int _num)
{
        //各方向の真偽確認
        switch (_num)
        {
        case 0: {
            return m_shotDirectionFront;
            break;
        }
        case 1: {
            return m_shotDirectionLeft;
            break;
        }
        case 2: {
            return m_shotDirectionRight;
            break;
        }
        case 3: {
            return m_shotDirectionBack;
            break;
        }
        case 4: {
            return m_shotDirectionFrontLeft;
            break;
        }
        case 5: {
            return m_shotDirectionFrontRight;
            break;
        }
        case 6: {
            return m_shotDirectionBackLeft;
            break;
        }
        case 7: {
            return m_shotDirectionBackRight;
            break;
        }
        default:
            break;
        }
    
    return false;
}

void EnemyObject::Draw()
{
    //大きさを変えるための処理
    DirectX::XMFLOAT4X4 big;
    //DX11MtxScale(m_expand, m_expand, m_expand, big);//大きくするための4x4を作製
    DX11MtxMultiply(big, m_bigMtx, m_mtx);//先頭に後ろ2つをかけたものを入れる、現在位置

    //描画
    ModelMgr::GetInstance().Draw(m_modelNum , big);
}

void EnemyObject::ReloadInit()
{
    //リロード時間が初期化されていないと０が入っているので０にする
    //0以外の数値なら初期化されているのでランダムにスタート値をセット
    if (m_reload != 0)
    {
        int num1;
        float num2;
        //少数なので倍率を上げる
        num1 = m_reload * 100;
        num2 = Random::GetInstance()->RandomGeneration(0,num1);
        //倍率を戻す
        num2 = num2 / 100;
        m_reloadTime = num2;
    }
}

void EnemyObject::HPDecrease(float _damage)
{
    m_hp = m_hp - _damage;
    //0以下なら死亡
    if (m_hp <= 0)
    {
        m_state = STATE::DEAD;
        m_hp = 0;
        //死亡SE再生
        Sound::instance().Play(SOUND_LABEL_EDEATH_SE);
    }
}


void EnemyObject::EffectReset()
{
    //進行カウントリセット
    //連続ヒットした時用にここで０にする
    m_effectCount = 0;
    //エフェクト処理に入る
    m_effectFlg = true;

    //エフェクトリセット
    BillBoard::GetInstance()->EEReset();

    //輪系のエフェクトのサイズを０にする
   /* m_billBoard[static_cast<int>(GAMEUIID::RING)]->SetSize(0.0f, 0.0f);
    m_billBoard[static_cast<int>(GAMEUIID::RINGHALF)]->SetSize(0.0f, 0.0f);*/
}

void EnemyObject::EffectDraw()
{
    //フラグが立っている＝エフェクト発生
    if (m_effectFlg == true)
    {
        if (m_state == STATE::DEAD)
        {
            //カウントアップ
            m_effectCount++;
            //死んでいるなら死亡エフェクト
            BillBoard::GetInstance()->EEDeath(m_imageNum,m_effectCount, GetPos());
            //終了
            if (m_effectCount >= 65)
            {
                //エフェクト処理を終わる
                m_effectFlg = false;
            }
        }
        else
        {
            //カウントアップ
            m_effectCount++;
            //死んでいないならヒットエフェクト
            BillBoard::GetInstance()->EEHit(GetPos());
            //終了
            if (m_effectCount >= 20)
            {
                //エフェクト処理を終わる
                m_effectFlg = false;
            }
        }
    }

    if (m_state != STATE::DEAD)
    {
        //HPバー
        if (m_hp > 0)
        {
            BillBoard::GetInstance()->EHPBer(m_hp, m_maxHP, GetPos());
        }
    }
}

void EnemyObject::BulletReload()
{
    if (m_reloadTime >= m_reload) {//リロード時間以上なら発射

        m_fireBulletFlg = true;//発射に変更
        m_reloadTime = 0;//リセット
    }
    else// if (m_HommingBulletWaittime < m_bulletReloadTime)//リロード時間
    {
        m_reloadTime += ImGui::GetIO().DeltaTime;
    }
}


