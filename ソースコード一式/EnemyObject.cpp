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
        //����������Ă��Ȃ��̂Ń��b�Z�[�W��\��
        Texture tex;
        MessageBox(nullptr, "���̓G���ݒ肳��Ă��܂���",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //���ʕϐ�������
        //GetCell().at(���̓G�̎�ޗ�).at(�l)
        m_modelNum = static_cast<ALLMODEL>(_csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MODEL_NUM)));//���f���i���o�[
        m_moveSpeed = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED));//�ړ����x
        m_hp = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::HP));//�̗�
        m_maxHP = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_HP));//�ő�̗�
        m_atk = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ATTACK));//�U����
        m_rotation = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ROTATION));//��]���邩
        m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT));//�P�t���[�����̍ő��]�p�x
        m_bulletType = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE));//�e�̎��
        m_reload = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::RELOAD));//�e�̎��;
        m_shotDirectionFront = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT));//�e��������
        m_shotDirectionLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_LEFT));//�e��������
        m_shotDirectionRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_RIGHT));//�e��������
        m_shotDirectionBack = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK));//�e��������
        m_shotDirectionFrontLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_LEFT));//�e��������
        m_shotDirectionFrontRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT_RIGHT));//�e��������
        m_shotDirectionBackLeft = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_LEFT));//�e��������
        m_shotDirectionBackRight = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_BACK_RIGHT));//�e��������
    }
 
	return true;
}

void EnemyObject::SetPos(float _x, float _z)
{
    //pos�X�V
    m_pos.x = _x;
    m_pos.z = _z;

    //�s��X�V
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
        // �ی�
        Texture tex;
        MessageBox(nullptr, "�G�l�~�[�I�u�W�F�N�g�F�����Ȑ��l�������Ă��܂�",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;
        return 0.0f;
    }
        break;
    default:
        break;
    }
    //�ی�
    Texture tex;
    MessageBox(nullptr, "�G�l�~�[�I�u�W�F�N�g�F�����Ȑ��l�������Ă��܂�",
        "Error!", MB_ICONERROR | MB_OK);
    tex.texture = nullptr;
    return 0.0f;
}

bool EnemyObject::GetShotDirection(int _num)
{
        //�e�����̐^�U�m�F
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
    //�傫����ς��邽�߂̏���
    DirectX::XMFLOAT4X4 big;
    //DX11MtxScale(m_expand, m_expand, m_expand, big);//�傫�����邽�߂�4x4���쐻
    DX11MtxMultiply(big, m_bigMtx, m_mtx);//�擪�Ɍ��2�����������̂�����A���݈ʒu

    //�`��
    ModelMgr::GetInstance().Draw(m_modelNum , big);
}

void EnemyObject::ReloadInit()
{
    //�����[�h���Ԃ�����������Ă��Ȃ��ƂO�������Ă���̂łO�ɂ���
    //0�ȊO�̐��l�Ȃ珉��������Ă���̂Ń����_���ɃX�^�[�g�l���Z�b�g
    if (m_reload != 0)
    {
        int num1;
        float num2;
        //�����Ȃ̂Ŕ{�����グ��
        num1 = m_reload * 100;
        num2 = Random::GetInstance()->RandomGeneration(0,num1);
        //�{����߂�
        num2 = num2 / 100;
        m_reloadTime = num2;
    }
}

void EnemyObject::HPDecrease(float _damage)
{
    m_hp = m_hp - _damage;
    //0�ȉ��Ȃ玀�S
    if (m_hp <= 0)
    {
        m_state = STATE::DEAD;
        m_hp = 0;
        //���SSE�Đ�
        Sound::instance().Play(SOUND_LABEL_EDEATH_SE);
    }
}


void EnemyObject::EffectReset()
{
    //�i�s�J�E���g���Z�b�g
    //�A���q�b�g�������p�ɂ����łO�ɂ���
    m_effectCount = 0;
    //�G�t�F�N�g�����ɓ���
    m_effectFlg = true;

    //�G�t�F�N�g���Z�b�g
    BillBoard::GetInstance()->EEReset();

    //�֌n�̃G�t�F�N�g�̃T�C�Y���O�ɂ���
   /* m_billBoard[static_cast<int>(GAMEUIID::RING)]->SetSize(0.0f, 0.0f);
    m_billBoard[static_cast<int>(GAMEUIID::RINGHALF)]->SetSize(0.0f, 0.0f);*/
}

void EnemyObject::EffectDraw()
{
    //�t���O�������Ă��遁�G�t�F�N�g����
    if (m_effectFlg == true)
    {
        if (m_state == STATE::DEAD)
        {
            //�J�E���g�A�b�v
            m_effectCount++;
            //����ł���Ȃ玀�S�G�t�F�N�g
            BillBoard::GetInstance()->EEDeath(m_imageNum,m_effectCount, GetPos());
            //�I��
            if (m_effectCount >= 65)
            {
                //�G�t�F�N�g�������I���
                m_effectFlg = false;
            }
        }
        else
        {
            //�J�E���g�A�b�v
            m_effectCount++;
            //����ł��Ȃ��Ȃ�q�b�g�G�t�F�N�g
            BillBoard::GetInstance()->EEHit(GetPos());
            //�I��
            if (m_effectCount >= 20)
            {
                //�G�t�F�N�g�������I���
                m_effectFlg = false;
            }
        }
    }

    if (m_state != STATE::DEAD)
    {
        //HP�o�[
        if (m_hp > 0)
        {
            BillBoard::GetInstance()->EHPBer(m_hp, m_maxHP, GetPos());
        }
    }
}

void EnemyObject::BulletReload()
{
    if (m_reloadTime >= m_reload) {//�����[�h���Ԉȏ�Ȃ甭��

        m_fireBulletFlg = true;//���˂ɕύX
        m_reloadTime = 0;//���Z�b�g
    }
    else// if (m_HommingBulletWaittime < m_bulletReloadTime)//�����[�h����
    {
        m_reloadTime += ImGui::GetIO().DeltaTime;
    }
}


