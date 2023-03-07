#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include <iostream>
#include <math.h>
#include "dx11mathutil.h"
#include "dx11util.h"
#include "Shader.h"
#include "shaderhashmap.h"
#include "DX11Settransform.h"
#include "quad2d.h"

// comptr
using Microsoft::WRL::ComPtr;

// �g�p���钸�_�V�F�[�_�[��
const char* vsfilename[] = {
	"shader/vsquad2d.hlsl"
};

// �g�p����s�N�Z���V�F�[�_�[��
const char* psfilename[] = {
	"shader/psquad2d.hlsl"
};

bool Quad2D::SetViewPortSize(uint32_t _width, uint32_t _height){
	ComPtr<ID3D11Buffer> CBViewPort;

	// �r���[�|�[�g�p�̒萔�o�b�t�@
	struct ConstantBufferViewPort {
		uint32_t	Width[4];			// �r���[�|�[�g��
		uint32_t	Height[4];			// �r���[�|�[�g����
	};

	// �r���[�|�[�g�萔�o�b�t�@�ɒl���Z�b�g
	bool sts = CreateConstantBufferWrite(
		GetDX11Device(),
		sizeof(ConstantBufferViewPort),
		&CBViewPort);
	if (!sts) {
		return false;
	}

	ConstantBufferViewPort cb;
	cb.Width[0] = _width;
	cb.Height[0] = _height;

	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = GetDX11DeviceContext()->Map(CBViewPort.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferViewPort));
		GetDX11DeviceContext()->Unmap(CBViewPort.Get(), 0);
	}
	else {
		return false;
	}

	// ���_�V�F�[�_�[�ɃZ�b�g
	GetDX11DeviceContext()->VSSetConstantBuffers(5, 1, CBViewPort.GetAddressOf());

	return true;
}

// ��`�̏�����
bool Quad2D::Init(uint32_t _width, 
	uint32_t _height, 
	DirectX::XMFLOAT4 _color ) {

	DX11MtxIdentity(m_worldmtx);	// �����p��

	m_colour = _color;//�F��ݒ�

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error", "error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��

	// ���_������
	updateVertex(_width, _height, _color, *m_uv2);

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = GetDX11Device();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Quad2D::Vertex),		// �P���_������o�C�g��
		4,							// ���_��
		m_vertex,					// ���_�f�[�^�i�[�������擪�A�h���X
		&m_vertexbuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		4,							// �C���f�b�N�X��
		index,						// �C���f�b�N�X�i�[�������擪�A�h���X
		&m_indexbuffer				// �C���f�b�N�X�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// �r���[�|�[�g�T�C�Y�Z�b�g
	SetViewPortSize(DX11GetScreenWidth(), DX11GetScreenHeight());

	return true;
}

bool Quad2D::Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, 
					float _widthpartition, float _heightpartition, float _heightnum){

	DX11MtxIdentity(m_worldmtx);	// �����p��

	m_colour = _color;//�F��ݒ�

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error", "error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��

	// ���_������
	updateVertex(_width, _height, _color, *m_uv2);

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = GetDX11Device();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Quad2D::Vertex),		// �P���_������o�C�g��
		4,							// ���_��
		m_vertex,					// ���_�f�[�^�i�[�������擪�A�h���X
		&m_vertexbuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		4,							// �C���f�b�N�X��
		index,						// �C���f�b�N�X�i�[�������擪�A�h���X
		&m_indexbuffer				// �C���f�b�N�X�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// �r���[�|�[�g�T�C�Y�Z�b�g
	SetViewPortSize(DX11GetScreenWidth(), DX11GetScreenHeight());

	//�A�j���[�V�����ɕK�v�Ȑ��l���Z�b�g
	//���l���O�ȉ��łȂ���Γ����
	if ((_widthpartition <= 0.0f) || (_heightpartition <= 0.0f) || (_heightnum <= 0.0f)){
		
		std::string str("UI�������̍ۂɁA�c���A�����A�g�p�c��̂ǂꂩ�ɂO�ȉ��̐��l�������Ă��܂� ");//SRV�^�̕ϊ��Ɏ��s
		MessageBox(nullptr, (str).c_str(), "error", MB_OK);
		return false;
	}
	else{
		m_widthpartition = _widthpartition;
		m_heightpartition = _heightpartition;
		m_heightnum = _heightnum;
	}
	m_loop = 0;
	m_idx = 0.0f;

	return true;
}

bool Quad2D::UiInitSetting(const char* _filename, uint32_t 
	_width, uint32_t _height, float _x, float _y){
	//UI�̏�����
	//XMFLOAT2 DVA;
	//g_UI.Init(50.0f, 50.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &DVA);
	Init(_width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	LoadTexture(_filename);
	//�ʒu�ݒ�
	SetPosition(_x, _y, 0);//�ő�1290�A730

	return false;
}

// �`��
void Quad2D::Draw() {
	//Z�o�b�t�@off
	TurnOffZbuffer();

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	// �e�N�X�`�����Z�b�g
	devcontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs;
	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps;
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X

	//Z�o�b�t�@�I��
	TurnOnZbuffer();
}

void Quad2D::UnInit() {

}

// �g��A�k��
void Quad2D::SetScale(float _sx, float _sy, float _sz) {
	DX11MtxScale(_sx, _sy, _sz, m_worldmtx);
}

// �ʒu���Z�b�g
void Quad2D::SetPosition(float _x, float _y, float _z) {
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;
	m_worldmtx._43 = _z;
}

// Z����]
void Quad2D::SetRotation(float _angle) {
	DX11MtxRotationZ(_angle, m_worldmtx);
}

void Quad2D::Flashing(float _value){
	alpha += _value;
	float sinValue = sin(alpha * DirectX::XM_PI);//�T�C���������o��
	sinValue = sinValue / 2;//�O�`�P�ɂȂ�l�Ɋ���
	sinValue += 0.5f;//�T�C���h�̊�l�����炷
	if (alpha > 1000.0f)//�J���X�g���Ȃ��l�ɏ�����
	{
		alpha = 0.0f;
	}

	updateVertex(m_width, m_height, XMFLOAT4(1.0f,1.0f,1.0f,sinValue), m_uv2[0]);

	updateVbuffer();

}

bool Quad2D::FaddeIN(float _value)
{
	if (m_faddeEnd < 1.0f)
	{
		alpha += _value;
		float sinValue = sin(alpha * DirectX::XM_PI);//�T�C���������o��
		sinValue = sinValue;//�O�`�P�ɂȂ�l�Ɋ���
		//sinValue += 0.5f;//�T�C���h�̊�l�����炷
		m_faddeEnd = sinValue;
		//m_colour.w = sinValue;
		//if (alpha > 1000.0f)//�J���X�g���Ȃ��l�ɏ�����
		//{
		//	alpha = 0.0f;
		//}

		updateVertex(m_width, m_height, XMFLOAT4(1.0f, 1.0f, 1.0f, sinValue), m_uv2[0]);
		//updateVertex(m_width, m_height, m_colour, m_uv2[0]);

		updateVbuffer();
		return false;
	}
	else
	{
		//�t�F�[�h���������Ȃ�
		return true;
	}
	
}

bool Quad2D::FaddeOUT(float _value)
{
	if (m_faddeEnd > 0.0f)
	{
		alpha += _value;
		float sinValue = sin(alpha * DirectX::XM_PI);//�T�C���������o��
		sinValue = sinValue;
		//sinValue += 0.5f;//�T�C���h�̊�l�����炷
		m_faddeEnd = sinValue;
		//if (alpha > 1000.0f)//�J���X�g���Ȃ��l�ɏ�����
		//{
		//	alpha = 0.0f;
		//}

		updateVertex(m_width, m_height, XMFLOAT4(1.0f, 1.0f, 1.0f, sinValue), m_uv2[0]);

		updateVbuffer();
		return false;
	}
	else
	{
		//�t�F�[�h���������Ȃ�
		return true;
	}
}

void Quad2D::FaddeReset()
{
	alpha = 0.0f;			//�A���t�@�l�p���l
	m_faddeEnd = 0.0f;		//�t�F�[�h�������I��邩�̔���Ɏg��
}

// ���_�f�[�^�X�V
void Quad2D::updateVertex(uint32_t _width, 
	uint32_t _height, 
	DirectX::XMFLOAT4 _color, 
	DirectX::XMFLOAT2* _uv) {

	m_width = static_cast<float>(_width);
	m_height = static_cast<float>(_height);

	Quad2D::Vertex	v[4] = {
		// ���W													// �J���[	// UV	
		XMFLOAT3(	-m_width / 2.0f,	-m_height / 2.0f, 0.0f),	_color,		_uv[0],
		XMFLOAT3(	 m_width / 2.0f,	-m_height / 2.0f, 0.0f),	_color,		_uv[1],
		XMFLOAT3(	-m_width / 2.0f,	 m_height / 2.0f, 0.0f),	_color,		_uv[2],
		XMFLOAT3(	 m_width / 2.0f,	 m_height / 2.0f, 0.0f),	_color,		_uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}

void Quad2D::LoadTexture(const char* _filename){
	// �e�N�X�`���ǂݍ���
	bool sts = CreateSRVfromFile(_filename,
		GetDX11Device(),
		GetDX11DeviceContext(),
		&m_tex, &m_srv);
	if (!sts) {
		std::string str("SRV �쐬�G���[ ");//SRV�^�̕ϊ��Ɏ��s
		std::string strfile(_filename);
		MessageBox(nullptr, (str + strfile).c_str(), "error", MB_OK);
		return;
	}
}

// ���_�o�b�t�@�X�V
void Quad2D::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetDX11DeviceContext()->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Quad2D::Vertex) * 4);
		GetDX11DeviceContext()->Unmap(m_vertexbuffer.Get(), 0);
	}
}

void Quad2D::updateAnime(int _interval){
	//2D�`��ݒ�J�n
	m_loop++;
	if (m_loop == _interval)//���t���[���ŃA�j����؂�ւ��邩
	{
		m_idx++;
		if (m_idx >= m_widthpartition)//�A�j���R�}�ő吔
		{
			m_idx = 0.0f;
		}

		m_loop = 0;

		XMFLOAT2 uv2[1][4] = {
			{
			{(0.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{(1.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{(0.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition},
			{(1.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition}
			}
		};

		updateVertex(m_width, m_height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		updateVbuffer();
	}
	else if (m_idx == 0)
	{
		//�ŏ��̃R�}�̎��̂ݎg�p
		XMFLOAT2 uv2[1][4] = {
			{
			{0.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{1.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{0.0f / m_widthpartition,m_heightnum / m_heightpartition},
			{1.0f / m_widthpartition,m_heightnum / m_heightpartition}
			}
		};

		updateVertex(m_width, m_height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		updateVbuffer();
	}
}

void Quad2D::updateAnime(int _interval, uint32_t _width, uint32_t _height){
	
		//2D�`��ݒ�J�n
		m_loop++;
		if (m_loop == _interval)//���t���[���ŃA�j����؂�ւ��邩
		{
			m_idx++;
			if (m_idx >= m_widthpartition)//�A�j���R�}�ő吔
			{
				m_idx = 0.0f;
			}

			m_loop = 0;

			XMFLOAT2 uv2[1][4] = {
				{
				{(0.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
				{(1.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
				{(0.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition},
				{(1.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition}
				}
			};

			updateVertex(_width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		}
		else if (m_idx == 0)
		{
			//�ŏ��̃R�}�̎��̂ݎg�p
			XMFLOAT2 uv2[1][4] = {
				{
				{0.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
				{1.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
				{0.0f / m_widthpartition,m_heightnum / m_heightpartition},
				{1.0f / m_widthpartition,m_heightnum / m_heightpartition}
				}
			};

			updateVertex(_width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		}
	
}

void Quad2D::updateAnimeOne(float _idx)
{
	//2D�`��ݒ�J�n
	m_idx = _idx;
		
	//�ő�R�}�����傫����΂O�ɖ߂�
	if (m_idx >= m_widthpartition)//�A�j���R�}�ő吔
	{
		m_idx = 0.0f;
	}

	if (m_idx != 0.0f)//0�łȂ���΂�������g��
	{

		XMFLOAT2 uv2[1][4] = {
			{
			{(0.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{(1.0f + m_idx) / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{(0.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition},
			{(1.0f + m_idx) / m_widthpartition,m_heightnum / m_heightpartition}
			}
		};

		updateVertex(m_width, m_height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		updateVbuffer();
	}
	else// if (m_idx == 0)
	{
		//�ŏ��̃R�}�̎��̂ݎg�p
		XMFLOAT2 uv2[1][4] = {
			{
			{0.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{1.0f / m_widthpartition,(m_heightnum - 1.0f) / m_heightpartition},
			{0.0f / m_widthpartition,m_heightnum / m_heightpartition},
			{1.0f / m_widthpartition,m_heightnum / m_heightpartition}
			}
		};

		updateVertex(m_width, m_height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv2[0]);
		updateVbuffer();
	}
}

void Quad2D::updateAnimeNo(){
	
	updateVertex(m_width, m_height, m_colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoSize(uint32_t _width, uint32_t _height){
	
	updateVertex(_width, _height, m_colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoColour(XMFLOAT4 _colour){
	
	updateVertex(m_width, m_height, _colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoSizeColour(uint32_t _width, uint32_t _height, XMFLOAT4 _colour){
	
	updateVertex(_width, _height, _colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoSizePos(uint32_t _width, uint32_t _height, float _x, float _y){
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;

	updateVertex(_width, _height, m_colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoColourPos(XMFLOAT4 _colour, float _x, float _y){
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;

	updateVertex(m_width, m_height, _colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoSizeColourPos(uint32_t _width, uint32_t _height, XMFLOAT4 _colour, float _x, float _y){
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;


	updateVertex(_width, _height, _colour, m_uv2[0]);

	updateVbuffer();
}

void Quad2D::updateAnimeNoPos(float _x, float _y){
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;

	updateVertex(m_width, m_height, m_colour, m_uv2[0]);

	updateVbuffer();
}
