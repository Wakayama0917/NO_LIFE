#pragma once
#include "SceneBase.h"
#include "CBillBoard.h"
#include "Quad2d.h"

class Title : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();

private:
	

	//�^�C�g���Ŏg�p����摜��
	enum class IMAGEID
	{
		TITLE_BG01,			//�w�i
		PRESS_ANY_BUTTON,		//�J�n����UI
		FRAME,				//�ǂ��I�����Ă��邩�̘g
		COMMAND,			//�G�f�B�^���[�h�A�Q�[�����[�h�Ȃǂ̐؂�ւ�
		END,				//�Ō�̍���
	};

	//�^�C�g���V�[�����̃V�[�����
	enum class TITLE_STATE
	{
		PRESS_ANY_BUTTON,	//PRESS_ANY_BUTTON��\�������ԍŏ��̃V�[��
		MAIN,				//���C���̃R�}���h�V�[��
	};
	//�؂�ւ��t���O
	TITLE_STATE m_titleState = TITLE_STATE::PRESS_ANY_BUTTON;

	//�R�}���h�̎��
	enum  class COMMAND_ID
	{
		MAIN_GAME, 
		EDITOR,
		END,
	};
	//���݂ǂ̃R�}���h���w���Ă��邩
	COMMAND_ID m_commandId = COMMAND_ID::MAIN_GAME;

	struct ModelDataList
	{
		IMAGEID			id;
		const char*		imagename;
		/*std::string		vsfilename;
		std::string		psfilename;
		std::string		texfolder;*/
	};

	std::vector<ModelDataList>	m_imageLlist =
	{
		{IMAGEID::TITLE_BG01,"assets/image/Title/TITLE.jpg"},
		{IMAGEID::PRESS_ANY_BUTTON,"assets/image/Title/PressAnyButton.png"},
		{IMAGEID::FRAME,"assets/image/Title/FRAME.png"},
		{IMAGEID::COMMAND,"assets/image/Title/COMMAND.png"},
		
	};

	//UI�A�X�}�[�g�|�C���^�\
	std::unique_ptr<Quad2D> m_ui[static_cast<int>(IMAGEID::END)];

	

//	//UI�̍Œ���̏��������s��
//	void UiInitSetting(IMAGEID _modelid);
//	void UiInitSetting(IMAGEID _modelid, uint32_t _width, uint32_t _height, float _x, float _y);
//	//���T�C�Y�A�c�T�C�Y�A�z�u�ʒuXY�B�A�j���[�V�����͂��Ȃ�
//	void UiUpdateSetting(IMAGEID _modelid, uint32_t _width, uint32_t _height, float _x,float _y);
};

