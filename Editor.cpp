#include "Editor.h"
#include "Input.h"
#include "mesh.h"
#include "ModelMgr.h"
#include "CCamera.h"
#include "Collider.h"
#include "XAudio2.h"
#include "EnumListStage.h"
#include "SceneChange.h"


void Editor::Imgui()
{
	// Start the Dear ImGui frame�A�J�n����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//�����܂�
	std::string str;
	ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	if (ImGui::Begin(u8"�X�e�[�W�쐬�t�h", nullptr, ImGuiWindowFlags_MenuBar))
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Separator();


		ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			
			if (ImGui::BeginTabItem(u8"�v���C���["))
			{
				//���̃G�f�B�^�[���ݒ�
				m_guiState = IMGUI_STATE::PLAYER;
				
				ImGui::EndTabItem();
				//����
				ImGui::Text(u8"�����̃v���C���[�X�e�[�^�X��ύX����");
				//�ړ����x
				ImGui::InputFloat(u8"�ړ����x", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)] < 0)
				{
					//�}�C�i�X�ɂȂ�����߂�
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)] = 0;
				}
				//�ő�̗�
				ImGui::InputFloat(u8"�ő�HP", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)], 10, 50);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)] < 0)
				{
					//�}�C�i�X�ɂȂ�����߂�
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)] = 0;
				}
				//�̗�
				ImGui::InputFloat(u8"����HP", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)], 10, 50);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)] < 0)
				{
					//�}�C�i�X�ɂȂ�����߂�
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)] = 0;
				}
				//�U����
				ImGui::Text(u8"���e�̍U���͂ɂ�����{�����U���́B�P�ŕϓ��Ȃ�");
				ImGui::InputFloat(u8"�U����", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)] < 0)
				{
					//�}�C�i�X�ɂȂ�����߂�
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)] = 0;
				}
				//�����[�h����
				ImGui::InputFloat(u8"�����[�h����", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)] < 0)
				{
					//�}�C�i�X�ɂȂ�����߂�
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)] = 0;
				}
				//�ۑ�����
				if (ImGui::Button(u8"�ۑ�����")) {
					m_saveFlg = true;
				}
			}
			else
			{
				//�I������Ă��Ȃ��̂Ńt���O��܂�
				m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] = false;
			}

			if (ImGui::BeginTabItem(u8"�X�L��"))
			{
				
				//���̃G�f�B�^�[���ݒ�
				m_guiState = IMGUI_STATE::SKILL;
				static const char* SkillName[]
				{
					u8"�͂��ɑ̗͂���",
					u8"�U���̓A�b�v",
					u8"��",
					u8"�ő�̗͏㏸",
					u8"�ړ����x�㏸",
					u8"�����[�h���x�㏸",
					u8"�O�e�ǉ�",
					u8"���e�ǉ�",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == true)
				{
					//����
					ImGui::Text(u8"�X�L���̃X�e�[�^�X��ύX����");
					ImGui::Text(u8"�^�u���ύX�����Ə����������");
					//�X�L���؂�ւ�
					ImGui::Text(u8"�X�L���̎�ސ؂�ւ�");
					ImGui::ListBox(u8"�G�؂�ւ�", &m_choiceSkill, SkillName, 
						static_cast<int>(SKILLTYPEID::END)-static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE));
					//m_choiceSkill = static_cast<ENEMY_TYPE>(select);//�I�����Ă���G�̕ύX
					//�グ��l
					ImGui::InputFloat(u8"�㏸�l", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)), 0.1, 5);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)) = 0;
					}
					//���݃��x��
					ImGui::InputFloat(u8"���x���A��{�͂O", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)) = 0;
					}
					//�ő僌�x��
					ImGui::InputFloat(u8"�ő僌�x��", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)) = 0;
					}
					//�m��
					ImGui::Text(u8"�����傫���قǓ�����₷���Ȃ�B���ł����݉\");
					ImGui::InputFloat(u8"���I�m���䗦", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)) = 0;
					}
					//�ۑ�����
					if (ImGui::Button(u8"�ۑ�����")) {
						m_saveFlg = true;
					}
				}
				
			}
			else
			{
				//�I������Ă��Ȃ��̂Ńt���O��܂�
				m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] = false;
			}

			if (ImGui::BeginTabItem(u8"�G�l�~�["))
			{
				//���̃G�f�B�^�[���ݒ�
				m_guiState = IMGUI_STATE::ENEMY;
				static const char* EnemyName[]
				{
					u8"�^���[�̓G",
					u8"���̏�ŏ㉺����G",
					u8"�ǔ����Ă���G",
					u8"�{�X�P",
				};
				static const char* BulletName[]
				{
					u8"�G�ʏ�e",
					u8"�j��ł��Ȃ��G�ʏ�e",
					u8"�z�[�~���O�e",
					u8"����",
					//u8"�v���C���[�m�[�}���e",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] == true)
				{
					//����
					ImGui::Text(u8"�G�l�~�[�̃X�e�[�^�X��ύX����");
					ImGui::Text(u8"�^�u���ύX�����Ə����������");
					ImGui::Text(u8"�z�[�~���O�e�̓v���C���[�ɂ�����Ə�Ɍ����Ĉړ�����");
					//�X�L���؂�ւ�
					ImGui::Text(u8"�G�̎�ސ؂�ւ�");
					ImGui::ListBox(u8"�G�؂�ւ�", &m_choiceEnemy, EnemyName, static_cast<int>(ENEMY_TYPE::END));
					//�e�؂�ւ�
					ImGui::Text(u8"�e�̎�ސ؂�ւ��A�I�񂾒e�𑕔�");
					ImGui::ListBox(u8"�e�؂�ւ�", &m_choiceBullet, BulletName, static_cast<int>(BULLET_TYPE::P_NORMAL));
					m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)) = m_choiceBullet;
				
					//�ړ����x
					ImGui::InputFloat(u8"�ړ����x", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)) = 0;
					}
					//�̗�
					ImGui::InputFloat(u8"�̗�", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)), 1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)) = 0;
					}
					//�U����
					ImGui::Text(u8"���e�̍U���͂ɂ�����{�����U���́B�P�ŕϓ��Ȃ�");
					ImGui::InputFloat(u8"�U����", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)) = 0;
					}
					//�ő��]�p�x
					ImGui::InputFloat(u8"�ő��]�p�x", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)) = 0;
					}
					//�����[�h���x
					ImGui::InputFloat(u8"�����[�h����", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) < 0)
					{
						//�}�C�i�X�ɂȂ�����߂�
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) = 0;
					}
					//�����������߂�
					if (m_choiceBullet == static_cast<int>(BULLET_TYPE::HOMMING))
					{
						bool _directionFlg = m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT));
						ImGui::Checkbox(u8"�z�[�~���O�e����", &_directionFlg);
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT)) = _directionFlg;
						for (int i = static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT) + 1; i < static_cast<int>(ENEMY_STATUSID::END); i++)
						{
							m_enemyChangeData.at(m_choiceEnemy).at(i) = false;
						}
					}
					else
					{
						for (int i = static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT); i < static_cast<int>(ENEMY_STATUSID::END); i++)
						{
							bool _directionFlg = m_enemyChangeData.at(m_choiceEnemy).at(i);
							switch (static_cast<ENEMY_STATUSID>(i))
							{
							case ENEMY_STATUSID::DIRECTION_FRONT: {
								ImGui::Checkbox(u8"�O���Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_LEFT: {
								ImGui::Checkbox(u8"���Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_RIGHT: {
								ImGui::Checkbox(u8"�E�Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK: {
								ImGui::Checkbox(u8"���Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_FRONT_LEFT: {
								ImGui::Checkbox(u8"���O�Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_FRONT_RIGHT: {
								ImGui::Checkbox(u8"�E�O�Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK_LEFT: {
								ImGui::Checkbox(u8"�����Ɍ���", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK_RIGHT: {
								ImGui::Checkbox(u8"�E���Ɍ���", &_directionFlg);
							}
								break;
							default:
								break;
							}
							m_enemyChangeData.at(m_choiceEnemy).at(i) = _directionFlg;
						}
					}
					//�ۑ�����
					if (ImGui::Button(u8"�ۑ�����")) {
						m_saveFlg = true;
					}
					
				}
			}
			else
			{
				//�I������Ă��Ȃ��̂Ńt���O��܂�
				m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] = false;
			}

			if (ImGui::BeginTabItem(u8"�e��"))
			{
				//���̃G�f�B�^�[���ݒ�
				m_guiState = IMGUI_STATE::BULLET;
				static const char* BulletName[]
				{
					u8"�G�ʏ�e",
					u8"�j��ł��Ȃ��G�ʏ�e",
					u8"�z�[�~���O�e",
					u8"����",
					u8"�v���C���[�m�[�}���e",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == true)
				{
					//����
					ImGui::Text(u8"�e�̃X�e�[�^�X��ύX����");
					ImGui::Text(u8"�^�u���ύX�����Ə����������F���݂͔j��ł��Ȃ��G�ʏ�e�͕ύX�s��");
					ImGui::Text(u8"�z�[�~���O�e�̓v���C���[�ɂ�����Ə�Ɍ����Ĉړ�����");
					//�e�؂�ւ�
					ImGui::Text(u8"�e�̎�ސ؂�ւ�");
					ImGui::ListBox(u8"�e�؂�ւ�", &m_choiceBullet, BulletName, static_cast<int>(BULLET_TYPE::END));
					if (m_choiceBullet != static_cast<int>(BULLET_TYPE::NONE))
					{
						//�ړ����x
						ImGui::InputFloat(u8"�ړ����x", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)) < 0)
						{
							//�}�C�i�X�ɂȂ�����߂�
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)) = 0;
						}
						//�U����
						ImGui::InputFloat(u8"�U����", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)) < 0)
						{
							//�}�C�i�X�ɂȂ�����߂�
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)) = 0;
						}
						//��������
						ImGui::InputFloat(u8"��������", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)) < 0)
						{
							//�}�C�i�X�ɂȂ�����߂�
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)) = 0;
						}
						//�ۑ�����
						if (ImGui::Button(u8"�ۑ�����")) {
							m_saveFlg = true;
						}
					}
					
				}
			}
			else
			{
				//�I������Ă��Ȃ��̂Ńt���O��܂�
				m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] = false;
			}

			if (ImGui::BeginTabItem(u8"�X�e�[�W"))
			{
				//���̃G�f�B�^�[���ݒ�
				m_guiState = IMGUI_STATE::STAGE;

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] == true)
				{
					//����
					ImGui::Text(u8"�X�e�[�W�̃X�e�[�^�X��ύX����");
					ImGui::Text(u8"�X�V�{�^���������ƌ��ݑI�����Ă���X�e�[�W���Ĕz�u����");
					ImGui::Text(u8"�z�[�~���O�e�̓v���C���[�ɂ�����Ə�Ɍ����Ĉړ�����");
					//�X�e�[�W�ύX
					ImGui::InputInt(u8"�ҏW����X�e�[�W��ύX����", &m_stageNumber, 1, 5);
					if (m_stageNumber < 0) m_stageNumber = 0;
					else if (m_stageNumber > MAXSTAGE) m_stageNumber = MAXSTAGE;
					//��������
					ImGui::InputFloat(u8"�����̒����A�T�O���ŏ��l", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)) < 50)
					{
						//50�ȉ��Ȃ�50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)) = 50;
					}
					//�c������
					ImGui::InputFloat(u8"�c���̒����A�T�O���ŏ��l", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)) < 50)
					{
						//50�ȉ��Ȃ�50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)) = 50;
					}
					//�G�̐ݒ�
					ImGui::InputFloat(u8"�G�P", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)) < 0)
					{
						//50�ȉ��Ȃ�50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)) = 0;
					}
					ImGui::InputFloat(u8"�G2", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)) < 0)
					{
						//50�ȉ��Ȃ�50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)) = 0;
					}
					ImGui::InputFloat(u8"�G3", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)) < 0)
					{
						//50�ȉ��Ȃ�50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)) = 0;
					}

					//�ۑ�����
					if (ImGui::Button(u8"�ۑ�����")) {
						m_saveFlg = true;
					}
					//�ۑ�����
					if (ImGui::Button(u8"�Ĕz�u����")) {
						m_resetFlg = true;
					}
				}
			}
			else
			{
				//�I������Ă��Ȃ��̂Ńt���O��܂�
				m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] = false;
			}

			ImGui::EndTabBar();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::EndChild();

		ImGui::EndGroup();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//�Q��
	ImGui::Begin(u8"�T�u");

	//�}�ffloat
	static float num;


	//�e�V�[�����Ƃɏo���������̂��o��
	switch (m_guiState)
	{
	case Editor::IMGUI_STATE::PLAYER: {
		ImGui::Text(u8"���@�FL�X�e�B�b�N�F�ړ�");
		ImGui::Text(u8"���@�FR�X�e�B�b�N�F�����ύX");

		//���L�[
		num = Input::GetInstance().GetLStick().x;
		ImGui::InputFloat("input float", &num);
		num = Input::GetInstance().GetLStick().y;
		ImGui::InputFloat("input float", &num);

		//�E�L�[
		num = Input::GetInstance().GetRStick().x;
		ImGui::InputFloat("input float", &num);
		num = Input::GetInstance().GetRStick().y;
		ImGui::InputFloat("input float", &num);
	}
		break;
	case Editor::IMGUI_STATE::SKILL: {

		ImGui::Text(u8"�X�L���S�Ă̓��I�m���ꗗ");

		if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == true)
		{
			for (int i = 0; i < static_cast<int>(SKILLTYPEID::END) - static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE); i++)
			{
				switch (static_cast<SKILLTYPEID>(i + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)))
				{
				case SKILLTYPEID::MAXHPUP_LITTLE:
					ImGui::Text(u8"�͂��ɑ̗͂���");
					break;
				case SKILLTYPEID::ATKUP:
					ImGui::Text(u8"�U���̓A�b�v");
					break;
				case SKILLTYPEID::HEEL:
					ImGui::Text(u8"��");
					break;
				case SKILLTYPEID::MAXHPUP:
					ImGui::Text(u8"�ő�̗͏㏸");
					break;
				case SKILLTYPEID::SPEEDUP:
					ImGui::Text(u8"�ړ����x�㏸");
					break;
				case SKILLTYPEID::RELOADUP:
					ImGui::Text(u8"�����[�h���x�㏸");
					break;
				case SKILLTYPEID::BULLETFRONT:
					ImGui::Text(u8"�O�e�ǉ�");
					break;
				case SKILLTYPEID::BULLETBACK:
					ImGui::Text(u8"���e�ǉ�");
					break;
				default:
					break;
				}
				//���������K�v���Ȃ��̂ŋ󔒂������Ă���
				ImGui::InputFloat(" ", &m_skillChangeData.at(i).at(static_cast<int>(SKILL_STATUSID::RATE)));
			}
		}
	}
		break;
	case Editor::IMGUI_STATE::ENEMY: {
		ImGui::Text(u8"���@�FL�X�e�B�b�N�F�ړ�");
		ImGui::Text(u8"���@�FR�X�e�B�b�N�F�����ύX");
	}
		break;
	case Editor::IMGUI_STATE::BULLET: {
		ImGui::Text(u8"���@�FL�X�e�B�b�N�F�ړ�");
		ImGui::Text(u8"���@�FR�X�e�B�b�N�F�����ύX");

		if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == true)
		{
			ImGui::Text(u8"0.2�Ŏ~�܂�悤�ɂ����Ă���");
			ImGui::Text(u8"���̍��ڂ̕ύX�͕ۑ�����Ȃ�");
			//��������
			ImGui::InputFloat(u8"���ˊԊu�ύX", &m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)), 0.1, 5);
			if (m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) < 0.2)
			{
				//�}�C�i�X�ɂȂ�����߂�
				m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) = 0.2;
			}
		}
	}
		break;
	case Editor::IMGUI_STATE::STAGE: {
		ImGui::Text(u8"���@�FL�X�e�B�b�N�F�ړ�");
		ImGui::Text(u8"���@�FR�X�e�B�b�N�F�����ύX");
	}
		break;
	default:
		break;
	}

	ImGui::End();


	// Rendering�A�㏈��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Init()
{
	DebugFlg = true;//�f�o�b�O���邩�ǂ���

	//CSV���̉�
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		m_csvData.push_back(std::make_unique<CSVFile>());
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
		MessageBox(nullptr, "�G�f�B�^�[�V�[���F�X�^���h�I�u�W�F�N�g�������ł��܂���",
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
		MessageBox(nullptr, "�G�f�B�^�[�V�[���F�X�J�C�h�[���I�u�W�F�N�g�������ł��܂���",
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
		MessageBox(nullptr, "�G�f�B�^�[�V�[���F�v���C���[�I�u�W�F�N�g�������ł��܂���",
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
		MessageBox(nullptr, "�G�f�B�^�[�V�[���F�G�l�~�[�}�l�[�W���[�I�u�W�F�N�g�������ł��܂���",
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
		MessageBox(nullptr, "�G�f�B�^�[�V�[���F�o���b�g�}�l�[�W���[�I�u�W�F�N�g�������ł��܂���",
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
	m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvRead("assets/csv/PlayerStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvRead("assets/csv/EnemyStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::BULLET)]->CsvRead("assets/csv/BulletStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::STAGE)]->CsvRead("assets/csv/StageData.csv", ',');
	m_csvData[static_cast<int>(CSVID::MODEL_SS)]->CsvRead("assets/csv/ModelData_stage_sky.csv", ',');
	m_csvData[static_cast<int>(CSVID::SKILL)]->CsvRead("assets/csv/SkillStatus.csv", ',');

	//�G���f���̓ǂݍ���
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::ENEMY)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::ENEMY)]->GetTexFolder().at(i));
	}
	//�e���f���̓ǂݍ���
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::BULLET)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::BULLET)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::BULLET)]->GetTexFolder().at(i));
	}
	//�v���C���[���f���̓ǂݍ���
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::PLAYER)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::PLAYER)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::PLAYER)]->GetTexFolder().at(i));
	}
	//�X�e�[�W�A�X�J�C�h�[�����f���̓ǂݍ���
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetTexFolder().at(i));
	}
	
	//�s�񏉊����A�ݒ�
	DX11MtxIdentity(m_mtx);
	m_mtx._41 -= 40.0f;

	m_skydome->Init();
	
	//��̏�����
	m_stand->Init();
	
	//�v���C���[�̏�����
	m_player->Init(m_csvData[static_cast<int>(CSVID::PLAYER)]);

	//�e�}�l�[�W���[������
	m_bulletManager->Init(m_csvData[static_cast<int>(CSVID::BULLET)], m_player);

	//�X�L���}�l�[�W���[������
	m_skillManager->Init(m_csvData[static_cast<int>(CSVID::SKILL)], m_player);
	//�X�e�[�W������
	m_stageNumber = 0;

	//�J�����Ƀv���C���[�Z�b�g
	CCamera::GetInstance()->SetPlayer(m_player);
	//�J�����ϐ����Z�b�g
	CCamera::GetInstance()->Reset();

	//�T�E���h
	Sound::instance().SetScene(SCENE_GAMESCENE);
	Sound::instance().SetVolumeAll(0.1f);
	Sound::instance().Play(SOUND_LABEL_GAME01_BGM);

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
	SceneStatus = SCENESTATUS::Editor;//�G�f�B�^�[�V�[���ɐݒ�

}

void Editor::Exit()
{
	m_skydome->Finalize();

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
	EnemyManager* exitEnemyManager = m_enemyManager.release();
	delete exitEnemyManager;
	//�o���b�g�}�l�[�W���[
	BulletManager* exitBulletManager = m_bulletManager.release();
	delete exitBulletManager;
	//�X�L���}�l�[�W���[
	SkillManager* exitSkillManager = m_skillManager.release();
	delete exitSkillManager;
	

	//���̃V�[�����w��
	NextSceneStatus = SCENESTATUS::Title;

	//�t���O���Z�b�g�A���̃V�[���̈�
	SceneFlgReset();

	
}

void Editor::Update()
{
	//�V�[���J�ڏ����ɓ����Ă��Ȃ���Ώ������s��
	if (m_sceneChangeFlg == false)
	{
		switch (m_guiState)
		{
		case Editor::IMGUI_STATE::PLAYER: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] == false)//������
			{
				//�X�e�[�W������
				m_stageNumber = 0;
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(PLAYER_STATUSID::END); i++)
				{
					//�v���C���[�̃X�e�[�^�X������
					m_playerChangeData[i] = m_csvData[static_cast<int>(CSVID::PLAYER)]->GetCell().at(0).at(i);
				}
				//�e������
				m_bulletManager->Reset();
				//�G������
				m_enemyManager->Reset();
				//�ʒu�������A�Ɠ����Ƀt�B�[���h�̑傫���Z�b�g
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				//��������������
				m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] = true;
			}
			else//�A�b�v�f�[�g
			{
				//�J������ݒ�
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//�L�[���͂ɂ��ړ�
				m_player->Update();
				
				if (m_player->GetFireBulletFlg())
				{
					//�e����
					m_bulletManager->FireBulletPlayer();
					Sound::instance().Play(SOUND_LABEL_PSHOOT_SE);
				}

				m_bulletManager->UpdateBulletsPlayer();

				//�f�[�^���e�̐��l�𔽉f
				m_player->SetAllStatus(m_playerChangeData);

				if (m_saveFlg)
				{
					//�v���C���[�̐��l���X�V
					for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
					{
						//�v���C���[�̃X�e�[�^�X������
						m_csvData[static_cast<int>(CSVID::PLAYER)]->SetCell(static_cast<PLAYER_STATUSID>(i), m_playerChangeData[i]);
					}

					//�f�[�^��ۑ�
					m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvWrite("assets/csv/PlayerStatus.csv", ',');

					//�t���O��܂�
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::SKILL: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == false)//������
			{
				//�X�e�[�W������
				m_stageNumber = 0;
				//���g�폜
				m_skillChangeData.clear();
				m_skillChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE); i < static_cast<int>(SKILLTYPEID::END); i++)
				{
					//����P�ǉ�
					m_skillChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(SKILL_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_skillChangeData.at(i - static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)).emplace_back(
							m_csvData[static_cast<int>(CSVID::SKILL)]->GetCell().at(i).at(id));
					}
				}

				//�e������
				m_bulletManager->Reset();
				//�G������
				m_enemyManager->Reset();
				//��������������
				m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] = true;
			}
			else//�A�b�v�f�[�g
			{
				//�J������ݒ�
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//�f�[�^���e�̐��l�𔽉f
				m_skillManager->SetAllStatus(m_choiceSkill + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE),
					m_skillChangeData.at(m_choiceSkill));

				if (m_saveFlg)
				{
					//�X�L���̐��l���X�V
					for (int i = 0; i < static_cast<int>(SKILL_STATUSID::END); i++)
					{
						//�X�L���̃X�e�[�^�X������
						m_csvData[static_cast<int>(CSVID::SKILL)]->SetCell(static_cast<SKILLTYPEID>(
							m_choiceSkill + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)),
							static_cast<SKILL_STATUSID>(i), m_skillChangeData.at(m_choiceSkill).at(i));
					}

					//�f�[�^��ۑ�
					m_csvData[static_cast<int>(CSVID::SKILL)]->CsvWrite("assets/csv/SkillStatus.csv", ',');

					//�t���O��܂�
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::ENEMY: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] == false)//������
			{
				//�X�e�[�W������
				m_stageNumber = 0;
				//�e���g�폜
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//����P�ǉ�
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//�G�l�~�[�폜
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//����P�ǉ�
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//�e������
				m_bulletManager->Reset();
				//�G������
				m_enemyManager->Reset();
				//�G�̔z�񒆐g�폜
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				//�g���G�𐶐�
				m_enemyManager->InitEditor(m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);

				//�ʒu�Z�b�g
				m_enemyManager->SetPosCenter(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//�v���C���[�ʒu�������A�Ɠ����Ƀt�B�[���h�̑傫���Z�b�g
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//������
				m_choiceBullet = m_enemyManager->GetEnemyList()[0]->GetBulletType();
				m_choiceEnemy = 0;
				m_choiceEnemyOld = m_choiceEnemy;
				m_choiceEnemyPrevious = 0;
				//��������������
				m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] = true;

			}
			else//�A�b�v�f�[�g
			{
				//�J������ݒ�
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//�L�[���͂ɂ��ړ�
				m_player->Update();

				//�G�̃A�b�v�f�[�g
				m_enemyManager->UpdateEditor(m_choiceEnemy, m_bulletManager);

				//�e�̃A�b�v�f�[�g
				m_bulletManager->UpdateBulletsEnemy();

				//�f�[�^���e�̐��l�𔽉f
				m_enemyManager->GetEnemyList()[m_choiceEnemy]->SetCsvStatus(m_enemyChangeData.at(m_choiceEnemy));

				//�I������Ă���G���ς�������A�S�Ă̐��l��������
				if (m_choiceEnemy != m_choiceEnemyOld)
				{
					//�I�񂾓G�̐��l��������
					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_enemyChangeData.at(m_choiceEnemy).at(id) = m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(m_choiceEnemy).at(id);
					}

					//�I�񂾓G��������
					m_enemyManager->GetEnemyList()[m_choiceEnemy]->SetCsvStatus(m_enemyChangeData.at(m_choiceEnemy));
					//�I��ł���e�̎�ނ��X�V�B
					m_choiceBullet = m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE));
					//�ǔ��̓G�̈ʒu��������
					m_enemyManager->GetEnemyList()[2]->SetPos(0, 0);

					//���������������̂ŁA�X�V
					m_choiceEnemyOld = m_choiceEnemy;
				}

				if (m_saveFlg)
				{
					//�G�̐��l���X�V
					for (int i = 0; i < static_cast<int>(ENEMY_STATUSID::END); i++)
					{

						//�G�̃X�e�[�^�X������
						m_csvData[static_cast<int>(CSVID::ENEMY)]->SetCell(static_cast<ENEMY_TYPE>(m_choiceEnemy),
							static_cast<ENEMY_STATUSID>(i), m_enemyChangeData.at(m_choiceEnemy).at(i));
					}

					//�f�[�^��ۑ�
					m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvWrite("assets/csv/EnemyStatus.csv", ',');

					//�t���O��܂�
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::BULLET: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == false)//������
			{
				//�X�e�[�W������
				m_stageNumber = 0;
				//�e���g�폜
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//����P�ǉ�
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//�G�l�~�[�폜
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//����P�ǉ�
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//�e������
				m_bulletManager->Reset();
				//�G������
				m_enemyManager->Reset();
				//�G�̔z�񒆐g�폜
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				//�g���G�𐶐�
				m_enemyArray.emplace_back(ENEMY_TYPE::ROTATION);//���̓G��ǉ�
				m_enemyManager->InitRandom(m_enemyArray, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);

				//�ʒu�Z�b�g
				m_enemyManager->SetPosCenter(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//�v���C���[�ʒu�������A�Ɠ����Ƀt�B�[���h�̑傫���Z�b�g
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//�e�̑I��ԍ���������
				m_choiceBullet = 0;
				//��������������
				m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] = true;
			}
			else//�A�b�v�f�[�g
			{
				//�J������ݒ�
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//�L�[���͂ɂ��ړ�
				m_player->Update();

				//�e�̎�ޕύX
				m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)) = m_choiceBullet;

				//�G�̃A�b�v�f�[�g
				m_enemyManager->Update(m_bulletManager);

				//�e�̃A�b�v�f�[�g
				m_bulletManager->UpdateBulletsEnemy();

				//�f�[�^���e�̐��l�𔽉f
				m_bulletManager->SetCsvStatus(static_cast<BULLET_TYPE>(m_choiceBullet), m_bulletChangeData.at(m_choiceBullet));
				m_enemyManager->GetEnemyList()[0]->SetCsvStatus(m_enemyChangeData.at(0));

				if (m_saveFlg)
				{
					//�X�L���̐��l���X�V
					for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
					{
						//�X�L���̃X�e�[�^�X������
						m_csvData[static_cast<int>(CSVID::BULLET)]->SetCell(static_cast<BULLET_TYPE>(m_choiceBullet),
							static_cast<BULLET_STATUSID>(i), m_bulletChangeData.at(m_choiceBullet).at(i));
					}

					//�f�[�^��ۑ�
					m_csvData[static_cast<int>(CSVID::BULLET)]->CsvWrite("assets/csv/BulletStatus.csv", ',');

					//�t���O��܂�
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::STAGE: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] == false)//������
			{
				//�X�e�[�W���g�폜
				m_stageChangeData.clear();
				m_stageChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < 10; i++)
				{
					//����P�ǉ�
					m_stageChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(STAGE_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_stageChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(i).at(id));
					}
				}
				//�e���g�폜
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//����P�ǉ�
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//�G�l�~�[�폜
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSV����f�[�^���擾
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//����P�ǉ�
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//�v�f�����Ă���
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//�e������
				m_bulletManager->Reset();
				//�G������
				m_enemyManager->Reset();
				//�g���G�𐶐�
				//�G������z�񏉊���
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				for (int _enemyType = static_cast<int>(STAGE_STATUSID::ENEMY_00); _enemyType < static_cast<int>(STAGE_STATUSID::END); _enemyType++)
				{
					for (int i = 0; i < m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(_enemyType); i++)
					{
						//���̃X�e�[�W�Ɏg���G������
						m_enemyArray.emplace_back(static_cast<ENEMY_TYPE>(_enemyType - static_cast<int>(STAGE_STATUSID::ENEMY_00)));
					}
				}
				m_enemyManager->InitRandom(m_enemyArray, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);//�G����
				//�ʒu�Z�b�g
				m_enemyManager->SetAllPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//�v���C���[�ʒu�������A�Ɠ����Ƀt�B�[���h�̑傫���Z�b�g
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//��������������
				m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] = true;
			}
			else//�A�b�v�f�[�g
			{
				//�J������ݒ�
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());
				
				//�L�[���͂ɂ��ړ�
				m_player->Update();
				//�G�̃A�b�v�f�[�g
				m_enemyManager->Update(m_bulletManager);

				//�e�̃A�b�v�f�[�g
				m_bulletManager->UpdateBulletsEnemy();



				if (m_saveFlg)
				{
					//�X�L���̐��l���X�V
					for (int i = 0; i < static_cast<int>(STAGE_STATUSID::END); i++)
					{
						//�X�L���̃X�e�[�^�X������
						m_csvData[static_cast<int>(CSVID::STAGE)]->SetCell(m_stageNumber,
							static_cast<STAGE_STATUSID>(i), m_stageChangeData.at(m_stageNumber).at(i));
					}

					//�f�[�^��ۑ�
					m_csvData[static_cast<int>(CSVID::STAGE)]->CsvWrite("assets/csv/StageData.csv", ',');

					//�t���O��܂�
					m_saveFlg = false;
				}

				if (m_resetFlg)
				{
					//��x�O�ɏo���ď�����������
					m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] = false;

					//�t���O��܂�
					m_resetFlg = false;
				}
			}
		}
		break;
		default:
			break;
		}

		//���C���ɖ߂�
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
			Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);//BGM���~�߂�
			//�V�[���J�ڏ����J�n�̈׃t���O�ύX
			m_sceneChangeFlg = true;
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
			}
			break;
		case SceneChange::SCENECHANGEID::FADDEOUT:
			//�J������ݒ�
			CCamera::GetInstance()->CameraDelay(m_player->GetMtx());

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

void Editor::Draw()
{
	//�X�e�[�W�ҏW��ʈȊO�͌Œ�
	if (m_guiState != Editor::IMGUI_STATE::STAGE)
	{
		//��̕`��A���ۂ̃X�e�[�W�傫�����g�p����ꍇ
		/*m_stand->Draw_transform(
			m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
			1.0f, m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));*/

		//��̕`��A�Œ�̑傫�����g�������ꍇ
		m_stand->Draw_transform(100,1,100);
	}
	
	//�X�J�C�h�[���`��
	m_skydome->Draw();
	
	switch (m_guiState)
	{
	case Editor::IMGUI_STATE::PLAYER: {
		//�v���C���[�`��
		m_player->Draw();
		//�e�\��
		m_bulletManager->DrawBullets();
	}
		break;
	case Editor::IMGUI_STATE::SKILL: {
		//��������
	}
		break;
	case Editor::IMGUI_STATE::ENEMY: {
		//�I�����Ă���G��`��
		m_enemyManager->DrawOneEditor(m_choiceEnemy);
		//�e�\��
		m_bulletManager->DrawBullets();
		//�v���C���[�`��
		m_player->Draw();
	}
		break;
	case Editor::IMGUI_STATE::BULLET: {
		//�G�`��
		m_enemyManager->DrawAll();
		//�e�\��
		m_bulletManager->DrawBullets();
		//�v���C���[�`��
		m_player->Draw();
	}
		break;
	case Editor::IMGUI_STATE::STAGE: {
		//��̕`��A���A���^�C���ŕό`����
		m_stand->Draw_transform(
			m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
			1.0f, m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
		//�G�`��
		m_enemyManager->DrawAll();
		//�e�\��
		m_bulletManager->DrawBullets();
		//�v���C���[�`��
		m_player->Draw();
	}
		break;
	default:
		break;
	}

	if (m_sceneChangeFlg == true)
	{
		SceneChange::GetInstance()->Draw();
	}
}

