#include "skydome.h"
#include "dx11mathutil.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Skydome::Init() {
	//行列初期化
	DX11MtxIdentity(m_mtx);

	//モデルを設定
	m_modelNum = ALLMODEL::SKY_NORMAL1;

	return true;
}

void Skydome::Draw() {
	//モデル描画
	//m_model->Draw(m_mtx);
	ModelMgr::GetInstance().Draw(m_modelNum, m_mtx);
}

void Skydome::Update(){
	
}

void Skydome::Finalize(){
	//m_model->Uninit();
}


