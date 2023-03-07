#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "DX11util.h"
#include "Shader.h"
/*----------------------------------------------------------------------

	�r���{�[�h�N���X	

-----------------------------------------------------------------------*/

class CBillBoard{
private:
	DirectX::XMFLOAT4X4			m_mtx;			// �r���{�[�h�p�̍s��
	DirectX::XMFLOAT3			m_pos;			// �r���{�[�h�̈ʒu���W
	float						m_XSize=100.0f;		// �r���{�[�h�̂w�T�C�Y
	float						m_YSize=100.0f;		// �r���{�[�h�̂x�T�C�Y
	DirectX::XMFLOAT4			m_Color= DirectX::XMFLOAT4(1,1,1,1);	// ���_�J���[�l
	ID3D11ShaderResourceView* 	m_srv = nullptr;			// Shader Resourceview�e�N�X�`��
	ID3D11Buffer*				m_vbuffer = nullptr;		// ���_�o�b�t�@
	ID3D11BlendState*			m_pBlendStateSrcAlpha = nullptr;//�����炭������F
	ID3D11BlendState*			m_pBlendStateOne = nullptr;
	ID3D11BlendState*			m_pBlendStateDefault = nullptr;
	ID3D11VertexShader*			m_pVertexShader = nullptr;	// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*			m_pPixelShader = nullptr;	// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*			m_pVertexLayout = nullptr;	// ���_�t�H�[�}�b�g��`
	ID3D11Resource*				m_res = nullptr;

	// ���_�t�H�[�}�b�g
	struct MyVertex{
		float				x,y,z;
		DirectX::XMFLOAT4	color;
		float				tu,tv;//�c����
	};

	MyVertex				m_Vertex[4];			// �r���{�[�h�̒��_���W
	DirectX::XMFLOAT2		m_uv[4];				// �e�N�X�`�����W

	//�A�j���p
	int m_loop = 0;//�A�j����؂�ւ���t���[�����A�Œ�Ȃ̂ł����œ����
	int m_idx = 0;//���Ԃ̃R�}���g�������߂鐔�A�Œ�Ȃ̂ł����œ����
	//��L�̓���������Ŏ�荞��ŁA���̐����o�������ăA�b�v�f�[�g���ŏ����������
	//�w��R�}����̃A�j���\
	float m_widthpartition;//�������������邩�̐�
	float m_heightpartition;//�c�����������邩�̐�
	float m_heightnum;//�c�̉���ڂ��g�p���邩�̐�

private:
	// �r���{�[�h�̒��_���W���v�Z
	void CalcVertex();
	// �r���{�[�h�p�̍s��𐶐�
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat);
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat,float _radian);//�m�x������ꍇ
	// �`��
	void Draw();
	// �\�[�X�A���t�@��ݒ肷��
	void SetBlendStateSrcAlpha();
	// �u�����h�X�e�[�g�𐶐�����
	void CreateBlendStateSrcAlpha();
	// ���Z������ݒ肷��
	void SetBlendStateOne();
	// �u�����h�X�e�[�g�i���Z�����j�𐶐�����
	void CreateBlendStateOne();
	// �f�t�H���g�̃u�����h�X�e�[�g��ݒ肷��
	void SetBlendStateDefault();
	// �f�t�H���g�̃u�����h�X�e�[�g�𐶐�����
	void CreateBlendStateDefault();
	// UV���W���Z�b�g����

public:
	void SetUV(DirectX::XMFLOAT2 uv[]) {//�e�N�X�`���摜���Z�b�g
		m_uv[0] = uv[0];
		m_uv[1] = uv[1];
		m_uv[2] = uv[2];
		m_uv[3] = uv[3];
		CalcVertex();
	}
	/*CBillBoard():m_pos.x(0),m_y(0),m_z(0),m_srv(nullptr){
	}*/

	//�A�j������
	//�`�悷��ׂ̏������A�����ʒu�A�摜�̑傫���A�F�B
	bool Init(DirectX::XMFLOAT3 _pos, float _xsize, float _ysize, DirectX::XMFLOAT4 _color);
	
	//�A�j������
	//�`�悷��ׂ̏������A�����ʒu�A�摜�̑傫���A�F�A�摜�̏c���������A�g�p��
	bool Init(DirectX::XMFLOAT3 _pos, float _xsize, float _ysize, DirectX::XMFLOAT4 _color,
		float _widthpartition, float _heightpartition, float _heightnum);
	
	//�J������
	void Dispose();

	// �f�X�g���N�^
	//��L�̊J�������������I�ɍs�����ƂŃ~�X�𖳂���
	virtual ~CBillBoard(){
		Dispose();
	}

	// �t�u���W���Z�b�g����
	//�r���{�[�h�̒��_���W
	void SetUV(float u[],float v[]){
		m_Vertex[0].tu = u[0];
		m_Vertex[0].tv = v[0];

		m_Vertex[1].tu = u[1];
		m_Vertex[1].tv = v[1];

		m_Vertex[2].tu = u[2];
		m_Vertex[2].tv = v[2];

		m_Vertex[3].tu = u[3];
		m_Vertex[3].tv = v[3];

		CalcVertex();						// �r���{�[�h�p�̒��_�f�[�^�쐬	
	}

	// �ʒu���w��
	void SetPosition(DirectX::XMFLOAT3 _pos) {//�`�悷��ʒu������
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
	}

	// �r���{�[�h��`��
	void DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat);//�`���p��
	void DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat,float _radian);//�`���p���Ɗm�x

	// �r���{�[�h�`����Z����
	//�G���d�Ȃ������ɐF�������邩�A�ԂƐ̉摜���d�Ȃ������ɗ΂ɂ��邩�݂����Ȋ���
	void DrawBillBoardAdd(const DirectX::XMFLOAT4X4& _cameramat);//��Ƃقړ����F�����Z

	// �r���{�[�h��Z���𒆐S�ɂ��ĉ�]�����ĕ`��
	void DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &_cameramat, float _radian);
	
	// �T�C�Y���Z�b�g
	void SetSize(float _x, float _y);
	//�T�C�Y���w�肵���������g��
	void SetSizeAdd(float _x, float _y);

	// �J���[���Z�b�g
	void SetColor(DirectX::XMFLOAT4 _col);//�����̂S�̂��

	//	�e�N�X�`���ǂݍ���
	bool LoadTexTure(const char* _filename);//���ʂɃt�@�C�������ꂽ��OK

	//1���������o�����悤
	void updateAnimeNo()
	{
		DirectX::XMFLOAT2 uv[4];
		uv[0] = { 0.0f , 0.0f};
		uv[1] = { 1.0f , 0.0f};
		uv[2] = { 0.0f , 1.0f};
		uv[3] = { 1.0f , 1.0f};
		SetUV(uv);
	}

	// �A�j���[�V�����X�V�A���[�v
	void updateAnimeLoop(int _interval);//���t���[��
	// �A�j���[�V�����X�V�A�P�T������I���
	bool updateAnime(int _interval);//�������t���[��
	// �V���O���g���p�ɃJ�E���g���O������󂯎��ꍇ
	bool updateAnime(int& _imageNum,int _count, int _interval);//���ݐ��l,�������t���[��
	

};