#pragma once
#include "gameobject.h"
#include "CModel.h"

class Skydome : public GameObject
{
private:
	//CModel* m_model;
public:
	bool Init();
	void Draw();
	void Update();
	void Finalize();

	/*void SetModelSky(CModel* _p){
		m_model = _p;
	}*/

	//動的にするため
	/*
	クラス内でスタティックにするとコンパイル時に生成される。
	動的扱いとなり、デリートをする必要も無くなる。全体で使用する物に関してはこちらの方が良い
	グローバル、スタティック、NEW３つだとグローバルはあり得ない、残り二つは場合による
	*/
	static Skydome * GetInstance() {
		static Skydome instance;
		return &instance;
	}
	/*Skydome();
	~Skydome();*/
};


