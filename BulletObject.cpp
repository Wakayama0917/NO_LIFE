#include "BulletObject.h"
#include "ModelMgr.h"

bool BulletObject::BulletCsvSet(CSVFile _csvFile)
{
    if (m_bulletType == BULLET_TYPE::END)
    {
        //����������Ă��Ȃ��̂Ń��b�Z�[�W��\��
        Texture tex;
        MessageBox(nullptr, "���̒e���ݒ肳��Ă��܂���",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //���ʕϐ�������
        //GetCell().at(���̓G�̎�ޗ�).at(�l)
        
        //���ʕϐ�������
        //GetCell().at(���̓G�̎�ޗ�).at(�l)
        m_modelNum = static_cast<ALLMODEL>(_csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::MODEL_NUM)));//���f���i���o�[
        m_attackSpeed = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED));//�U�����x
        //m_reload = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::RELOAD));//���̒e�����܂ł̎���
        m_attackPower = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER));//�U����
        m_liveTime = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME));//��������
        m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::MAX_ROT));//�P�t���[�����̍ő��]�p�x
        m_expand = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::SIZE));//�g�嗦�AGameObject�̒��ɓ����Ă�
        m_radius = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::RADIUS));//���a�A�����蔻��p
    }

    return true;
}

void BulletObject::SetAllStatus(float _value[static_cast<int>(BULLET_STATUSID::END)])
{   
    //�X�e�[�^�X�ꊇ�ύX
    m_modelNum = static_cast<ALLMODEL>(_value[(static_cast<int>(BULLET_STATUSID::MODEL_NUM))]);//���f���i���o�[
    m_attackSpeed = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED))];//�U�����x
    //m_reload = _value[(static_cast<int>(BULLET_STATUSID::RELOAD))];//���̒e�����܂ł̎���
    m_attackPower = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_POWER))];//�U����
    m_liveTime = _value[(static_cast<int>(BULLET_STATUSID::LIVE_TIME))];//��������
    m_maxRot = _value[(static_cast<int>(BULLET_STATUSID::MAX_ROT))];//�P�t���[�����̍ő��]�p�x
    m_expand = _value[(static_cast<int>(BULLET_STATUSID::SIZE))];//�g�嗦�AGameObject�̒��ɓ����Ă�
    m_radius = _value[(static_cast<int>(BULLET_STATUSID::RADIUS))];//���a�A�����蔻��p
}

void BulletObject::SetAllStatus(std::vector<float> _value)
{
    if (_value.size() == static_cast<int>(BULLET_STATUSID::END))
    {
        //�X�e�[�^�X�ꊇ�ύX
        m_modelNum = static_cast<ALLMODEL>(_value[(static_cast<int>(BULLET_STATUSID::MODEL_NUM))]);//���f���i���o�[
        m_attackSpeed = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED))];//�U�����x
        //m_reload = _value[(static_cast<int>(BULLET_STATUSID::RELOAD))];//���̒e�����܂ł̎���
        m_attackPower = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_POWER))];//�U����
        m_liveTime = _value[(static_cast<int>(BULLET_STATUSID::LIVE_TIME))];//��������
        m_maxRot = _value[(static_cast<int>(BULLET_STATUSID::MAX_ROT))];//�P�t���[�����̍ő��]�p�x
        m_expand = _value[(static_cast<int>(BULLET_STATUSID::SIZE))];//�g�嗦�AGameObject�̒��ɓ����Ă�
        m_radius = _value[(static_cast<int>(BULLET_STATUSID::RADIUS))];//���a�A�����蔻��p
    }
    else
    {
        //�v�f������������
        Texture tex;
        MessageBox(nullptr, "�e�̗v�f�������������ł�",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

    }
   

}

void BulletObject::DirectionSet(XMFLOAT4X4 _mtx,float _angle)
{
    XMFLOAT4 axisY;
    XMFLOAT4 axisZ;
    XMFLOAT4X4 mtx = _mtx;

    //Y�������o��
    axisY.x = mtx._21;
    axisY.y = mtx._22;
    axisY.z = mtx._23;
    axisY.w = 0.0f;

    XMFLOAT4 qt, qty;//�N�H�[�^�j�I��

    //�s�񂩂�N�I�[�^�j�I���𐶐�
    DX11GetQtfromMatrix(mtx, qt);

    DX11QtRotationAxis(qty, axisY, _angle);

    //�N�I�[�^�j�I������
    XMFLOAT4 tempqt1;
    DX11QtMul(tempqt1, qt, qty);

    //�N�I�[�^�j�I�����m�[�}���C�Y
    DX11QtNormalize(tempqt1, tempqt1);

    //�N�I�[�^�j�I������s����쐬
    DX11MtxFromQt(mtx, tempqt1);

    //Z�������o��
    axisZ.x = mtx._31;
    axisZ.y = mtx._32;
    axisZ.z = mtx._33;
    axisZ.w = 0.0f;

    m_direction = DirectX::XMFLOAT3(axisZ.x, axisZ.y, axisZ.z);//�ݒ�
}

float BulletObject::GetCSVStatus(BULLET_STATUSID _statusid)
{
    switch (_statusid)
    {
    case BULLET_STATUSID::ATTACK_SPEED: {
        return m_attackSpeed;
    }
        break;
    case BULLET_STATUSID::ATTACK_POWER: {
        return m_attackPower;
    }
        break;
    case BULLET_STATUSID::LIVE_TIME: {
        return m_liveTime;
    }
        break;
    case BULLET_STATUSID::MAX_ROT: {
        return m_maxRot;
    }
        break;
    case BULLET_STATUSID::SIZE: {
        return m_expand;
    }
        break;
    case BULLET_STATUSID::RADIUS: {
        return m_radius;
    }
        break;
    case BULLET_STATUSID::END: {
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

void BulletObject::Draw()
{
    //���f���`��
    if (m_state == STATE::LIVE) {
        //�傫����ς��邽�߂̏���
        DirectX::XMFLOAT4X4 big;
        //DX11MtxScale(m_expand, m_expand, m_expand, big);//�傫�����邽�߂�4x4���쐻
        DX11MtxMultiply(big, m_bigMtx, m_mtx);//�擪�Ɍ��2�����������̂�����A���݈ʒu

        //�`��
        ModelMgr::GetInstance().Draw(m_modelNum, big);
        
    }
}
