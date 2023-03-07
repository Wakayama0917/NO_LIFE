#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include "dx11mathutil.h"
#include "dx11util.h"
#include "Shader.h"
#include "shaderhashmap.h"
#include "DX11Settransform.h"
#include "uncopyable.h"


// comptr�A�X�}�[�g�|�C���^�錾
using Microsoft::WRL::ComPtr;

// ��`�N���X
class Quad2D : public Uncopyable{
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
	Quad2D::Vertex			m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	ComPtr<ID3D11ShaderResourceView> m_srv;			// SRV�i�V�F�[�_�[���\�[�X�r���[�j
	ComPtr<ID3D11Resource>	m_tex;					// �e�N�X�`�����\�[�X
	DirectX::XMFLOAT4		m_colour;				// �F�̏��
	float					alpha = 0.0f;			//�A���t�@�l�v�Z�p���l
	float					m_faddeEnd = 0.0f;		//�t�F�[�h�������I��邩�̔���Ɏg��
	

	//�A�j���p
	int m_loop;//�A�j����؂�ւ���t���[�����A�Œ�Ȃ̂ł����œ����
	float m_idx;//���Ԃ̃R�}���g�������߂鐔�A�Œ�Ȃ̂ł����œ����
	//��L�̓���������Ŏ�荞��ŁA���̐����o�������ăA�b�v�f�[�g���ŏ����������
	//�w��R�}����̃A�j���\
	float m_widthpartition;//�������������邩�̐�
	float m_heightpartition;//�c�����������邩�̐�
	float m_heightnum;//�c�̉���ڂ��g�p���邩�̐�

	//�ŏ��̃R�}�̔z��A1/1�̏ꍇ
	XMFLOAT2 m_uv2[1][4] = {
		{
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
		}
	};

public:

	/*************************************************************
	* �}�f����
	*************************************************************/
	
	// ��`�̏�����
	bool Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color);
	// ��`�̏������A�j��,�����������A�c���������A�g�p�c��
	bool Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color,
				float _widthpartition,float _heightpartition,float _heightnum);
	//�������ɕK�v�Ȃ��̂��܂Ƃ߂��֐�
	bool UiInitSetting(const char* _filename, uint32_t _width, uint32_t _height, float _x, float _y);

	// �`��
	void Draw();

	//�폜�A���͉����Ȃ�
	//�J�������̓X�}�[�g�|�C���^�ōs���Ă���
	void UnInit();
public:
	/*************************************************************
	* �A�b�v�f�[�g�F�����A�j���[�V��������^�C�v�̊֐�
	*************************************************************/
	// �A�j���[�V�����X�V�A�I�[�o�[���[�h
	void updateAnime(int _interval);//���t���[��
	void updateAnime(int _interval, uint32_t _width, uint32_t _height);//���t���[���A�傫���ς��鎞
	void updateAnimeOne(float _idx);//�w�肵���R�}��`�悷��B

	/*************************************************************
	* �A�b�v�f�[�g�F�����A�j���[�V���������Ȃ��^�C�v�̊֐�
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
	* �}�f�����A�擾��ύX�������镨
	*************************************************************/
	
	// Z����]
	void SetRotation(float _angle);

	//�_�ŏ����A�A���t�@�l�����A��������l������
	void Flashing(float _value);
	//�摜�����X�Ɍ�����悤�ɂ���B�t�F�[�h�C��
	bool FaddeIN(float _value);
	//�摜�����X�ɓ����ɂ���B�t�F�[�h�A�E�g
	bool FaddeOUT(float _value);
	//�t�F�[�h�n�̐��l������������
	void FaddeReset();

	//�A���t�@�l������������
	void FlashingReset() {
		alpha = 0.5f;
	}

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

	// ���_�f�[�^�X�V
	void updateVertex(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	
	// �e�N�X�`���Z�b�g�A�ǂݍ���
	void LoadTexture(const char* _filename);

	//XY���W��Ԃ�
	XMFLOAT2 GetXYPos() {
		return XMFLOAT2(m_worldmtx._41, m_worldmtx._42);
	}
	//�T�C�Y��Ԃ�
	XMFLOAT2 GetSize() {
		return XMFLOAT2(m_width, m_height);
	}
	//�J���[��Ԃ�
	XMFLOAT4 GetColour() {
		return m_colour;
	}
	//���l�̃t���O��Ԃ�
	float Getfadde() {
		return m_faddeEnd;
	}

private:
	/*************************************************************
	*���������݂̂Ŏg�p���Ă��镨 
	*************************************************************/
	// �g��A�k��
	//���݂̍s��ɑ΂��Ă��Ă���
	void SetScale(float _sx, float _sy, float _sz);

	// �ʒu���Z�b�g
	void SetPosition(float _x, float _y, float _z);

	// �r���[�|�[�g�̃T�C�Y���Z�b�g�A�`�掞�̑傫��
	bool SetViewPortSize(uint32_t _width, uint32_t _height);

	// ���_�o�b�t�@�X�V
	void updateVbuffer();

};