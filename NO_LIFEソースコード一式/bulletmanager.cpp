#include <vector>
#include "ModelMgr.h"
#include "HommingMissile.h"
#include "bulletmanager.h"
#include "imgui.h"
#include "EnumListPlayer.h"



void BulletManager::InitEditor(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player, int _bulletType)
{
	//プレイヤーのポインタ格納
	m_sPlayer = _player;

	//敵のモデルパスを保存、読み込みは各シーンINIT内で行い生成速度を上げる
	for (size_t i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_modelNameList[i] = _csvFile->GetModelName().at(i);
	}
	
	//弾の数値を初期化
	for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
	{
		//設定する敵の持つ弾のステータスを入れる
		m_bulletChangeData[i] = _csvFile->GetCell().at(_bulletType).at(i);
	}
	
}

void BulletManager::Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player)
{
	//プレイヤーのポインタ格納
	m_sPlayer = _player;

	//敵のモデルパスを保存、読み込みは各シーンINIT内で行い生成速度を上げる
	for (size_t i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_modelNameList[i] = _csvFile->GetModelName().at(i);
	}

	//弾の数値を初期化
	for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_cell.push_back(vector<float>());

		for (int j = 0; j < static_cast<int>(BULLET_STATUSID::END); j++)
		{
			//設定する敵の持つ弾のステータスを入れる
			m_cell.at(i).push_back(_csvFile->GetCell().at(i).at(j));
		}
	}
	
}

void BulletManager::FireHommingMissile(unique_ptr<EnemyObject>& _enemyObject, ENEMY_SHOT_DIRECTION _num)
{
	std::unique_ptr<HomingMissile> p;
	p = std::make_unique<HomingMissile>();
	//弾の初期化
	p->Init();//初期処理

	//ステータス配置
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::HOMMING)));
	
	//当たり判定用のコリジョンなどを初期化
	p->SetModel();

	// ターゲットを入れる
	p->SetTarget(XMFLOAT3(m_sPlayer->GetPos()));

	//発射方向セット
	p->SetDirection(_enemyObject->GetMtx(),_num);

	//ミサイル速度セット
	p->SetVelocity(_enemyObject->GetPos());

	//初期位置セット
	p->SetInitialPos(_enemyObject->GetPos());

	//ベクトル
	p->SetInitailQuaternion(_enemyObject->GetMtx());

	//当たり判定
	p->CollisionInitOld();

	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::bulletDelete(std::vector<std::unique_ptr<BulletObject>>& _bullets)
{
	//死んでいる弾を削除する
	auto it = _bullets.begin();
	while (it != _bullets.end()) {
		if ((*it)->GetState() == STATE::DEAD) {
			/*BulletObject* exitBullet = it->release();
			delete exitBullet;*/
			it = _bullets.erase(it);
			
		}
		else ++it;//1文なので短縮
	}

	

	//死んでいる弾を削除する
	/*for (int i = 0; i < _bullets.size(); i++)
	{
		if (_bullets[i]->isLive() == false) {
			BulletObject* exitBullet = _bullets[i].release();
			delete exitBullet;
			_bullets.erase(i);
			
		}
	}*/
}

void BulletManager::FireBullet(unique_ptr<EnemyObject>& _enemyObject, ENEMY_SHOT_DIRECTION _num)
{
	std::unique_ptr<NormalBullet> p;
	p = std::make_unique<NormalBullet>();
	//弾の初期化
	p->Init();//初期処理
	//ステータス配置
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::NORMAL)));
	
	//当たり判定用のコリジョンなどを初期化
	p->SetModel();
	//初期位置セット
	p->SetInitialPos(XMFLOAT3(_enemyObject->GetPos()));
	//発射方向セット
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(_enemyObject->GetMtx(), _num);
	//当たり判定
	p->CollisionInitOld();

	//最後尾に弾を追加
	//vectorコンテナにユニークポインタをセット
	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::FireBullet(PLAYER_SHOT_DIRECTION _num)
{
	std::unique_ptr<NormalBullet> p;
	p = std::make_unique<NormalBullet>();
	//弾の初期化
	p->Init();//初期処理
	//ステータス配置
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::P_NORMAL)));

	//当たり判定用のコリジョンなどを初期化
	p->SetModel();
	//初期位置セット
	p->SetInitialPos(XMFLOAT3(m_sPlayer->GetPos()));
	//発射方向セット
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(m_sPlayer->GetMtx(), _num);
	//当たり判定
	p->CollisionInitOld();

	//最後尾に弾を追加
	//vectorコンテナにユニークポインタをセット
	m_bulletsPlayer.emplace_back(std::move(p));
}

void BulletManager::FireBulletEditor(unique_ptr<EnemyObject>& _enemyObject,PLAYER_SHOT_DIRECTION _num)
{
	std::unique_ptr<NormalBullet> p;
	p = std::make_unique<NormalBullet>();
	//弾の初期化
	p->Init();//初期処理
	//ステータス配置
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::P_NORMAL)));

	//当たり判定用のコリジョンなどを初期化
	p->SetModel();
	//初期位置セット
	p->SetInitialPos(XMFLOAT3(_enemyObject->GetPos()));
	//発射方向セット
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(_enemyObject->GetMtx(), _num);
	//当たり判定
	p->CollisionInitOld();

	//最後尾に弾を追加
	//vectorコンテナにユニークポインタをセット
	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::FireBulletEnemy(unique_ptr<EnemyObject>& _enemyObject)
{
	//各方向の審議判定
	for (int i = 0; i < static_cast<int>(ENEMY_SHOT_DIRECTION::END); i++)
	{
		//この方向が真なら
		if (_enemyObject->GetShotDirection(i)) {

			//エネミーの持つタイプの弾を発射する
			switch (static_cast<BULLET_TYPE>(_enemyObject->GetBulletType()))
			{
			case BULLET_TYPE::NORMAL: {
				FireBullet(_enemyObject,static_cast<ENEMY_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::HARD: {
				break;
			}
			case BULLET_TYPE::HOMMING: {
				FireHommingMissile(_enemyObject, static_cast<ENEMY_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::NONE: {
				break;
			}
			case BULLET_TYPE::P_NORMAL: {
				//エディター用にセット、基本はプレイヤー用のやつで発射
				FireBulletEditor(_enemyObject, static_cast<PLAYER_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::END: {
				break;
			}
			default:
				break;
			}
		}
	}
	
	//弾を撃ったのでフラグを折る
	_enemyObject->FireBulletFlgFalse();
}

void BulletManager::FireBulletPlayer()
{
	//各方向の審議判定
	for (int i = 0; i < static_cast<int>(PLAYER_SHOT_DIRECTION::END); i++)
	{
		//この方向が真なら
		if (m_sPlayer->GetShotDirection(i)) {

			//プレイヤーの持つタイプの弾を発射する
			switch (static_cast<BULLET_TYPE>(m_sPlayer->GetBulletType()))
			{
			case BULLET_TYPE::NORMAL: {
				FireBullet(static_cast<PLAYER_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::HARD: {
				break;
			}
			case BULLET_TYPE::HOMMING: {
				//FireHommingMissile(_enemyObject, static_cast<PLAYER_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::NONE: {
				break;
			}
			case BULLET_TYPE::P_NORMAL: {
				FireBullet(static_cast<PLAYER_SHOT_DIRECTION>(i));
				break;
			}
			case BULLET_TYPE::END: {
				break;
			}
			default:
				break;
			}
		}
	}

	//弾を撃ったのでフラグを折る
	m_sPlayer->FireBulletFlgFalse();
}

void BulletManager::UpdateBulletsEnemy()
{
	//敵側全弾を更新
	for (auto& b : m_bulletsEnemy) {
		//移動
		b->Move(m_sPlayer->GetPos());
		b->Update();
	}
	
	//敵：死んでいる弾を削除する
	bulletDelete(m_bulletsEnemy);
}

void BulletManager::UpdateBulletsPlayer()
{
	//プレイヤー側全弾を更新
	for (auto& b : m_bulletsPlayer) {

		//移動
		b->Move(m_sPlayer->GetPos());
		b->Update();
	}

	//プレイヤー：死んでいる弾を削除する
	bulletDelete(m_bulletsPlayer);
}

void BulletManager::UpdateBullets(std::vector<std::unique_ptr<EnemyObject>>& _enemy)
{
	//敵側全弾を更新
	for (auto& b : m_bulletsEnemy) {
		b->Move(m_sPlayer->GetPos());
	}
	//*******************************************************
	//プレイヤー側全弾を更新
	for (auto& b : m_bulletsPlayer) {
		//先に動かしてから当たり判定
		b->Move(m_sPlayer->GetPos());
	}
	//当たり判定を取る
	Collider::GetInstance()->AllOnCollision(m_sPlayer, _enemy, m_bulletsPlayer, m_bulletsEnemy);

	//敵：死んでいる弾を削除する
	bulletDelete(m_bulletsEnemy);
	//プレイヤー：死んでいる弾を削除する
	bulletDelete(m_bulletsPlayer);

	//メモリを再調整
	/*m_bulletsEnemy.shrink_to_fit();
	m_bulletsPlayer.shrink_to_fit();*/
}

void BulletManager::DrawBullets()
{
	//敵全弾描画
	for (auto& b : m_bulletsEnemy) {
		b->Draw();
		//b->GetSphere().Draw();
		

	}

	//プレイヤー全弾描画
	for (auto& b : m_bulletsPlayer) {
		b->Draw();
		//b->GetSphere().Draw();

	}
}

void BulletManager::SetCsvStatus(BULLET_TYPE _bulletType, std::vector<float> _vallue)
{
	//弾の数値を初期化
	for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
	{
		//設定する敵の持つ弾のステータスを入れる
		m_cell.at(static_cast<int>(_bulletType)).at(i) = _vallue[i];
	}
}

void BulletManager::Reset()
{
	//弾を削除
	m_bulletsEnemy.erase(m_bulletsEnemy.begin(), m_bulletsEnemy.end());
	m_bulletsPlayer.erase(m_bulletsPlayer.begin(), m_bulletsPlayer.end());
	////中身を無くす
	//m_bulletsEnemy.clear();
	//m_bulletsPlayer.clear();
	////メモリを再調整
	//m_bulletsEnemy.shrink_to_fit();
	//m_bulletsPlayer.shrink_to_fit();
}


