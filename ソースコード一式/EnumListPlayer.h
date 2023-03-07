#pragma once

//�v���C���[�f�[�^�̍���
enum class PLAYER_STATUSID
{
    MODEL_NUM,              //���f���i���o�[
    MOVE_SPEED,             //�ړ����x
    MAXHP,                  //�ő�̗�
    HP,                     //�̗�
    ATTACK,                 //�U����
    BULLET_TYPE,            //�e�̎��
    RELOAD,                 //���̒e�����܂ł̎���
    DIRECTION_FRONT,        //�e��������
    DIRECTION_LEFT,         //�e��������
    DIRECTION_RIGHT,        //�e��������
    DIRECTION_BACK,         //�e��������
    DIRECTION_FRONT_LEFT,   //���O
    DIRECTION_FRONT_RIGHT,  //�E�O
    DIRECTION_BACK_LEFT,    //�����
    DIRECTION_BACK_RIGHT,   //�E���

    END,                    //�Ō�̐��l
};

//�v���C���[�̒e�����̎��ɂ����g�����X�g
enum class PLAYER_SHOT_DIRECTION
{
    DIRECTION_FRONT,     //�e��������
    DIRECTION_LEFT,     //�e��������
    DIRECTION_RIGHT,     //�e��������
    DIRECTION_BACK,     //�e��������
    DIRECTION_FRONT_LEFT,   //���O
    DIRECTION_FRONT_RIGHT,  //�E�O
    DIRECTION_BACK_LEFT,    //�����
    DIRECTION_BACK_RIGHT,   //�E���

    END,                //�Ō�̐��l
};

//�q�b�g���̉��o�p�t���O
enum class PHitEffectFlg
{
    NONE,		    //��������
    HIT,			//�G�̍U��������������
    DEATH,			//���S�����Ƃ�

    END,			//�������p
};