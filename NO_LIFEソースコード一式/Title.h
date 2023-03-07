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
	

	//タイトルで使用する画像名
	enum class IMAGEID
	{
		TITLE_BG01,			//背景
		PRESS_ANY_BUTTON,		//開始時のUI
		FRAME,				//どれを選択しているかの枠
		COMMAND,			//エディタモード、ゲームモードなどの切り替え
		END,				//最後の項目
	};

	//タイトルシーン中のシーン状態
	enum class TITLE_STATE
	{
		PRESS_ANY_BUTTON,	//PRESS_ANY_BUTTONを表示する一番最初のシーン
		MAIN,				//メインのコマンドシーン
	};
	//切り替えフラグ
	TITLE_STATE m_titleState = TITLE_STATE::PRESS_ANY_BUTTON;

	//コマンドの種類
	enum  class COMMAND_ID
	{
		MAIN_GAME, 
		EDITOR,
		END,
	};
	//現在どのコマンドを指しているか
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

	//UI、スマートポインタ―
	std::unique_ptr<Quad2D> m_ui[static_cast<int>(IMAGEID::END)];

	

//	//UIの最低限の初期化を行う
//	void UiInitSetting(IMAGEID _modelid);
//	void UiInitSetting(IMAGEID _modelid, uint32_t _width, uint32_t _height, float _x, float _y);
//	//横サイズ、縦サイズ、配置位置XY。アニメーションはしない
//	void UiUpdateSetting(IMAGEID _modelid, uint32_t _width, uint32_t _height, float _x,float _y);
};

