#pragma once

//�G�l�~�[�̃i���o�����O
enum class ENEMY_TYPE
{
    ROTATION,     //��]����G
    LOOP,       //���񂷂�G
    HOMMING,    //�ǔ�����G
    BOSS1,       //�{�X�P

    END,        //�Ō�A�G���[�m�F�p
};

//�G�l�~�[�f�[�^�̍���
enum class ENEMY_STATUSID
{
    //MODEL_NAME,         //���f���t�@�C���p�X
    //TEX_FOLDER,         //�e�N�X�`���p�X
    MODEL_NUM,              //���f���i���o�[
    MOVE_SPEED,             //�ړ����x
    HP,                     //�̗�
    MAX_HP,                 //�ő�̗�
    ATTACK,                 //�U����
    ROTATION,               //��]���邩
    MAX_ROT,                //�P�t���[�����̍ő��]�p�x
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
    
    END,                //�Ō�̐��l
};

//�G�l�~�[�̒e�����̎��ɂ����g�����X�g
enum class ENEMY_SHOT_DIRECTION
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
