#include "ModelMgr.h"

void ModelMgr::Init()
{
	//実体化
	m_csvModelData = std::make_unique<CSVFile>();
	//モデルのパスが入ったファイルを読み込む
	m_csvModelData->CsvRead("assets/csv/ModelDataAll.csv", ',');
	//全て読み込み
	for (int i = 0; i < m_csvModelData->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvModelData->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvModelData->GetTexFolder().at(i));
	}
}

bool ModelMgr::LoadModel(std::string _modelfilename, std::string _vsfilename,
	std::string _psfilename, std::string _texfilefolder){

	std::unique_ptr<CModel>	p;

	p = std::make_unique<CModel>();

	// モデル読み込み
	bool sts = p->Init(_modelfilename.c_str(), _vsfilename.c_str(), _psfilename.c_str(), _texfilefolder.c_str());
	if (!sts) {
		return false;
	}

	// unordered_mapコンテナに格納
	m_modelhashmap[_modelfilename].swap(p);

	return true;
}

void ModelMgr::Draw(ALLMODEL _modelNum, XMFLOAT4X4& _mtxworld)
{
	//指定されたモデルの実態を呼び出し、マトリックスを渡して描画
	ModelMgr::GetInstance().GetModelPtr(m_csvModelData->GetModelName().at(
		static_cast<int>(_modelNum)))->Draw(_mtxworld);
}
