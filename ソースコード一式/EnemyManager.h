#pragma once
#include "EnemyRotation.h"
#include "EnemyHoming.h"
#include "EnemyLoop.h"


// �O���錾
class BulletManager;

class EnemyManager
{
private:

	std::vector<std::unique_ptr<EnemyObject>>	m_Enemy;//�G���܂Ƃ߂ē����

	std::mt19937 m_random;//���������p�G�𐶐�
	std::mt19937 m_X;//���������pX��
	std::mt19937 m_Z;//���������pY��

public:
	/*************************************************************
	* �֐�
	*************************************************************/
	//�����������A���f���p�X�̃x�N�^�[���󂯎��ꊇ�ŏ���������
	void InitEditor(std::unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);//�G�f�B�^�[�p�A�S�Ă̓G��1�̂���
	void InitRandom(std::vector<ENEMY_TYPE> _enemyList,unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player);//�G�̎�ނ̔z����󂯎�萶��
	void Update(std::unique_ptr<BulletManager>& _bulletManager);
	void DrawAll();
	//���񂾓G�̍폜
	void EnemyDelete();
	//�S�Ă̓G������ł��邩���m�F
	bool AllDead();
	//�z������S������
	void Reset();
	/*void InitAllOne(std::vector<std::string> _modelName);*/
	//�G�f�B�^�[�V�[���p
	void SetPosCenter(float _X, float _Z);//���S�ɓG���ړ�
	void DrawOneEditor(int _num);//�I�΂ꂽ�L������`��
	void UpdateEditor(int _num, std::unique_ptr<BulletManager>& _bulletManager);
private:
	//�e�L�����̏�����
	void InitRotation(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitLoop(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitHoming(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	void InitBoss1(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player>& _player);
	
public:
	/*************************************************************
	* SET�AGET�n�@�֐�
	*************************************************************/
	std::vector<std::unique_ptr<EnemyObject>>& GetEnemyList() {
		return m_Enemy;
	}

	void SetAllPos(float X,float Z);//�X�e�[�W�̃T�C�Y���󂯎��S�Ă̓G�̈ʒu��������
};

