#pragma once
//#include "dx11mathutil.h"
#include "gameobject.h"
#include "Player.h"
#include "EnumListEnemy.h"
#include "CBillBoard.h"

//class CBillBoard;

class EnemyObject : public GameObject
{

protected:

    /*************************************************************
   * �ϐ�
   *************************************************************/
    //���������̓G����ݒ�
    ENEMY_TYPE m_enemyNum = ENEMY_TYPE::END;

    //�v���C���[�̃|�C���^���i�[����悤
    std::shared_ptr<Player> m_sPlayer;

    //�e��ł����肷��p�̃t���O
    bool m_fireBulletFlg = false;
    //bool m_fireBulletFlg[8];//�e�������ƂɃ����[�h���Ԃ�ς���ꍇ
   
    //�����[�h���ԗp�A�f���^�^�C��������
    float m_reloadTime = 0;

    //�X�e�[�W�̕����L������p�̕ϐ�
    float m_stageX = 0;
    float m_stageZ = 0;

    /*************************************************************
     * �G�t�F�N�g�ނɎg���ϐ�
    *************************************************************/
    //�g�p����UI�̃��X�g
    //enum class GAMEUIID
    //{
    //    RING,		    //�n�ʂɏo�郊���O
    //    RINGHALF,		//�΂߃����O
    //    Delete,			//���S���̃G�t�F�N�g

    //    END,			//�Ō�
    //};
    ////�r���{�[�h
    //std::vector<std::unique_ptr<CBillBoard>> m_billBoard;
    //�G�t�F�N�g�̐i�s�J�E���g
    int m_effectCount = 0;
    //�G�t�F�N�g�����ɓ��邩�̃t���O
    bool m_effectFlg = 0;
    //��Ƃ��Ď����Ă����AUI�̕`������ɗ��p
    XMFLOAT4X4 m_mtxEffect;
    //�A�j���[�V�����̍ۂɉ����ڂ��g�����̕ϐ�
    int m_imageNum = 0;
    

    /*************************************************************
    * CSV����󂯎��ϐ�
    *************************************************************/
    float m_moveSpeed = 0;          //�ړ����x
    float m_hp = 0;                 //�̗�
    float m_maxHP;                  //�G�̍ő�HP
    float m_atk = 0;                //�U����
    //float m_atkSpeed = 0;         //�U�����x
    //float m_atk = 0;              //�U����
    bool  m_rotation = false;       //��]���邩
    float m_maxRot = 0;             //�P�t���[�����̍ő��]�p�x
    int   m_bulletType = 0;         //�e�̎��
    float m_reload = 0;             //���̒e�����܂ł̎���
    //�e��������
    int   m_shotDirectionFront = 0;         //�O
    int   m_shotDirectionLeft = 0;          //��
    int   m_shotDirectionRight = 0;         //�E
    int   m_shotDirectionBack = 0;          //���
    int   m_shotDirectionFrontLeft = 0;     //���O
    int   m_shotDirectionFrontRight = 0;    //�E�O
    int   m_shotDirectionBackLeft = 0;      //�����
    int   m_shotDirectionBackRight = 0;     //�E���

public:
    /*************************************************************
    * �֐���
    *************************************************************/
    //m_enemyNum������������Ă��Ȃ��ƃ��b�Z�[�W�\�L���Ď~�܂�
    bool EnemyCsvSet(CSVFile _csvFile);//CSV�󂯎��
    void FireBulletFlgFalse() {
        //�t���O��܂�A��������Ȃ�
        m_fireBulletFlg = false;
    }
   
    //�����[�h���Ԃ��Z�b�g�A�����[�h���Ԃ�CSV����Z�b�g���ꂽ�ザ��Ȃ��ƂO�ɂȂ�B
    void ReloadInit();

    //�_���[�W���󂯂�֐�
    void HPDecrease(float _damage);
    //�񕜂���֐�
    //void HPIncrease(float _damage);

    /*************************************************************
    * �G�t�F�N�g�n�֐�
    *************************************************************/
    //���Z�b�g�A�q�b�g���ɍs���B
    void EffectReset();
    //�`��
    void EffectDraw();

    /*************************************************************
    * SET�AGET�֐�
    *************************************************************/
    //�Z�b�g�n
    //�|�W�V�����Z�b�g
    void SetPos(float _x, float _z);	
    //�Q�[���J�n���A�X�e�[�W�̏c�������󂯎��Z�b�g
    void SetStageSize(float _x, float _z);	
    //CSV�n�X�e�[�^�X�ꊇ�ύX�x�N�^�[�^
    void SetCsvStatus(std::vector<float> _value);
   
    //�Q�b�g�n
    //�e�̎��
    int GetBulletType() {
        return m_bulletType;
    }
    //���˂��邩
    bool GetFireBulletFlg()
    {
        return m_fireBulletFlg;
    }
    //CSV�œǂݎ�鐔�l���擾����
    float GetCSVStatus(ENEMY_STATUSID _statusid);

    //�e�����̐^�U�����
    bool GetShotDirection(int _num);

    /*************************************************************
    * �o�[�`�����֐�
    *************************************************************/
    virtual bool Update(const DirectX::XMFLOAT3 _TargetPosition, float _l) { return true; };
    virtual bool Init(std::unique_ptr<CSVFile>& _csvFile, std::unique_ptr<Player>& _player) { return true; };   //������
    virtual void Update() = 0;                                  //�X�V
    virtual void Draw();                                    //�`��
    virtual void DrawWithAxis() {};                             //XYZ���܂߂ĕ`��
    virtual void Finalize() = 0;                                //�I������

protected:
    /*************************************************************
   * �v���C�x�[�g�֐�
   *************************************************************/
    virtual void EnemyMove() = 0;//�e�L�����̈ړ�������Ƃ��̊֐�
    //�f���^�^�C�������Z���Ĕ��˂��邩�����߂�֐�
    void BulletReload();//���̋��𔭎˂��邩�̊֐�
};


