#include <vector>
#include "ModelMgr.h"
#include "HommingMissile.h"
#include "bulletmanager.h"
#include "imgui.h"
#include "EnumListPlayer.h"



void BulletManager::InitEditor(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player, int _bulletType)
{
	//�v���C���[�̃|�C���^�i�[
	m_sPlayer = _player;

	//�G�̃��f���p�X��ۑ��A�ǂݍ��݂͊e�V�[��INIT���ōs���������x���グ��
	for (size_t i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_modelNameList[i] = _csvFile->GetModelName().at(i);
	}
	
	//�e�̐��l��������
	for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
	{
		//�ݒ肷��G�̎��e�̃X�e�[�^�X������
		m_bulletChangeData[i] = _csvFile->GetCell().at(_bulletType).at(i);
	}
	
}

void BulletManager::Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player)
{
	//�v���C���[�̃|�C���^�i�[
	m_sPlayer = _player;

	//�G�̃��f���p�X��ۑ��A�ǂݍ��݂͊e�V�[��INIT���ōs���������x���グ��
	for (size_t i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_modelNameList[i] = _csvFile->GetModelName().at(i);
	}

	//�e�̐��l��������
	for (int i = 0; i < static_cast<int>(BULLET_TYPE::END); i++)
	{
		m_cell.push_back(vector<float>());

		for (int j = 0; j < static_cast<int>(BULLET_STATUSID::END); j++)
		{
			//�ݒ肷��G�̎��e�̃X�e�[�^�X������
			m_cell.at(i).push_back(_csvFile->GetCell().at(i).at(j));
		}
	}
	
}

void BulletManager::FireHommingMissile(unique_ptr<EnemyObject>& _enemyObject, ENEMY_SHOT_DIRECTION _num)
{
	std::unique_ptr<HomingMissile> p;
	p = std::make_unique<HomingMissile>();
	//�e�̏�����
	p->Init();//��������

	//�X�e�[�^�X�z�u
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::HOMMING)));
	
	//�����蔻��p�̃R���W�����Ȃǂ�������
	p->SetModel();

	// �^�[�Q�b�g������
	p->SetTarget(XMFLOAT3(m_sPlayer->GetPos()));

	//���˕����Z�b�g
	p->SetDirection(_enemyObject->GetMtx(),_num);

	//�~�T�C�����x�Z�b�g
	p->SetVelocity(_enemyObject->GetPos());

	//�����ʒu�Z�b�g
	p->SetInitialPos(_enemyObject->GetPos());

	//�x�N�g��
	p->SetInitailQuaternion(_enemyObject->GetMtx());

	//�����蔻��
	p->CollisionInitOld();

	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::bulletDelete(std::vector<std::unique_ptr<BulletObject>>& _bullets)
{
	//����ł���e���폜����
	auto it = _bullets.begin();
	while (it != _bullets.end()) {
		if ((*it)->GetState() == STATE::DEAD) {
			/*BulletObject* exitBullet = it->release();
			delete exitBullet;*/
			it = _bullets.erase(it);
			
		}
		else ++it;//1���Ȃ̂ŒZ�k
	}

	

	//����ł���e���폜����
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
	//�e�̏�����
	p->Init();//��������
	//�X�e�[�^�X�z�u
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::NORMAL)));
	
	//�����蔻��p�̃R���W�����Ȃǂ�������
	p->SetModel();
	//�����ʒu�Z�b�g
	p->SetInitialPos(XMFLOAT3(_enemyObject->GetPos()));
	//���˕����Z�b�g
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(_enemyObject->GetMtx(), _num);
	//�����蔻��
	p->CollisionInitOld();

	//�Ō���ɒe��ǉ�
	//vector�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::FireBullet(PLAYER_SHOT_DIRECTION _num)
{
	std::unique_ptr<NormalBullet> p;
	p = std::make_unique<NormalBullet>();
	//�e�̏�����
	p->Init();//��������
	//�X�e�[�^�X�z�u
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::P_NORMAL)));

	//�����蔻��p�̃R���W�����Ȃǂ�������
	p->SetModel();
	//�����ʒu�Z�b�g
	p->SetInitialPos(XMFLOAT3(m_sPlayer->GetPos()));
	//���˕����Z�b�g
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(m_sPlayer->GetMtx(), _num);
	//�����蔻��
	p->CollisionInitOld();

	//�Ō���ɒe��ǉ�
	//vector�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	m_bulletsPlayer.emplace_back(std::move(p));
}

void BulletManager::FireBulletEditor(unique_ptr<EnemyObject>& _enemyObject,PLAYER_SHOT_DIRECTION _num)
{
	std::unique_ptr<NormalBullet> p;
	p = std::make_unique<NormalBullet>();
	//�e�̏�����
	p->Init();//��������
	//�X�e�[�^�X�z�u
	//p->SetAllStatus(m_bulletChangeData);
	p->SetAllStatus(m_cell.at(static_cast<int>(BULLET_TYPE::P_NORMAL)));

	//�����蔻��p�̃R���W�����Ȃǂ�������
	p->SetModel();
	//�����ʒu�Z�b�g
	p->SetInitialPos(XMFLOAT3(_enemyObject->GetPos()));
	//���˕����Z�b�g
	//p->SetDirection(_enemyObject->GetMtx());
	p->SetDirection(_enemyObject->GetMtx(), _num);
	//�����蔻��
	p->CollisionInitOld();

	//�Ō���ɒe��ǉ�
	//vector�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	m_bulletsEnemy.emplace_back(std::move(p));
}

void BulletManager::FireBulletEnemy(unique_ptr<EnemyObject>& _enemyObject)
{
	//�e�����̐R�c����
	for (int i = 0; i < static_cast<int>(ENEMY_SHOT_DIRECTION::END); i++)
	{
		//���̕������^�Ȃ�
		if (_enemyObject->GetShotDirection(i)) {

			//�G�l�~�[�̎��^�C�v�̒e�𔭎˂���
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
				//�G�f�B�^�[�p�ɃZ�b�g�A��{�̓v���C���[�p�̂�Ŕ���
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
	
	//�e���������̂Ńt���O��܂�
	_enemyObject->FireBulletFlgFalse();
}

void BulletManager::FireBulletPlayer()
{
	//�e�����̐R�c����
	for (int i = 0; i < static_cast<int>(PLAYER_SHOT_DIRECTION::END); i++)
	{
		//���̕������^�Ȃ�
		if (m_sPlayer->GetShotDirection(i)) {

			//�v���C���[�̎��^�C�v�̒e�𔭎˂���
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

	//�e���������̂Ńt���O��܂�
	m_sPlayer->FireBulletFlgFalse();
}

void BulletManager::UpdateBulletsEnemy()
{
	//�G���S�e���X�V
	for (auto& b : m_bulletsEnemy) {
		//�ړ�
		b->Move(m_sPlayer->GetPos());
		b->Update();
	}
	
	//�G�F����ł���e���폜����
	bulletDelete(m_bulletsEnemy);
}

void BulletManager::UpdateBulletsPlayer()
{
	//�v���C���[���S�e���X�V
	for (auto& b : m_bulletsPlayer) {

		//�ړ�
		b->Move(m_sPlayer->GetPos());
		b->Update();
	}

	//�v���C���[�F����ł���e���폜����
	bulletDelete(m_bulletsPlayer);
}

void BulletManager::UpdateBullets(std::vector<std::unique_ptr<EnemyObject>>& _enemy)
{
	//�G���S�e���X�V
	for (auto& b : m_bulletsEnemy) {
		b->Move(m_sPlayer->GetPos());
	}
	//*******************************************************
	//�v���C���[���S�e���X�V
	for (auto& b : m_bulletsPlayer) {
		//��ɓ������Ă��瓖���蔻��
		b->Move(m_sPlayer->GetPos());
	}
	//�����蔻������
	Collider::GetInstance()->AllOnCollision(m_sPlayer, _enemy, m_bulletsPlayer, m_bulletsEnemy);

	//�G�F����ł���e���폜����
	bulletDelete(m_bulletsEnemy);
	//�v���C���[�F����ł���e���폜����
	bulletDelete(m_bulletsPlayer);

	//���������Ē���
	/*m_bulletsEnemy.shrink_to_fit();
	m_bulletsPlayer.shrink_to_fit();*/
}

void BulletManager::DrawBullets()
{
	//�G�S�e�`��
	for (auto& b : m_bulletsEnemy) {
		b->Draw();
		//b->GetSphere().Draw();
		

	}

	//�v���C���[�S�e�`��
	for (auto& b : m_bulletsPlayer) {
		b->Draw();
		//b->GetSphere().Draw();

	}
}

void BulletManager::SetCsvStatus(BULLET_TYPE _bulletType, std::vector<float> _vallue)
{
	//�e�̐��l��������
	for (int i = 0; i < static_cast<int>(BULLET_STATUSID::END); i++)
	{
		//�ݒ肷��G�̎��e�̃X�e�[�^�X������
		m_cell.at(static_cast<int>(_bulletType)).at(i) = _vallue[i];
	}
}

void BulletManager::Reset()
{
	//�e���폜
	m_bulletsEnemy.erase(m_bulletsEnemy.begin(), m_bulletsEnemy.end());
	m_bulletsPlayer.erase(m_bulletsPlayer.begin(), m_bulletsPlayer.end());
	////���g�𖳂���
	//m_bulletsEnemy.clear();
	//m_bulletsPlayer.clear();
	////���������Ē���
	//m_bulletsEnemy.shrink_to_fit();
	//m_bulletsPlayer.shrink_to_fit();
}


