#pragma once

//�G�l�~�[�̃i���o�����O
enum class BULLET_TYPE
{
    NORMAL,     //�G�ʏ�e
    HARD,       //�j��ł��Ȃ��ʏ�e
    HOMMING,    //�ǔ��e
    NONE,       //��������
    P_NORMAL,   //�v���C���[�̃m�[�}���e

    END,        //�Ō�A�G���[�m�F�p
};

//�G�l�~�[�f�[�^�̍���
enum class BULLET_STATUSID
{
    MODEL_NUM,          //���f���i���o�[
    ATTACK_SPEED,       //�U�����x
    ATTACK_POWER,       //�U����
    LIVE_TIME,          //��������
    MAX_ROT,            //�P�t���[�����̍ő��]�p�x
    SIZE,               //�g�嗦
    RADIUS,             //���a�A�����蔻��p

    END,                //�Ō�̐��l
};


