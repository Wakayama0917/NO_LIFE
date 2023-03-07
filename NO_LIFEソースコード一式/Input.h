//------------------------------------------------------------------
//! @file  Input.h
//! @brief �L�[�{�[�h�E�}�E�X�ƃR���g���[���[�̓��͏���
//! @autor Noriyuki Iwamoto
//! @date  7/1�`
//------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <dinput.h>
#include <wrl/client.h>
#include "CWindow.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"xinput.lib")
#pragma comment (lib,"dxguid.lib")
#pragma warning(disable : 4995)

using Microsoft::WRL::ComPtr;

/// <summary>
/// ���̓N���X
/// </summary>
class Input {
public:
	enum class Vibration {
		NONE,		//�U���Ȃ�
		SOON,		//����
		GRADUALLY,  //���X��
		WAVE_UP,	//�g�ł�(�㏸)
		WAVE_DOWN,	//�g�ł�(���~)
	};
private:
	/*************************************************************
	* �R���g���[���[�p
	*************************************************************/
	_XINPUT_STATE		 x_state;						//���̏��
	WORD			     x_statebuffer;					//���ݎg�p���Ă��炸
	_XINPUT_STATE		 x_buffer;						//�ߋ��̏��(1�t���[���O)
	Vibration			 x_vibstate;					//���̐U���X�e�[�^�X
	_XINPUT_VIBRATION	 x_vibration;					//�U��
	float ThresholdL = 65535 / 4; //�������l�A�v���X�ƃ}�C�i�X�̕��𑫂������̂��S�Ŋ����Ă���
	float ThresholdR = 15000; //�������l�A������������悤

	/*************************************************************
	* �L�[�{�[�h�E�}�E�X�p
	*************************************************************/
	ComPtr<IDirectInput8>		m_dinput;
	ComPtr<IDirectInputDevice8>	m_dikeyboard;
	ComPtr<IDirectInputDevice8>	m_dimouse;
	char					m_keybuffer[256];		// �L�[�{�[�h�o�b�t�@
	char					m_oldkeybuffer[256];	// �O��̓��̓L�[�{�[�h�o�b�t�@
	DIMOUSESTATE2			m_MouseState;			// �}�E�X�̏��
	DIMOUSESTATE2			m_MouseBuffer;			// �ߋ��̃}�E�X�̏��
	DIMOUSESTATE2			m_MouseStateTrigger;	// �}�E�X�̏��(Trigger)
	DIMOUSESTATE2			m_MouseStateRelease;	// �}�E�X�̏��(Release)
	POINT					m_MousePoint;			// �}�E�X���W
	int						m_width;				// �}�E�X�̂w���W�ő�
	int						m_height;				// �}�E�X�̂x���W�ő�
	HWND					m_hwnd;
	Input() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr) {}
public:
	Input(const Input&) = delete;//���g����
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;//���g����
	Input& operator=(Input&&) = delete;
	static Input& GetInstance() {
		static Input Instance;
		return Instance;
	}
	~Input() { XInputEnable(false); }

	/*************************************************************
	* ����
	*************************************************************/
	//���������A�Q�𓝍�
	void Init(HINSTANCE, HWND, int, int);		
	//�L�[�{�[�h�E�}�E�X������
	bool KeyBoardInit(HINSTANCE, HWND, int, int);	
	//�R���g���[���[������
	bool ControllerInit();	
	//�X�V����
	void Update();				

	/*************************************************************
	* �R���g���[���[�p
	*************************************************************/
	//�R���g���[���[�ڑ��m�F
	bool CheckControllerConnection();	
	//�R���g���[���[��ԍX�V
	void ControllerState();
	//�E�X�e�B�b�N�擾
	XMFLOAT2 GetRStick();
	//�E�X�e�B�b�N�擾
	XMFLOAT2 GetRStickPlayer();
	//���X�e�B�b�N�擾
	XMFLOAT2 GetLStick();
	//���X�e�B�b�N�擾
	XMFLOAT2 GetLStickPlayer();
	//�E�g���K�[���擾
	float GetRTrigger();						
	//���g���K�[���擾
	float GetLTrigger();							
	//�R���g���[���[���͉���(�p��)
	bool CheckButtonPress(WORD);						
	//�R���g���[���[���͉���(�u��)
	bool CheckButtonTrigger(WORD);						
	//�R���g���[���[���͉���(�u��)
	bool CheckButtonRelease(WORD);
	//�ǂꂩ�̃{�^���������ꂽ��^��Ԃ�����
	bool CheckButtonAll();
	//�U���X�V����
	void VibrationUpdate();
	//�U���J�n
	void SetVibration(float,float,Vibration);		
	//�U����~
	void StopVibration();

	/*************************************************************
	* �L�[�{�[�h�E�}�E�X�p
	*************************************************************/
	//�}�E�X��Ԏ擾
	void MouseState();
	//�}�E�X���W�擾
	XMINT2 GetMousePos()const;
	//�}�E�X�z�C�[���擾
	int GetMouseWheel();
	//�}�E�X������(�p��)
	bool CheckMouseLButtonPress()const;
	//�}�E�X�E����(�p��)
	bool CheckMouseRButtonPress()const;		
	//�}�E�X��������(�p��)
	bool CheckMouseCButtonPress()const;	
	//�}�E�X������(�u��)
	bool CheckMouseLButtonTrigger()const;	
	//�}�E�X�E����(�u��)
	bool CheckMouseRButtonTrigger()const;		
	//�}�E�X��������(�u��)
	bool CheckMouseCButtonTrigger()const;	
	//�}�E�X������(�u��)
	bool CheckMouseLButtonRelease()const;		
	//�}�E�X�E����(�u��)
	bool CheckMouseRButtonRelease()const;
	//�}�E�X��������(�u��)
	bool CheckMouseCButtonRelease()const;
	//�L�[�{�[�h�X�V
	void KeyBoardState();
	//�L�[����(�p��)
	bool CheckKeyPress(int);
	//�L�[����(�u��)
	bool CheckKeyTrigger(int);				
	//�L�[����(�u��)
	bool CheckKeyRelease(int);
	//�ǂꂩ�̃L�[�������ꂽ��^��Ԃ�����
	bool CheckKeyAll();
};