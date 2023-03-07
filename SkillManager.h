#pragma once
#include"Skill.h"
#include "EnumListSkill.h"
#include "Player.h"
#include "Quad2d.h"

class SkillManager
{
private:
	//プレイヤーのポインタを格納するよう
	std::shared_ptr<Player> m_sPlayer;

	//スキル系
	std::vector<std::unique_ptr<Skill>> m_skill;
	//変数
	int m_setposs = 425;
	int m_selectNum = 0;//選択画面用
	SKILLTYPEID m_skillbox[3];
	int m_allRate = 0;//合計の当選確率を入れる
	// 抽選する用の配列、レベルが最大でないものだけ入れる
	std::vector<int> m_selectAllBox;

public:
	/*************************************************************
	* 通常弾関数
	*************************************************************/
	bool Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);		//初期化
	void Reset();		//リセット
	void SkillSelect(); //スキル抽選
	bool Update();		//抽選、選択まで
	void Draw();		//描画

	/*************************************************************
	* GET SET関数
	*************************************************************/
	void SetAllStatus(int _skillNum, std::vector<float> _value);
private:
	int SelectSkillRote();		//作られた確率込みのBOXの中から１つ抽選する関数
};


