#pragma once

//�X�L���̍���
enum class SKILL_STATUSID
{
	UP_VALUE,		//�㏸������l
	LEVEL,			//���݃��x��
	LEVELMAX,		//�ő僌�x��
	RATE,			//���I�m��
			
	END,			//�Ō�̍���
};

//�X�L����ʂŎg���I�u�W�F�N�g�̃��X�g
enum class SKILLTYPEID
{
	BACKGROUND,		//�w�i
	FLAME,			//�I�����Ă��鍀�ڂ�\���t���[��
	CHARACTER1,		//�L�����N�^�[�P

	MAXHPUP_LITTLE,	//�͂��ɍő�̗͂��㏸�A�S����������̊O��X�L��

	ATKUP,			//�U����UP
	HEEL,			//��
	MAXHPUP,		//�ő�̗�UP
	SPEEDUP,		//�ړ����xUP
	RELOADUP,		//���̒e���˂܂ł̊��o��UP
	BULLETFRONT,	//�O�����̒e��ǉ�
	BULLETBACK,		//�������̒e��ǉ�


	END,			//�Ō�
};
