#pragma once
#include "EnemyRotation.h"
#include "EnemyHoming.h"
#include "EnemyLoop.h"


// 前方宣言
class BulletManager;

class EnemyManager
{
private:

	std::vector<std::unique_ptr<EnemyObject>>	m_Enemy;//敵をまとめて入れる

	std::mt19937 m_random;//乱数発生用敵を生成
	std::mt19937 m_X;//乱数発生用X軸
	std::mt19937 m_Z;//乱数発生用Y軸

public:
	/*************************************************************
	* 関数
	*************************************************************/
	//初期化処理、モデルパスのベクターを受け取り一括で初期化する
	void InitEditor(std::unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);//エディター用、全ての敵を1体ずつ
	void InitRandom(std::vector<ENEMY_TYPE> _enemyList,unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);//敵の種類の配列を受け取り生成
	void Update(std::unique_ptr<BulletManager>& _bulletManager);
	void DrawAll();
	//死んだ敵の削除
	void EnemyDelete();
	//全ての敵が死んでいるかを確認
	bool AllDead();
	//配列を完全初期化
	void Reset();
	/*void InitAllOne(std::vector<std::string> _modelName);*/
	//エディターシーン用
	void SetPosCenter(float _X, float _Z);//中心に敵を移動
	void DrawOneEditor(int _num);//選ばれたキャラを描画
	void UpdateEditor(int _num, std::unique_ptr<BulletManager>& _bulletManager);
private:
	//各キャラの初期化
	void InitRotation(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitLoop(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitHoming(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitBoss1(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	
public:
	/*************************************************************
	* SET、GET系　関数
	*************************************************************/
	std::vector<std::unique_ptr<EnemyObject>>& GetEnemyList() {
		return m_Enemy;
	}

	void SetAllPos(float X,float Z);//ステージのサイズを受け取り全ての敵の位置を初期化
};

