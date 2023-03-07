#include "CBillBoard.h"
#include "Shader.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#include "dx11util.h"

// �r���{�[�h�̒��_���W���v�Z
void CBillBoard::CalcVertex(){
	m_Vertex[0].x = -m_XSize/2;
	m_Vertex[0].y =  m_YSize/2;
	m_Vertex[0].z =0.0f;
	m_Vertex[0].color = m_Color;
	m_Vertex[0].tu = m_uv[0].x;
	m_Vertex[0].tv = m_uv[0].y;

	m_Vertex[1].x =  m_XSize/2;
	m_Vertex[1].y =  m_YSize/2;
	m_Vertex[1].z =0.0f;
	m_Vertex[1].color = m_Color;
	m_Vertex[1].tu = m_uv[1].x;
	m_Vertex[1].tv = m_uv[1].y;

	m_Vertex[2].x = -m_XSize / 2;
	m_Vertex[2].y = -m_YSize / 2;
	m_Vertex[2].z = 0;
	m_Vertex[2].color = m_Color;
	m_Vertex[2].tu = m_uv[2].x;
	m_Vertex[2].tv = m_uv[2].y;

	m_Vertex[3].x = m_XSize/2;
	m_Vertex[3].y = -m_YSize/2;
	m_Vertex[3].z =0.0f;
	m_Vertex[3].color = m_Color;
	m_Vertex[3].tu = m_uv[3].x;
	m_Vertex[3].tv = m_uv[3].y;

	if (m_vbuffer==nullptr) {

		ID3D11Device* dev;
		dev = GetDX11Device();

		// ���_�o�b�t�@�쐬�i��ŕύX�\�ȁj
		bool sts = CreateVertexBufferWrite(dev, sizeof(MyVertex), 4, m_Vertex, &m_vbuffer);
		if (!sts) {
			MessageBox(nullptr, "create vertex buffer erro(CBillBoard)", "error", MB_OK);
		}
	}
	else {
		// �쐬�ς݂Ȃ� ���e������������ 
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();
		HRESULT hr = devcontext->Map(m_vbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(MyVertex) * 4);
			devcontext->Unmap(m_vbuffer, 0);
		}
	}
}

// �T�C�Y���Z�b�g
void CBillBoard::SetSize(float _x, float _y){
	m_XSize = _x;
	m_YSize = _y;
	//CalcVertex();
}

void CBillBoard::SetSizeAdd(float _x, float _y)
{
	m_XSize += _x;
	m_YSize += _y;
	CalcVertex();
}

// �J���[�l���Z�b�g
void CBillBoard::SetColor(XMFLOAT4  _col){
	m_Color = _col;
	CalcVertex();
}

// �r���{�[�h�p�̍s��𐶐�
void CBillBoard::CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat){

	m_mtx._11 =_cameramat._11;
	m_mtx._12 =_cameramat._21;
	m_mtx._13 =_cameramat._31;

	m_mtx._21 =_cameramat._12;
	m_mtx._22 =_cameramat._22; 
	m_mtx._23 =_cameramat._32;

	m_mtx._31 =_cameramat._13;
	m_mtx._32 =_cameramat._23; 
	m_mtx._33 =_cameramat._33;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_mtx._14=0;
	m_mtx._24=0;
	m_mtx._34=0;
	m_mtx._44=1;
}

void CBillBoard::CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat, float _radian)
{
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;
	XMFLOAT4X4 mtx = _cameramat;

	//Y�������o��
	axisY.x = mtx._11;
	axisY.y = mtx._12;
	axisY.z = mtx._13;
	axisY.w = 0.0f;

	XMFLOAT4 qt, qty;//�N�H�[�^�j�I��

	//�s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(mtx, qt);

	DX11QtRotationAxis(qty, axisY, _radian);//�w��m�x���ς���

	//�N�I�[�^�j�I������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qty);

	//�N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt1, tempqt1);

	//�N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(mtx, tempqt1);


	m_mtx._11 = mtx._11;
	m_mtx._12 = mtx._12;
	m_mtx._13 = mtx._13;

	m_mtx._21 = mtx._21;
	m_mtx._22 = mtx._22;
	m_mtx._23 = mtx._23;

	m_mtx._31 = mtx._31;
	m_mtx._32 = mtx._32;
	m_mtx._33 = mtx._33;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_mtx._14 = 0;
	m_mtx._24 = 0;
	m_mtx._34 = 0;
	m_mtx._44 = 1;
}

// �`��
void CBillBoard::Draw() {
	// �y�o�b�t�@������
	TurnOffZbuffer();

	ID3D11DeviceContext* devcontext;

	devcontext = GetDX11DeviceContext();

	// �V�F�[�_�[���\�[�X�r���[���s�N�Z���V�F�[�_�[�փZ�b�g
	devcontext->PSSetShaderResources(0, 1, &m_srv);

	// ���[���h�ϊ��s��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mtx);

	// ���_�o�b�t�@���Z�b�g
	unsigned int stride = sizeof(MyVertex);
	unsigned  offset = 0;

	// ���_�o�b�t�@���Z�b�g
	devcontext->IASetVertexBuffers(0, 1, &m_vbuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
//	m_devcontext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(m_pVertexLayout);
	
	// 4�p�`�`��
	devcontext->VSSetShader(m_pVertexShader, nullptr, 0);	// ���_�V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(m_pPixelShader, nullptr, 0);	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->Draw(4,0);									// 4���_�`��(���_�o�b�t�@�݂̂ŕ`��)

	// �y�o�b�t�@�L����
	TurnOnZbuffer();
}

// �r���{�[�h��`��
void CBillBoard::DrawBillBoard(const XMFLOAT4X4& _cameramat){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(_cameramat);

	// �`��
	Draw();
}

void CBillBoard::DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat, float _radian)
{
	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(_cameramat,_radian);

	// �`��
	Draw();
}

// �r���{�[�h��`��
void CBillBoard::DrawBillBoardAdd(const XMFLOAT4X4& _cameramat){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(_cameramat);

	// �`��
	//�A���t�@�u�����f�B���O���Z�b�g����
	SetBlendStateOne();

	// �`��
	Draw();

	//�A���t�@�u�����f�B���O���Z�b�g����
	SetBlendStateDefault();

	//�A���t�@�u�����f�B���O�Ŕ������������Z�b�g����
}

void CBillBoard::SetBlendStateDefault() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	devcontext->OMSetBlendState(m_pBlendStateDefault, blendFactor, 0xffffffff);
}

void CBillBoard::SetBlendStateSrcAlpha() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();
	devcontext->OMSetBlendState(m_pBlendStateSrcAlpha, blendFactor, 0xffffffff);
}

void CBillBoard::SetBlendStateOne() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//�F�Ɠ����x�w��
	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	devcontext->OMSetBlendState(m_pBlendStateOne, blendFactor, 0xffffffff);
}

void CBillBoard::CreateBlendStateSrcAlpha(){
	
	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &m_pBlendStateSrcAlpha);
}

void CBillBoard::CreateBlendStateOne() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &m_pBlendStateOne);
}

void CBillBoard::CreateBlendStateDefault() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &m_pBlendStateDefault);
}

bool CBillBoard::Init(DirectX::XMFLOAT3 _pos,
	float _xsize, float _ysize, DirectX::XMFLOAT4 _color){
	
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
		m_XSize = _xsize;
		m_YSize = _ysize;
		m_Color = _color;

		// �f�o�C�X�擾
		ID3D11Device* dev = GetDX11Device();
		// �f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		bool sts = CreateVertexShader(dev,
			"shader/vsbillboard.hlsl",
			"main",
			"vs_4_0",
			layout,
			numElements,
			&m_pVertexShader,
			&m_pVertexLayout);

		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			dev,							// �f�o�C�X�I�u�W�F�N�g
			"shader/psbillboard.hlsl",
			"main",
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		CalcVertex();						// �r���{�[�h�p�̒��_�f�[�^�쐬	

		CreateBlendStateSrcAlpha();			// �A���t�@�u�����f�B���O�p�u�����h�X�e�[�g����
		CreateBlendStateOne();				// ���Z�����p�̃u�����h�X�e�[�g����
		CreateBlendStateDefault();			// �f�t�H���g�̃u�����h�X�e�[�g����

		return true;

}

bool CBillBoard::Init(DirectX::XMFLOAT3 _pos, float _xsize, float _ysize,
	DirectX::XMFLOAT4 _color, float _widthpartition, float _heightpartition, float _heightnum){
	
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
		m_XSize = _xsize;
		m_YSize = _ysize;
		m_Color = _color;

		// �f�o�C�X�擾
		ID3D11Device* dev = GetDX11Device();
		// �f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		bool sts = CreateVertexShader(dev,
			"shader/vsbillboard.hlsl",
			"main",
			"vs_4_0",
			layout,
			numElements,
			&m_pVertexShader,
			&m_pVertexLayout);

		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			dev,							// �f�o�C�X�I�u�W�F�N�g
			"shader/psbillboard.hlsl",
			"main",
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		CalcVertex();						// �r���{�[�h�p�̒��_�f�[�^�쐬	

		CreateBlendStateSrcAlpha();			// �A���t�@�u�����f�B���O�p�u�����h�X�e�[�g����
		CreateBlendStateOne();				// ���Z�����p�̃u�����h�X�e�[�g����
		CreateBlendStateDefault();			// �f�t�H���g�̃u�����h�X�e�[�g����

		//�A�j���p�̏�����
		m_idx = 0.0f;
		m_loop = 0;
		m_widthpartition = _widthpartition;//�������������邩�̐�
		m_heightpartition = _heightpartition;//�c�����������邩�̐�
		m_heightnum = _heightnum;//�c�̉���ڂ��g�p���邩�̐�

		return true;
	
}

void CBillBoard::Dispose(){
	if (m_vbuffer != nullptr) {
		m_vbuffer->Release();
		m_vbuffer = nullptr;
	}

	if (m_srv != nullptr) {
		m_srv->Release();
		m_srv = nullptr;
	}

	if (m_pBlendStateOne != nullptr) {
		m_pBlendStateOne->Release();
		m_pBlendStateOne = nullptr;
	}

	if (m_pBlendStateSrcAlpha != nullptr) {
		m_pBlendStateSrcAlpha->Release();
		m_pBlendStateSrcAlpha = nullptr;
	}

	if (m_pBlendStateDefault != nullptr) {
		m_pBlendStateDefault->Release();
		m_pBlendStateDefault = nullptr;
	}

	if (m_pPixelShader != nullptr) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	if (m_pVertexShader != nullptr) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	if (m_pVertexLayout != nullptr) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}

// �r���{�[�h��`��(�y���ŉ�])
void CBillBoard::DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &_cameramat, float _angle){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(_cameramat);

	// �y����]�s����쐬
	DirectX::XMFLOAT4X4 matRotZ;
	DirectX::XMFLOAT3 axisZ = { _cameramat._13, _cameramat._23, _cameramat._33 };

	// �w�莲��]�̍s����쐬����
	DX11MtxRotationAxis(axisZ, _angle, matRotZ);

	// �s����������A�ʒu���Z�b�g
	DX11MtxMultiply(m_mtx, m_mtx, matRotZ);
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
	
	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateSrcAlpha();

	// �`��
	Draw();

	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateSrcAlpha();

}

//	�e�N�X�`���ǂݍ���
bool CBillBoard::LoadTexTure(const char* _filename){

	// ���łɓǂݍ��܂�Ă���e�N�X�`��������Ή������
	if(m_srv!=nullptr){
		m_srv->Release();
		m_srv=nullptr;
	}

	ID3D11Device* dev = GetDX11Device();
	ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

	bool sts = CreateSRVfromFile(_filename,
		dev, devcontext, &m_res,&m_srv);

	if (sts == false) {
		MessageBox(nullptr, "textureload error(CBillBoard LoadTexture)", "error", MB_OK);
		return false;
	}

	return true;
}

void CBillBoard::updateAnimeLoop(int _interval){
	//2D�`��ݒ�J�n
	m_loop++;
	if (m_loop == _interval){//���t���[���ŃA�j����؂�ւ��邩
		m_idx++;
		if (m_idx >= m_widthpartition){//�A�j���R�}�ő吔
			m_idx = 0.0f;
		}

		m_loop = 0;

		DirectX::XMFLOAT2 uv[4];
		uv[0] = { (0.0f + m_idx) / m_widthpartition ,0.0f / m_heightpartition };
		uv[1] = { (1.0f + m_idx) / m_widthpartition ,0.0f / m_heightpartition };
		uv[2] = { (0.0f + m_idx) / m_widthpartition ,1.0f / m_heightpartition };
		uv[3] = { (1.0f + m_idx) / m_widthpartition ,1.0f / m_heightpartition };
		SetUV(uv);
	}
	else if (m_idx == 0.0f){//float����ɔ���
		DirectX::XMFLOAT2 uv[4];
		uv[0] = { 0.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
		uv[1] = { 1.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
		uv[2] = { 0.0f / m_widthpartition ,m_heightnum / m_heightpartition };
		uv[3] = { 1.0f / m_widthpartition ,m_heightnum / m_heightpartition };
		SetUV(uv);
	}
}

bool CBillBoard::updateAnime(int _interval)
{
	//�摜�̒[�܂ŕ`�悵�Ă��Ȃ����
	if (m_idx < m_widthpartition)
	{
		//2D�`��ݒ�J�n
		m_loop++;
		if (m_loop == _interval) {//���t���[���ŃA�j����؂�ւ��邩
			m_idx++;
			m_loop = 0;

			DirectX::XMFLOAT2 uv[4];
			uv[0] = { (0.0f + static_cast<float>(m_idx)) / m_widthpartition ,0.0f / m_heightpartition };
			uv[1] = { (1.0f + static_cast<float>(m_idx)) / m_widthpartition ,0.0f / m_heightpartition };
			uv[2] = { (0.0f + static_cast<float>(m_idx)) / m_widthpartition ,1.0f / m_heightpartition };
			uv[3] = { (1.0f + static_cast<float>(m_idx)) / m_widthpartition ,1.0f / m_heightpartition };
			SetUV(uv);
		}
		else if (m_idx == 0) {
			DirectX::XMFLOAT2 uv[4];
			uv[0] = { 0.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
			uv[1] = { 1.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
			uv[2] = { 0.0f / m_widthpartition ,m_heightnum / m_heightpartition };
			uv[3] = { 1.0f / m_widthpartition ,m_heightnum / m_heightpartition };
			SetUV(uv);
		}

		//�Ō�܂ł����Ă��Ȃ��Ȃ�U
		return false;
	}
	else
	{
		//�ő�܂ł����Ă���̂Ő^��Ԃ�
		return true;
	}
	
}

bool CBillBoard::updateAnime(int& _imageNum, int _count, int _interval)
{
	//�摜�̒[�܂ŕ`�悵�Ă��Ȃ����
	if (_imageNum < m_widthpartition)
	{
		//2D�`��ݒ�J�n
		//m_loop++;
		if (_count % _interval == 0) {//���t���[���ŃA�j����؂�ւ��邩
			_imageNum++;
			//m_loop = 0;

			DirectX::XMFLOAT2 uv[4];
			uv[0] = { (0.0f + static_cast<float>(_imageNum)) / m_widthpartition ,0.0f / m_heightpartition };
			uv[1] = { (1.0f + static_cast<float>(_imageNum)) / m_widthpartition ,0.0f / m_heightpartition };
			uv[2] = { (0.0f + static_cast<float>(_imageNum)) / m_widthpartition ,1.0f / m_heightpartition };
			uv[3] = { (1.0f + static_cast<float>(_imageNum)) / m_widthpartition ,1.0f / m_heightpartition };
			SetUV(uv);
		}
		else if (_imageNum == 0) {
			DirectX::XMFLOAT2 uv[4];
			uv[0] = { 0.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
			uv[1] = { 1.0f / m_widthpartition ,(m_heightnum - 1.0f) / m_heightpartition };
			uv[2] = { 0.0f / m_widthpartition ,m_heightnum / m_heightpartition };
			uv[3] = { 1.0f / m_widthpartition ,m_heightnum / m_heightpartition };
			SetUV(uv);
		}

		//�Ō�܂ł����Ă��Ȃ��Ȃ�U
		return false;
	}
	else
	{
		//�ő�܂ł����Ă���̂Ő^��Ԃ�
		return true;
	}

}
