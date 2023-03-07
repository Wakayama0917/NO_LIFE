#pragma once
#include <vector>
#include <memory>

class Quad2D;
class SceneChange
{
public:
	//使用する進行のリスト
	enum class SCENECHANGEID
	{
		FADDEIN,		//フェードイン処理中
		FADDEOUT,		//フェードアウト処理中
		END,			//処理終了
	};
private:
	//UI生成
	//上向き
	std::vector<std::unique_ptr<Quad2D>> m_sceneChangeuiUP;
	int m_countUP = 0;
	//下向き
	std::vector<std::unique_ptr<Quad2D>> m_sceneChangeuiDOWN;
	int m_countDOWN = 0;
	//進行状態を管理するフラグ
	SCENECHANGEID m_sceneChangeState = SCENECHANGEID::FADDEIN;

public:
	SceneChange();

	//シングルトン
	static SceneChange* GetInstance() {
		static SceneChange instance;
		return &instance;
	}

	//初期化処理
	void init();
	//フェード処理を1つにまとめたもの、最初にinitだけはいる
	bool UpdateAll();
	//フェードイン
	bool UpdateIN();
	//フェードアウト
	bool UpdateOUT();
	//描画
	void Draw();

	//フラグを取得
	SCENECHANGEID GetSceneChangeFlg() { return m_sceneChangeState; }
};


