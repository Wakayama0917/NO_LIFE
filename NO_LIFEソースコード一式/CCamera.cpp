#include <array>//�z��A�ÓI�ł��邪
#include "CCamera.h"


void CCamera::CameraFixed(DirectX::XMFLOAT4X4 _getMtx) {//�J���������Œ�Ǐ]

	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[�̏����󂯎��

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y��
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//�J�����ʒu�v�Z
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;


	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos;//�v���C���[�̈ʒu��ǂ�������悤�ɍ쐻

	//�J�����̈ʒu�����߂�A�������Œ�ɂ��邽�ߒ萔��������
	eye.x = pos.x + 0.0f * -100;
	eye.y = (pos.y + 45) + 0.0f * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	eye.z = pos.z + 1.0f * -100;

	//�J����������A�W���o���������̂���
	up = yaxis;

	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraFPS(DirectX::XMFLOAT4X4 _getMtx){//FPS�J����
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[�����󂯎��

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//�����_�v�Z
	XMFLOAT3 lookat;
	lookat.x = mtx._41 + zaxis.x;
	lookat.y = mtx._42 + zaxis.y;
	lookat.z = mtx._43 + zaxis.z;

	//�J�����ʒu�v�Z
	XMFLOAT3 eye;
	eye.x = mtx._41;
	eye.y = mtx._42;
	eye.z = mtx._43;

	//�J���������
	XMFLOAT3 up;
	up.x = mtx._21;
	up.y = mtx._22;
	up.z = mtx._23;

	//�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::Camera2FPS(DirectX::XMFLOAT4X4 _getMtx){//FPS�J�������Ǐ]
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[�̏����󂯎��

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y��
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//�J�����ʒu�v�Z
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;


	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos;//�v���C���[�̈ʒu��ǂ�������悤�ɍ쐻

	//�J�����̈ʒu�����߂�
	eye.x = pos.x + zaxis.x * -100;
	eye.y = (pos.y + 45) + zaxis.y * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	eye.z = pos.z + zaxis.z * -100;

	//�J����������A�W���o���������̂���
	up = yaxis;
	
	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelayold(DirectX::XMFLOAT4X4 _getMtx){//�x���J����
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[���i�[

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y��
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	//�J�����ʒu�v�Z
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1){
		svidx = 0;
		firstflag = true;
	}

	//30�t���[���O�̏����g�p���ăJ�������쐬
	int useidx;
	useidx = svidx - 30;
	if (useidx < 0){
		if (firstflag == false){
			useidx = 0;
		}
		else{
			useidx = svidx + zaxis.size() - 30;
		}
	}

	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	//�J���������
	up = yaxis[useidx];

	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraEditor(DirectX::XMFLOAT4X4 _getMtx)
{
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[�̏����󂯎��

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y��
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//�J�����ʒu�v�Z
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42 + 10;//�J�����ʒu����O�ɂ��炷
	pos.z = mtx._43 + 10;//�J�����ʒu��O�ɂ��炷


	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos;//�v���C���[�̈ʒu��ǂ�������悤�ɍ쐻

	//�J�����̈ʒu�����߂�A�������Œ�ɂ��邽�ߒ萔��������
	eye.x = pos.x + zaxis.x * -100;
	eye.y = (pos.y + 90) + zaxis.y * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	eye.z = pos.z + 1.0f * -40;

	//�J����������A�W���o���������̂���
	up = yaxis;

	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelay(DirectX::XMFLOAT4X4 _getMtx)
{
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[���i�[

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y��
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	static float AAA = 0;
	//static bool BBB = false;
	//if (BBB == false)
	//{
	//	AAA += 0.1;
	//	if (AAA >= 0.9f)
	//	{
	//		BBB = true;
	//	}
	//}
	//else if (BBB == true)
	//{
	//	AAA -= 0.1;
	//	if (AAA <= -0.9f)
	//	{
	//		//AAA = -0.1;
	//		BBB = false;
	//	}
	//}

	//�J�����ʒu�v�Z
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41 + AAA;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1) {
		svidx = 0;
		firstflag = true;
	}

	//30�t���[���O�̏����g�p���ăJ�������쐬
	int useidx;
	useidx = svidx - 1;
	if (useidx < 0) {
		if (firstflag == false) {
			useidx = 0;
		}
		else {
			useidx = svidx + zaxis.size() - 1;
		}
	}

	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos[useidx];

	/*eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;*/

	//eye.x = pos[useidx].x + 0.0f * -100;
	//eye.y = (pos[useidx].y + 115) + 0.0f * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	//eye.z = pos[useidx].z + 1.0f * -100;

	//static float AAA = 0;
	//static bool BBB = false;
	//if (BBB == false)
	//{
	//	AAA += 0.01;
	//	if (AAA >= 0.1f)
	//	{
	//		BBB = true;
	//	}
	//}
	//else if (BBB == true)
	//{
	//	AAA -= 0.01;
	//	if (AAA <= -0.1f)
	//	{
	//		AAA = -0.1;
	//		//BBB = false;
	//	}
	//}
	
	eye.x = pos[useidx].x + 0.0f * -100;
	eye.y = (pos[useidx].y + 115) + 0.0f * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	eye.z = pos[useidx].z + 1.0f * -100;

	//�J���������
	up = yaxis[useidx];

	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelay(PHitEffectFlg _pHitEffectFlg, DirectX::XMFLOAT4X4 _getMtx)
{
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//�v���C���[���i�[

	//�r���[�ϊ��s��
	static int svidx = 0;
	static bool firstflag = false;

	//Z���擾
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y��
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	//�U�����󂯂��Ƃ�
	if (_pHitEffectFlg == PHitEffectFlg::HIT)
	{
		switch (m_cameraEffectFlg)
		{
		case CCamera::CameraEffectFlg::ONE: {
			m_HitNum += 0.2f;
			if (m_HitNum >= 1.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::TWO;
			}
		}
			break;
		case CCamera::CameraEffectFlg::TWO: {
			m_HitNum -= 0.2f;
			if (m_HitNum <= -1.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::FINISH;
			}
		}
			break;
		case CCamera::CameraEffectFlg::FINISH:
			m_HitNum = 0;
			m_cameraEffectFlg = CameraEffectFlg::ONE;
			m_sPlayer->SetPHitEffectFlg(PHitEffectFlg::NONE);
			break;
		default:
			break;
		}
	
	}

	//�J�����ʒu�v�Z
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41 + m_HitNum;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1) {
		svidx = 0;
		firstflag = true;
	}

	//30�t���[���O�̏����g�p���ăJ�������쐬
	int useidx;
	useidx = svidx - 1;
	if (useidx < 0) {
		if (firstflag == false) {
			useidx = 0;
		}
		else {
			useidx = svidx + zaxis.size() - 1;
		}
	}

	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos[useidx];

	//�v���C���[�����񂾂Ƃ��̋���
	if (_pHitEffectFlg == PHitEffectFlg::DEATH)
	{
		switch (m_cameraEffectFlg)
		{
		case CCamera::CameraEffectFlg::ONE: {
			m_DeathNum += 0.002f;
			if (m_DeathNum >= 0.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::TWO;
			}
		}
		break;
		case CCamera::CameraEffectFlg::TWO: {
			m_DeathNum -= 0.002f;
			if (m_DeathNum <= -0.5f)
			{
				//�J��Ԃ�
				m_cameraEffectFlg = CameraEffectFlg::ONE;
			}
		}
		break;
		case CCamera::CameraEffectFlg::FINISH:
			break;
		default:
			break;
		}

	}
	eye.x = pos[useidx].x + m_DeathNum * -100;
	eye.y = (pos[useidx].y + 115) + 0.0f * -100;//�X����p�x�̂��߂ɂS�T�����Ă���
	eye.z = pos[useidx].z + 1.0f * -100;

	//�J���������
	up = yaxis[useidx];

	//�r���[�ϊ��s�񐶐��A�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//��L�ŃJ�����N���X���ɒl���i�[�A���̌�l�𔽉f�����ăZ�b�g����
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::Reset()
{
	m_HitNum = 0;
	m_DeathNum = 0;
	m_cameraEffectFlg = CameraEffectFlg::ONE;
}

