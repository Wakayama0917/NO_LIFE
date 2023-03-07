#include "EnemyManager.h"
#include "ModelMgr.h"
#include "bulletmanager.h"
#include "Boss1.h"
//#include "BillBoard.h"


void EnemyManager::InitEditor(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player)
{
	InitRotation(_csvFile,_player);
	InitLoop(_csvFile, _player);
	InitHoming(_csvFile, _player);
	InitBoss1(_csvFile, _player);
}

void EnemyManager::InitRandom(std::vector<ENEMY_TYPE> _enemyList, unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player)
{
	if (_enemyList.size() == 0)
	{
		std::random_device rand_dev;//シード値決定用乱数
		// シード値セット
		m_random.seed(rand_dev());
		//乱数生成、ステージの幅に合わせるため２で割る。端の分だけ調節
		std::uniform_int_distribution<int32_t> randX(0, static_cast<int>(ENEMY_TYPE::END)-1);

		for (int i = 0; i < 3; i++)
		{
			int x = randX(m_random);

			switch (static_cast<ENEMY_TYPE>(x))
			{
			case ENEMY_TYPE::ROTATION:
				InitRotation(_csvFile, _player);
				break;
			case ENEMY_TYPE::LOOP:
				InitLoop(_csvFile, _player);
				break;
			case ENEMY_TYPE::HOMMING:
				InitHoming(_csvFile, _player);
				break;
			case ENEMY_TYPE::END:
				break;
			default:
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < _enemyList.size(); i++)
		{
			switch (_enemyList[i])
			{
			case ENEMY_TYPE::ROTATION:
				InitRotation(_csvFile, _player);
				break;
			case ENEMY_TYPE::LOOP:
				InitLoop(_csvFile, _player);
				break;
			case ENEMY_TYPE::HOMMING:
				InitHoming(_csvFile, _player);
				break;
			case ENEMY_TYPE::BOSS1:
				InitBoss1(_csvFile, _player);
				break;
			case ENEMY_TYPE::END:
				break;
			default:
				break;
			}
		}
	}
	
}

void EnemyManager::Update(std::unique_ptr<BulletManager>& _bulletManager)
{
	//敵の数だけ繰り返す
	for (int i = 0; i < GetEnemyList().size(); i++)
	{
		if (GetEnemyList()[i]->GetState() == STATE::LIVE)
		{
			//敵のアップデート
			GetEnemyList()[i]->Update();

			//敵の弾発射
			if (GetEnemyList()[i]->GetFireBulletFlg())
			{
				//弾発射
				_bulletManager->FireBulletEnemy(GetEnemyList()[i]);
			}
		}
	}
}

void EnemyManager::InitRotation(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player)
{
	//実体化回転
	std::unique_ptr<EnemyRotation> p;
	p = std::make_unique<EnemyRotation>();
	//初期化
	p->Init(_csvFile, _player);
	p->ReloadInit();
	//配列に追加
	m_Enemy.emplace_back(std::move(p));
	
}

void EnemyManager::InitLoop(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player)
{
	//実体化ループ
	std::unique_ptr<EnemyLoop> p;
	p = std::make_unique<EnemyLoop>();
	//初期化
	p->Init(_csvFile, _player);
	p->ReloadInit();
	//追加
	m_Enemy.emplace_back(std::move(p));
}

void EnemyManager::InitHoming(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player)
{
	//実体化回転
	std::unique_ptr<EnemyHoming> p;
	p = std::make_unique<EnemyHoming>();
	//初期化
	p->Init(_csvFile, _player);
	p->ReloadInit();
	//追加
	m_Enemy.emplace_back(std::move(p));
}

void EnemyManager::InitBoss1(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player)
{
	//実体化回転
	std::unique_ptr<Boss1> p;
	p = std::make_unique<Boss1>();
	//初期化
	p->Init(_csvFile, _player);
	p->ReloadInit();
	//追加
	m_Enemy.emplace_back(std::move(p));
}

void EnemyManager::SetAllPos(float _X, float _Z)
{
	std::random_device rand_dev;//シード値決定用乱数
	// シード値セット
	m_X.seed(rand_dev());
	m_Z.seed(rand_dev());
	//乱数生成、ステージの幅に合わせるため２で割る。端の分だけ調節
	std::uniform_int_distribution<int32_t> randX(-(_X / 2)+10, (_X / 2)-10);
	std::uniform_int_distribution<int32_t> randZ(-(_Z / 2)+30, (_Z / 2)-10);

	//敵の数だけ繰り返す
	for (int i = 0; i < m_Enemy.size(); i++)
	{
		int x = randX(m_X);
		int z = randZ(m_Z);
		m_Enemy[i]->SetPos(x,z);//位置をセット

		m_Enemy[i]->SetStageSize(_X,_Z);//ステージサイズをセット
	}
	
}

void EnemyManager::DrawAll()
{
	for (auto& b : m_Enemy) {
		if (b->GetState() == STATE::LIVE)
		{
			b->Draw();
		}
		//エフェクトは発生
		b->EffectDraw();
	}
}

void EnemyManager::EnemyDelete()
{
	//死んでいる敵を削除する
	auto it = m_Enemy.begin();
	while (it != m_Enemy.end())
	{
		if ((*it)->GetState() == STATE::LIVE)
		{
			it = m_Enemy.erase(it);
		}
		else it++;
	}
}

bool EnemyManager::AllDead()
{
	for (int i = 0; i < m_Enemy.size(); i++)
	{
		if (m_Enemy[i]->GetState() == STATE::LIVE) {
			return false;
		}
	}
	return true;
}

void EnemyManager::Reset()
{
	m_Enemy.erase(m_Enemy.begin(),m_Enemy.end());
	
}

void EnemyManager::SetPosCenter(float _X,float _Z)
{
	//敵の数だけ繰り返す
	for (int i = 0; i < m_Enemy.size(); i++)
	{
		m_Enemy[i]->SetPos(1, 1);//位置をセット

		m_Enemy[i]->SetStageSize(_X, _Z);//ステージサイズをセット
	}
}

void EnemyManager::DrawOneEditor(int _num)
{
	m_Enemy[_num]->Draw();
}

void EnemyManager::UpdateEditor(int _num,std::unique_ptr<BulletManager>& _bulletManager)
{
	//敵のアップデート
	GetEnemyList()[_num]->Update();
	//敵の弾発射
	if (GetEnemyList()[_num]->GetFireBulletFlg())
	{
		//弾発射
		_bulletManager->FireBulletEnemy(GetEnemyList()[_num]);
	}
}
