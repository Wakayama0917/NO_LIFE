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

// 使用する頂点シェーダー名
const char* vsfilename[] = {
	"shader/vsquad2d.hlsl"
};

// 使用するピクセルシェーダー名
const char* psfilename[] = {
	"shader/psquad2d.hlsl"
};

bool UI::InitSetting(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv)
{
	DX11MtxIdentity(m_worldmtx);	// 初期姿勢

	m_colour = _color;//色を設定

	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error", "error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数

	// 頂点初期化
	updateVertex(_width, _height, _color, _uv);

	// デバイスを取得
	ID3D11Device* device;
	device = GetDX11Device();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(UI::Vertex),		// １頂点当たりバイト数
		4,							// 頂点数
		m_vertex,					// 頂点データ格納メモリ先頭アドレス
		&m_vertexbuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// インデックスバッファを生成
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// デバイスオブジェクト
		4,							// インデックス数
		index,						// インデックス格納メモリ先頭アドレス
		&m_indexbuffer				// インデックスバッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// ビューポートサイズセット
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
	// テクスチャ読み込み
	bool sts = CreateSRVfromFile(_filename,
		GetDX11Device(),
		GetDX11DeviceContext(),
		&m_tex, &m_srv);
	if (!sts) {
		std::string str("SRV 作成エラー ");//SRV型の変換に失敗
		std::string strfile(_filename);
		MessageBox(nullptr, (str + strfile).c_str(), "error", MB_OK);
		return;
	}
}
