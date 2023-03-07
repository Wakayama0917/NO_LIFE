#pragma once
#include "MainGame.h"
#include "Title.h"
#include "Editor.h"
//#include	"SceneBase.h"

class Game_Manager : public SceneBase
{
	Game_Manager() {
	}

	//どのシーンを回すかの全体フラグ
	SCENESTATUS ManagerStatus = SCENESTATUS::Title;
public:
	//シングルトン
	static Game_Manager* GetInstance() {
		static Game_Manager instance;
		return &instance;
	}

	bool GameFlg = true;

	void GameInit();//初期化
	void GameInput(uint64_t _dt);//キー入力
	void GameRender();//描画前処理
	void GameAllUpdate(uint64_t _dt);

	void GameUpdate(uint64_t _dt);
	void GameDispose();

private:
	//スマートポインター、タイトル
	std::unique_ptr<Title> m_title;
	//スマートポインター、メインゲーム
	std::unique_ptr<Game> m_mainGame;
	//スマートポインター、エディターモード
	std::unique_ptr<Editor> m_editor;

	//次のシーンを実体化させる
	void MakeObject();
};

