#include "Input.h"

void Input::Init(HINSTANCE hInst, HWND hwnd, int width, int height)
{
	if (CheckControllerConnection())
		Input::ControllerInit();
	Input::KeyBoardInit(hInst, hwnd, width, height);
}

bool Input::KeyBoardInit(HINSTANCE hInst, HWND hwnd, int width, int height)
{
	HRESULT	hr;
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);
	if (FAILED(hr))
		return false;
	// �L�[�{�[�h�f�o�C�X����
	m_dinput->CreateDevice(GUID_SysKeyboard, &m_dikeyboard, NULL);
	if (FAILED(hr))
		return false;
	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = m_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false;
	// �������x���̐ݒ�
	hr = m_dikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;
	// �}�E�X�f�o�C�X����
	m_dinput->CreateDevice(GUID_SysMouse, &m_dimouse, NULL);
	if (FAILED(hr))
		return false;
	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = m_dimouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
		return false;
	// �������x���̐ݒ�
	hr = m_dimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;
	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;							// ���Βl���[�h
	m_dimouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// �����[�h�̐ݒ�

	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = DIJOFS_X;
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = 0;
	diprg.lMax = width - 1;

	m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// �w�����͈̔͂��w��
	diprg.diph.dwObj = DIJOFS_Y;
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = 0;
	diprg.lMax = height - 1;
	m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// �x�����͈̔͂��w��

	m_hwnd = hwnd;

	m_height = height;
	m_width = width;

	return true;
}

bool Input::ControllerInit()
{
	//�f�b�h�]�[��
	if ((x_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		x_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(x_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			x_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		x_state.Gamepad.sThumbLX = 0;
		x_state.Gamepad.sThumbLY = 0;
	}
	x_vibration.wLeftMotorSpeed = 0;
	x_vibration.wRightMotorSpeed = 0;
	return true;
}

void Input::Update()
{
	ControllerState();
	VibrationUpdate();
	
	Input::KeyBoardState();
	Input::MouseState();
}

void Input::VibrationUpdate()
{
	if (x_vibstate==Vibration::SOON) 
	{
		XInputSetState(0, &x_vibration);
	}
	else if(x_vibstate==Vibration::GRADUALLY)
	{
		static _XINPUT_VIBRATION x_vib;
		x_vib.wLeftMotorSpeed += x_vibration.wLeftMotorSpeed / 60;
		x_vib.wRightMotorSpeed += x_vibration.wRightMotorSpeed / 60;
		//�C��
		if (x_vib.wLeftMotorSpeed > x_vibration.wLeftMotorSpeed)
			x_vib.wLeftMotorSpeed = x_vibration.wLeftMotorSpeed;
		if (x_vib.wRightMotorSpeed > x_vibration.wRightMotorSpeed)
			x_vib.wRightMotorSpeed = x_vibration.wRightMotorSpeed;
		XInputSetState(0, &x_vib);
	}
	else if(x_vibstate==Vibration::WAVE_UP)
	{
		static _XINPUT_VIBRATION x_vib;
		x_vib.wLeftMotorSpeed += x_vibration.wLeftMotorSpeed / 60;
		x_vib.wRightMotorSpeed += x_vibration.wRightMotorSpeed / 60;
		//�C��
		if (x_vib.wLeftMotorSpeed > x_vibration.wLeftMotorSpeed)
			x_vib.wLeftMotorSpeed = x_vibration.wLeftMotorSpeed;
		if (x_vib.wRightMotorSpeed > x_vibration.wRightMotorSpeed)
			x_vib.wRightMotorSpeed = x_vibration.wRightMotorSpeed;
		XInputSetState(0, &x_vib);
		//���E�l���B
		if (x_vib.wLeftMotorSpeed == x_vibration.wLeftMotorSpeed && x_vib.wRightMotorSpeed == x_vibration.wRightMotorSpeed)
			x_vibstate = Vibration::WAVE_DOWN;
	}
	else if (x_vibstate==Vibration::WAVE_DOWN) 
	{
		static _XINPUT_VIBRATION x_vib;
		x_vib.wLeftMotorSpeed -= x_vibration.wLeftMotorSpeed / 60;
		x_vib.wRightMotorSpeed -= x_vibration.wRightMotorSpeed / 60;
		//�C��
		if (x_vib.wLeftMotorSpeed < 0)
			x_vib.wLeftMotorSpeed = 0;
		if (x_vib.wRightMotorSpeed < 0)
			x_vib.wRightMotorSpeed = 0;
		XInputSetState(0, &x_vib);
		//���E�l���B
		if (x_vib.wLeftMotorSpeed == 0 && x_vib.wRightMotorSpeed == 0)
			x_vibstate = Vibration::NONE;
	}
	else if(x_vibstate==Vibration::NONE)	//�U���Ȃ�
	{
		x_vibration.wLeftMotorSpeed = 0;
		x_vibration.wRightMotorSpeed = 0;
		XInputSetState(0, &x_vibration);
	}
}

bool Input::CheckControllerConnection()
{
	ZeroMemory(&x_state, sizeof(x_state));
	DWORD state = XInputGetState(0,&x_state);
	if (state == ERROR_SUCCESS)
		return true;	//�ڑ��m�F
	else
		return false;	//�ڑ��Ȃ�
}

void Input::ControllerState()
{
	x_buffer = x_state;//���g�ۑ�
	ZeroMemory(&x_state, sizeof(x_state));//���g���������鏈��
	DWORD state = XInputGetState(0, &x_state);//���g�m�F�p�i���A���̃R���g���[���[�̐��l�j
}

XMFLOAT2 Input::GetRStick()
{
	return { (float)x_state.Gamepad.sThumbRX, (float)x_state.Gamepad.sThumbRY};
}

XMFLOAT2 Input::GetRStickPlayer()
{
	//�v���C���[����p�ɐ�������
	XMFLOAT2 RS;
	RS.x = (float)x_state.Gamepad.sThumbRX;
	RS.y = (float)x_state.Gamepad.sThumbRY;

	if ((RS.x > ThresholdR) || (RS.x < -ThresholdR))
	{
		RS.x = RS.x / ((double)65535 / 10);
	}
	else RS.x = 0;

	if ((RS.y > ThresholdR) || (RS.y < -ThresholdR))
	{
		RS.y = RS.y / ((double)65535 / 10);
	}
	else RS.y = 0;

	return { RS.x, RS.y };
}

XMFLOAT2 Input::GetLStick()
{
	
	return { (float)x_state.Gamepad.sThumbLX, (float)x_state.Gamepad.sThumbLY };
}

XMFLOAT2 Input::GetLStickPlayer()
{
	//�v���C���[����p�ɐ�������
	XMFLOAT2 LS;
	LS.x = x_state.Gamepad.sThumbLX;
	LS.y = x_state.Gamepad.sThumbLY;

	if ((LS.x > ThresholdL) || (LS.x < -ThresholdL))
	{
		LS.x = LS.x / ((double)65535 / 2);
	}
	else LS.x = 0;

	if ((LS.y > ThresholdL) || (LS.y < -ThresholdL))
	{
		LS.y = LS.y / ((double)65535 / 2);
	}
	else LS.y = 0;

	return { LS.x, LS.y };
}

float Input::GetRTrigger()
{
	return x_state.Gamepad.bRightTrigger;
}

float Input::GetLTrigger()
{
	return x_state.Gamepad.bLeftTrigger;
}

bool Input::CheckButtonPress(WORD button)
{
	if (x_state.Gamepad.wButtons & button)
		return true;
	else
		return false;
}

bool Input::CheckButtonTrigger(WORD button)
{
	if (x_state.Gamepad.wButtons & button &~x_buffer.Gamepad.wButtons)
		return true;
	else
		return false;

	
}

bool Input::CheckButtonRelease(WORD button)
{
	if (~x_state.Gamepad.wButtons & button & x_buffer.Gamepad.wButtons)
		return true;
	else
		return false;
}

bool Input::CheckButtonAll()
{
	//�P�t���[���O�ƍ��̋L�����Ă����񂪈Ⴆ�ΐ^��Ԃ�
	if (x_state.Gamepad.wButtons != x_buffer.Gamepad.wButtons)
		return true;
	else
		return false;
}

void Input::SetVibration(float left, float right, Vibration vib)
{
	x_vibstate = vib;
	x_vibration.wLeftMotorSpeed = left;
	x_vibration.wRightMotorSpeed = right;
}

void Input::StopVibration()
{
	x_vibstate = Vibration::NONE;
}

void Input::MouseState()
{
	HRESULT	hr;

	DIMOUSESTATE2		mouseStateOld = m_MouseState;

	GetCursorPos(&m_MousePoint);
	ScreenToClient(m_hwnd, &m_MousePoint);
	
	// �f�o�C�X�̔F��
	hr = m_dimouse->Acquire();

	hr = m_dimouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	
	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < 8; cnt++)
		{
			m_MouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ m_MouseState.rgbButtons[cnt]) & m_MouseState.rgbButtons[cnt]);
			m_MouseStateRelease.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ m_MouseState.rgbButtons[cnt]) & mouseStateOld.rgbButtons[cnt]);
		}
	}
	else {
		if (hr == DIERR_INPUTLOST) {
			// �f�o�C�X�̔F��
			hr = m_dimouse->Acquire();
		}
	}
}

XMINT2 Input::GetMousePos() const
{
	return {m_MousePoint.x,m_MousePoint.y};
}

bool Input::CheckMouseLButtonPress() const
{
	if (m_MouseState.rgbButtons[0] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseRButtonPress() const
{
	if (m_MouseState.rgbButtons[1] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseCButtonPress() const
{
	if (m_MouseState.rgbButtons[2] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseLButtonTrigger() const
{
	if (m_MouseStateTrigger.rgbButtons[0] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseRButtonTrigger() const
{
	if (m_MouseStateTrigger.rgbButtons[1] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseCButtonTrigger() const
{
	if (m_MouseStateTrigger.rgbButtons[2] & 0x80) {
		return true;
	}
	else
		return false;
}

bool Input::CheckMouseLButtonRelease() const
{
	if (m_MouseStateRelease.rgbButtons[1] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseRButtonRelease() const
{
	if (m_MouseStateRelease.rgbButtons[2] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckMouseCButtonRelease() const
{
	if (m_MouseStateRelease.rgbButtons[3] & 0x80)
		return true;
	else
		return false;
}

int Input::GetMouseWheel()
{
	return m_MouseState.lZ;
}

void Input::KeyBoardState()
{
	HRESULT	hr;
	// �f�o�C�X�̔F��
	hr = m_dikeyboard->Acquire();
	// �O��̏�Ԃ�ۑ�
	memcpy(&m_oldkeybuffer, m_keybuffer, sizeof(m_keybuffer));
	hr = m_dikeyboard->GetDeviceState(sizeof(m_keybuffer), (LPVOID)&m_keybuffer);
	if (hr == DIERR_INPUTLOST) {
		// �f�o�C�X�̔F��
		hr = m_dikeyboard->Acquire();
	}
}

bool Input::CheckKeyPress(int keynum)
{
	if (m_keybuffer[keynum] & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckKeyTrigger(int keynum)
{
	if (((m_oldkeybuffer[keynum]^ m_keybuffer[keynum]) & m_keybuffer[keynum]) & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckKeyRelease(int keynum)
{
	if (((m_oldkeybuffer[keynum] ^ m_keybuffer[keynum]) & m_oldkeybuffer[keynum]) & 0x80)
		return true;
	else
		return false;
}

bool Input::CheckKeyAll()
{
	if (m_keybuffer[256] != m_oldkeybuffer[256])
		return true;
	else
		return false;
}
