#pragma once

//ゲームシーンの状態遷移を表す
enum class GAMESTATE
{
	INIT,				//初期化処理ステージごとに通るもの
	MAINGAME,			//メインゲーム
	CLEAR,				//クリア時
	GAMEOVER,			//ゲームオーバー時
	CHOICESKILL,		//スキル選択画面

	END
};
