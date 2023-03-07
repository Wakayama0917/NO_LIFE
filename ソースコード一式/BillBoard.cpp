#include "BillBoard.h"
#include "CCamera.h"
#include "dx11mathutil.h"


void BillBoard::Init()
{
    //行列初期化、ビルボードの向き設定用
    DX11MtxIdentity(m_mtxEffect);
    //ビルボード実体化
    //敵のエフェクト
    for (int i = 0; i < static_cast<int>(BEnemyEffectID::END); i++)
    {
        m_bEnemyEffect.push_back(std::make_unique<CBillBoard>());
    }
    //敵のUI実体化
    for (int i = 0; i < static_cast<int>(BEnemyUIID::END); i++)
    {
        m_bEnemyUI.push_back(std::make_unique<CBillBoard>());
    }

    //初期化用に使うデータ
    XMFLOAT3 Pos{ 0,0,0 };

    //敵のエフェクトに使用する物全て読み込み
    //徐々に拡大するリング地面版
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->Init(Pos, 10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->LoadTexTure("assets/image/battle/ERing01.png");
    //徐々に拡大するリング斜め版
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->Init(Pos, 0.0f, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->LoadTexTure("assets/image/battle/ERingHalf01.png");
    //ビルボードエフェクト
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->Init(Pos, 15.0f, 15.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 1.0f, 1.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->LoadTexTure("assets/image/battle/EDelete.png");

    //敵のUI実体化
    //敵のHPバー読み込み
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->Init(Pos, 10.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->LoadTexTure("assets/white.bmp");
}

void BillBoard::Finalize()
{
    //敵のエフェクト削除
    for (int i = 0; i < m_bEnemyEffect.size(); i++)
    {
        m_bEnemyEffect[i]->Dispose();
        CBillBoard* exitCBillBoard = m_bEnemyEffect[i].release();
        delete exitCBillBoard;
    }
    m_bEnemyEffect.erase(m_bEnemyEffect.begin(), m_bEnemyEffect.end());

    //敵のUI削除
    for (int i = 0; i < m_bEnemyUI.size(); i++)
    {
        m_bEnemyUI[i]->Dispose();
        CBillBoard* exitCBillBoard = m_bEnemyUI[i].release();
        delete exitCBillBoard;
    }
    m_bEnemyUI.erase(m_bEnemyUI.begin(), m_bEnemyUI.end());


}

void BillBoard::EEReset()
{
    //輪系のエフェクトのサイズを０にする
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSize(0.0f, 0.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSize(0.0f, 0.0f);
}

void BillBoard::EEHit(XMFLOAT3 _pos)
{
    //UIアップデート
    //輪地面
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetPosition(_pos);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSizeAdd(1.0f, 1.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->updateAnimeNo();
    //描画
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->DrawBillBoard(m_mtxEffect, -90.0f);
}

void BillBoard::EEDeath(int& _imageNum,int _count, XMFLOAT3 _pos)
{
    //UI
    if ((_count > 10) && (_count < 35))
    {
        //輪地面
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetPosition(_pos);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSizeAdd(1.0f, 1.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->updateAnimeNo();
    }
    else// if (_count > 35)
    {
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSize(0.0f, 0.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->updateAnimeNo();
    }

    if ((_count > 35) && (_count < 55))
    {
        //m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSize(0.0f, 0.0f);

        //輪斜め
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetPosition(_pos);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSizeAdd(1.0f, 1.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->updateAnimeNo();
    }
    else
    {
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSize(0.0f, 0.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->updateAnimeNo();
    }

    //エフェクト
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->SetPosition(_pos);
    //m_billBoard[1]->SetSizeAdd(0.1f, 0.1f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->updateAnime(_imageNum,_count,3);

    //描画
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->DrawBillBoard(m_mtxEffect, -90.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
}

void BillBoard::EHPBer(int _hp, int _maxhp, XMFLOAT3 _pos)
{
    //敵の頭上にポジションセット
    XMFLOAT3 pos = _pos;
    pos.y += 5;
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetPosition(pos);

    //HPバーの長さ
    double probability;
    //色用
    float colorRate;
    //割合計算
    probability = (double)_hp / (double)_maxhp;
    //色を決める
    colorRate =  (1 - probability);
    //カラーセット
    XMFLOAT4 color = {1.0f,1.0f - colorRate,1.0f - colorRate,1.0f};
    //色を設定
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetColor(color);

    //描画する長さを決定
    probability = probability * 10.0f;

    //表示する大きさを計算
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetSize(probability,1);
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->updateAnimeNo();

    //描画
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());

}


