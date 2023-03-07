#pragma once
#include "Collider.h"
#include "HommingMissile.h"
#include "NormalBullet.h"
#include "BoundingSphere.h"
#include "EnumListBullet.h"
#include "EnumListPlayer.h"
#include "BulletObject.h"


class BulletManager
{
	/*************************************************************
	* 変数
	*************************************************************/
protected:
	//プレイヤーのポインタを格納するよう
	std::shared_ptr<Player> m_sPlayer;
	
	//void UpdateBullets();
	std::vector<std::unique_ptr<BulletObject>>	m_bulletsEnemy;//弾
	std::vector<std::unique_ptr<BulletObject>>	m_bulletsPlayer;//弾
	//いらなくなる
	//std::vector<std::unique_ptr<NormalBullet>>	m_normalBullets;//弾
	//std::vector<std::unique_ptr<HomingMissile>>	m_HommingMissile;//ホーミング

	//モデルのパス名保存用リスト
	string m_modelNameList[static_cast<int>(BULLET_TYPE::END)];
	//弾のステータス変更時に使用する
	float m_bulletChangeData[static_cast<int>(BULLET_STATUSID::END)];
	// 要素保管、CSVのデータを一括で保存
	std::vector<std::vector<float>> m_cell;
private:
	bool m_hitFlg = false;
	
public:
	/*************************************************************
	* 関数
	*************************************************************/
	//初期化、これを所有する敵の情報や弾の情報を格納
	void InitEditor(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player, int _bulletType);
	//初期化、データを全て保存
	void Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);
	//自動で弾を発射させる、敵
	void FireBulletEnemy(unique_ptr<EnemyObject>& _enemyObject);
	//自動で弾を発射させる、プレイヤー
	void FireBulletPlayer();
	//敵の弾更新
	void UpdateBulletsEnemy();
	//プレイヤーの弾
	void UpdateBulletsPlayer();
	//配列内全ての弾の更新、プレイヤーも追加版
	void UpdateBullets(std::vector<std::unique_ptr<EnemyObject>>& _enemy);
	//通常弾描画
	void DrawBullets();
	//CSVステータスをセット
	void SetCsvStatus(BULLET_TYPE _bulletType, std::vector<float> _vallue);
	//要素を更新
	void SetData(BULLET_STATUSID _id,float _value) {
		m_bulletChangeData[static_cast<int>(_id)] = _value;	
	}
	void Reset();//中身を初期化


	/*************************************************************
	* 通常弾関数
	*************************************************************/
	//弾発射、初期化
	void FireBullet(unique_ptr<EnemyObject>& _enemyObject,ENEMY_SHOT_DIRECTION _num);//敵
	void FireBullet(PLAYER_SHOT_DIRECTION _num);//プレイヤー
	void FireBulletEditor(unique_ptr<EnemyObject>& _enemyObject,PLAYER_SHOT_DIRECTION _num);//プレイヤー

	/*************************************************************
	* ホーミング弾関数
	*************************************************************/
	//ミサイル発射、初期化
	void FireHommingMissile(unique_ptr<EnemyObject>& _enemyObject, ENEMY_SHOT_DIRECTION _num);//プレイヤーに向けて

private:
	void bulletDelete(std::vector<std::unique_ptr<BulletObject>>& _bullets);
	
	
};



