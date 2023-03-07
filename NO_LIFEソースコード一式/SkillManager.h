#pragma once
#include"Skill.h"
#include "EnumListSkill.h"
#include "Player.h"
#include "Quad2d.h"

class SkillManager
{
private:
	//�v���C���[�̃|�C���^���i�[����悤
	std::shared_ptr<Player> m_sPlayer;

	//�X�L���n
	std::vector<std::unique_ptr<Skill>> m_skill;
	//�ϐ�
	int m_setposs = 425;
	int m_selectNum = 0;//�I����ʗp
	SKILLTYPEID m_skillbox[3];
	int m_allRate = 0;//���v�̓��I�m��������
	// ���I����p�̔z��A���x�����ő�łȂ����̂��������
	std::vector<int> m_selectAllBox;

public:
	/*************************************************************
	* �ʏ�e�֐�
	*************************************************************/
	bool Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);		//������
	void Reset();		//���Z�b�g
	void SkillSelect(); //�X�L�����I
	bool Update();		//���I�A�I���܂�
	void Draw();		//�`��

	/*************************************************************
	* GET SET�֐�
	*************************************************************/
	void SetAllStatus(int _skillNum, std::vector<float> _value);
private:
	int SelectSkillRote();		//���ꂽ�m�����݂�BOX�̒�����P���I����֐�
};


