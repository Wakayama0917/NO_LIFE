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
	// Start the Dear ImGui frame、開始処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ここまで
	std::string str;
	ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	if (ImGui::Begin(u8"ステージ作成ＵＩ", nullptr, ImGuiWindowFlags_MenuBar))
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Separator();


		ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			
			if (ImGui::BeginTabItem(u8"プレイヤー"))
			{
				//何のエディターか設定
				m_guiState = IMGUI_STATE::PLAYER;
				
				ImGui::EndTabItem();
				//説明
				ImGui::Text(u8"初期のプレイヤーステータスを変更する");
				//移動速度
				ImGui::InputFloat(u8"移動速度", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)] < 0)
				{
					//マイナスになったら戻す
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)] = 0;
				}
				//最大体力
				ImGui::InputFloat(u8"最大HP", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)], 10, 50);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)] < 0)
				{
					//マイナスになったら戻す
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)] = 0;
				}
				//体力
				ImGui::InputFloat(u8"現在HP", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)], 10, 50);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)] < 0)
				{
					//マイナスになったら戻す
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)] = 0;
				}
				//攻撃力
				ImGui::Text(u8"※弾の攻撃力にかける倍率＝攻撃力。１で変動なし");
				ImGui::InputFloat(u8"攻撃力", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)] < 0)
				{
					//マイナスになったら戻す
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)] = 0;
				}
				//リロード時間
				ImGui::InputFloat(u8"リロード時間", &m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)], 0.1, 5);
				if (m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)] < 0)
				{
					//マイナスになったら戻す
					m_playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)] = 0;
				}
				//保存する
				if (ImGui::Button(u8"保存する")) {
					m_saveFlg = true;
				}
			}
			else
			{
				//選択されていないのでフラグを折る
				m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] = false;
			}

			if (ImGui::BeginTabItem(u8"スキル"))
			{
				
				//何のエディターか設定
				m_guiState = IMGUI_STATE::SKILL;
				static const char* SkillName[]
				{
					u8"僅かに体力を回復",
					u8"攻撃力アップ",
					u8"回復",
					u8"最大体力上昇",
					u8"移動速度上昇",
					u8"リロード速度上昇",
					u8"前弾追加",
					u8"後ろ弾追加",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == true)
				{
					//説明
					ImGui::Text(u8"スキルのステータスを変更する");
					ImGui::Text(u8"タブが変更されると初期化される");
					//スキル切り替え
					ImGui::Text(u8"スキルの種類切り替え");
					ImGui::ListBox(u8"敵切り替え", &m_choiceSkill, SkillName, 
						static_cast<int>(SKILLTYPEID::END)-static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE));
					//m_choiceSkill = static_cast<ENEMY_TYPE>(select);//選択している敵の変更
					//上げる値
					ImGui::InputFloat(u8"上昇値", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)), 0.1, 5);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)) < 0)
					{
						//マイナスになったら戻す
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::UP_VALUE)) = 0;
					}
					//現在レベル
					ImGui::InputFloat(u8"レベル、基本は０", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)) < 0)
					{
						//マイナスになったら戻す
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVEL)) = 0;
					}
					//最大レベル
					ImGui::InputFloat(u8"最大レベル", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)) < 0)
					{
						//マイナスになったら戻す
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::LEVELMAX)) = 0;
					}
					//確率
					ImGui::Text(u8"数が大きいほど当たりやすくなる。％打ち込み可能");
					ImGui::InputFloat(u8"当選確率比率", &m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)), 1, 50);
					if (m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)) < 0)
					{
						//マイナスになったら戻す
						m_skillChangeData.at(m_choiceSkill).at(static_cast<int>(SKILL_STATUSID::RATE)) = 0;
					}
					//保存する
					if (ImGui::Button(u8"保存する")) {
						m_saveFlg = true;
					}
				}
				
			}
			else
			{
				//選択されていないのでフラグを折る
				m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] = false;
			}

			if (ImGui::BeginTabItem(u8"エネミー"))
			{
				//何のエディターか設定
				m_guiState = IMGUI_STATE::ENEMY;
				static const char* EnemyName[]
				{
					u8"タワーの敵",
					u8"その場で上下する敵",
					u8"追尾してくる敵",
					u8"ボス１",
				};
				static const char* BulletName[]
				{
					u8"敵通常弾",
					u8"破壊できない敵通常弾",
					u8"ホーミング弾",
					u8"無し",
					//u8"プレイヤーノーマル弾",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] == true)
				{
					//説明
					ImGui::Text(u8"エネミーのステータスを変更する");
					ImGui::Text(u8"タブが変更されると初期化される");
					ImGui::Text(u8"ホーミング弾はプレイヤーにあたると上に向けて移動する");
					//スキル切り替え
					ImGui::Text(u8"敵の種類切り替え");
					ImGui::ListBox(u8"敵切り替え", &m_choiceEnemy, EnemyName, static_cast<int>(ENEMY_TYPE::END));
					//弾切り替え
					ImGui::Text(u8"弾の種類切り替え、選んだ弾を装備");
					ImGui::ListBox(u8"弾切り替え", &m_choiceBullet, BulletName, static_cast<int>(BULLET_TYPE::P_NORMAL));
					m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)) = m_choiceBullet;
				
					//移動速度
					ImGui::InputFloat(u8"移動速度", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)) < 0)
					{
						//マイナスになったら戻す
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MOVE_SPEED)) = 0;
					}
					//体力
					ImGui::InputFloat(u8"体力", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)), 1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)) < 0)
					{
						//マイナスになったら戻す
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::HP)) = 0;
					}
					//攻撃力
					ImGui::Text(u8"※弾の攻撃力にかける倍率＝攻撃力。１で変動なし");
					ImGui::InputFloat(u8"攻撃力", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)) < 0)
					{
						//マイナスになったら戻す
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::ATTACK)) = 0;
					}
					//最大回転角度
					ImGui::InputFloat(u8"最大回転角度", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)) < 0)
					{
						//マイナスになったら戻す
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT)) = 0;
					}
					//リロード速度
					ImGui::InputFloat(u8"リロード時間", &m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)), 0.1, 5);
					if (m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) < 0)
					{
						//マイナスになったら戻す
						m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) = 0;
					}
					//撃つ方向を決める
					if (m_choiceBullet == static_cast<int>(BULLET_TYPE::HOMMING))
					{
						bool _directionFlg = m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::DIRECTION_FRONT));
						ImGui::Checkbox(u8"ホーミング弾撃つ", &_directionFlg);
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
								ImGui::Checkbox(u8"前方に撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_LEFT: {
								ImGui::Checkbox(u8"左に撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_RIGHT: {
								ImGui::Checkbox(u8"右に撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK: {
								ImGui::Checkbox(u8"後ろに撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_FRONT_LEFT: {
								ImGui::Checkbox(u8"左前に撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_FRONT_RIGHT: {
								ImGui::Checkbox(u8"右前に撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK_LEFT: {
								ImGui::Checkbox(u8"左後ろに撃つ", &_directionFlg);
							}
								break;
							case ENEMY_STATUSID::DIRECTION_BACK_RIGHT: {
								ImGui::Checkbox(u8"右後ろに撃つ", &_directionFlg);
							}
								break;
							default:
								break;
							}
							m_enemyChangeData.at(m_choiceEnemy).at(i) = _directionFlg;
						}
					}
					//保存する
					if (ImGui::Button(u8"保存する")) {
						m_saveFlg = true;
					}
					
				}
			}
			else
			{
				//選択されていないのでフラグを折る
				m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] = false;
			}

			if (ImGui::BeginTabItem(u8"弾幕"))
			{
				//何のエディターか設定
				m_guiState = IMGUI_STATE::BULLET;
				static const char* BulletName[]
				{
					u8"敵通常弾",
					u8"破壊できない敵通常弾",
					u8"ホーミング弾",
					u8"無し",
					u8"プレイヤーノーマル弾",
				};

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == true)
				{
					//説明
					ImGui::Text(u8"弾のステータスを変更する");
					ImGui::Text(u8"タブが変更されると初期化される：現在は破壊できない敵通常弾は変更不可");
					ImGui::Text(u8"ホーミング弾はプレイヤーにあたると上に向けて移動する");
					//弾切り替え
					ImGui::Text(u8"弾の種類切り替え");
					ImGui::ListBox(u8"弾切り替え", &m_choiceBullet, BulletName, static_cast<int>(BULLET_TYPE::END));
					if (m_choiceBullet != static_cast<int>(BULLET_TYPE::NONE))
					{
						//移動速度
						ImGui::InputFloat(u8"移動速度", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)) < 0)
						{
							//マイナスになったら戻す
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED)) = 0;
						}
						//攻撃力
						ImGui::InputFloat(u8"攻撃力", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)) < 0)
						{
							//マイナスになったら戻す
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER)) = 0;
						}
						//生存時間
						ImGui::InputFloat(u8"発生時間", &m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)), 0.1, 5);
						if (m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)) < 0)
						{
							//マイナスになったら戻す
							m_bulletChangeData.at(m_choiceBullet).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME)) = 0;
						}
						//保存する
						if (ImGui::Button(u8"保存する")) {
							m_saveFlg = true;
						}
					}
					
				}
			}
			else
			{
				//選択されていないのでフラグを折る
				m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] = false;
			}

			if (ImGui::BeginTabItem(u8"ステージ"))
			{
				//何のエディターか設定
				m_guiState = IMGUI_STATE::STAGE;

				ImGui::EndTabItem();
				if (m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] == true)
				{
					//説明
					ImGui::Text(u8"ステージのステータスを変更する");
					ImGui::Text(u8"更新ボタンを押すと現在選択しているステージを再配置する");
					ImGui::Text(u8"ホーミング弾はプレイヤーにあたると上に向けて移動する");
					//ステージ変更
					ImGui::InputInt(u8"編集するステージを変更する", &m_stageNumber, 1, 5);
					if (m_stageNumber < 0) m_stageNumber = 0;
					else if (m_stageNumber > MAXSTAGE) m_stageNumber = MAXSTAGE;
					//横幅長さ
					ImGui::InputFloat(u8"横幅の長さ、５０が最小値", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)) < 50)
					{
						//50以下なら50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)) = 50;
					}
					//縦幅長さ
					ImGui::InputFloat(u8"縦幅の長さ、５０が最小値", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)) < 50)
					{
						//50以下なら50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)) = 50;
					}
					//敵の設定
					ImGui::InputFloat(u8"敵１", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)) < 0)
					{
						//50以下なら50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_00)) = 0;
					}
					ImGui::InputFloat(u8"敵2", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)) < 0)
					{
						//50以下なら50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_01)) = 0;
					}
					ImGui::InputFloat(u8"敵3", &m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)), 1, 5);
					if (m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)) < 0)
					{
						//50以下なら50
						m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::ENEMY_02)) = 0;
					}

					//保存する
					if (ImGui::Button(u8"保存する")) {
						m_saveFlg = true;
					}
					//保存する
					if (ImGui::Button(u8"再配置する")) {
						m_resetFlg = true;
					}
				}
			}
			else
			{
				//選択されていないのでフラグを折る
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

	//２つ目
	ImGui::Begin(u8"サブ");

	//凡庸float
	static float num;


	//各シーンごとに出したいものを出す
	switch (m_guiState)
	{
	case Editor::IMGUI_STATE::PLAYER: {
		ImGui::Text(u8"自機：Lスティック：移動");
		ImGui::Text(u8"自機：Rスティック：向き変更");

		//左キー
		num = Input::GetInstance().GetLStick().x;
		ImGui::InputFloat("input float", &num);
		num = Input::GetInstance().GetLStick().y;
		ImGui::InputFloat("input float", &num);

		//右キー
		num = Input::GetInstance().GetRStick().x;
		ImGui::InputFloat("input float", &num);
		num = Input::GetInstance().GetRStick().y;
		ImGui::InputFloat("input float", &num);
	}
		break;
	case Editor::IMGUI_STATE::SKILL: {

		ImGui::Text(u8"スキル全ての当選確率一覧");

		if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == true)
		{
			for (int i = 0; i < static_cast<int>(SKILLTYPEID::END) - static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE); i++)
			{
				switch (static_cast<SKILLTYPEID>(i + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)))
				{
				case SKILLTYPEID::MAXHPUP_LITTLE:
					ImGui::Text(u8"僅かに体力を回復");
					break;
				case SKILLTYPEID::ATKUP:
					ImGui::Text(u8"攻撃力アップ");
					break;
				case SKILLTYPEID::HEEL:
					ImGui::Text(u8"回復");
					break;
				case SKILLTYPEID::MAXHPUP:
					ImGui::Text(u8"最大体力上昇");
					break;
				case SKILLTYPEID::SPEEDUP:
					ImGui::Text(u8"移動速度上昇");
					break;
				case SKILLTYPEID::RELOADUP:
					ImGui::Text(u8"リロード速度上昇");
					break;
				case SKILLTYPEID::BULLETFRONT:
					ImGui::Text(u8"前弾追加");
					break;
				case SKILLTYPEID::BULLETBACK:
					ImGui::Text(u8"後ろ弾追加");
					break;
				default:
					break;
				}
				//何も書く必要がないので空白を一つ入れている
				ImGui::InputFloat(" ", &m_skillChangeData.at(i).at(static_cast<int>(SKILL_STATUSID::RATE)));
			}
		}
	}
		break;
	case Editor::IMGUI_STATE::ENEMY: {
		ImGui::Text(u8"自機：Lスティック：移動");
		ImGui::Text(u8"自機：Rスティック：向き変更");
	}
		break;
	case Editor::IMGUI_STATE::BULLET: {
		ImGui::Text(u8"自機：Lスティック：移動");
		ImGui::Text(u8"自機：Rスティック：向き変更");

		if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == true)
		{
			ImGui::Text(u8"0.2で止まるようにいしている");
			ImGui::Text(u8"この項目の変更は保存されない");
			//生存時間
			ImGui::InputFloat(u8"発射間隔変更", &m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)), 0.1, 5);
			if (m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) < 0.2)
			{
				//マイナスになったら戻す
				m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::RELOAD)) = 0.2;
			}
		}
	}
		break;
	case Editor::IMGUI_STATE::STAGE: {
		ImGui::Text(u8"自機：Lスティック：移動");
		ImGui::Text(u8"自機：Rスティック：向き変更");
	}
		break;
	default:
		break;
	}

	ImGui::End();


	// Rendering、後処理
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Init()
{
	DebugFlg = true;//デバッグするかどうか

	//CSV実体化
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		m_csvData.push_back(std::make_unique<CSVFile>());
	}
	
	//台実体化
	if (m_stand == nullptr)
	{
		m_stand = std::make_unique<Stand>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "エディターシーン：スタンドオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//スカイドーム実体化
	if (m_skydome == nullptr)
	{
		m_skydome = std::make_unique<Skydome>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "エディターシーン：スカイドームオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//プレイヤー実体化
	if (m_player == nullptr)
	{
		m_player = std::make_unique<Player>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "エディターシーン：プレイヤーオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//エネミーマネージャー実体化
	if (m_enemyManager == nullptr)
	{
		m_enemyManager = std::make_unique<EnemyManager>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "エディターシーン：エネミーマネージャーオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//バレットマネージャー実体化
	if (m_bulletManager == nullptr)
	{
		m_bulletManager = std::make_unique<BulletManager>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "エディターシーン：バレットマネージャーオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
	//スキルマネージャー実体化
	if (m_skillManager == nullptr)
	{
		m_skillManager = std::make_unique<SkillManager>();
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "メインゲーム：スキルマネージャーオブジェクトが生成できません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}

	//CSV読み込み
	m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvRead("assets/csv/PlayerStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvRead("assets/csv/EnemyStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::BULLET)]->CsvRead("assets/csv/BulletStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::STAGE)]->CsvRead("assets/csv/StageData.csv", ',');
	m_csvData[static_cast<int>(CSVID::MODEL_SS)]->CsvRead("assets/csv/ModelData_stage_sky.csv", ',');
	m_csvData[static_cast<int>(CSVID::SKILL)]->CsvRead("assets/csv/SkillStatus.csv", ',');

	//敵モデルの読み込み
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::ENEMY)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::ENEMY)]->GetTexFolder().at(i));
	}
	//弾モデルの読み込み
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::BULLET)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::BULLET)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::BULLET)]->GetTexFolder().at(i));
	}
	//プレイヤーモデルの読み込み
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::PLAYER)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::PLAYER)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::PLAYER)]->GetTexFolder().at(i));
	}
	//ステージ、スカイドームモデルの読み込み
	for (int i = 0; i < m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvData[static_cast<int>(CSVID::MODEL_SS)]->GetTexFolder().at(i));
	}
	
	//行列初期化、設定
	DX11MtxIdentity(m_mtx);
	m_mtx._41 -= 40.0f;

	m_skydome->Init();
	
	//台の初期化
	m_stand->Init();
	
	//プレイヤーの初期化
	m_player->Init(m_csvData[static_cast<int>(CSVID::PLAYER)]);

	//弾マネージャー初期化
	m_bulletManager->Init(m_csvData[static_cast<int>(CSVID::BULLET)], m_player);

	//スキルマネージャー初期化
	m_skillManager->Init(m_csvData[static_cast<int>(CSVID::SKILL)], m_player);
	//ステージ初期化
	m_stageNumber = 0;

	//カメラにプレイヤーセット
	CCamera::GetInstance()->SetPlayer(m_player);
	//カメラ変数リセット
	CCamera::GetInstance()->Reset();

	//サウンド
	Sound::instance().SetScene(SCENE_GAMESCENE);
	Sound::instance().SetVolumeAll(0.1f);
	Sound::instance().Play(SOUND_LABEL_GAME01_BGM);

	//フェード時のカメラ位置の為、プレイヤー位置初期化
	m_player->SetStartPos(
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
	
	//シーン遷移フラグ初期化
	switch (SceneChange::GetInstance()->GetSceneChangeFlg())
	{
	case SceneChange::SCENECHANGEID::FADDEIN:
		//画面に何もないため本処理開始
		m_sceneChangeFlg = false;
		break;
	case SceneChange::SCENECHANGEID::FADDEOUT:
		//別画面から移ってきているためフェードアウト処理に移行
		m_sceneChangeFlg = true;
		break;
	case SceneChange::SCENECHANGEID::END:
		break;
	default:
		break;
	}
	//シーン遷移中身初期化
	SceneChange::GetInstance()->init();

	//初期化終了
	SceneFlgInit();
	SceneStatus = SCENESTATUS::Editor;//エディターシーンに設定

}

void Editor::Exit()
{
	m_skydome->Finalize();

	//全ての要素削除
	//CSV
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		CSVFile* exitCsv = m_csvData[i].release();
		delete exitCsv;
	}

	//台
	Stand* exitStand = m_stand.release();
	delete exitStand;
	//スカイドーム
	Skydome* exitSkydome = m_skydome.release();
	delete exitSkydome;
	//エネミーマネージャー
	EnemyManager* exitEnemyManager = m_enemyManager.release();
	delete exitEnemyManager;
	//バレットマネージャー
	BulletManager* exitBulletManager = m_bulletManager.release();
	delete exitBulletManager;
	//スキルマネージャー
	SkillManager* exitSkillManager = m_skillManager.release();
	delete exitSkillManager;
	

	//次のシーンを指定
	NextSceneStatus = SCENESTATUS::Title;

	//フラグリセット、次のシーンの為
	SceneFlgReset();

	
}

void Editor::Update()
{
	//シーン遷移処理に入っていなければ処理を行う
	if (m_sceneChangeFlg == false)
	{
		switch (m_guiState)
		{
		case Editor::IMGUI_STATE::PLAYER: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] == false)//初期化
			{
				//ステージ初期化
				m_stageNumber = 0;
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(PLAYER_STATUSID::END); i++)
				{
					//プレイヤーのステータスを入れる
					m_playerChangeData[i] = m_csvData[static_cast<int>(CSVID::PLAYER)]->GetCell().at(0).at(i);
				}
				//弾初期化
				m_bulletManager->Reset();
				//敵初期化
				m_enemyManager->Reset();
				//位置初期化、と同時にフィールドの大きさセット
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				//初期化処理完了
				m_ResetFlg[static_cast<int>(IMGUI_STATE::PLAYER)] = true;
			}
			else//アップデート
			{
				//カメラを設定
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//キー入力による移動
				m_player->Update();
				
				if (m_player->GetFireBulletFlg())
				{
					//弾発射
					m_bulletManager->FireBulletPlayer();
					Sound::instance().Play(SOUND_LABEL_PSHOOT_SE);
				}

				m_bulletManager->UpdateBulletsPlayer();

				//データ内容の数値を反映
				m_player->SetAllStatus(m_playerChangeData);

				if (m_saveFlg)
				{
					//プレイヤーの数値を更新
					for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
					{
						//プレイヤーのステータスを入れる
						m_csvData[static_cast<int>(CSVID::PLAYER)]->SetCell(static_cast<PLAYER_STATUSID>(i), m_playerChangeData[i]);
					}

					//データを保存
					m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvWrite("assets/csv/PlayerStatus.csv", ',');

					//フラグを折る
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::SKILL: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] == false)//初期化
			{
				//ステージ初期化
				m_stageNumber = 0;
				//中身削除
				m_skillChangeData.clear();
				m_skillChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE); i < static_cast<int>(SKILLTYPEID::END); i++)
				{
					//列を１つ追加
					m_skillChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(SKILL_STATUSID::END); id++)
					{
						//要素を入れていく
						m_skillChangeData.at(i - static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)).emplace_back(
							m_csvData[static_cast<int>(CSVID::SKILL)]->GetCell().at(i).at(id));
					}
				}

				//弾初期化
				m_bulletManager->Reset();
				//敵初期化
				m_enemyManager->Reset();
				//初期化処理完了
				m_ResetFlg[static_cast<int>(IMGUI_STATE::SKILL)] = true;
			}
			else//アップデート
			{
				//カメラを設定
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//データ内容の数値を反映
				m_skillManager->SetAllStatus(m_choiceSkill + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE),
					m_skillChangeData.at(m_choiceSkill));

				if (m_saveFlg)
				{
					//スキルの数値を更新
					for (int i = 0; i < static_cast<int>(SKILL_STATUSID::END); i++)
					{
						//スキルのステータスを入れる
						m_csvData[static_cast<int>(CSVID::SKILL)]->SetCell(static_cast<SKILLTYPEID>(
							m_choiceSkill + static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)),
							static_cast<SKILL_STATUSID>(i), m_skillChangeData.at(m_choiceSkill).at(i));
					}

					//データを保存
					m_csvData[static_cast<int>(CSVID::SKILL)]->CsvWrite("assets/csv/SkillStatus.csv", ',');

					//フラグを折る
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::ENEMY: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] == false)//初期化
			{
				//ステージ初期化
				m_stageNumber = 0;
				//弾中身削除
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//列を１つ追加
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//要素を入れていく
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//エネミー削除
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//列を１つ追加
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//要素を入れていく
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//弾初期化
				m_bulletManager->Reset();
				//敵初期化
				m_enemyManager->Reset();
				//敵の配列中身削除
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				//使う敵を生成
				m_enemyManager->InitEditor(m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);

				//位置セット
				m_enemyManager->SetPosCenter(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//プレイヤー位置初期化、と同時にフィールドの大きさセット
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//初期化
				m_choiceBullet = m_enemyManager->GetEnemyList()[0]->GetBulletType();
				m_choiceEnemy = 0;
				m_choiceEnemyOld = m_choiceEnemy;
				m_choiceEnemyPrevious = 0;
				//初期化処理完了
				m_ResetFlg[static_cast<int>(IMGUI_STATE::ENEMY)] = true;

			}
			else//アップデート
			{
				//カメラを設定
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//キー入力による移動
				m_player->Update();

				//敵のアップデート
				m_enemyManager->UpdateEditor(m_choiceEnemy, m_bulletManager);

				//弾のアップデート
				m_bulletManager->UpdateBulletsEnemy();

				//データ内容の数値を反映
				m_enemyManager->GetEnemyList()[m_choiceEnemy]->SetCsvStatus(m_enemyChangeData.at(m_choiceEnemy));

				//選択されている敵が変わった時、全ての数値を初期化
				if (m_choiceEnemy != m_choiceEnemyOld)
				{
					//選んだ敵の数値を初期化
					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//要素を入れていく
						m_enemyChangeData.at(m_choiceEnemy).at(id) = m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(m_choiceEnemy).at(id);
					}

					//選んだ敵を初期化
					m_enemyManager->GetEnemyList()[m_choiceEnemy]->SetCsvStatus(m_enemyChangeData.at(m_choiceEnemy));
					//選んでいる弾の種類を更新。
					m_choiceBullet = m_enemyChangeData.at(m_choiceEnemy).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE));
					//追尾の敵の位置を初期化
					m_enemyManager->GetEnemyList()[2]->SetPos(0, 0);

					//初期化完了したので、更新
					m_choiceEnemyOld = m_choiceEnemy;
				}

				if (m_saveFlg)
				{
					//敵の数値を更新
					for (int i = 0; i < static_cast<int>(ENEMY_STATUSID::END); i++)
					{

						//敵のステータスを入れる
						m_csvData[static_cast<int>(CSVID::ENEMY)]->SetCell(static_cast<ENEMY_TYPE>(m_choiceEnemy),
							static_cast<ENEMY_STATUSID>(i), m_enemyChangeData.at(m_choiceEnemy).at(i));
					}

					//データを保存
					m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvWrite("assets/csv/EnemyStatus.csv", ',');

					//フラグを折る
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::BULLET: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] == false)//初期化
			{
				//ステージ初期化
				m_stageNumber = 0;
				//弾中身削除
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//列を１つ追加
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//要素を入れていく
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//エネミー削除
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//列を１つ追加
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//要素を入れていく
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//弾初期化
				m_bulletManager->Reset();
				//敵初期化
				m_enemyManager->Reset();
				//敵の配列中身削除
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				//使う敵を生成
				m_enemyArray.emplace_back(ENEMY_TYPE::ROTATION);//柱の敵を追加
				m_enemyManager->InitRandom(m_enemyArray, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);

				//位置セット
				m_enemyManager->SetPosCenter(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//プレイヤー位置初期化、と同時にフィールドの大きさセット
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//弾の選択番号を初期化
				m_choiceBullet = 0;
				//初期化処理完了
				m_ResetFlg[static_cast<int>(IMGUI_STATE::BULLET)] = true;
			}
			else//アップデート
			{
				//カメラを設定
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

				//キー入力による移動
				m_player->Update();

				//弾の種類変更
				m_enemyChangeData.at(0).at(static_cast<int>(ENEMY_STATUSID::BULLET_TYPE)) = m_choiceBullet;

				//敵のアップデート
				m_enemyManager->Update(m_bulletManager);

				//弾のアップデート
				m_bulletManager->UpdateBulletsEnemy();

				//データ内容の数値を反映
				m_bulletManager->SetCsvStatus(static_cast<BULLET_TYPE>(m_choiceBullet), m_bulletChangeData.at(m_choiceBullet));
				m_enemyManager->GetEnemyList()[0]->SetCsvStatus(m_enemyChangeData.at(0));

				if (m_saveFlg)
				{
					//スキルの数値を更新
					for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
					{
						//スキルのステータスを入れる
						m_csvData[static_cast<int>(CSVID::BULLET)]->SetCell(static_cast<BULLET_TYPE>(m_choiceBullet),
							static_cast<BULLET_STATUSID>(i), m_bulletChangeData.at(m_choiceBullet).at(i));
					}

					//データを保存
					m_csvData[static_cast<int>(CSVID::BULLET)]->CsvWrite("assets/csv/BulletStatus.csv", ',');

					//フラグを折る
					m_saveFlg = false;
				}
			}
		}
			break;
		case Editor::IMGUI_STATE::STAGE: {
			if (m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] == false)//初期化
			{
				//ステージ中身削除
				m_stageChangeData.clear();
				m_stageChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < 10; i++)
				{
					//列を１つ追加
					m_stageChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(STAGE_STATUSID::END); id++)
					{
						//要素を入れていく
						m_stageChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(i).at(id));
					}
				}
				//弾中身削除
				m_bulletChangeData.clear();
				m_bulletChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
				{
					//列を１つ追加
					m_bulletChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(BULLET_STATUSID::END); id++)
					{
						//要素を入れていく
						m_bulletChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::BULLET)]->GetCell().at(i).at(id));
					}
				}
				//エネミー削除
				m_enemyChangeData.clear();
				m_enemyChangeData.shrink_to_fit();
				//CSVからデータを取得
				for (int i = 0; i < static_cast<int>(ENEMY_TYPE::END); i++)
				{
					//列を１つ追加
					m_enemyChangeData.push_back(vector<float>());

					for (int id = 0; id < static_cast<int>(ENEMY_STATUSID::END); id++)
					{
						//要素を入れていく
						m_enemyChangeData.at(i).emplace_back(m_csvData[static_cast<int>(CSVID::ENEMY)]->GetCell().at(i).at(id));
					}
				}
				//弾初期化
				m_bulletManager->Reset();
				//敵初期化
				m_enemyManager->Reset();
				//使う敵を生成
				//敵を入れる配列初期化
				m_enemyArray.clear();
				m_enemyArray.shrink_to_fit();
				for (int _enemyType = static_cast<int>(STAGE_STATUSID::ENEMY_00); _enemyType < static_cast<int>(STAGE_STATUSID::END); _enemyType++)
				{
					for (int i = 0; i < m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(_enemyType); i++)
					{
						//このステージに使う敵を入れる
						m_enemyArray.emplace_back(static_cast<ENEMY_TYPE>(_enemyType - static_cast<int>(STAGE_STATUSID::ENEMY_00)));
					}
				}
				m_enemyManager->InitRandom(m_enemyArray, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);//敵生成
				//位置セット
				m_enemyManager->SetAllPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));

				//プレイヤー位置初期化、と同時にフィールドの大きさセット
				m_player->SetStartPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
				m_player->SetPos(-20, m_player->GetPos().z);
				//初期化処理完了
				m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] = true;
			}
			else//アップデート
			{
				//カメラを設定
				CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());
				
				//キー入力による移動
				m_player->Update();
				//敵のアップデート
				m_enemyManager->Update(m_bulletManager);

				//弾のアップデート
				m_bulletManager->UpdateBulletsEnemy();



				if (m_saveFlg)
				{
					//スキルの数値を更新
					for (int i = 0; i < static_cast<int>(STAGE_STATUSID::END); i++)
					{
						//スキルのステータスを入れる
						m_csvData[static_cast<int>(CSVID::STAGE)]->SetCell(m_stageNumber,
							static_cast<STAGE_STATUSID>(i), m_stageChangeData.at(m_stageNumber).at(i));
					}

					//データを保存
					m_csvData[static_cast<int>(CSVID::STAGE)]->CsvWrite("assets/csv/StageData.csv", ',');

					//フラグを折る
					m_saveFlg = false;
				}

				if (m_resetFlg)
				{
					//一度外に出して初期化させる
					m_ResetFlg[static_cast<int>(IMGUI_STATE::STAGE)] = false;

					//フラグを折る
					m_resetFlg = false;
				}
			}
		}
		break;
		default:
			break;
		}

		//メインに戻る
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
			Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);//BGMを止める
			//シーン遷移処理開始の為フラグ変更
			m_sceneChangeFlg = true;
		}
	}
	else//シーン遷移処理に入る。
	{
		switch (SceneChange::GetInstance()->GetSceneChangeFlg())
		{
		case SceneChange::SCENECHANGEID::FADDEIN:
			//フェードイン処理、終了したらtrueが帰る
			if (SceneChange::GetInstance()->UpdateIN())
			{
				SceneFlgDispose();//終了処理に入る
			}
			break;
		case SceneChange::SCENECHANGEID::FADDEOUT:
			//カメラを設定
			CCamera::GetInstance()->CameraDelay(m_player->GetMtx());

			//フェードアウト処理、終了したらtrueが帰る
			if (SceneChange::GetInstance()->UpdateOUT())
			{
				//本処理開始
				m_sceneChangeFlg = false;
			}
			break;
		case SceneChange::SCENECHANGEID::END:
			//IN処理に入る為に初期化
			SceneChange::GetInstance()->init();
			break;
		default:
			break;
		}

	}
}

void Editor::Draw()
{
	//ステージ編集画面以外は固定
	if (m_guiState != Editor::IMGUI_STATE::STAGE)
	{
		//台の描画、実際のステージ大きさを使用する場合
		/*m_stand->Draw_transform(
			m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
			1.0f, m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));*/

		//台の描画、固定の大きさを使いたい場合
		m_stand->Draw_transform(100,1,100);
	}
	
	//スカイドーム描画
	m_skydome->Draw();
	
	switch (m_guiState)
	{
	case Editor::IMGUI_STATE::PLAYER: {
		//プレイヤー描画
		m_player->Draw();
		//弾表示
		m_bulletManager->DrawBullets();
	}
		break;
	case Editor::IMGUI_STATE::SKILL: {
		//何も無し
	}
		break;
	case Editor::IMGUI_STATE::ENEMY: {
		//選択している敵を描画
		m_enemyManager->DrawOneEditor(m_choiceEnemy);
		//弾表示
		m_bulletManager->DrawBullets();
		//プレイヤー描画
		m_player->Draw();
	}
		break;
	case Editor::IMGUI_STATE::BULLET: {
		//敵描画
		m_enemyManager->DrawAll();
		//弾表示
		m_bulletManager->DrawBullets();
		//プレイヤー描画
		m_player->Draw();
	}
		break;
	case Editor::IMGUI_STATE::STAGE: {
		//台の描画、リアルタイムで変形する
		m_stand->Draw_transform(
			m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
			1.0f, m_stageChangeData.at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
		//敵描画
		m_enemyManager->DrawAll();
		//弾表示
		m_bulletManager->DrawBullets();
		//プレイヤー描画
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

