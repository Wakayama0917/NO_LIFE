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
	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// カメラが必要
	DirectX::XMFLOAT3 eye(0, 00, -30);		// カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// 注視点
	DirectX::XMFLOAT3 up(0, 1, 0);			// カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);					// カメラのデータ

	// 平行光源をセット
	DX11LightInit(DirectX::XMFLOAT4(1, 100, -1, 0));
	

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// スクリーン幅
		Application::CLIENT_HEIGHT			// スクリーンの高さ
		);

	//input初期化
	Input::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// スクリーン幅
		Application::CLIENT_HEIGHT			// スクリーンの高さ
	);


	// アルファブレンド有効化
	TurnOnAlphablend();

	//最初のシーンを実体化
	MakeObject();

	//GUIの初期化
	imguiInit();

	//モデル読み込み
	ModelMgr::GetInstance().Init();
	//ビルボード読み込み
	BillBoard::GetInstance()->Init();
	
	
}

void Game_Manager::GameInput(uint64_t _dt) {
	CDirectInput::GetInstance().GetKeyBuffer();//キー入力を毎回取る

	//キー入力取る
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
				// タイトル初期化
				m_title->Init();
			}
			else
			{
				// まだ読み込まれていなかった場合
				Texture tex;
				MessageBox(nullptr, "タイトルシーンがヌルです",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}


			Skydome::GetInstance()->Init();
		}
		//アップデート、描画処理
		// タイトル更新
		m_title->Update();
		//描画前処理
		GameRender();
		// メインゲーム描画
		m_title->Draw();
		if (m_title->DebugFlg) {
			//Imgui描画
			//m_title->Imgui();
		}
		// 描画後処理
		DX11AfterRender();

		//終了時
		if (m_title->SceneDisposeFlg) {

			// タイトル終了処理
			m_title->Exit();

			//次のシーンを決定
			ManagerStatus = m_title->NextSceneStatus;


			//所有権を移行して削除。
			Title* exitTitle = m_title.release();
			delete exitTitle;

			//次のシーンを実体化
			MakeObject();
		}
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		if (m_mainGame->SceneInitFlg) {
			if (m_mainGame != nullptr)
			{
				// メインゲーム初期化
				m_mainGame->Init();
			}
			else
			{
				// まだ読み込まれていなかった場合
				Texture tex;
				MessageBox(nullptr, "メインゲームシーンがヌルです",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}

			Skydome::GetInstance()->Init();
		}

		//アップデート、描画処理
		// メインゲーム更新
		m_mainGame->Update();
		//描画前処理
		GameRender();
		// メインゲーム描画
		m_mainGame->Draw();
		if (m_mainGame->DebugFlg) {
			//Imgui描画
			//m_mainGame->Imgui();
		}
		// 描画後処理
		DX11AfterRender();

		//終了時
		if (m_mainGame->SceneDisposeFlg) {
			// メインゲーム終了処理
			m_mainGame->Exit();

			//次のシーンを決定
			ManagerStatus = m_mainGame->NextSceneStatus;

			//所有権を移行して削除。
			Game* exitGame = m_mainGame.release();
			delete exitGame;

			//次のシーンを実体化
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
				// タイトル初期化
				m_editor->Init();
			}
			else
			{
				// まだ読み込まれていなかった場合
				Texture tex;
				MessageBox(nullptr, "エディターシーンがヌルです",
					"Error!", MB_ICONERROR | MB_OK);
				tex.texture = nullptr;
			}

			Skydome::GetInstance()->Init();
		}
		//アップデート、描画処理
		// タイトル更新
		m_editor->Update();

		//描画前処理
		GameRender();
		// メインゲーム描画
		m_editor->Draw();
		//シーン遷移がされていなければ
		if (m_editor->m_sceneChangeFlg == false)
		{
			//Imgui描画、他のシーンと違い絶対描画
			m_editor->Imgui();
		}
		// 描画後処理
		DX11AfterRender();

		//終了時
		if (m_editor->SceneDisposeFlg) {

			// タイトル終了処理
			m_editor->Exit();

			//次のシーンを決定
			ManagerStatus = m_editor->NextSceneStatus;

			//最終で行うもの
			//DX11Uninit();

			//所有権を移行して削除。
			Editor* exitEditor = m_editor.release();
			delete exitEditor;

			//次のシーンを実体化
			MakeObject();
		}

		break;
	}
	case SceneBase::SCENESTATUS::End: {
		//ゲームを終了する
		GameFlg = false;

		//最終で行うもの
		DX11Uninit();
		break;
	}
	default:
		break;
	}

	//以下どのシーンでも枚フレーム行うもの
	//キー入力
	Input::GetInstance().Update();
}

void Game_Manager::GameUpdate(uint64_t _dt) {

	// 空中戦更新
	m_mainGame->Update();

}

void Game_Manager::GameRender() {

	float col[4] = { 1,0,0,1 };

	// 描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// プロジェクション変換行列取得
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ビュー変換行列を取得
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	
}

void Game_Manager::GameDispose() {

	switch (ManagerStatus)
	{
	case SceneBase::SCENESTATUS::Title: {
		//タイトルで終わったら破棄
		m_title->Exit();
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		// メインゲームで終わったら破棄
		m_mainGame->Exit();
		break;
	}
	case SceneBase::SCENESTATUS::GameBoss: {
		break;
	}
	case SceneBase::SCENESTATUS::Editor: {
		//エディターモードで終わったら破棄
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
		//タイトルシーンを実体化
		m_title = std::make_unique<Title>();
		break;
	}
	case SceneBase::SCENESTATUS::Game: {
		//ゲームシーンを実体化
		m_mainGame = std::make_unique<Game>();
		break;
	}
	case SceneBase::SCENESTATUS::GameBoss: {
		break;
	}
	case SceneBase::SCENESTATUS::Editor: {
		//エディターシーンを実体化
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
