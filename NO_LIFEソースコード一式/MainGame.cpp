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
// IMGUIウインドウ
void Game::Imgui() {

	// Start the Dear ImGui frame、開始処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ここまで

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 300));

	ImGui::Begin("config 1");

	//フレームレート
	//ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);

	static int num;

	ImGui::Text(u8"ステージ数、敵と味方のHP表記");
	ImGui::Text(u8"ステージ１０でクリア");
	ImGui::Text(u8"現在のステージ：%d", m_stageNumber + 1);
	//HP
	num = m_player->GetCSVStatus(PLAYER_STATUSID::HP);
	ImGui::Text(u8"自身のHP：%d", num);
	for (int i = 0; i < m_enemyManager->GetEnemyList().size(); i++)
	{
		num = m_enemyManager->GetEnemyList()[i]->GetCSVStatus(ENEMY_STATUSID::HP);
		ImGui::Text(u8"敵%dのHP：%d",i,num);
		
	}
	

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// Rendering、後処理
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
#endif//DEBUG

void Game::Init() {

	DebugFlg = true;//デバッグするかどうか

	//CSVオブジェクトを必要な数だけ実体化
	for (int i = 0; i < static_cast<int>(CSVID::END); i++)
	{
		m_csvData.push_back(std::make_unique<CSVFile>());
	}
	//UIオブジェクトを必要な数だけ実体化
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		m_ui.push_back(std::make_unique<Quad2D>());
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
		MessageBox(nullptr, "メインゲーム：スタンドオブジェクトが生成できません",
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
		MessageBox(nullptr, "メインゲーム：スカイドームオブジェクトが生成できません",
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
		MessageBox(nullptr, "メインゲーム：プレイヤーオブジェクトが生成できません",
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
		MessageBox(nullptr, "メインゲーム：エネミーマネージャーオブジェクトが生成できません",
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
		MessageBox(nullptr, "メインゲーム：バレットマネージャーオブジェクトが生成できません",
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
	//data.csv_read(入力ファイル名, ヘッダーの有無, インデックスの有無, 区切り文字);
	m_csvData[static_cast<int>(CSVID::PLAYER)]->CsvRead("assets/csv/PlayerStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::ENEMY)]->CsvRead("assets/csv/EnemyStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::BULLET)]->CsvRead("assets/csv/BulletStatus.csv", ',');
	m_csvData[static_cast<int>(CSVID::STAGE)]->CsvRead("assets/csv/StageData.csv", ',');
	m_csvData[static_cast<int>(CSVID::MODEL_SS)]->CsvRead("assets/csv/ModelData_stage_sky.csv", ',');
	m_csvData[static_cast<int>(CSVID::SKILL)]->CsvRead("assets/csv/SkillStatus.csv", ',');
	
	// スカイドーム初期化
	m_skydome->Init();

	//台の初期化
	m_stand->Init();

	//プレイヤーの初期化
	m_player->Init(m_csvData[static_cast<int>(CSVID::PLAYER)]);
	//m_player->SetModel();
	
	////弾マネージャー初期化
	m_bulletManager->Init(m_csvData[static_cast<int>(CSVID::BULLET)], m_player);

	//スキルマネージャー初期化
	m_skillManager->Init(m_csvData[static_cast<int>(CSVID::SKILL)], m_player);

	//カメラにプレイヤーセット
	CCamera::GetInstance()->SetPlayer(m_player);
	//カメラ変数リセット
	CCamera::GetInstance()->Reset();

	//UI初期化
	//HPフレーム
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->LoadTexture("assets/image/battle/HPFlame.png");
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoSize((m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP)*2)+10, 70.0f);
	m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) + 150, 630);//最大1290、730
	//HPバー
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->LoadTexture("assets/image/battle/white.png");
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoSize(m_player->GetCSVStatus(PLAYER_STATUSID::HP) * 2, 60.0f);
	m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::HP) + 150, 630);//最大1290、730
	//HPロゴ
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->Init(150.0f, 150.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->LoadTexture("assets/image/battle/HPLogo.png");
	m_ui[static_cast<int>(GAMEUIID::HPLOGO)]->updateAnimeNoPos(100, 630);//最大1290、730

	//クリアロゴ
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->LoadTexture("assets/image/battle/Clear.png");
	m_ui[static_cast<int>(GAMEUIID::CLEAR)]->updateAnimeNoPos(645, 365);//最大1290、730
	//ゲームオーバーロゴ
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->LoadTexture("assets/image/battle/Destroyed.png");
	m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->updateAnimeNoPos(645, 365);//最大1290、730
	//ダメージ時のノイズ
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->Init(1290.0f, 730.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 1.0f, 1.0f);
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->LoadTexture("assets/image/battle/PDamage.png");
	m_ui[static_cast<int>(GAMEUIID::NOISE)]->updateAnimeNoPos(645, 365);//最大1290、730
	//数字
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->Init(100.0f, 100.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 1.0f, 1.0f);
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->LoadTexture("assets/image/battle/suuji2.png");
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeNoPos(75, 75);//最大1290、730
	m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeOne(0.0f);//初期化０にしておく


	//サウンド
	Sound::instance().SetScene(SCENE_GAMESCENE);
	Sound::instance().SetVolumeAll(0.1f);
	Sound::instance().Play(SOUND_LABEL_GAME01_BGM);
	//Sound::instance().Set_Volume(SOUND_LABEL_GAME01_BGM, 0.1f);
	
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
	SceneStatus = SCENESTATUS::Game;//ゲームシーンに設定
}

void Game::Exit() {

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
	m_enemyManager->Reset();//初期化
	EnemyManager* exitEnemyManager = m_enemyManager.release();
	delete exitEnemyManager;
	//バレットマネージャー
	BulletManager* exitBulletManager = m_bulletManager.release();
	delete exitBulletManager;
	//スキルマネージャー
	SkillManager* exitSkillManager = m_skillManager.release();
	delete exitSkillManager;
	//UI削除
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		Quad2D* exitUI = m_ui[i].release();
		delete exitUI;
	}

	//次のシーンを指定
	NextSceneStatus = SCENESTATUS::Title;

	//フラグリセット、次のシーンの為
	SceneFlgReset();
}

void Game::Update() {
	
	//カメラを設定
	CCamera::GetInstance()->CameraDelay(m_player->GetPHitEffectFlg(), m_player->GetMtx());

	//シーン遷移処理に入っていなければ処理を行う
	if (m_sceneChangeFlg == false)
	{
		switch (m_gameState)
		{
		case GAMESTATE::INIT: {

			//ボス戦に飛ぶ
			//m_stageNumber = 9;


			//フェードのフラグをリセット
			m_fadeFlg = false;
			//スキル関連のリセット
			m_ui[static_cast<int>(GAMEUIID::CLEAR)]->FaddeReset();
			m_skillManager->Reset();
			//HPフレーム設定
			m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoSize((m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) * 2) + 10, 70.0f);
			m_ui[static_cast<int>(GAMEUIID::HPFLAME)]->updateAnimeNoPos(m_player->GetCSVStatus(PLAYER_STATUSID::MAXHP) + 150, 630);//最大1290、730
			//弾初期化
			m_bulletManager->Reset();
			//敵配置
			m_enemyManager->Reset();//初期化
			//敵を入れる配列初期化
			/*m_enemyList.clear();
			m_enemyList.shrink_to_fit();*/
			m_enemyList.erase(m_enemyList.begin(), m_enemyList.end());
			for (int _enemyType = static_cast<int>(STAGE_STATUSID::ENEMY_00); _enemyType < static_cast<int>(STAGE_STATUSID::END); _enemyType++)
			{
				for (int i = 0; i < m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(_enemyType); i++)
				{
					//このステージに使う敵を入れる
					m_enemyList.emplace_back(static_cast<ENEMY_TYPE>(_enemyType - static_cast<int>(STAGE_STATUSID::ENEMY_00)));
				}
			}
			m_enemyManager->InitRandom(m_enemyList, m_csvData[static_cast<int>(CSVID::ENEMY)], m_player);//敵生成
			if (m_stageNumber != 9)
			{
				//位置セット
				m_enemyManager->SetAllPos(
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
					m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
			}
			else//ボスステージなら
			{
				m_enemyManager->GetEnemyList()[0]->SetPos(0,0);
			}
			
			//プレイヤ一初期化
			//位置初期化
			m_player->SetStartPos(
				m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
				m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
			m_player->Reset();//プレイヤー変数リセット
			//カメラ変数リセット
			CCamera::GetInstance()->Reset();
			//メインゲーム開始
			m_gameState = GAMESTATE::MAINGAME;
		}
			break;
		case GAMESTATE::MAINGAME: {
			if (m_player->GetState() == STATE::LIVE)
			{
				//敵のアップデート
				m_enemyManager->Update(m_bulletManager);

				//プレイヤーのアップデート
				m_player->Update();
				if (m_player->GetFireBulletFlg())
				{
					//弾発射
					m_bulletManager->FireBulletPlayer();
					Sound::instance().Play(SOUND_LABEL_PSHOOT_SE);
				}
				//弾のアップデート
				m_bulletManager->UpdateBullets(m_enemyManager->GetEnemyList());

				//m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnime(30);

				//HPバー更新
				m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoSize(
					m_player->GetCSVStatus(PLAYER_STATUSID::HP) * 2, 60.0f);
				m_ui[static_cast<int>(GAMEUIID::HPBAR)]->updateAnimeNoPos(
					m_player->GetCSVStatus(PLAYER_STATUSID::HP) + 150, 630);//最大1290、730
				if (m_player->GetPHitEffectFlg() == PHitEffectFlg::HIT)
				{
					m_ui[static_cast<int>(GAMEUIID::NOISE)]->updateAnime(3);
				}

				if (m_enemyManager->AllDead())
				{
					if (m_player->GetState() == STATE::LIVE)
					{
						//全ての敵が死んでいればゲームクリア
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
					//最終ステージクリアなので終わる
					if (m_stageNumber == 9)
					{
						//ゲームクリア
						Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);
						//シーン遷移処理開始の為フラグ変更
						m_sceneChangeFlg = true;
					}
					else
					{
						//当選処理
						m_skillManager->SkillSelect();
						//スキル抽選画面に飛ぶ
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
				SceneFlgDispose();//終了処理に入る
			}

		}
			break;
		case GAMESTATE::CHOICESKILL: {

			if (m_skillManager->Update())
			{
				//ステージ進攻
				m_stageNumber++;
				//スキル処理が終わったら次のステージに移る
				m_gameState = GAMESTATE::INIT;
				m_ui[static_cast<int>(GAMEUIID::NUMBER)]->updateAnimeOne(m_stageNumber);//表示する数字を更新
			}

		}
			break;
		case GAMESTATE::END: {

		}
			break;
		default:
			break;
		}

		//g_PX.idle();//ボタン入力を更新
		if (Input::GetInstance().CheckButtonPress(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
			Sound::instance().Stop(SOUND_LABEL_GAME01_BGM);
			//シーン遷移処理開始の為フラグ変更
			m_sceneChangeFlg = true;
		}

	}
	else//シーン遷移処理に入る。
	{
		switch (SceneChange::GetInstance()->GetSceneChangeFlg())
		{
		case SceneChange::SCENECHANGEID::FADDEIN: {
			//フェードイン処理、終了したらtrueが帰る
			if (SceneChange::GetInstance()->UpdateIN())
			{
				SceneFlgDispose();//終了処理に入る
			}
		}
			break;
		case SceneChange::SCENECHANGEID::FADDEOUT: {
			//フェードアウト処理、終了したらtrueが帰る
			if (SceneChange::GetInstance()->UpdateOUT())
			{
				//本処理開始
				m_sceneChangeFlg = false;
			}
		}
			break;
		case SceneChange::SCENECHANGEID::END: {
			//IN処理に入る為に初期化
			SceneChange::GetInstance()->init();
		}
			break;
		default:
			break;
		}
	}
}

void Game::Draw() {

	//スカイドーム描画
	m_skydome->Draw();
	//台の描画
	m_stand->Draw_transform(
		m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_X)),
		1.0f,m_csvData[static_cast<int>(CSVID::STAGE)]->GetCell().at(m_stageNumber).at(static_cast<int>(STAGE_STATUSID::STAGE_Z)));
	
	
	//プレイヤー描画
	m_player->Draw();
	//敵描画
	m_enemyManager->DrawAll();

	//弾表示
	m_bulletManager->DrawBullets();

	//UIを描画
	for (int i = 0; i < static_cast<int>(GAMEUIID::END); i++)
	{
		if (i == static_cast<int>(GAMEUIID::CLEAR))
		{
			if (m_gameState == GAMESTATE::CLEAR)
			{
				//クリアロゴ
				m_ui[static_cast<int>(GAMEUIID::CLEAR)]->Draw();
			}
		}
		else if (i == static_cast<int>(GAMEUIID::GAMEOVER))
		{
			if (m_gameState == GAMESTATE::GAMEOVER)
			{
				//ゲームオーバーロゴ
				m_ui[static_cast<int>(GAMEUIID::GAMEOVER)]->Draw();
			}
		}
		else
		{
			//スキル画面UI表示
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
					//常に描画するもの
					m_ui[i]->Draw();
				}
			}
		}

		

	}

	//シーン遷移描画
	if (m_sceneChangeFlg == true)
	{
		SceneChange::GetInstance()->Draw();
	}
}

