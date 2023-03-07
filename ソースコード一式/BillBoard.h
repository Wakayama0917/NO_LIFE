#pragma once
#include "CBillBoard.h"

class BillBoard
{
public:
	//�V���O���g��
	static BillBoard* GetInstance() {
		static BillBoard instance;
		return &instance;
	}

private:
	//�G�G�t�F�N�g
	std::vector<std::unique_ptr<CBillBoard>> m_bEnemyEffect;
	//�GUI(HP�o�[�Ȃ�)
	std::vector<std::unique_ptr<CBillBoard>> m_bEnemyUI;
	//�p�x���w�肷�邽�߂̃}�g���b�N�X
	XMFLOAT4X4 m_mtxEffect;

public:
	
	//�G�̃G�t�F�N�g�Ɏg���摜
	enum class BEnemyEffectID
	{
		RING,		    //�n�ʂɏo�郊���O
		RINGHALF,		//�΂߃����O
		DEATH,			//���S���̃G�t�F�N�g
	
		END,			//�Ō�
	};

	//�G�Ɏg���G�t�F�N�g�ȊO�̉摜
	enum class BEnemyUIID
	{
		HPBAR,			//HP�o�[

		END,			//�Ō�
	};
	
public:
	/*************************************************************
	* �S�Ăɋ��ʂ��Ďg���֐�
	*************************************************************/
	//�S�Ă������ǂݍ��݁A��x�̂�
	void Init();
	//�폜
	void Finalize();

public:
	/*************************************************************
	* �G�̃G�t�F�N�g�Ɏg���֐��AEE�̓G�l�~�[�G�t�F�N�g�̗�
	*************************************************************/
	//���Z�b�g�A�q�b�g���ɍs���B
	void EEReset();
	//�q�b�g���̃G�t�F�N�g
	void EEHit(XMFLOAT3 _pos);
	//���S���̃G�t�F�N�g
	void EEDeath(int& _imageNum, int _count, XMFLOAT3 _pos);
	
	/*************************************************************
	* �G��UI�n
	*************************************************************/
	//�G��HP�o�[�̕\��
	void EHPBer(int _hp, int _maxhp, XMFLOAT3 _pos);

};

