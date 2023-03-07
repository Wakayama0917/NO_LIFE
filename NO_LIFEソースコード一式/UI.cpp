//#include <string>
#include "dx11mathutil.h"
//#include "dx11util.h"
//#include "Shader.h"
//#include "shaderhashmap.h"
//#include "DX11Settransform.h"
//#include "quad2d.h"
#include "UI.h"

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

bool UI::InitSetting(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv)
{
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
	updateVertex(_width, _height, _color, _uv);

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = GetDX11Device();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(UI::Vertex),		// �P���_������o�C�g��
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

bool UI::Init(const char* _filename, uint32_t _width, uint32_t _height, float _x, float _y)
{
	return false;
}

void UI::Draw()
{
}

void UI::UnInit()
{
}

void UI::LoadTexture(const char* _filename)
{
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
