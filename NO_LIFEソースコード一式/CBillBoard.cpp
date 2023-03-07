#include "CBillBoard.h"
#include "Shader.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#include "dx11util.h"

// ビルボードの頂点座標を計算
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

		// 頂点バッファ作成（後で変更可能な）
		bool sts = CreateVertexBufferWrite(dev, sizeof(MyVertex), 4, m_Vertex, &m_vbuffer);
		if (!sts) {
			MessageBox(nullptr, "create vertex buffer erro(CBillBoard)", "error", MB_OK);
		}
	}
	else {
		// 作成済みなら 内容を書き換える 
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

// サイズをセット
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

// カラー値をセット
void CBillBoard::SetColor(XMFLOAT4  _col){
	m_Color = _col;
	CalcVertex();
}

// ビルボード用の行列を生成
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

	//Y軸を取り出す
	axisY.x = mtx._11;
	axisY.y = mtx._12;
	axisY.z = mtx._13;
	axisY.w = 0.0f;

	XMFLOAT4 qt, qty;//クォータニオン

	//行列からクオータニオンを生成
	DX11GetQtfromMatrix(mtx, qt);

	DX11QtRotationAxis(qty, axisY, _radian);//指定確度分変える

	//クオータニオン合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qty);

	//クオータニオンをノーマライズ
	DX11QtNormalize(tempqt1, tempqt1);

	//クオータニオンから行列を作成
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

// 描画
void CBillBoard::Draw() {
	// Ｚバッファ無効化
	TurnOffZbuffer();

	ID3D11DeviceContext* devcontext;

	devcontext = GetDX11DeviceContext();

	// シェーダーリソースビューをピクセルシェーダーへセット
	devcontext->PSSetShaderResources(0, 1, &m_srv);

	// ワールド変換行列
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mtx);

	// 頂点バッファをセット
	unsigned int stride = sizeof(MyVertex);
	unsigned  offset = 0;

	// 頂点バッファをセット
	devcontext->IASetVertexBuffers(0, 1, &m_vbuffer, &stride, &offset);
	// インデックスバッファをセット
//	m_devcontext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(m_pVertexLayout);
	
	// 4角形描画
	devcontext->VSSetShader(m_pVertexShader, nullptr, 0);	// 頂点シェーダーをセット
	devcontext->PSSetShader(m_pPixelShader, nullptr, 0);	// ピクセルシェーダーをセット
	devcontext->Draw(4,0);									// 4頂点描画(頂点バッファのみで描画)

	// Ｚバッファ有効化
	TurnOnZbuffer();
}

// ビルボードを描画
void CBillBoard::DrawBillBoard(const XMFLOAT4X4& _cameramat){

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(_cameramat);

	// 描画
	Draw();
}

void CBillBoard::DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat, float _radian)
{
	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(_cameramat,_radian);

	// 描画
	Draw();
}

// ビルボードを描画
void CBillBoard::DrawBillBoardAdd(const XMFLOAT4X4& _cameramat){

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(_cameramat);

	// 描画
	//アルファブレンディングをセットする
	SetBlendStateOne();

	// 描画
	Draw();

	//アルファブレンディングをセットする
	SetBlendStateDefault();

	//アルファブレンディングで半透明合成をセットする
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
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//色と透明度指定
	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	devcontext->OMSetBlendState(m_pBlendStateOne, blendFactor, 0xffffffff);
}

void CBillBoard::CreateBlendStateSrcAlpha(){
	
	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

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
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

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
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

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

		// デバイス取得
		ID3D11Device* dev = GetDX11Device();
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
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

		// ピクセルシェーダーを生成
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			dev,							// デバイスオブジェクト
			"shader/psbillboard.hlsl",
			"main",
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		CalcVertex();						// ビルボード用の頂点データ作成	

		CreateBlendStateSrcAlpha();			// アルファブレンディング用ブレンドステート生成
		CreateBlendStateOne();				// 加算合成用のブレンドステート生成
		CreateBlendStateDefault();			// デフォルトのブレンドステート生成

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

		// デバイス取得
		ID3D11Device* dev = GetDX11Device();
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
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

		// ピクセルシェーダーを生成
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			dev,							// デバイスオブジェクト
			"shader/psbillboard.hlsl",
			"main",
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		CalcVertex();						// ビルボード用の頂点データ作成	

		CreateBlendStateSrcAlpha();			// アルファブレンディング用ブレンドステート生成
		CreateBlendStateOne();				// 加算合成用のブレンドステート生成
		CreateBlendStateDefault();			// デフォルトのブレンドステート生成

		//アニメ用の初期化
		m_idx = 0.0f;
		m_loop = 0;
		m_widthpartition = _widthpartition;//横を何分割するかの数
		m_heightpartition = _heightpartition;//縦を何分割するかの数
		m_heightnum = _heightnum;//縦の何列目を使用するかの数

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

// ビルボードを描画(Ｚ軸で回転)
void CBillBoard::DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &_cameramat, float _angle){

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(_cameramat);

	// Ｚ軸回転行列を作成
	DirectX::XMFLOAT4X4 matRotZ;
	DirectX::XMFLOAT3 axisZ = { _cameramat._13, _cameramat._23, _cameramat._33 };

	// 指定軸回転の行列を作成する
	DX11MtxRotationAxis(axisZ, _angle, matRotZ);

	// 行列を合成し、位置をセット
	DX11MtxMultiply(m_mtx, m_mtx, matRotZ);
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
	
	//アルファブレンディングをセットする
//	SetBlendStateSrcAlpha();

	// 描画
	Draw();

	//アルファブレンディングをセットする
//	SetBlendStateSrcAlpha();

}

//	テクスチャ読み込み
bool CBillBoard::LoadTexTure(const char* _filename){

	// すでに読み込まれているテクスチャがあれば解放する
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
	//2D描画設定開始
	m_loop++;
	if (m_loop == _interval){//何フレームでアニメを切り替えるか
		m_idx++;
		if (m_idx >= m_widthpartition){//アニメコマ最大数
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
	else if (m_idx == 0.0f){//float相手に判定
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
	//画像の端まで描画していなければ
	if (m_idx < m_widthpartition)
	{
		//2D描画設定開始
		m_loop++;
		if (m_loop == _interval) {//何フレームでアニメを切り替えるか
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

		//最後までいっていないなら偽
		return false;
	}
	else
	{
		//最大までいっているので真を返す
		return true;
	}
	
}

bool CBillBoard::updateAnime(int& _imageNum, int _count, int _interval)
{
	//画像の端まで描画していなければ
	if (_imageNum < m_widthpartition)
	{
		//2D描画設定開始
		//m_loop++;
		if (_count % _interval == 0) {//何フレームでアニメを切り替えるか
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

		//最後までいっていないなら偽
		return false;
	}
	else
	{
		//最大までいっているので真を返す
		return true;
	}

}
