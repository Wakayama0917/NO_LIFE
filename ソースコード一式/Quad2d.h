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


// comptr、スマートポインタ宣言
using Microsoft::WRL::ComPtr;

// 矩形クラス
class Quad2D : public Uncopyable{
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
	Quad2D::Vertex			m_vertex[4];			// 矩形４頂点
	float					m_width;				// 幅
	float					m_height;				// 高さ
	ComPtr<ID3D11ShaderResourceView> m_srv;			// SRV（シェーダーリソースビュー）
	ComPtr<ID3D11Resource>	m_tex;					// テクスチャリソース
	DirectX::XMFLOAT4		m_colour;				// 色の情報
	float					alpha = 0.0f;			//アルファ値計算用数値
	float					m_faddeEnd = 0.0f;		//フェード処理を終わるかの判定に使う
	

	//アニメ用
	int m_loop;//アニメを切り替えるフレーム数、固定なのでここで入れる
	float m_idx;//何番のコマを使うか決める数、固定なのでここで入れる
	//上記の二つを初期化で取り込んで、その数を覚えさせてアップデート内で初期化すれば
	//指定コマからのアニメ可能
	float m_widthpartition;//横を何分割するかの数
	float m_heightpartition;//縦を何分割するかの数
	float m_heightnum;//縦の何列目を使用するかの数

	//最初のコマの配列、1/1の場合
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
	* 凡庸処理
	*************************************************************/
	
	// 矩形の初期化
	bool Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color);
	// 矩形の初期化アニメ,横幅分割数、縦幅分割数、使用縦列
	bool Init(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color,
				float _widthpartition,float _heightpartition,float _heightnum);
	//初期化に必要なものをまとめた関数
	bool UiInitSetting(const char* _filename, uint32_t _width, uint32_t _height, float _x, float _y);

	// 描画
	void Draw();

	//削除、今は何もない
	//開放処理はスマートポインタで行っている
	void UnInit();
public:
	/*************************************************************
	* アップデート：分割アニメーションするタイプの関数
	*************************************************************/
	// アニメーション更新、オーバーロード
	void updateAnime(int _interval);//何フレーム
	void updateAnime(int _interval, uint32_t _width, uint32_t _height);//何フレーム、大きさ変える時
	void updateAnimeOne(float _idx);//指定したコマを描画する。

	/*************************************************************
	* アップデート：分割アニメーションさせないタイプの関数
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
	* 凡庸処理、取得や変更を加える物
	*************************************************************/
	
	// Z軸回転
	void SetRotation(float _angle);

	//点滅処理、アルファ値増減、増減する値を入れる
	void Flashing(float _value);
	//画像を徐々に見えるようにする。フェードイン
	bool FaddeIN(float _value);
	//画像を徐々に透明にする。フェードアウト
	bool FaddeOUT(float _value);
	//フェード系の数値を初期化する
	void FaddeReset();

	//アルファ値を初期化する
	void FlashingReset() {
		alpha = 0.5f;
	}

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

	// 頂点データ更新
	void updateVertex(uint32_t _width, uint32_t _height, DirectX::XMFLOAT4 _color, DirectX::XMFLOAT2* _uv);
	
	// テクスチャセット、読み込み
	void LoadTexture(const char* _filename);

	//XY座標を返す
	XMFLOAT2 GetXYPos() {
		return XMFLOAT2(m_worldmtx._41, m_worldmtx._42);
	}
	//サイズを返す
	XMFLOAT2 GetSize() {
		return XMFLOAT2(m_width, m_height);
	}
	//カラーを返す
	XMFLOAT4 GetColour() {
		return m_colour;
	}
	//α値のフラグを返す
	float Getfadde() {
		return m_faddeEnd;
	}

private:
	/*************************************************************
	*内部処理のみで使用している物 
	*************************************************************/
	// 拡大、縮小
	//現在の行列に対してしている
	void SetScale(float _sx, float _sy, float _sz);

	// 位置をセット
	void SetPosition(float _x, float _y, float _z);

	// ビューポートのサイズをセット、描画時の大きさ
	bool SetViewPortSize(uint32_t _width, uint32_t _height);

	// 頂点バッファ更新
	void updateVbuffer();

};