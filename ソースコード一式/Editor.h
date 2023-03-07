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
#include "SkillManager.h"
#include "EnumListGameState.h"
#include "EnumListSkill.h"

class Editor : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();

private:
	//�G�f�B�^�[�Ŏg�p����CSV�t�@�C���̃��X�g
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
	//�e�̏�������CSV
	std::unique_ptr<CSVFile> m_csvBulletData;

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

	enum class IMGUI_STATE
	{
		PLAYER,		//�v���C���[�G�f�B�b�g
		SKILL,		//�X�L���G�f�B�b�g
		ENEMY,		//�G�l�~�[�G�f�B�b�g
		BULLET,		//�e�G�f�B�b�g
		STAGE,		//�X�e�[�W�G�f�B�b�g
		END,		//�Ō�̍���
	};
	//���݂ǂ��̃G�f�B�^�[�V�[���𑀍삵�Ă��邩�B
	IMGUI_STATE m_guiState = IMGUI_STATE::PLAYER;

	
	//��Ƃ��Ď����Ă����A�J�����ȂǂɎg�p����
	XMFLOAT4X4 m_mtx;
	bool m_ResetFlg[static_cast<int>(IMGUI_STATE::END)];//�������������s�����ǂ����̃t���O
	bool m_saveFlg = false;//�Z�[�u���邩�ǂ����̃t���O

	//�v���C���[�X�e�[�^�X�ύX���Ɏg�p
	float m_playerChangeData[static_cast<int>(PLAYER_STATUSID::END)] = { 0 };
	//�X�L���X�e�[�^�X�ύX���Ɏg�p
	std::vector<std::vector<float>> m_skillChangeData;
	int m_choiceSkill = 0;//���̃X�L����I�����Ă��邩
	
	//�G�̃X�e�[�^�X�ύX���Ɏg�p����
	std::vector<std::vector<float>> m_enemyChangeData;
	int m_choiceEnemy = 0;//���̓G��I�����Ă��邩
	int m_choiceEnemyOld = 0;//���̓G��I�����Ă��邩
	//���݂̃t���[����m_choiceEnemy��m_choiceEnemyPrevious���ד����Ȃ炻�̂܂�
	//�Ⴆ�΁A�ύX�������ƂɂȂ�̂ŏ������������s���ׂ̕ϐ�
	int m_choiceEnemyPrevious = 0;
	std::vector<ENEMY_TYPE> m_enemyArray;//�G�̔z��A���̉�������p
	
	//�e�̃X�e�[�^�X�ύX���Ɏg�p����
	std::vector<std::vector<float>> m_bulletChangeData;
	int m_choiceBullet = 0;//���̒e��I�����Ă��邩

	//�X�e�[�W�̃X�e�[�^�X�ύX���Ɏg�p����
	std::vector<std::vector<float>> m_stageChangeData;
	int m_stageNumber = 0;//���݂̃X�e�[�W��
	bool m_resetFlg = 0;
	
	
	

};


