#include "stand.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Stand::Init(){
	//行列初期化
	DX11MtxIdentity(m_mtx);

	//台の位置を下げる、mtxで書き出しているのでそこをいじる。
	m_mtx._42 -= 1.5f;

	//モデルを設定
	m_modelNum = ALLMODEL::STAGE_NORMAL1;

	return true;
}

void Stand::Draw(){
	//モデル描画
	//m_model->Draw(m_mtx);
}

void Stand::Draw_transform(float _x, float _y, float _z){
	DirectX::XMFLOAT4X4 big, big2;
	DX11MtxScale(_x, _y, _z, big);//大きくするための4x4を作製
	DX11MtxMultiply(big2, big, m_mtx);//先頭に後ろ2つをかけたものを入れる、現在位置
	
	//m_model->Draw(big2);
	ModelMgr::GetInstance().Draw(ALLMODEL::STAGE_NORMAL1, big2);
}

void Stand::Update(){
	//今は何も無し
}

void Stand::Finalize(){
	//m_model->Uninit();
}


