#pragma once
#include "CSV.h"
#include "EnumListSkill.h"
#include "Quad2d.h"

class Skill
{
private:
	std::unique_ptr<Quad2D> m_image;//�摜�i�[
	std::unique_ptr<Quad2D> m_text;//�X�L�������p�̉摜

	SKILLTYPEID m_SkillType = SKILLTYPEID::END;
	/*************************************************************
	* CSV����󂯎��ϐ�
	*************************************************************/
	float m_UPvalue = 0;
	float m_Level = 0;
	float m_Levelmax = 0;
	float m_Rate = 0;
public:
	/*************************************************************
    * �֐���
    *************************************************************/
	bool SkillCsvSet(std::unique_ptr<CSVFile>& _csvFile);//CSV�󂯎��
	//���x��UP
	void LevelUP();

	/*************************************************************
	* GET�ASET�֐�
	*************************************************************/
	//CSV�œǂݎ�鐔�l���擾����
	float GetCSVStatus(SKILL_STATUSID _statusid);

	std::unique_ptr<Quad2D>& GetImage() { 
		return m_image; 
	}
	std::unique_ptr<Quad2D>& GetText() {
		return m_text;
	}

	//SET�n

	void SetSkillType(SKILLTYPEID _skillType);
	//�X�e�[�^�X�𔽉f������A�G�f�B�^�[�p
	void SetAllStatus(std::vector<float> _value);
};

