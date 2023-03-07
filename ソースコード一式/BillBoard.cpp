#include "BillBoard.h"
#include "CCamera.h"
#include "dx11mathutil.h"


void BillBoard::Init()
{
    //�s�񏉊����A�r���{�[�h�̌����ݒ�p
    DX11MtxIdentity(m_mtxEffect);
    //�r���{�[�h���̉�
    //�G�̃G�t�F�N�g
    for (int i = 0; i < static_cast<int>(BEnemyEffectID::END); i++)
    {
        m_bEnemyEffect.push_back(std::make_unique<CBillBoard>());
    }
    //�G��UI���̉�
    for (int i = 0; i < static_cast<int>(BEnemyUIID::END); i++)
    {
        m_bEnemyUI.push_back(std::make_unique<CBillBoard>());
    }

    //�������p�Ɏg���f�[�^
    XMFLOAT3 Pos{ 0,0,0 };

    //�G�̃G�t�F�N�g�Ɏg�p���镨�S�ēǂݍ���
    //���X�Ɋg�傷�郊���O�n�ʔ�
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->Init(Pos, 10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->LoadTexTure("assets/image/battle/ERing01.png");
    //���X�Ɋg�傷�郊���O�΂ߔ�
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->Init(Pos, 0.0f, 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->LoadTexTure("assets/image/battle/ERingHalf01.png");
    //�r���{�[�h�G�t�F�N�g
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->Init(Pos, 15.0f, 15.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 1.0f, 1.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->LoadTexTure("assets/image/battle/EDelete.png");

    //�G��UI���̉�
    //�G��HP�o�[�ǂݍ���
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->Init(Pos, 10.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->LoadTexTure("assets/white.bmp");
}

void BillBoard::Finalize()
{
    //�G�̃G�t�F�N�g�폜
    for (int i = 0; i < m_bEnemyEffect.size(); i++)
    {
        m_bEnemyEffect[i]->Dispose();
        CBillBoard* exitCBillBoard = m_bEnemyEffect[i].release();
        delete exitCBillBoard;
    }
    m_bEnemyEffect.erase(m_bEnemyEffect.begin(), m_bEnemyEffect.end());

    //�G��UI�폜
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
    //�֌n�̃G�t�F�N�g�̃T�C�Y���O�ɂ���
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSize(0.0f, 0.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSize(0.0f, 0.0f);
}

void BillBoard::EEHit(XMFLOAT3 _pos)
{
    //UI�A�b�v�f�[�g
    //�֒n��
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetPosition(_pos);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->SetSizeAdd(1.0f, 1.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->updateAnimeNo();
    //�`��
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->DrawBillBoard(m_mtxEffect, -90.0f);
}

void BillBoard::EEDeath(int& _imageNum,int _count, XMFLOAT3 _pos)
{
    //UI
    if ((_count > 10) && (_count < 35))
    {
        //�֒n��
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

        //�֎΂�
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetPosition(_pos);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSizeAdd(1.0f, 1.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->updateAnimeNo();
    }
    else
    {
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->SetSize(0.0f, 0.0f);
        m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->updateAnimeNo();
    }

    //�G�t�F�N�g
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->SetPosition(_pos);
    //m_billBoard[1]->SetSizeAdd(0.1f, 0.1f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->updateAnime(_imageNum,_count,3);

    //�`��
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RING)]->DrawBillBoard(m_mtxEffect, -90.0f);
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::RINGHALF)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
    m_bEnemyEffect[static_cast<int>(BEnemyEffectID::DEATH)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
}

void BillBoard::EHPBer(int _hp, int _maxhp, XMFLOAT3 _pos)
{
    //�G�̓���Ƀ|�W�V�����Z�b�g
    XMFLOAT3 pos = _pos;
    pos.y += 5;
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetPosition(pos);

    //HP�o�[�̒���
    double probability;
    //�F�p
    float colorRate;
    //�����v�Z
    probability = (double)_hp / (double)_maxhp;
    //�F�����߂�
    colorRate =  (1 - probability);
    //�J���[�Z�b�g
    XMFLOAT4 color = {1.0f,1.0f - colorRate,1.0f - colorRate,1.0f};
    //�F��ݒ�
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetColor(color);

    //�`�悷�钷��������
    probability = probability * 10.0f;

    //�\������傫�����v�Z
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->SetSize(probability,1);
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->updateAnimeNo();

    //�`��
    m_bEnemyUI[static_cast<int>(BEnemyUIID::HPBAR)]->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());

}


