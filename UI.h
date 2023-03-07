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

// comptr、スマートポインタ宣言
using Microsoft::WRL::ComPtr;

// 矩形クラス
class UI : public Uncopyable {
public:
	// 頂点データ
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;		// 座標		
		DirectX::XMFLOAT4 color;	// カラー
		DirectX::XMFLOAT2 tex;		// テクスチャ
	};
private:
	DirectX::XMFLOAT4X4		m_worldmtx;				// ワールド変換行列
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// インデックスバッファ
	UI::Vertex				m_vertex[4];			// 矩形４頂点
	float					m_width;				// 幅
	float					m_height;				// 高さ
	ComPtr<ID3D11ShaderResourceView> m_srv;			// SRV（シェーダーリソースビュー）
	ComPtr<ID3D11Resource>	m_tex;					// テクスチャリソース
	DirectX::XMFLOAT4		m_colour;				// 色の情報

public:
	/*************************************************************
	* 凡庸処理
	*************************************************************/
	
	// 矩形の初期化
	bool InitSetting(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	//初期化に必要なものをまとめた関数
	bool Init(const char* _filename, uint32_t _width, uint32_t _height, float _x, float _y);
	// 描画
	void Draw();
	//削除、今は何もない
	//開放処理はスマートポインタで行っている
	void UnInit();
	// テクスチャセット、読み込み
	void LoadTexture(const char* _filename);

public:
	/*************************************************************
	* アップデート関数
	*************************************************************/
	//アニメーションしない場合、アップデートのみ行う
	void updateAnimeNo();
	//アニメーションしない場合、大きさ変更
	void updateAnimeNoSize(uint32_t _width, uint32_t _height);
	//アニメーションしない場合、色変更
	void updateAnimeNoColour(XMFLOAT4 _colour);
	//ポジション変更
	void updateAnimeNoPos(float _x, float _y);
	//アニメーションしない場合、大きさ変更と色変更
	void updateAnimeNoSizeColour(uint32_t _width, uint32_t _height, XMFLOAT4 _colour);
	//大きさとポジション変更
	void updateAnimeNoSizePos(uint32_t _width, uint32_t _height, float _x, float _y);
	//色とポジション変更
	void updateAnimeNoColourPos(XMFLOAT4 _colour, float _x, float _y);
	//大きさと色とポジション変更
	void updateAnimeNoSizeColourPos(uint32_t _width, uint32_t _height, XMFLOAT4 _colour, float _x, float _y);

public:
	/*************************************************************
	* その他の処理
	*************************************************************/
	// Z軸回転
	void SetRotation(float _angle);
	// 頂点データ更新
	void updateVertex(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	// 頂点バッファ更新
	void updateVbuffer();
private:
	/*************************************************************
	* プライベート
	*************************************************************/
	// ビューポートのサイズをセット、描画時の大きさ
	bool SetViewPortSize(uint32_t _width, uint32_t _height);

	/*************************************************************
	* 凡庸処理、取得系
	*************************************************************/
	
	// 矩形の進行方向を取得（元画像が上向きに作られていることを前提）
	DirectX::XMFLOAT3 GetForward() {
		DirectX::XMFLOAT3 forward;
		forward.x = m_worldmtx._21;
		forward.y = m_worldmtx._22;
		forward.z = m_worldmtx._23;
		return forward;
	}

	// 矩形の横方向を取得
	DirectX::XMFLOAT3 GetSide() {
		DirectX::XMFLOAT3 side;
		side.x = m_worldmtx._11;
		side.y = m_worldmtx._12;
		side.z = m_worldmtx._13;
		return side;
	}

	//XY座標を返す
	XMFLOAT2 GetXYPos() {
		return XMFLOAT2(m_worldmtx._41, m_worldmtx._42);
	}
	//サイズを返す
	XMFLOAT2 GetSize() {
		return XMFLOAT2(m_width, m_height);
	}
	//サイズを返す
	XMFLOAT4 GetColour() {
		return m_colour;
	}

};