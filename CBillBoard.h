#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "DX11util.h"
#include "Shader.h"
/*----------------------------------------------------------------------

	ビルボードクラス	

-----------------------------------------------------------------------*/

class CBillBoard{
private:
	DirectX::XMFLOAT4X4			m_mtx;			// ビルボード用の行列
	DirectX::XMFLOAT3			m_pos;			// ビルボードの位置座標
	float						m_XSize=100.0f;		// ビルボードのＸサイズ
	float						m_YSize=100.0f;		// ビルボードのＹサイズ
	DirectX::XMFLOAT4			m_Color= DirectX::XMFLOAT4(1,1,1,1);	// 頂点カラー値
	ID3D11ShaderResourceView* 	m_srv = nullptr;			// Shader Resourceviewテクスチャ
	ID3D11Buffer*				m_vbuffer = nullptr;		// 頂点バッファ
	ID3D11BlendState*			m_pBlendStateSrcAlpha = nullptr;//おそらく混ぜる色
	ID3D11BlendState*			m_pBlendStateOne = nullptr;
	ID3D11BlendState*			m_pBlendStateDefault = nullptr;
	ID3D11VertexShader*			m_pVertexShader = nullptr;	// 頂点シェーダー入れ物
	ID3D11PixelShader*			m_pPixelShader = nullptr;	// ピクセルシェーダー入れ物
	ID3D11InputLayout*			m_pVertexLayout = nullptr;	// 頂点フォーマット定義
	ID3D11Resource*				m_res = nullptr;

	// 頂点フォーマット
	struct MyVertex{
		float				x,y,z;
		DirectX::XMFLOAT4	color;
		float				tu,tv;//縦横幅
	};

	MyVertex				m_Vertex[4];			// ビルボードの頂点座標
	DirectX::XMFLOAT2		m_uv[4];				// テクスチャ座標

	//アニメ用
	int m_loop = 0;//アニメを切り替えるフレーム数、固定なのでここで入れる
	int m_idx = 0;//何番のコマを使うか決める数、固定なのでここで入れる
	//上記の二つを初期化で取り込んで、その数を覚えさせてアップデート内で初期化すれば
	//指定コマからのアニメ可能
	float m_widthpartition;//横を何分割するかの数
	float m_heightpartition;//縦を何分割するかの数
	float m_heightnum;//縦の何列目を使用するかの数

private:
	// ビルボードの頂点座標を計算
	void CalcVertex();
	// ビルボード用の行列を生成
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat);
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& _cameramat,float _radian);//確度をつける場合
	// 描画
	void Draw();
	// ソースアルファを設定する
	void SetBlendStateSrcAlpha();
	// ブレンドステートを生成する
	void CreateBlendStateSrcAlpha();
	// 加算合成を設定する
	void SetBlendStateOne();
	// ブレンドステート（加算合成）を生成する
	void CreateBlendStateOne();
	// デフォルトのブレンドステートを設定する
	void SetBlendStateDefault();
	// デフォルトのブレンドステートを生成する
	void CreateBlendStateDefault();
	// UV座標をセットする

public:
	void SetUV(DirectX::XMFLOAT2 uv[]) {//テクスチャ画像をセット
		m_uv[0] = uv[0];
		m_uv[1] = uv[1];
		m_uv[2] = uv[2];
		m_uv[3] = uv[3];
		CalcVertex();
	}
	/*CBillBoard():m_pos.x(0),m_y(0),m_z(0),m_srv(nullptr){
	}*/

	//アニメ無し
	//描画する為の初期化、書く位置、画像の大きさ、色。
	bool Init(DirectX::XMFLOAT3 _pos, float _xsize, float _ysize, DirectX::XMFLOAT4 _color);
	
	//アニメあり
	//描画する為の初期化、書く位置、画像の大きさ、色、画像の縦横分割数、使用列
	bool Init(DirectX::XMFLOAT3 _pos, float _xsize, float _ysize, DirectX::XMFLOAT4 _color,
		float _widthpartition, float _heightpartition, float _heightnum);
	
	//開放処理
	void Dispose();

	// デストラクタ
	//上記の開放処理を自動的に行うことでミスを無くす
	virtual ~CBillBoard(){
		Dispose();
	}

	// ＵＶ座標をセットする
	//ビルボードの頂点座標
	void SetUV(float u[],float v[]){
		m_Vertex[0].tu = u[0];
		m_Vertex[0].tv = v[0];

		m_Vertex[1].tu = u[1];
		m_Vertex[1].tv = v[1];

		m_Vertex[2].tu = u[2];
		m_Vertex[2].tv = v[2];

		m_Vertex[3].tu = u[3];
		m_Vertex[3].tv = v[3];

		CalcVertex();						// ビルボード用の頂点データ作成	
	}

	// 位置を指定
	void SetPosition(DirectX::XMFLOAT3 _pos) {//描画する位置を入れる
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_pos.z = _pos.z;
	}

	// ビルボードを描画
	void DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat);//描く姿勢
	void DrawBillBoard(const DirectX::XMFLOAT4X4& _cameramat,float _radian);//描く姿勢と確度

	// ビルボード描画加算合成
	//絵が重なった時に色を混ぜるか、赤と青の画像が重なった時に緑にするかみたいな感じ
	void DrawBillBoardAdd(const DirectX::XMFLOAT4X4& _cameramat);//上とほぼ同じ色を加算

	// ビルボードをZ軸を中心にして回転させて描画
	void DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &_cameramat, float _radian);
	
	// サイズをセット
	void SetSize(float _x, float _y);
	//サイズを指定した分だけ拡大
	void SetSizeAdd(float _x, float _y);

	// カラーをセット
	void SetColor(DirectX::XMFLOAT4 _col);//いつもの４つのやつ

	//	テクスチャ読み込み
	bool LoadTexTure(const char* _filename);//普通にファイル名入れたらOK

	//1枚を書き出す時よう
	void updateAnimeNo()
	{
		DirectX::XMFLOAT2 uv[4];
		uv[0] = { 0.0f , 0.0f};
		uv[1] = { 1.0f , 0.0f};
		uv[2] = { 0.0f , 1.0f};
		uv[3] = { 1.0f , 1.0f};
		SetUV(uv);
	}

	// アニメーション更新、ループ
	void updateAnimeLoop(int _interval);//何フレーム
	// アニメーション更新、１週したら終わり
	bool updateAnime(int _interval);//引数何フレーム
	// シングルトン用にカウントを外部から受け取る場合
	bool updateAnime(int& _imageNum,int _count, int _interval);//現在数値,引数何フレーム
	

};