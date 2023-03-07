#pragma once
#include "CSV.h"
#include "EnumListSkill.h"
#include "Quad2d.h"

class Skill
{
private:
	std::unique_ptr<Quad2D> m_image;//画像格納
	std::unique_ptr<Quad2D> m_text;//スキル説明用の画像

	SKILLTYPEID m_SkillType = SKILLTYPEID::END;
	/*************************************************************
	* CSVから受け取る変数
	*************************************************************/
	float m_UPvalue = 0;
	float m_Level = 0;
	float m_Levelmax = 0;
	float m_Rate = 0;
public:
	/*************************************************************
    * 関数類
    *************************************************************/
	bool SkillCsvSet(std::unique_ptr<CSVFile>& _csvFile);//CSV受け取り
	//レベルUP
	void LevelUP();

	/*************************************************************
	* GET、SET関数
	*************************************************************/
	//CSVで読み取る数値を取得する
	float GetCSVStatus(SKILL_STATUSID _statusid);

	std::unique_ptr<Quad2D>& GetImage() { 
		return m_image; 
	}
	std::unique_ptr<Quad2D>& GetText() {
		return m_text;
	}

	//SET系

	void SetSkillType(SKILLTYPEID _skillType);
	//ステータスを反映させる、エディター用
	void SetAllStatus(std::vector<float> _value);
};

