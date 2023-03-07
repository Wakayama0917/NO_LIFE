#pragma once
#include "gameobject.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "EnumListPlayer.h"



class Player :public GameObject {
private:
	/*************************************************************
   * CSV�p�ϐ�
   *************************************************************/
	float m_moveSpeed = 0;          //�ړ����x
	float m_maxhp = 0;				//�ő�̗�
	float m_hp = 0;                 //�̗�
	float m_atk = 0;              //�U����
	//bool  m_rotation = false;       //��]���邩
	//float m_maxRot = 0;             //�P�t���[�����̍ő��]�p�x
	int   m_bulletType = 0;         //�e�̎��
	float m_reload;                 //���̒e�����܂ł̎���
	//�e��������
	int   m_shotDirectionFront = 0;         //�O
	int   m_shotDirectionLeft = 0;          //��
	int   m_shotDirectionRight = 0;         //�E
	int   m_shotDirectionBack = 0;          //���
	int   m_shotDirectionFrontLeft = 0;     //���O
	int   m_shotDirectionFrontRight = 0;    //�E�O
	int   m_shotDirectionBackLeft = 0;      //�����
	int   m_shotDirectionBackRight = 0;     //�E���


	/*************************************************************
   * �ϐ�
   *************************************************************/
private:
	PHitEffectFlg m_pHitEffectFlg = PHitEffectFlg::NONE;

	// ���݂̎p����\���N�H�[�^�j�I��
	DirectX::XMFLOAT4 m_Quaternion;
	//�e��ł����肷��p�̃t���O
	bool m_fireBulletFlg;
	//bool m_fireBulletFlg[8];//�e�������ƂɃ����[�h���Ԃ�ς���ꍇ
	// 
	//�����[�h���ԗp�A�f���^�^�C��������
	float m_reloadTime = 0.0f;

	//�X�e�[�W�̕����L������p�̕ϐ�
	float m_stageX = 0;
	float m_stageZ = 0;
public:
	/*************************************************************
   * ��{�I�Ȋ֐�
   *************************************************************/
	
	bool Init();//������CSV�ݒ�Ȃ��̏ꍇ
	bool Init(std::unique_ptr<CSVFile>& _csvFile);
	void Reset();//���̃X�e�[�W�ɍs���ۂ̏�����

	void Draw();

	void DrawWithAxis();

	void Update();

	void Finalize();
	/*************************************************************
   * �ΐ펞�Ɏg�p����֐�
   *************************************************************/
	void FireBulletFlgFalse() {
		//�t���O��܂�A��������Ȃ�
		m_fireBulletFlg = false;
	}
	//�f���^�^�C�������Z���Ĕ��˂��邩�����߂�֐�
	void EnemyBulletReload();
	//�G��e�ɓ����������̊֐��A�������̗͂�����
	//�q�b�g���̉��o�t���O�����ĂĂ���
	void HPDecrease(float _damage);

	/*************************************************************
   * �X�L���l�����Ɏg�p����֐�
   *************************************************************/
	//�񕜂���֐�
	void HPIncrease(float _value);
	//�ő�HPUP
	void UPMAXHP(float _value);
	//�ړ����xUP
	void UPSpeed(float _value);
	// �U����UP
	void UPAttack(float _value);
	//�����[�h���x�A�b�v
	void UPReload(float _value);
	//�O�����e�ǉ��A���x�����󂯎��
	void UPFrontBullet(float _level);
	//�������e�ǉ��A���x�����󂯎��
	void UPBackBullet(float _level);
	
	

	 /*************************************************************
	*�@GET SET�֐�
	*************************************************************/
	//SET
	void SetPos(float _x, float _z);	//�|�W�V�����Z�b�g
	void SetStartPos(float _x, float _z);	//�Q�[���J�n���A�X�e�[�W�̏c�������󂯎��Z�b�g
	void SetAxis(float _x,float _y);	//�������Z�b�g
	void SetAllStatus(float _playerChangeData[static_cast<int>(PLAYER_STATUSID::END)]);
	void SetPHitEffectFlg(PHitEffectFlg _pHitEffectFlg);

	//GET
	//�e�����̐^�U�����
	bool GetShotDirection(int _num);
	//�Q�b�g�n
	int GetBulletType() {
		//�e�̎��
		return m_bulletType;
	}
	bool GetFireBulletFlg()
	{
		//���˂��邩
		return m_fireBulletFlg;
	}
	//CSV�œǂݎ�鐔�l���擾����
	float GetCSVStatus(PLAYER_STATUSID _statusid);
	//�q�b�g���̉��o�t���O���擾
	PHitEffectFlg GetPHitEffectFlg() { return m_pHitEffectFlg; };

private:
   /*************************************************************
   * �֐��ރv���C�x�[�g
   *************************************************************/
   //m_enemyNum������������Ă��Ȃ��ƃ��b�Z�[�W�\�L���Ď~�܂�
	bool PlayerCsvSet(std::unique_ptr<CSVFile>& _csvFile);//CSV�󂯎��

	//void Advance(bool _keyInput,XMFLOAT3 _angle);//�i�ޏ����̕���
	void UpdatePlayerMove(float _LX, float _LY);//�i�ޏ����̕���
	void UpdatePlayerDirection(float _RX, float _RY);
	//�f���^�^�C�������Z���Ĕ��˂��邩�����߂�֐�
	void BulletReload();//���̋��𔭎˂��邩�̊֐�

};