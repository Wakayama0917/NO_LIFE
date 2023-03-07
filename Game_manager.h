#pragma once
#include "MainGame.h"
#include "Title.h"
#include "Editor.h"
//#include	"SceneBase.h"

class Game_Manager : public SceneBase
{
	Game_Manager() {
	}

	//�ǂ̃V�[�����񂷂��̑S�̃t���O
	SCENESTATUS ManagerStatus = SCENESTATUS::Title;
public:
	//�V���O���g��
	static Game_Manager* GetInstance() {
		static Game_Manager instance;
		return &instance;
	}

	bool GameFlg = true;

	void GameInit();//������
	void GameInput(uint64_t _dt);//�L�[����
	void GameRender();//�`��O����
	void GameAllUpdate(uint64_t _dt);

	void GameUpdate(uint64_t _dt);
	void GameDispose();

private:
	//�X�}�[�g�|�C���^�[�A�^�C�g��
	std::unique_ptr<Title> m_title;
	//�X�}�[�g�|�C���^�[�A���C���Q�[��
	std::unique_ptr<Game> m_mainGame;
	//�X�}�[�g�|�C���^�[�A�G�f�B�^�[���[�h
	std::unique_ptr<Editor> m_editor;

	//���̃V�[�������̉�������
	void MakeObject();
};

