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

// 使用する頂点シェーダー名
const char* vsfilename[] = {
	"shader/vsquad2d.hlsl"
};

// 使用するピクセルシェーダー名
const char* psfilename[] = {
	"shader/psquad2d.hlsl"
};

bool Quad2D::SetViewPortSize(uint32_t _width, uint32_t _height){
	ComPtr<ID3D11Buffer> CBViewPort;

	// ビューポート用の定数バッファ
	struct ConstantBufferViewPort {
		uint32_t	Width[4];			// ビューポート幅
		uint32_t	Height[4];			// ビューポート高さ
	};

	// ビューポート定数バッファに値をセット
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

	// 頂点シェーダーにセット
	GetDX11DeviceContext()->VSSetConstantBuffers(5, 1, CBViewPort.GetAddressOf());

	return true;
}

// 矩形の初期化
bool Quad2D::Init(uint32_t _width, 
	uint32_t _height, 
	DirectX::XMFLOAT4 _color ) {

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
	updateVertex(_width, _height, _color, *m_uv2);

	// デバイスを取得
	ID3D11Device* device;
	device = GetDX11Device();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Quad2D::Vertex),		// １頂点当たりバイト数
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

bool Quad2D::Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, 
					float _widthpartition, float _heightpartition, float _heightnum){

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
	updateVertex(_width, _height, _color, *m_uv2);

	// デバイスを取得
	ID3D11Device* device;
	device = GetDX11Device();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Quad2D::Vertex),		// １頂点当たりバイト数
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

	//アニメーションに必要な数値をセット
	//数値が０以下でなければ入れる
	if ((_widthpartition <= 0.0f) || (_heightpartition <= 0.0f) || (_heightnum <= 0.0f)){
		
		std::string str("UI初期化の際に、縦幅、横幅、使用縦列のどれかに０以下の数値が入っています ");//SRV型の変換に失敗
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
	//UIの初期化
	//XMFLOAT2 DVA;
	//g_UI.Init(50.0f, 50.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &DVA);
	Init(_width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	LoadTexture(_filename);
	//位置設定
	SetPosition(_x, _y, 0);//最大1290、730

	return false;
}

// 描画
void Quad2D::Draw() {
	//Zバッファoff
	TurnOffZbuffer();

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	// テクスチャをセット
	devcontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());

	// 座標変換用の行列をセット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー取得
	ID3D11VertexShader* vs;
	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps;
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// 頂点レイアウト取得
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス

	//Zバッファオン
	TurnOnZbuffer();
}

void Quad2D::UnInit() {

}

// 拡大、縮小
void Quad2D::SetScale(float _sx, float _sy, float _sz) {
	DX11MtxScale(_sx, _sy, _sz, m_worldmtx);
}

// 位置をセット
void Quad2D::SetPosition(float _x, float _y, float _z) {
	m_worldmtx._41 = _x;
	m_worldmtx._42 = _y;
	m_worldmtx._43 = _z;
}

// Z軸回転
void Quad2D::SetRotation(float _angle) {
	DX11MtxRotationZ(_angle, m_worldmtx);
}

void Quad2D::Flashing(float _value){
	alpha += _value;
	float sinValue = sin(alpha * DirectX::XM_PI);//サインを引き出す
	sinValue = sinValue / 2;//０～１になる様に割る
	sinValue += 0.5f;//サイン派の基準値をずらす
	if (alpha > 1000.0f)//カンストしない様に初期化
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
		float sinValue = sin(alpha * DirectX::XM_PI);//サインを引き出す
		sinValue = sinValue;//０～１になる様に割る
		//sinValue += 0.5f;//サイン派の基準値をずらす
		m_faddeEnd = sinValue;
		//m_colour.w = sinValue;
		//if (alpha > 1000.0f)//カンストしない様に初期化
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
		//フェード処理をしない
		return true;
	}
	
}

bool Quad2D::FaddeOUT(float _value)
{
	if (m_faddeEnd > 0.0f)
	{
		alpha += _value;
		float sinValue = sin(alpha * DirectX::XM_PI);//サインを引き出す
		sinValue = sinValue;
		//sinValue += 0.5f;//サイン派の基準値をずらす
		m_faddeEnd = sinValue;
		//if (alpha > 1000.0f)//カンストしない様に初期化
		//{
		//	alpha = 0.0f;
		//}

		updateVertex(m_width, m_height, XMFLOAT4(1.0f, 1.0f, 1.0f, sinValue), m_uv2[0]);

		updateVbuffer();
		return false;
	}
	else
	{
		//フェード処理をしない
		return true;
	}
}

void Quad2D::FaddeReset()
{
	alpha = 0.0f;			//アルファ値用数値
	m_faddeEnd = 0.0f;		//フェード処理を終わるかの判定に使う
}

// 頂点データ更新
void Quad2D::updateVertex(uint32_t _width, 
	uint32_t _height, 
	DirectX::XMFLOAT4 _color, 
	DirectX::XMFLOAT2* _uv) {

	m_width = static_cast<float>(_width);
	m_height = static_cast<float>(_height);

	Quad2D::Vertex	v[4] = {
		// 座標													// カラー	// UV	
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

// 頂点バッファ更新
void Quad2D::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetDX11DeviceContext()->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Quad2D::Vertex) * 4);
		GetDX11DeviceContext()->Unmap(m_vertexbuffer.Get(), 0);
	}
}

void Quad2D::updateAnime(int _interval){
	//2D描画設定開始
	m_loop++;
	if (m_loop == _interval)//何フレームでアニメを切り替えるか
	{
		m_idx++;
		if (m_idx >= m_widthpartition)//アニメコマ最大数
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
		//最初のコマの時のみ使用
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
	
		//2D描画設定開始
		m_loop++;
		if (m_loop == _interval)//何フレームでアニメを切り替えるか
		{
			m_idx++;
			if (m_idx >= m_widthpartition)//アニメコマ最大数
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
			//最初のコマの時のみ使用
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
	//2D描画設定開始
	m_idx = _idx;
		
	//最大コマ数より大きければ０に戻す
	if (m_idx >= m_widthpartition)//アニメコマ最大数
	{
		m_idx = 0.0f;
	}

	if (m_idx != 0.0f)//0でなければこちらを使う
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
		//最初のコマの時のみ使用
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
