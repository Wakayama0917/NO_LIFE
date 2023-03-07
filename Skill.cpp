#include "Skill.h"
#include "Mesh.h"



bool Skill::SkillCsvSet(std::unique_ptr<CSVFile>& _csvFile)
{
    if (m_SkillType == SKILLTYPEID::END)
    {
        //初期化されていないのでメッセージを表示
        Texture tex;
        MessageBox(nullptr, "何のスキルか設定されていません",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //実体生成
        m_image = std::make_unique<Quad2D>();

       //スキル系のみ行う処理
        if (static_cast<int>(m_SkillType) >= static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE))
        {
            //スキル系のみテキストが必要なので実体化する
            m_text = std::make_unique<Quad2D>();

            //共通変数初期化
            //GetCell().at(このスキルの種類列).at(値)
            m_UPvalue = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::UP_VALUE));//上昇値
            m_Level = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::LEVEL));//現在レベル
            m_Levelmax = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::LEVELMAX));//最大レベル
            m_Rate = _csvFile->GetCell().at(static_cast<int>(m_SkillType)).at(static_cast<int>(SKILL_STATUSID::RATE));//当選確率
        }
    }

    return true;
}

void Skill::LevelUP()
{
    //１レベル上昇
    m_Level += 1;

    //最大レベルになっていないか保険
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
        // 保険
        Texture tex;
        MessageBox(nullptr, "スキルオブジェクト：無効な数値が入っています",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;
        return 0.0f;
    }
        break;
    default:
        break;
    }

    // 保険
    Texture tex;
    MessageBox(nullptr, "スキルオブジェクト：無効な数値が入っています",
        "Error!", MB_ICONERROR | MB_OK);
    tex.texture = nullptr;
    return 0.0f;


}

void Skill::SetSkillType(SKILLTYPEID _skillType)
{
    //何のスキルカセット
    m_SkillType = _skillType;
}

void Skill::SetAllStatus(std::vector<float> _value)
{
    m_UPvalue = _value[static_cast<int>(SKILL_STATUSID::UP_VALUE)];
    m_Level = _value[static_cast<int>(SKILL_STATUSID::LEVEL)];
    m_Levelmax = _value[static_cast<int>(SKILL_STATUSID::LEVELMAX)];
    m_Rate = _value[static_cast<int>(SKILL_STATUSID::RATE)];
}
