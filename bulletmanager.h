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
	* �ϐ�
	*************************************************************/
protected:
	//�v���C���[�̃|�C���^���i�[����悤
	std::shared_ptr<Player> m_sPlayer;
	
	//void UpdateBullets();
	std::vector<std::unique_ptr<BulletObject>>	m_bulletsEnemy;//�e
	std::vector<std::unique_ptr<BulletObject>>	m_bulletsPlayer;//�e
	//����Ȃ��Ȃ�
	//std::vector<std::unique_ptr<NormalBullet>>	m_normalBullets;//�e
	//std::vector<std::unique_ptr<HomingMissile>>	m_HommingMissile;//�z�[�~���O

	//���f���̃p�X���ۑ��p���X�g
	string m_modelNameList[static_cast<int>(BULLET_TYPE::END)];
	//�e�̃X�e�[�^�X�ύX���Ɏg�p����
	float m_bulletChangeData[static_cast<int>(BULLET_STATUSID::END)];
	// �v�f�ۊǁACSV�̃f�[�^���ꊇ�ŕۑ�
	std::vector<std::vector<float>> m_cell;
private:
	bool m_hitFlg = false;
	
public:
	/*************************************************************
	* �֐�
	*************************************************************/
	//�������A��������L����G�̏���e�̏����i�[
	void InitEditor(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player, int _bulletType);
	//�������A�f�[�^��S�ĕۑ�
	void Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);
	//�����Œe�𔭎˂�����A�G
	void FireBulletEnemy(unique_ptr<EnemyObject>& _enemyObject);
	//�����Œe�𔭎˂�����A�v���C���[
	void FireBulletPlayer();
	//�G�̒e�X�V
	void UpdateBulletsEnemy();
	//�v���C���[�̒e
	void UpdateBulletsPlayer();
	//�z����S�Ă̒e�̍X�V�A�v���C���[���ǉ���
	void UpdateBullets(std::vector<std::unique_ptr<EnemyObject>>& _enemy);
	//�ʏ�e�`��
	void DrawBullets();
	//CSV�X�e�[�^�X���Z�b�g
	void SetCsvStatus(BULLET_TYPE _bulletType, std::vector<float> _vallue);
	//�v�f���X�V
	void SetData(BULLET_STATUSID _id,float _value) {
		m_bulletChangeData[static_cast<int>(_id)] = _value;	
	}
	void Reset();//���g��������


	/*************************************************************
	* �ʏ�e�֐�
	*************************************************************/
	//�e���ˁA������
	void FireBullet(unique_ptr<EnemyObject>& _enemyObject,ENEMY_SHOT_DIRECTION _num);//�G
	void FireBullet(PLAYER_SHOT_DIRECTION _num);//�v���C���[
	void FireBulletEditor(unique_ptr<EnemyObject>& _enemyObject,PLAYER_SHOT_DIRECTION _num);//�v���C���[

	/*************************************************************
	* �z�[�~���O�e�֐�
	*************************************************************/
	//�~�T�C�����ˁA������
	void FireHommingMissile(unique_ptr<EnemyObject>& _enemyObject, ENEMY_SHOT_DIRECTION _num);//�v���C���[�Ɍ�����

private:
	void bulletDelete(std::vector<std::unique_ptr<BulletObject>>& _bullets);
	
	
};



