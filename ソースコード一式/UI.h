#pragma once
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include <wrl/client.h>
//#include <string>
//#include "dx11mathutil.h"
//#include "dx11util.h"
//#include "Shader.h"
#include "shaderhashmap.h"
//#include "DX11Settransform.h"
#include "uncopyable.h"

// comptr�A�X�}�[�g�|�C���^�錾
using Microsoft::WRL::ComPtr;

// ��`�N���X
class UI : public Uncopyable {
public:
	// ���_�f�[�^
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};
private:
	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	UI::Vertex				m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	ComPtr<ID3D11ShaderResourceView> m_srv;			// SRV�i�V�F�[�_�[���\�[�X�r���[�j
	ComPtr<ID3D11Resource>	m_tex;					// �e�N�X�`�����\�[�X
	DirectX::XMFLOAT4		m_colour;				// �F�̏��

public:
	/*************************************************************
	* �}�f����
	*************************************************************/
	
	// ��`�̏�����
	bool InitSetting(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	//�������ɕK�v�Ȃ��̂��܂Ƃ߂��֐�
	bool Init(const char* _filename, uint32_t _width, uint32_t _height, float _x, float _y);
	// �`��
	void Draw();
	//�폜�A���͉����Ȃ�
	//�J�������̓X�}�[�g�|�C���^�ōs���Ă���
	void UnInit();
	// �e�N�X�`���Z�b�g�A�ǂݍ���
	void LoadTexture(const char* _filename);

public:
	/*************************************************************
	* �A�b�v�f�[�g�֐�
	*************************************************************/
	//�A�j���[�V�������Ȃ��ꍇ�A�A�b�v�f�[�g�̂ݍs��
	void updateAnimeNo();
	//�A�j���[�V�������Ȃ��ꍇ�A�傫���ύX
	void updateAnimeNoSize(uint32_t _width, uint32_t _height);
	//�A�j���[�V�������Ȃ��ꍇ�A�F�ύX
	void updateAnimeNoColour(XMFLOAT4 _colour);
	//�|�W�V�����ύX
	void updateAnimeNoPos(float _x, float _y);
	//�A�j���[�V�������Ȃ��ꍇ�A�傫���ύX�ƐF�ύX
	void updateAnimeNoSizeColour(uint32_t _width, uint32_t _height, XMFLOAT4 _colour);
	//�傫���ƃ|�W�V�����ύX
	void updateAnimeNoSizePos(uint32_t _width, uint32_t _height, float _x, float _y);
	//�F�ƃ|�W�V�����ύX
	void updateAnimeNoColourPos(XMFLOAT4 _colour, float _x, float _y);
	//�傫���ƐF�ƃ|�W�V�����ύX
	void updateAnimeNoSizeColourPos(uint32_t _width, uint32_t _height, XMFLOAT4 _colour, float _x, float _y);

public:
	/*************************************************************
	* ���̑��̏���
	*************************************************************/
	// Z����]
	void SetRotation(float _angle);
	// ���_�f�[�^�X�V
	void updateVertex(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	// ���_�o�b�t�@�X�V
	void updateVbuffer();
private:
	/*************************************************************
	* �v���C�x�[�g
	*************************************************************/
	// �r���[�|�[�g�̃T�C�Y���Z�b�g�A�`�掞�̑傫��
	bool SetViewPortSize(uint32_t _width, uint32_t _height);

	/*************************************************************
	* �}�f�����A�擾�n
	*************************************************************/
	
	// ��`�̐i�s�������擾�i���摜��������ɍ���Ă��邱�Ƃ�O��j
	DirectX::XMFLOAT3 GetForward() {
		DirectX::XMFLOAT3 forward;
		forward.x = m_worldmtx._21;
		forward.y = m_worldmtx._22;
		forward.z = m_worldmtx._23;
		return forward;
	}

	// ��`�̉��������擾
	DirectX::XMFLOAT3 GetSide() {
		DirectX::XMFLOAT3 side;
		side.x = m_worldmtx._11;
		side.y = m_worldmtx._12;
		side.z = m_worldmtx._13;
		return side;
	}

	//XY���W��Ԃ�
	XMFLOAT2 GetXYPos() {
		return XMFLOAT2(m_worldmtx._41, m_worldmtx._42);
	}
	//�T�C�Y��Ԃ�
	XMFLOAT2 GetSize() {
		return XMFLOAT2(m_width, m_height);
	}
	//�T�C�Y��Ԃ�
	XMFLOAT4 GetColour() {
		return m_colour;
	}

};