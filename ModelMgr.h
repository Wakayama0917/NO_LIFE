#pragma once
#include <memory>
#include <wrl/client.h>
#include <unordered_map>
//#include <DirectXMath.h>
#include "CModel.h"
#include "CSV.h"
#include "EnumAllModel.h"
#include "DX11Settransform.h"


using Microsoft::WRL::ComPtr;

class ModelMgr {
private:
	ModelMgr(){
	}
	std::unordered_map<std::string, std::unique_ptr<CModel>> m_modelhashmap;

	//使用するモデルを読み込む
	std::unique_ptr<CSVFile> m_csvModelData;

public:
	ModelMgr(const ModelMgr&) = delete;
	ModelMgr& operator=(const ModelMgr&) = delete;
	ModelMgr(ModelMgr&&) = delete;
	ModelMgr& operator=(ModelMgr&&) = delete;

	static ModelMgr& GetInstance() {
		static ModelMgr Instance;
		return Instance;
	}

	virtual ~ModelMgr() {
		Finalize();
	}

	void Finalize() {
		for (auto itr = m_modelhashmap.begin(); itr != m_modelhashmap.end(); ++itr) {
			(itr->second)->Uninit();
		}
	}

	//CSV実体化させてそれに書いてる全てのモデルを読み込み
	void Init();
	//モデル初期化、シェーダー情報なども生成
	bool LoadModel(std::string _modelfilename, std::string _vsfilename, std::string _psfilename, std::string _texfilefolder);
	//指定したモデルを描画する、引数はマトリックス
	void Draw(ALLMODEL _modelNum, XMFLOAT4X4& mtxworld);

	//指定されたファイルパスのモデルが存在すれば返す
	CModel* GetModelPtr(std::string _key) {
		// 存在するかを確かめる
		auto it = m_modelhashmap.find(_key);
		if (it == m_modelhashmap.end()) {
			return nullptr;
		}
		return m_modelhashmap[_key].get();
	}
	//指定されたモデルが存在すれば返す
	CModel* GetModelPtr(ALLMODEL _modelNum) {
		// 存在するかを確かめる
		auto it = m_modelhashmap.find(m_csvModelData->GetModelName().at(static_cast<int>(_modelNum)));
		if (it == m_modelhashmap.end()) {
			return nullptr;
		}
		return m_modelhashmap[m_csvModelData->GetModelName().at(static_cast<int>(_modelNum))].get();
	}
};