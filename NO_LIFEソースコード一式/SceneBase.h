#pragma once
#include "myimgui.h"

/*************************************************************
* 各シーンの基底となるクラス
*************************************************************/
class SceneBase
{
protected:
private:
	
	
public:
	//どのシーンかを判断する
	enum class SCENESTATUS
	{
		Title,
		Game,
		GameBoss,
		Editor,
		End,
	};
	SCENESTATUS SceneStatus;//そのシーンが何かを表す

	SCENESTATUS NextSceneStatus;//次のシーンが何かを入れる
	//初期化処理を行ったか
	bool SceneInitFlg = true;
	//シーンの終了処理するかどうか
	bool SceneDisposeFlg = false;

	//フラグの状態を変える関数
	//初期化完了
	void SceneFlgInit() {
		SceneInitFlg = false;
		SceneDisposeFlg = false;
	}
	//シーン終了処理開始、そのシーンが終了する時に使用、クリア時など
	void SceneFlgDispose() {
		SceneInitFlg = false;
		SceneDisposeFlg = true;
	}
	//フラグ状態を初期化、別のシーンに移動する際に使う
	void SceneFlgReset() {
		SceneInitFlg = true;
		SceneDisposeFlg = false;
	}

	//デバッグ処理をオンオフどちらにするか
	//trueオン、falseオフ
	bool DebugFlg = false;
	//シーン遷移処理に入るかどうか
	bool m_sceneChangeFlg = false;

	virtual void Imgui(){};
	virtual void Init(){};
	virtual void Exit(){};
	virtual void Update(){};
	virtual void Draw(){};

	////次何のシーンに行くか
	//SCENESTATUS NextScene() {
	//	
	//}

	//シーン移動時のアニメーション
	
};

