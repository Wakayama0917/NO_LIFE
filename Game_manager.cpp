#include <cinttypes>
#include "dx11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "Application.h"
//#include "CDirectInput.h"
#include "skydome.h"
#include "Input.h"
#include "mesh.h"

#include "game_manager.h"
//#include "SceneChange.h"
#include "CSV.h"
#include "ModelMgr.h"
#include "BillBoard.h"


void Game_Manager::GameInit() {
	// DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// �J�������K�v
	DirectX::XMFLOAT3 eye(0, 00, -30);		// �J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// �����_
	DirectX::XMFLOAT3 up(0, 1, 0);			// �J�����̏�����x�N�g��

	CCamera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);					// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(DirectX::XMFLOAT4(1, 100, -1, 0));
	

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// �X�N���[����
		Application::CLIENT_HEIGHT			// �X�N���[���̍���
		);

	//input������
	Input::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// �X�N���[����
		Application::CLIENT_HEIGHT			// �X�N���[���̍���
	);


	// �A���t�@�u�����h�L����
	TurnOnAlphablend();

	//�ŏ��̃V�[�������̉�
	MakeObject();

	//GUI�̏�����
	imguiInit();

	//���f���ǂݍ���
	ModelMgr::GetInstance().Init();
	//�r���{�[�h�ǂݍ���
	BillBoard::GetInstance()->Init();
	
	
}

void Game_Manager::GameInput(uint64_t _dt) {
	CDirectInput::GetInstance().GetKeyBuffer();//�L�[���͂𖈉���

	//�L�[���͎��
	Input::GetInstance().Update();
}

void Game_Manager::GameAllUpdate(uint64_t _dt)
{
	switch (ManagerStatus)
	{
	case SceneBase::SCENESTATUS::Title: {
		if (m_title->SceneInitFlg) {
			if (m_title != nullptr)
			{
				// �^�C�g��������
				m_title->Init();
			}
			else
			{
				// �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
				Texture tex;
				MessageBox(nullptr, "�^�C�g���V�[�����k���ł�",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}


			Skydome::GetInstance()->Init();
		}
		//�A�b�v�f�[�g�A�`�揈��
		// �^�C�g���X�V
		m_title->Update();
		//�`��O����
		GameRender();
		// ���C���Q�[���`��
		m_title->Draw();
		if (m_title->DebugFlg) {
			//Imgui�`��
			//m_title->Imgui();
		}
		// �`��㏈��
		DX11AfterRender();

		//�I����
		if (m_title->SceneDisposeFlg) {

			// �^�C�g���I������
			m_title->Exit();

			//���̃V�[��������
			ManagerStatus = m_title->NextSceneStatus;


			//���L�����ڍs���č폜�B
			Title* exitTitle = m_title.release();
			delete exitTitle;

			//���̃V�[�������̉�
			MakeObject();
		}
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		if (m_mainGame->SceneInitFlg) {
			if (m_mainGame != nullptr)
			{
				// ���C���Q�[��������
				m_mainGame->Init();
			}
			else
			{
				// �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
				Texture tex;
				MessageBox(nullptr, "���C���Q�[���V�[�����k���ł�",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}

			Skydome::GetInstance()->Init();
		}

		//�A�b�v�f�[�g�A�`�揈��
		// ���C���Q�[���X�V
		m_mainGame->Update();
		//�`��O����
		GameRender();
		// ���C���Q�[���`��
		m_mainGame->Draw();
		if (m_mainGame->DebugFlg) {
			//Imgui�`��
			//m_mainGame->Imgui();
		}
		// �`��㏈��
		DX11AfterRender();

		//�I����
		if (m_mainGame->SceneDisposeFlg) {
			// ���C���Q�[���I������
			m_mainGame->Exit();

			//���̃V�[��������
			ManagerStatus = m_mainGame->NextSceneStatus;

			//���L�����ڍs���č폜�B
			Game* exitGame = m_mainGame.release();
			delete exitGame;

			//���̃V�[�������̉�
			MakeObject();
		}
		break;
	}
	case SceneBase::SCENESTATUS::GameBoss: {
		break;
	}
	case SceneBase::SCENESTATUS::Editor: {

		if (m_editor->SceneInitFlg) {
			if (m_editor != nullptr)
			{
				// �^�C�g��������
				m_editor->Init();
			}
			else
			{
				// �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
				Texture tex;
				MessageBox(nullptr, "�G�f�B�^�[�V�[�����k���ł�",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}

			Skydome::GetInstance()->Init();
		}
		//�A�b�v�f�[�g�A�`�揈��
		// �^�C�g���X�V
		m_editor->Update();

		//�`��O����
		GameRender();
		// ���C���Q�[���`��
		m_editor->Draw();
		//�V�[���J�ڂ�����Ă��Ȃ����
		if (m_editor->m_sceneChangeFlg == false)
		{
			//Imgui�`��A���̃V�[���ƈႢ��Ε`��
			m_editor->Imgui();
		}
		// �`��㏈��
		DX11AfterRender();

		//�I����
		if (m_editor->SceneDisposeFlg) {

			// �^�C�g���I������
			m_editor->Exit();

			//���̃V�[��������
			ManagerStatus = m_editor->NextSceneStatus;

			//�ŏI�ōs������
			//DX11Uninit();

			//���L�����ڍs���č폜�B
			Editor* exitEditor = m_editor.release();
			delete exitEditor;

			//���̃V�[�������̉�
			MakeObject();
		}

		break;
	}
	case SceneBase::SCENESTATUS::End: {
		//�Q�[�����I������
		GameFlg = false;

		//�ŏI�ōs������
		DX11Uninit();
		break;
	}
	default:
		break;
	}

	//�ȉ��ǂ̃V�[���ł����t���[���s������
	//�L�[����
	Input::GetInstance().Update();
}

void Game_Manager::GameUpdate(uint64_t _dt) {

	// �󒆐�X�V
	m_mainGame->Update();

}

void Game_Manager::GameRender() {

	float col[4] = { 1,0,0,1 };

	// �`��O����
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// �v���W�F�N�V�����ϊ��s��擾
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// �r���[�ϊ��s����擾
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	
}

void Game_Manager::GameDispose() {

	switch (ManagerStatus)
	{
	case SceneBase::SCENESTATUS::Title: {
		//�^�C�g���ŏI�������j��
		m_title->Exit();
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		// ���C���Q�[���ŏI�������j��
		m_mainGame->Exit();
		break;
	}
	case SceneBase::SCENESTATUS::GameBoss: {
		break;
	}
	case SceneBase::SCENESTATUS::Editor: {
		//�G�f�B�^�[���[�h�ŏI�������j��
		m_editor->Exit();
		break;
	}
	case SceneBase::SCENESTATUS::End: {
		break;
	}
	default:
		break;
	}
	

	//Skydome::GetInstance()->Finalize();
	DX11Uninit();
}

void Game_Manager::MakeObject()
{
	switch (ManagerStatus)
	{
	case SceneBase::SCENESTATUS::Title: {
		//�^�C�g���V�[�������̉�
		m_title = std::make_unique<Title>();
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		//�Q�[���V�[�������̉�
		m_mainGame = std::make_unique<Game>();
		break;
	}
	case SceneBase::SCENESTATUS::GameBoss: {
		break;
	}
	case SceneBase::SCENESTATUS::Editor: {
		//�G�f�B�^�[�V�[�������̉�
		m_editor = std::make_unique<Editor>();
		break;
	}
	case SceneBase::SCENESTATUS::End: {
		break;
	}
	default:
		break;
	}
}
