#include <directxmath.h>
#include "Memory.h"
#include "Player.h"
#include "EnumListPlayer.h"


using namespace DirectX;

class CCamera{
	//���������Ȃ��ꏊ�Ȃ̂Ńv���C�x�[�g����
	CCamera(){
	}
public:

	//void FixedCamera(DirectX::XMFLOAT3 _getPos);//�Œ蒍���J����
	void CameraFixed(DirectX::XMFLOAT4X4 _getMtx);//�J���������Œ�Ǐ]
	;
	void CameraFPS(DirectX::XMFLOAT4X4 _getMtx);//FPS�J����

	void Camera2FPS(DirectX::XMFLOAT4X4 _getMtx);//FPS�J�������Ǐ]

	void CameraDelayold(DirectX::XMFLOAT4X4 _getMtx);//�x���J����

	void CameraEditor(DirectX::XMFLOAT4X4 _getMtx);//�G�f�B�^�[�V�[���̃J�����A�����

	void CameraDelay(DirectX::XMFLOAT4X4 _getMtx);//�x���J����

	void CameraDelay(PHitEffectFlg _pHitEffectFlg,DirectX::XMFLOAT4X4 _getMtx);//�x���J����

	//�������A�f�[�^��S�ĕۑ�
	void SetPlayer(shared_ptr<Player>& _player) {
		//�v���C���[�̃|�C���^�i�[
		m_sPlayer = _player;
	}

	void Reset();//���̃X�e�[�W�ɍs���ۂ̏�����


	CCamera(const CCamera&) = delete;
	CCamera& operator=(const CCamera&) = delete;
	CCamera(CCamera&&) = delete;
	CCamera& operator=(CCamera&&) = delete;

	static CCamera* GetInstance(){
		static CCamera instance;
		return &instance;
	}

	void Init(float _nearclip, float _farclip, float _fov, 
		float _width, float _height,
		XMFLOAT3 _eye, XMFLOAT3 _lookat, XMFLOAT3 _up){

		SetProjection(_nearclip,_farclip,_fov,_width,_height);
		SetCamera(_eye,_lookat,_up);
	}

	void SetNear(float _nearclip){
		m_near = _nearclip;
	}

	void SetFar(float _farclip){
		m_far = _farclip;
	}

	void SetFov(float _fov){
		m_Fov = _fov;
	}

	void SetAspect(float _width,float _height){
		m_Aspect = _width/_height;
	}

	void SetProjection(float _nearclip,float _farclip,float _fov,float _width,float _height){
		SetNear(_nearclip);
		SetFar(_farclip);
		SetFov(_fov);
		SetAspect(_width,_height);
		CreateProjectionMatrix();
	}
		
	void SetCamera(const XMFLOAT3& _eye, const XMFLOAT3& _lookat, const XMFLOAT3& _up){

		SetEye(_eye);
		SetLookat(_lookat);
		SetUp(_up);
		CreateCameraMatrix();
	}

	void SetEye(const XMFLOAT3& _eye){
		m_Eye = _eye;
	}

	void SetLookat(const XMFLOAT3& _lookat){
		m_Lookat = _lookat;
	}

	void SetUp(const XMFLOAT3& _up){
		m_Up = _up;
	}

	void CreateCameraMatrix(){
		ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x, m_Lookat.y, m_Lookat.z, 0.0f);
		ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

		ALIGN16 XMMATRIX camera;
		camera = XMMatrixLookAtLH(Eye, At, Up);

		XMStoreFloat4x4(&m_Camera, camera);
	}

	void CreateProjectionMatrix(){

		ALIGN16 XMMATRIX projection;

		projection = XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

		XMStoreFloat4x4(&m_Projection, projection);

	}

	const XMFLOAT4X4& GetCameraMatrix(){
		return m_Camera;
	}

	const XMFLOAT4X4& GetProjectionMatrix(){
		return m_Projection;
	}

	float GetFov() const {
		return m_Fov;
	}

	const XMFLOAT3& GetEye() const {
		return m_Eye;
	}

	const XMFLOAT3& GetLookat() const {
		return m_Lookat;
	}

	const XMFLOAT3& GetUp() const {
		return m_Up;
	}

	

private:
	XMFLOAT4X4		m_Projection;

	XMFLOAT4X4		m_Camera;

	XMFLOAT3		m_Eye;				// �J�����ʒu
	XMFLOAT3		m_Lookat;			// �����_
	XMFLOAT3		m_Up;				// ������x�N�g��

	float			m_near;
	float			m_Aspect;
	float			m_Fov;
	float			m_far;

	float m_HitNum = 0;
	float m_DeathNum = 0;
	//�q�b�g���̉��o�p
	enum class CameraEffectFlg
	{
		ONE,		    //��������
		TWO,			//�G�̍U��������������
		FINISH,			//���S�����Ƃ�
	};
	CameraEffectFlg m_cameraEffectFlg = CameraEffectFlg::ONE;

	//�v���C���[�̃|�C���^���i�[����悤
	std::shared_ptr<Player> m_sPlayer;
};