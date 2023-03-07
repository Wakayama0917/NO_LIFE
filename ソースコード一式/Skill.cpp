#include "Skill.h"
#include "Mesh.h"



bool Skill::SkillCsvSet(std::unique_ptr<CSVFile>& _csvFile)
{
    if (m_SkillType == SKILLTYPEID::END)
    {
        //����������Ă��Ȃ��̂Ń��b�Z�[�W��\��
        Texture tex;
        MessageBox(nullptr, "���̃X�L�����ݒ肳��Ă��܂���",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //���̐���
        m_image = std::make_unique<Quad2D>();

       //�X�L���n�̂ݍs������
        if (static_cast<int>(m_SkillType) >= static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE))
        {
            //�X�L���n�̂݃e�L�X�g���K�v�Ȃ̂Ŏ��̉�����
            m_text = std::make_unique<Quad2D>();

            //���ʕϐ�������
            //GetCell().at(���̃X�L���̎�ޗ�).at(�l)
            m_UPvalue = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::UP_VALUE));//�㏸�l
            m_Level = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::LEVEL));//���݃��x��
            m_Levelmax = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::LEVELMAX));//�ő僌�x��
            m_Rate = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::RATE));//���I�m��
        }
    }

    return true;
}

void Skill::LevelUP()
{
    //�P���x���㏸
    m_Level += 1;

    //�ő僌�x���ɂȂ��Ă��Ȃ����ی�
    if (m_Levelmax < m_Level)
    {
        m_Level = m_Levelmax;
    }
}



float Skill::GetCSVStatus(SKILL_STATUSID _statusid)
{
    switch (_statusid)
    {
    case SKILL_STATUSID::UP_VALUE: {
        return m_UPvalue;
    }
        break;
    case SKILL_STATUSID::LEVEL: {
        return m_Level;
    }
        break;
    case SKILL_STATUSID::LEVELMAX: {
        return m_Levelmax;
    }
        break;
    case SKILL_STATUSID::RATE: {
        return m_Rate;
    }
        break;
    case SKILL_STATUSID::END: {
        // �ی�
        Texture tex;
        MessageBox(nullptr, "�X�L���I�u�W�F�N�g�F�����Ȑ��l�������Ă��܂�",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;
        return 0.0f;
    }
        break;
    default:
        break;
    }

    // �ی�
    Texture tex;
    MessageBox(nullptr, "�X�L���I�u�W�F�N�g�F�����Ȑ��l�������Ă��܂�",
        "Error!", MB_ICONERROR | MB_OK);
    tex.texture = nullptr;
    return 0.0f;


}

void Skill::SetSkillType(SKILLTYPEID _skillType)
{
    //���̃X�L���J�Z�b�g
    m_SkillType = _skillType;
}

void Skill::SetAllStatus(std::vector<float> _value)
{
    m_UPvalue = _value[static_cast<int>(SKILL_STATUSID::UP_VALUE)];
    m_Level = _value[static_cast<int>(SKILL_STATUSID::LEVEL)];
    m_Levelmax = _value[static_cast<int>(SKILL_STATUSID::LEVELMAX)];
    m_Rate = _value[static_cast<int>(SKILL_STATUSID::RATE)];
}
