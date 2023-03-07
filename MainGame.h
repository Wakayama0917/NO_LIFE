#pragma once
#include "SceneBase.h"
#include "CBillBoard.h"
#include "Quad2d.h"
#include "CSV.h"
#include "EnemyManager.h"
#include "Stand.h"
#include "Skydome.h"
#include "Player.h"
#include "bulletmanager.h"
#include "EnumListGameState.h"
#include "EnumListSkill.h"
#include "SkillManager.h"



class Game : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();

private:
	//���C���Ŏg�p����CSV�t�@�C���̃��X�g
	enum class CSVID
	{
		//START,		//�ŏ��̐��l
		PLAYER,			//�v���C���[
		ENEMY,			//�G
		BULLET,			//�e
		STAGE,			//�X�e�[�W�i�傫���A�ʒu�Ȃǁj
		MODEL_SS,		//�X�e�[�W�A�X�J�C�h�[���̃��f���f�[�^
		SKILL,			//�X�L���f�[�^
		END,			//�Ō�
	};
	std::vector<std::unique_ptr<CSVFile>> m_csvData;

	//��
	std::unique_ptr<Stand> m_stand;
	// �X�J�C�h�[���I�u�W�F�N�g����
	std::unique_ptr<Skydome> m_skydome;
	//�v���C���[�A�|�C���^���e�G�Ɏ�������
	std::shared_ptr<Player> m_player;
	//�G�l�~�[�}�l�[�W���[����
	std::unique_ptr<EnemyManager> m_enemyManager;
	//�o���b�g�}�l�[�W���[����
	std::unique_ptr<BulletManager> m_bulletManager;
	//�X�L���}�l�[�W���[����
	std::unique_ptr<SkillManager> m_skillManager;
	//�e�X�e�[�W�̓G�̎�ނ��i�[����z��
	std::vector<ENEMY_TYPE> m_enemyList;
	

	int m_stageNumber = 0;//���݂̃X�e�[�W��
	GAMESTATE m_gameState = GAMESTATE::INIT;//���݂̃��C���Q�[�����̏�Ԃ��ǂ���
	bool m_fadeFlg = false;

	//�g�p����UI�̃��X�g
	enum class GAMEUIID
	{
		HPFLAME,		//HP�t���[��
		HPBAR,			//�������茸�����肷��o�[
		HPLOGO,			//HP���S�}�[�N

		CLEAR,			//�N���A���S
		GAMEOVER,		//�Q�[���I�[�o�[���S

		NOISE,			//�m�C�Y

		NUMBER,			//����

		END,			//�Ō�
	};
	//UI����
	std::vector<std::unique_ptr<Quad2D>> m_ui;
	

};




