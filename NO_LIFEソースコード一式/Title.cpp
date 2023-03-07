#include "Title.h"
#include "Input.h"
#include "mesh.h"
#include "SceneChange.h"



void Title::Imgui()
{
	// Start the Dear ImGui frame、開始処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ここまで

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

	//フレームレート
	ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);

	switch (m_titleState)
	{
	case Title::TITLE_STATE::PRESS_ANY_BUTTON: {
		//ボタン点滅調整
		std::string str;
		str = u8"PRESS_ANY_BUTTONの点滅速度切り替え";
		ImGui::Text(str.c_str());
		//調整するためのバー
		ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);

		str = u8"アニメーション調整";
		ImGui::Text(str.c_str());
		//調整するためのバー
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



	////２つ目
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	//ImGui::SetNextWindowPos(ImVec2(980, 20));
	//ImGui::SetNextWindowSize(ImVec2(280, 300));
	//
	//ImGui::Begin(u8"操作説明");

	//std::string str;
	//str = u8"自機：左右キー：向きを変える";
	//ImGui::Text(str.c_str());
	//str = u8"自機：w：前進";
	//ImGui::Text(str.c_str());
	//str = u8"自機：s：後退";
	//ImGui::Text(str.c_str());

	////左キー
	//static float num;
	//num = Input::GetInstance().GetLStick().x;
	//ImGui::InputFloat("input float", &num);
	//num = Input::GetInstance().GetLStick().y;
	//ImGui::InputFloat("input float", &num);

	////右キー
	//num = Input::GetInstance().GetRStick().x;
	//ImGui::InputFloat("input float", &num);
	//num = Input::GetInstance().GetRStick().y;
	//ImGui::InputFloat("input float", &num);

	////if (Hit == true)
	////{
	////	num = 777.0f;
	////	ImGui::InputFloat("判定", &num);

	////	ImGui::Text("TRUE");
	////	
	////	//ImGui::Text("fps: %.2f", ImGui::GetIO().DeltaTime);
	////}
	////else
	////{
	////	num = 0.0f;
	////	ImGui::InputFloat("判定", &num);
	////	ImGui::InputFloat("判定", &ImGui::GetIO().DeltaTime);
	////}

	//ImGui::End();

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();

	// Rendering、後処理
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Title::Init()
{
	DebugFlg = true;//デバッグするかどうか

	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		if (m_ui[i] == nullptr)
		{
			//UIを実体化
			m_ui[i] = std::make_unique<Quad2D>();
		}
		else
		{
			// まだ読み込まれていなかった場合
			Texture tex;
			MessageBox(nullptr, "タイトルシーン：UIオブジェクトの中身に何か入っています",
				"Error!", MB_ICONERROR | MB_OK);
			tex.texture = nullptr;
		}
	}

	//一括初期化、デバッグ用
	//for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	//{
	//	//生成したオブジェクトを初期化する
	//	UiInitSetting(static_cast<IMAGEID>(i));
	//}

	//タイトル背景
	m_ui[static_cast<int>(IMAGEID::TITLE_BG01)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::TITLE_BG01)].imagename, 
		MAXWIDTH, MAXHEIGHT, MAXWIDTH_HALF, MAXHEIGHT_HALF);
	//プレスエニーボタン
	m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)].imagename,
		1500, 500, MAXWIDTH_HALF, MAXHEIGHT_HALF + 100);
	//フレーム
	m_ui[static_cast<int>(IMAGEID::FRAME)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::FRAME)].imagename,
		800, 150, MAXWIDTH_HALF - 200, MAXHEIGHT_HALF - 25);
	//コマンド
	m_ui[static_cast<int>(IMAGEID::COMMAND)]->UiInitSetting
	(m_imageLlist[static_cast<int>(IMAGEID::COMMAND)].imagename,
		MAXWIDTH, MAXHEIGHT, MAXWIDTH_HALF, MAXHEIGHT_HALF);
	
	//一括更新
	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		//表示できるように更新
		m_ui[i]->updateAnimeNo();
	}

	

	//シーン遷移フラグ初期化
	switch (SceneChange::GetInstance()->GetSceneChangeFlg())
	{
	case SceneChange::SCENECHANGEID::FADDEIN:
		//画面に何もないため本処理開始
		m_sceneChangeFlg = false;
		break;
	case SceneChange::SCENECHANGEID::FADDEOUT:
		//別画面から移ってきているためフェードアウト処理に移行
		m_sceneChangeFlg =	true;
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
	SceneStatus = SCENESTATUS::Title;//タイトルシーンに設定

}

void Title::Exit()
{
	for (int i = 0; i < static_cast<int>(IMAGEID::END); i++)
	{
		//所有権を移行して削除。
		Quad2D* exitUi = m_ui[i].release();
		delete exitUi;
	}
	
	//次のシーンを指定
	//NextSceneStatus = SCENESTATUS::Game;

	//フラグリセット、次のシーンの為
	SceneFlgReset();
}

void Title::Update()
{
	//シーン遷移処理に入っていなければ処理を行う
	if (m_sceneChangeFlg == false)
	{
		switch (m_titleState)
		{
		case Title::TITLE_STATE::PRESS_ANY_BUTTON: {

			//点滅処置
			m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->Flashing(0.01f);

			//何かのボタンが押されたらゲームを開始する、コントローラー版/エンターが押されたらゲームを開始する、キーボード版
			if (Input::GetInstance().CheckButtonAll() || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {
				m_titleState = TITLE_STATE::MAIN;
			}
			

			break;
		}
		case Title::TITLE_STATE::MAIN: {

			//選択しているコマンドをわからせるフレーム
			m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNo();
			//コマンド
			m_ui[static_cast<int>(IMAGEID::COMMAND)]->updateAnimeNo();
			//点滅処置
			//m_ui[static_cast<int>(IMAGEID::FRAME)]->Flashing(0.01f);

			if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) || (Input::GetInstance().CheckKeyTrigger(DIK_UP))) {
				//一番上のコマンドならそれ以上、上に移動しない
				if (m_commandId != COMMAND_ID::MAIN_GAME)
				{
					//num = 何番目のコマンドになるか
					//現在の番号を入れる
					int num = static_cast<int>(m_commandId);

					//一つ上のコマンドにする
					//enumで作成しているため、上げる＝数値を引く、下げる＝数値を上げる
					num -= 1;

					//変更されたコマンドに更新する
					m_commandId = static_cast<COMMAND_ID>(num);

					//選択UIの位置を更新
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 140);
				}
			}
			else if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) || (Input::GetInstance().CheckKeyTrigger(DIK_DOWN))) {
				//一番下のコマンドならそれ以上、下に移動しない
				if (m_commandId != COMMAND_ID::END)
				{
					//num = 何番目のコマンドになるか
					//現在の番号を入れる
					int num = static_cast<int>(m_commandId);

					//一つ下のコマンドにする
					//enumで作成しているため、上げる＝数値を引く、下げる＝数値を上げる
					num += 1;

					//変更されたコマンドに更新する
					m_commandId = static_cast<COMMAND_ID>(num);

					//選択UIの位置を更新
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y + 140);
				}
			}

			if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {

				//シーン遷移処理開始の為フラグ変更
				m_sceneChangeFlg = true;
			}
			else if (Input::GetInstance().CheckButtonRelease(XINPUT_GAMEPAD_B) || (Input::GetInstance().CheckKeyTrigger(DIK_BACKSPACE))) {
				//プレスエニーボタンの画面に戻す
				m_titleState = TITLE_STATE::PRESS_ANY_BUTTON;

				if (m_commandId == COMMAND_ID::EDITOR)
				{
					//選択UIの位置を更新
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 140);
				}
				else if (m_commandId == COMMAND_ID::END)
				{
					//選択UIの位置を更新
					m_ui[static_cast<int>(IMAGEID::FRAME)]->updateAnimeNoPos
					(m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().x,
						m_ui[static_cast<int>(IMAGEID::FRAME)]->GetXYPos().y - 280);
				}
				//選択しているコマンドを初期化
				m_commandId = COMMAND_ID::MAIN_GAME;
				//アルファ値を初期化
				m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->FlashingReset();
				//m_ui[static_cast<int>(IMAGEID::FRAME)]->FlashingReset();
			}
			break;
		}
		default:
			break;
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

				//次のシーンを指定
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

void Title::Draw()
{
	//背景は常に表示
	m_ui[static_cast<int>(IMAGEID::TITLE_BG01)]->Draw();


	switch (m_titleState)
	{
	case Title::TITLE_STATE::PRESS_ANY_BUTTON: {
		//プレスエニーボタンロゴ
		m_ui[static_cast<int>(IMAGEID::PRESS_ANY_BUTTON)]->Draw();

		break;
	}
	case Title::TITLE_STATE::MAIN: {
		//選択しているコマンドをわからせるフレーム
		m_ui[static_cast<int>(IMAGEID::FRAME)]->Draw();
		//コマンド
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

