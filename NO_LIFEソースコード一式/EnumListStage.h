#pragma once

//�X�e�[�W�f�[�^�̍���
enum class STAGE_STATUSID
{
    STAGE_MODEL_NUM,        //�X�e�[�W���f���i���o�[
    SKY_MODEL_NUM,          //�X�J�C���f���i���o�[
    STAGE_X,                //�X�e�[�WX��
    STAGE_Z,                //�X�e�[�WZ��
    ENEMY_00,               //ENEMY_TYPE�O�Ԗ�
    ENEMY_01,               //ENEMY_TYPE1�Ԗ�
    ENEMY_02,               //ENEMY_TYPE2�Ԗ�
    ENEMY_BOSS1,               //ENEMY_TYPE2�Ԗ�
    
    END,                    //�Ō�̐��l
};

//��ƃX�J�C�h�[���̃i���o�[
enum class MODEL_SS
{
	STAGE_NORMAL1,
	STAGE_BOSS1,
	SKY_NORMAL1,
	SKY_BOSS1,
};



//�X�e�[�W�̍ő吔�A�O����̃J�E���g�ɂȂ��Ă���
#define MAXSTAGE 9
