#include "MainGame.h"
#include "Input.h"
#include "mesh.h"
#include "ModelMgr.h"
#include "CCamera.h"
#include "Collider.h"
#include "EnumListStage.h"
#include "XAudio2.h"
#include "SceneChange.h"




#ifdef DEBUG
// IMGUI�E�C���h�E
void Game::Imgui() {

	// Start the Dear ImGui frame�A�J�n����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//�����܂�

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 300));

	ImGui::Begin("config 1");

	//�t���[�����[�g
	//ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);

	static int num;

	ImGui::Text(u8"�X�e�[�W���A�G�Ɩ�����HP�\�L");
	ImGui::Text(u8"�X�e�[�W�P�O�ŃN���A");
	ImGui::Text(u8"���݂̃X�e�[�W�F%d", m_stageNumber + 1);
	//HP
	num = m_player->GetCSVStatus(PLAYER_STATUSID::HP);
	ImGui::Text(u8"���g��HP�F%d", num);
	for (int i = 0; i < m_enemyManager->GetEnemyList().size(); i++)
	{
		num = m_enemyManager->GetEnemyList()[i]->GetCSVStatus(ENEMY_STATUSID::HP);
		ImGui::Text(u8"�G%d��HP�F%d",i,num);
		
	}
	

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// Rendering�A�㏈��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
#endif//DEBUG

void Game::Init() {

	DebugFlg = true;//�f�o�b�O���邩�ǂ���

	//CSV�I�u�W�F�N�g��K�v�Ȑ��������̉�
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		m_csvData.push_back(std::make_unique<CSVFile>());
	}
	//UI�I�u�W�F�N�g��K�v�Ȑ��������̉�
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		m_ui.push_back(std::make_unique<Quad2D>());
	}
	

	//����̉�
	if (m_stand == nullptr)
	{
		m_stand = std::make_unique<Stand>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�X�^���h�I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//�X�J�C�h�[�����̉�
	if (m_skydome == nullptr)
	{
		m_skydome = std::make_unique<Skydome>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�X�J�C�h�[���I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//�v���C���[���̉�
	if (m_player == nullptr)
	{
		m_player = std::make_unique<Player>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�v���C���[�I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//�G�l�~�[�}�l�[�W���[���̉�
	if (m_enemyManager == nullptr)
	{
		m_enemyManager = std::make_unique<EnemyManager>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�G�l�~�[�}�l�[�W���[�I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//�o���b�g�}�l�[�W���[���̉�
	if (m_bulletManager == nullptr)
	{
		m_bulletManager = std::make_unique<BulletManager>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�o���b�g�}�l�[�W���[�I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//�X�L���}�l�[�W���[���̉�
	if (m_skillManager == nullptr)
	{
		m_skillManager = std::make_unique<SkillManager>();
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���C���Q�[���F�X�L���}�l�[�W���[�I�u�W�F�N�g�������ł��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	
	//CSV�ǂݍ���
	//data.csv_read(���̓t�@�C����, �w�b�_�[�̗L��, �C���f�b�N�X�̗L��, ��؂蕶��);
	m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvRead("assets/csv/PlayerStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvRead("assets/csv/EnemyStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::BULLET)]->CsvRead("assets/csv/BulletStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::STAGE)]->CsvRead("assets/csv/StageData.csv", ',');
	m_csvData[static_cast<int>(CSVID::MODEL_SS)]->CsvRead("assets/csv/ModelData_stage_sky.csv", ',');
	m_csvData[static_cast<int>(CSVID::SKILL)]->CsvRead("assets/csv/SkillStatus.csv", ',');
	
	// �X�J�C�h�[��������
	m_skydome->Init();

	//��̏�����
	m_stand->Init();

	//�v���C���[�̏�����
	m_player->Init(m_csvData[static_cast<int>(CSVID::PLAYER)]);
	//m_player->SetModel();
	
	////�e�}�l�[�W���[������
	m_bulletManager->Init(m_csvData[static_cast<int>(CSVID::BULLET)], m_player);

	//�X�L���}�l�[�W���[������
	m_skillManager->Init(m_csvData[static_cast<int>(CSVID::SKILL)], m_player);

	//�J�����Ƀv���C���[�Z�b�g
	CCamera::GetInstance()->SetPlayer(m_player);
	//�J�����ϐ����Z�b�g
	CCamera::GetInstance()->Reset();

	//UI������
	//HP�t���[��
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->LoadTexture("assets/image/battle/HPFlame.png");
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoSize((m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP)*2)+10, 70.0f);
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) + 150, 630);//�ő�1290�A730
	//HP�o�[
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->LoadTexture("assets/image/battle/white.png");
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoSize(m_player->GetCSVStatus(PLAYER_STATUSID::HP) * 2, 60.0f);
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::HP) + 150, 630);//�ő�1290�A730
	//HP���S
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->Init(150.0f, 150.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->LoadTexture("assets/image/battle/HPLogo.png");
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->updateAnimeNoPos(100, 630);//�ő�1290�A730

	//�N���A���S
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->LoadTexture("assets/image/battle/Clear.png");
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->updateAnimeNoPos(645, 365);//�ő�1290�A730
	//�Q�[���I�[�o�[���S
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->LoadTexture("assets/image/battle/Destroyed.png");
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->updateAnimeNoPos(645, 365);//�ő�1290�A730
	//�_���[�W���̃m�C�Y
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 1.0f, 1.0f);
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->LoadTexture("assets/image/battle/PDamage.png");
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->updateAnimeNoPos(645, 365);//�ő�1290�A730
	//����
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->Init(100.0f, 100.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 1.0f, 1.0f);
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->LoadTexture("assets/image/battle/suuji2.png");
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeNoPos(75, 75);//�ő�1290�A730
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeOne(0.0f);//�������O�ɂ��Ă���


	//�T�E���h
	Sound::instance().SetScene(SCENE_GAMESCENE);
	Sound::instance().SetVolumeAll(0.1f);
	Sound::instance().Play(SOUND_LABEL_GAME01_BGM);
	//Sound::instance().Set_Volume(SOUND_LABEL_GAME01_BGM, 0.1f);
	
	//�t�F�[�h���̃J�����ʒu�ׁ̈A�v���C���[�ʒu������
	m_player->SetStartPos(
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
	//�V�[���J�ڃt���O������
	switch (SceneChange::GetInstance()->GetSceneChangeFlg())
	{
	case SceneChange::SCENECHANGEID::FADDEIN:
		//��ʂɉ����Ȃ����ߖ{�����J�n
		m_sceneChangeFlg = false;
		break;
	case SceneChange::SCENECHANGEID::FADDEOUT:
		//�ʉ�ʂ���ڂ��Ă��Ă��邽�߃t�F�[�h�A�E�g�����Ɉڍs
		m_sceneChangeFlg = true;
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
	SceneStatus = SCENESTATUS::Game;//�Q�[���V�[���ɐݒ�
}

void Game::Exit() {

	//�S�Ă̗v�f�폜
	//CSV
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		CSVFile* exitCsv = m_csvData[i].release();
		delete exitCsv;
	}

	//��
	Stand* exitStand = m_stand.release();
	delete exitStand;
	//�X�J�C�h�[��
	Skydome* exitSkydome = m_skydome.release();
	delete exitSkydome;
	//�G�l�~�[�}�l�[�W���[
	m_enemyManager->Reset();//������
	EnemyManager* exitEnemyManager = m_enemyManager.release();
	delete exitEnemyManager;
	//�o���b�g�}�l�[�W���[
	BulletManager* exitBulletManager = m_bulletManager.release();
	delete exitBulletManager;
	//�X�L���}�l�[�W���[
	SkillManager* exitSkillManager = m_skillManager.release();
	delete exitSkillManager;
	//UI�폜
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		Quad2D* exitUI = m_ui[i].release();
		delete exitUI;
	}

	//���̃V�[�����w��
	NextSceneStatus = SCENESTATUS::Title;

	//�t���O���Z�b�g�A���̃V�[���̈�
	SceneFlgReset();
}

void Game::Update() {
	
	//�J������ݒ�
	CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

	//�V�[���J�ڏ����ɓ����Ă��Ȃ���Ώ������s��
	if (m_sceneChangeFlg == false)
	{
		switch (m_gameState)
		{
		case GAMESTATE::INIT: {

			//�{�X��ɔ��
			//m_stageNumber = 9;


			//�t�F�[�h�̃t���O�����Z�b�g
			m_fadeFlg = false;
			//�X�L���֘A�̃��Z�b�g
			m_ui[static_cast<int>(GAMEUIID::CLEAR)]->FaddeReset();
			m_skillManager->Reset();
			//HP�t���[���ݒ�
			m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoSize((m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) * 2) + 10, 70.0f);
			m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) + 150, 630);//�ő�1290�A730
			//�e������
			m_bulletManager->Reset();
			//�G�z�u
			m_enemyManager->Reset();//������
			//�G������z�񏉊���
			/*m_enemyList.clear();
			m_enemyList.shrink_to_fit();*/
			m_enemyList.erase(m_enemyList.begin(), m_enemyList.end());
			for (int _enemyType = static_cast<int>(STAGE_STATUSID::ENEMY_00); _enemyType < static_cast<int>(STAGE_STATUSID::END); _enemyType++)
			{
				for (int i = 0; i < m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(_enemyType); i++)
				{
					//���̃X�e�[�W�Ɏg���G������
					m_enemyList.emplace_back(static_cast<ENEMY_TYPE>(_enemyType - static_cast<int>(STAGE_STATUSID::ENEMY_00)));
				}
			}
			m_enemyManager->InitRandom(m_enemyList, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);//�G����
			if (m_stageNumber != 9)
			{
				//�ʒu�Z�b�g
				m_enemyManager->SetAllPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
			}
			else//�{�X�X�e�[�W�Ȃ�
			{
				m_enemyManager->GetEnemyList()[0]->SetPos(0,0);
			}
			
			//�v���C���ꏉ����
			//�ʒu������
			m_player->SetStartPos(
				m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
				m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
			m_player->Reset();//�v���C���[�ϐ����Z�b�g
			//�J�����ϐ����Z�b�g
			CCamera::GetInstance()->Reset();
			//���C���Q�[���J�n
			m_gameState = GAMESTATE::MAINGAME;
		}
			break;
		case GAMESTATE::MAINGAME: {
			if (m_player->GetState() == STATE::LIVE)
			{
				//�G�̃A�b�v�f�[�g
				m_enemyManager->Update(m_bulletManager);

				//�v���C���[�̃A�b�v�f�[�g
				m_player->Update();
				if (m_player->GetFireBulletFlg())
				{
					//�e����
					m_bulletManager->FireBulletPlayer();
					Sound::instance().Play(SOUND_LABEL_PSHOOT_SE);
				}
				//�e�̃A�b�v�f�[�g
				m_bulletManager->UpdateBullets(m_enemyManager->GetEnemyList());

				//m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnime(30);

				//HP�o�[�X�V
				m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoSize(
					m_player->GetCSVStatus(PLAYER_STATUSID::HP) * 2, 60.0f);
				m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoPos(
					m_player->GetCSVStatus(PLAYER_STATUSID::HP) + 150, 630);//�ő�1290�A730
				if (m_player->GetPHitEffectFlg() == PHitEffectFlg::HIT)
				{
					m_ui[static_cast<int>(GAMEUIID::NOISE)]->updateAnime(3);
				}

				if (m_enemyManager->AllDead())
				{
					if (m_player->GetState() == STATE::LIVE)
					{
						//�S�Ă̓G������ł���΃Q�[���N���A
						m_gameState = GAMESTATE::CLEAR;

					}
				}

				
			}
			else if (m_player->GetState() == STATE::DEAD)
			{
				m_gameState = GAMESTATE::GAMEOVER;
				Sound::instance().Play(SOUND_LABEL_PDEAD_SE);
			}
		}
			break;
		case GAMESTATE::CLEAR: {

			if (m_fadeFlg == false)
			{
				m_ui[static_cast<int>(GAMEUIID::CLEAR)]->FaddeIN(0.01);

				if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {
					m_fadeFlg = true;
				}
			}
			else
			{
				if (m_ui[static_cast<int>(GAMEUIID::CLEAR)]->FaddeOUT(0.01))
				{
					//�ŏI�X�e�[�W�N���A�Ȃ̂ŏI���
					if (m_stageNumber == 9)
					{
						//�Q�[���N���A
						Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);
						//�V�[���J�ڏ����J�n�̈׃t���O�ύX
						m_sceneChangeFlg = true;
					}
					else
					{
						//���I����
						m_skillManager->SkillSelect();
						//�X�L�����I��ʂɔ��
						m_gameState = GAMESTATE::CHOICESKILL;
					}
					
				}
			}
		}
			break;
		case GAMESTATE::GAMEOVER: {
			m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->FaddeIN(0.002);

			if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {
				Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);
				SceneFlgDispose();//�I�������ɓ���
			}

		}
			break;
		case GAMESTATE::CHOICESKILL: {

			if (m_skillManager->Update())
			{
				//�X�e�[�W�i�U
				m_stageNumber++;
				//�X�L���������I������玟�̃X�e�[�W�Ɉڂ�
				m_gameState = GAMESTATE::INIT;
				m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeOne(m_stageNumber);//�\�����鐔�����X�V
			}

		}
			break;
		case GAMESTATE::END: {

		}
			break;
		default:
			break;
		}

		//g_PX.idle();//�{�^�����͂��X�V
		if (Input::GetInstance().CheckButtonPress(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
			Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);
			//�V�[���J�ڏ����J�n�̈׃t���O�ύX
			m_sceneChangeFlg = true;
		}

	}
	else//�V�[���J�ڏ����ɓ���B
	{
		switch (SceneChange::GetInstance()->GetSceneChangeFlg())
		{
		case SceneChange::SCENECHANGEID::FADDEIN: {
			//�t�F�[�h�C�������A�I��������true���A��
			if (SceneChange::GetInstance()->UpdateIN())
			{
				SceneFlgDispose();//�I�������ɓ���
			}
		}
			break;
		case SceneChange::SCENECHANGEID::FADDEOUT: {
			//�t�F�[�h�A�E�g�����A�I��������true���A��
			if (SceneChange::GetInstance()->UpdateOUT())
			{
				//�{�����J�n
				m_sceneChangeFlg = false;
			}
		}
			break;
		case SceneChange::SCENECHANGEID::END: {
			//IN�����ɓ���ׂɏ�����
			SceneChange::GetInstance()->init();
		}
			break;
		default:
			break;
		}
	}
}

void Game::Draw() {

	//�X�J�C�h�[���`��
	m_skydome->Draw();
	//��̕`��
	m_stand->Draw_transform(
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
		1.0f,m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
	
	
	//�v���C���[�`��
	m_player->Draw();
	//�G�`��
	m_enemyManager->DrawAll();

	//�e�\��
	m_bulletManager->DrawBullets();

	//UI��`��
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		if (i == static_cast<int>(GAMEUIID::CLEAR))
		{
			if (m_gameState == GAMESTATE::CLEAR)
			{
				//�N���A���S
				m_ui[static_cast<int>(GAMEUIID::CLEAR)]->Draw();
			}
		}
		else if (i == static_cast<int>(GAMEUIID::GAMEOVER))
		{
			if (m_gameState == GAMESTATE::GAMEOVER)
			{
				//�Q�[���I�[�o�[���S
				m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->Draw();
			}
		}
		else
		{
			//�X�L�����UI�\��
			if (m_gameState == GAMESTATE::CHOICESKILL)
			{
				m_skillManager->Draw();
			}
			else
			{
				if (i == static_cast<int>(GAMEUIID::NOISE))
				{
					if (m_player->GetPHitEffectFlg() == PHitEffectFlg::HIT)
					{
						m_ui[static_cast<int>(GAMEUIID::NOISE)]->Draw();
					}
				}
				else
				{
					//��ɕ`�悷�����
					m_ui[i]->Draw();
				}
			}
		}

		

	}

	//�V�[���J�ڕ`��
	if (m_sceneChangeFlg == true)
	{
		SceneChange::GetInstance()->Draw();
	}
}

