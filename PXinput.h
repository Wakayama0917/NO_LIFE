#pragma once
#include <Windows.h>
//#include <windef.h>
#pragma comment (lib, "xinput.lib")
#include <xinput.h>



//struct XKEY_STATE
//{
//	bool XDIK_A;
//	bool XDIK_B;
//	bool XDIK_X;
//	bool XDIK_Y;
//	bool XDIK_UP;//�\���L�[��
//	bool XDIK_DOWN;//�\���L�[��
//	bool XDIK_LEFT;//�\���L�[��
//	bool XDIK_RIGHT;//�\���L�[�E
//	bool XDIK_START;//�X�^�[�g
//	bool XDIK_BACK;//�I�v�V����
//	bool XDIK_LEFT_SHOULDER;//LB
//	bool XDIK_RIGHT_SHOULDER;//RB
//	bool XDIK_LEFT_THUMBT;//L3
//	bool XDIK_RIGHT_THUMB;//R3
//	
//};

class PXinput
{
private:


	//XKEY_STATE KEY;//�e�{�^���̏�Ԃ������Ă�z
	struct XKEY
	{
		bool NOW_KEY[20];
		bool OLD_KEY[20];
		float LX, LY;//���X�e�B�b�N�̐��l
		float RX, RY;//�E�X�e�B�b�N�̐��l
	};
	XKEY m_xkeySts;
public:
	enum XKEY_NUM
	{
		XDIK_A,
		XDIK_B,
		XDIK_X,
		XDIK_Y,
		XDIK_UP,//�\���L�[��
		XDIK_DOWN,//�\���L�[��
		XDIK_LEFT,//�\���L�[��
		XDIK_RIGHT,//�\���L�[�E
		XDIK_START,//�X�^�[�g
		XDIK_BACK,//�I�v�V����
		XDIK_LEFT_SHOULDER,//LB
		XDIK_RIGHT_SHOULDER,//RB
		XDIK_LEFT_THUMBT,//L3
		XDIK_RIGHT_THUMB,//R3
		XDIK_END//�Ō�̔ԍ��Afor���p
	};
	
	
	void idle();//�{�^�����͂��X�V

	//----------------------------------
	// �L�[��������Ă��邩�ǂ������`�F�b�N����
	//		keyno :�@�`�F�b�N�������L�[�ԍ�
	//	�߂�l
	//		true : �w�肳�ꂽ�L�[��������Ă���
	//----------------------------------
	bool Trigger(bool keyno);//�����ꂽ�u��
	bool Release(bool keyno);//�������u��
	bool Check(bool keyno);//������Ă��邩���f�A�������p


	XKEY XKEYreturn(){//���l������
		return m_xkeySts;
	}

	//XINPUT_STATE

private:
#define MAX_CONTROLLERS  4  //XInput���F���ł���̂�4�܂�
#define Threshold   65535/4 //�������l�A�v���X�ƃ}�C�i�X�̕��𑫂������̂��S�Ŋ����Ă���


	//���Ԗڂ̃R���g���[���[�����f����悤
	struct CONTROLER_STATE{
		XINPUT_STATE state;
		bool bConnected;
	};
	CONTROLER_STATE m_padNum[MAX_CONTROLLERS];//�R���g���[���[�̑䐔�𔻕�
	
	
	HRESULT UpdateControllerState();//�ڑ��𒲂ׂ�
	
};

