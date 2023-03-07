#include "Title.h"
#include "Input.h"
#include "mesh.h"
#include "SceneChange.h"



void Title::Imgui()
{
	// Start the Dear ImGui frame�A�J�n����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//�����܂�

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(350, 400));

	ImGui::Begin("config 1");

	static float slider1 = 0.0;
	static char text1[8] = "";
	static float color[] = { 1.0f,1.0f,1.0f,1.0f };
	static const char* items[]
	{
		"aaa",
		"bbb",
		"ccc",
	};
	static int selected = 0;

	//�t���[�����[�g
	ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);

	switch (m_titleState)
	{
	case Title::TITLE_STATE::PRESS_ANY_BUTTON: {
		//�{�^���_�Œ���
		std::string str;
		str = u8"PRESS_ANY_BUTTON�̓_�ő��x�؂�ւ�";
		ImGui::Text(str.c_str());
		//�������邽�߂̃o�[
		ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);

		str = u8"�A�j���[�V��������";
		ImGui::Text(str.c_str());
		//�������邽�߂̃o�[
		ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);
		break;
	}
	case Title::TITLE_STATE::MAIN: {
		ImGui::ColorEdit4("edit", color);
		ImGui::ColorPicker4("picker", color);

		ImGui::ListBox("hitnum", &selected, items, 3);

		
		break;
	}
	default:
		break;
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	/*if (ImGui::Begin("my window"))
	{
		if (ImGui::BeginChild(1))
		{
			ImGui::Button("dummy1");
			ImGui::Button("dummy2");

			ImGui::EndChild();
		}

		ImGui::End();
	}*/



	////�Q��
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	//ImGui::SetNextWindowPos(ImVec2(980, 20));
	//ImGui::SetNextWindowSize(ImVec2(280, 300));
	//
	//ImGui::Begin(u8"�������");

	//std::string str;
	//str = u8"���@�F���E�L�[�F������ς���";
	//ImGui::Text(str.c_str());
	//str = u8"���@�Fw�F�O�i";
	//ImGui::Text(str.c_str());
	//str = u8"���@�Fs�F���";
	//ImGui::Text(str.c_str());

	////���L�[
	//static float num;
	//num = Input::GetInstance().GetLStick().x;
	//ImGui::InputFloat("input float", &num);
	//num = Input::GetInstance().GetLStick().y;
	//ImGui::InputFloat("input float", &num);

	////�E�L�[
	//num = Input::GetInstance().GetRStick().x;
	//ImGui::InputFloat("input float", &num);
	//num = Input::GetInstance().GetRStick().y;
	//ImGui::InputFloat("input float", &num);

	////if (Hit == true)
	////{
	////	num = 777.0f;
	////	ImGui::InputFloat("����", &num);

	////	ImGui::Text("TRUE");
	////	
	////	//ImGui::Text("fps: %.2f", ImGui::GetIO().DeltaTime);
	////}
	////else
	////{
	////	num = 0.0f;
	////	ImGui::InputFloat("����", &num);
	////	ImGui::InputFloat("����", &ImGui::GetIO().DeltaTime);
	////}

	//ImGui::End();

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();

	// Rendering�A�㏈��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Title::Init()
{
	DebugFlg = true;//�f�o�b�O���邩�ǂ���

	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		if (m_ui[i] == nullptr)
		{
			//UI�����̉�
			m_ui[i] = std::make_unique<Quad2D>();
		}
		else
		{
			// �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
			Texture tex;
			MessageBox(nullptr, "�^�C�g���V�[���FUI�I�u�W�F�N�g�̒��g�ɉ��������Ă��܂�",
				"Error!", MB_ICONERROR | MB_OK);
			tex.texture = nullptr;
		}
	}

	//�ꊇ�������A�f�o�b�O�p
	//for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	//{
	//	//���������I�u�W�F�N�g������������
	//	UiInitSetting(static_cast<IMAGEID>(i));
	//}

	//�^�C�g���w�i
	m_ui[static_cast<int>(IMAGEID::TITLE_BG01)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::TITLE_BG01)].imagename, 
		MAXWIDTH, MAXHEIGHT, MAXWIDTH_HALF, MAXHEIGHT_HALF);
	//�v���X�G�j�[�{�^��
	m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)].imagename,
		1500, 500, MAXWIDTH_HALF, MAXHEIGHT_HALF + 100);
	//�t���[��
	m_ui[static_cast<int>(IMAGEID::FRAME)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::FRAME)].imagename,
		800, 150, MAXWIDTH_HALF - 200, MAXHEIGHT_HALF - 25);
	//�R�}���h
	m_ui[static_cast<int>(IMAGEID::COMMAND)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::COMMAND)].imagename,
		MAXWIDTH, MAXHEIGHT, MAXWIDTH_HALF, MAXHEIGHT_HALF);
	
	//�ꊇ�X�V
	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		//�\���ł���悤�ɍX�V
		m_ui[i]->updateAnimeNo();
	}

	

	//�V�[���J�ڃt���O������
	switch (SceneChange::GetInstance()->GetSceneChangeFlg())
	{
	case SceneChange::SCENECHANGEID::FADDEIN:
		//��ʂɉ����Ȃ����ߖ{�����J�n
		m_sceneChangeFlg = false;
		break;
	case SceneChange::SCENECHANGEID::FADDEOUT:
		//�ʉ�ʂ���ڂ��Ă��Ă��邽�߃t�F�[�h�A�E�g�����Ɉڍs
		m_sceneChangeFlg =	true;
		break;
	case SceneChange::SCENECHANGEID::END:
		break;
	default:
		break;
	}
	//�V�[���J�ڒ��g������
	SceneChange::GetInstance()->init();

	//�������I��
	SceneFlgInit();
	SceneStatus = SCENESTATUS::Title;//�^�C�g���V�[���ɐݒ�

}

void Title::Exit()
{
	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		//���L�����ڍs���č폜�B
		Quad2D* exitUi = m_ui[i].release();
		delete exitUi;
	}
	
	//���̃V�[�����w��
	//NextSceneStatus = SCENESTATUS::Game;

	//�t���O���Z�b�g�A���̃V�[���̈�
	SceneFlgReset();
}

void Title::Update()
{
	//�V�[���J�ڏ����ɓ����Ă��Ȃ���Ώ������s��
	if (m_sceneChangeFlg == false)
	{
		switch (m_titleState)
		{
		case Title::TITLE_STATE::PRESS_ANY_BUTTON: {

			//�_�ŏ��u
			m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->Flashing(0.01f);

			//�����̃{�^���������ꂽ��Q�[�����J�n����A�R���g���[���[��/�G���^�[�������ꂽ��Q�[�����J�n����A�L�[�{�[�h��
			if (Input::GetInstance().CheckButtonAll() || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {
				m_titleState = TITLE_STATE::MAIN;
			}
			

			break;
		}
		case Title::TITLE_STATE::MAIN: {

			//�I�����Ă���R�}���h���킩�点��t���[��
			m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNo();
			//�R�}���h
			m_ui[static_cast<int>(IMAGEID::COMMAND)]->updateAnimeNo();
			//�_�ŏ��u
			//m_ui[static_cast<int>(IMAGEID::FRAME)]->Flashing(0.01f);

			if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) || (Input::GetInstance().CheckKeyTrigger(DIK_UP))) {
				//��ԏ�̃R�}���h�Ȃ炻��ȏ�A��Ɉړ����Ȃ�
				if (m_commandId != COMMAND_ID::MAIN_GAME)
				{
					//num = ���Ԗڂ̃R�}���h�ɂȂ邩
					//���݂̔ԍ�������
					int num = static_cast<int>(m_commandId);

					//���̃R�}���h�ɂ���
					//enum�ō쐬���Ă��邽�߁A�グ�遁���l�������A�����遁���l���グ��
					num -= 1;

					//�ύX���ꂽ�R�}���h�ɍX�V����
					m_commandId = static_cast<COMMAND_ID>(num);

					//�I��UI�̈ʒu���X�V
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 140);
				}
			}
			else if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) || (Input::GetInstance().CheckKeyTrigger(DIK_DOWN))) {
				//��ԉ��̃R�}���h�Ȃ炻��ȏ�A���Ɉړ����Ȃ�
				if (m_commandId != COMMAND_ID::END)
				{
					//num = ���Ԗڂ̃R�}���h�ɂȂ邩
					//���݂̔ԍ�������
					int num = static_cast<int>(m_commandId);

					//����̃R�}���h�ɂ���
					//enum�ō쐬���Ă��邽�߁A�グ�遁���l�������A�����遁���l���グ��
					num += 1;

					//�ύX���ꂽ�R�}���h�ɍX�V����
					m_commandId = static_cast<COMMAND_ID>(num);

					//�I��UI�̈ʒu���X�V
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y + 140);
				}
			}

			if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {

				//�V�[���J�ڏ����J�n�̈׃t���O�ύX
				m_sceneChangeFlg = true;
			}
			else if (Input::GetInstance().CheckButtonRelease(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
				//�v���X�G�j�[�{�^���̉�ʂɖ߂�
				m_titleState = TITLE_STATE::PRESS_ANY_BUTTON;

				if (m_commandId == COMMAND_ID::EDITOR)
				{
					//�I��UI�̈ʒu���X�V
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 140);
				}
				else if (m_commandId == COMMAND_ID::END)
				{
					//�I��UI�̈ʒu���X�V
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 280);
				}
				//�I�����Ă���R�}���h��������
				m_commandId = COMMAND_ID::MAIN_GAME;
				//�A���t�@�l��������
				m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->FlashingReset();
				//m_ui[static_cast<int>(IMAGEID::FRAME)]->FlashingReset();
			}
			break;
		}
		default:
			break;
		}
	}
	else//�V�[���J�ڏ����ɓ���B
	{
		switch (SceneChange::GetInstance()->GetSceneChangeFlg())
		{
		case SceneChange::SCENECHANGEID::FADDEIN:
			//�t�F�[�h�C�������A�I��������true���A��
			if (SceneChange::GetInstance()->UpdateIN())
			{
				SceneFlgDispose();//�I�������ɓ���

				//���̃V�[�����w��
				switch (m_commandId)
				{
				case Title::COMMAND_ID::MAIN_GAME:
					NextSceneStatus = SCENESTATUS::Game;
					break;
				case Title::COMMAND_ID::EDITOR:
					NextSceneStatus = SCENESTATUS::Editor;
					break;
				case Title::COMMAND_ID::END:
					NextSceneStatus = SCENESTATUS::End;
					break;
				default:
					break;
				}
			}
			break;
		case SceneChange::SCENECHANGEID::FADDEOUT:
			//�t�F�[�h�A�E�g�����A�I��������true���A��
			if (SceneChange::GetInstance()->UpdateOUT())
			{
				//�{�����J�n
				m_sceneChangeFlg = false;
			}
			break;
		case SceneChange::SCENECHANGEID::END:
			//IN�����ɓ���ׂɏ�����
			SceneChange::GetInstance()->init();
			break;
		default:
			break;
		}
		


		
	}
	

}

void Title::Draw()
{
	//�w�i�͏�ɕ\��
	m_ui[static_cast<int>(IMAGEID::TITLE_BG01)]->Draw();


	switch (m_titleState)
	{
	case Title::TITLE_STATE::PRESS_ANY_BUTTON: {
		//�v���X�G�j�[�{�^�����S
		m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->Draw();

		break;
	}
	case Title::TITLE_STATE::MAIN: {
		//�I�����Ă���R�}���h���킩�点��t���[��
		m_ui[static_cast<int>(IMAGEID::FRAME)]->Draw();
		//�R�}���h
		m_ui[static_cast<int>(IMAGEID::COMMAND)]->Draw();
		
		break;
	}
	default:
		break;
	}

	if (m_sceneChangeFlg == true)
	{
		SceneChange::GetInstance()->Draw();
	}
}

