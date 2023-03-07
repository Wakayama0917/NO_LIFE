#include <array>//配列、静的であるが
#include "CCamera.h"


void CCamera::CameraFixed(DirectX::XMFLOAT4X4 _getMtx) {//カメラ向き固定追従

	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤーの情報を受け取る

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y軸
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//カメラ位置計算
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;


	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos;//プレイヤーの位置を追いかけるように作製

	//カメラの位置を決める、向きを固定にするため定数をかける
	eye.x = pos.x + 0.0f * -100;
	eye.y = (pos.y + 45) + 0.0f * -100;//傾ける角度のために４５足している
	eye.z = pos.z + 1.0f * -100;

	//カメラ上向き、ジンバル無効化のため
	up = yaxis;

	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraFPS(DirectX::XMFLOAT4X4 _getMtx){//FPSカメラ
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤー情報を受け取る

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//注視点計算
	XMFLOAT3 lookat;
	lookat.x = mtx._41 + zaxis.x;
	lookat.y = mtx._42 + zaxis.y;
	lookat.z = mtx._43 + zaxis.z;

	//カメラ位置計算
	XMFLOAT3 eye;
	eye.x = mtx._41;
	eye.y = mtx._42;
	eye.z = mtx._43;

	//カメラ上向き
	XMFLOAT3 up;
	up.x = mtx._21;
	up.y = mtx._22;
	up.z = mtx._23;

	//カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::Camera2FPS(DirectX::XMFLOAT4X4 _getMtx){//FPSカメラ後ろ追従
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤーの情報を受け取る

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y軸
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//カメラ位置計算
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;


	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos;//プレイヤーの位置を追いかけるように作製

	//カメラの位置を決める
	eye.x = pos.x + zaxis.x * -100;
	eye.y = (pos.y + 45) + zaxis.y * -100;//傾ける角度のために４５足している
	eye.z = pos.z + zaxis.z * -100;

	//カメラ上向き、ジンバル無効化のため
	up = yaxis;
	
	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelayold(DirectX::XMFLOAT4X4 _getMtx){//遅れるカメラ
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤー情報格納

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y軸
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	//カメラ位置計算
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1){
		svidx = 0;
		firstflag = true;
	}

	//30フレーム前の情報を使用してカメラを作成
	int useidx;
	useidx = svidx - 30;
	if (useidx < 0){
		if (firstflag == false){
			useidx = 0;
		}
		else{
			useidx = svidx + zaxis.size() - 30;
		}
	}

	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	//カメラ上向き
	up = yaxis[useidx];

	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraEditor(DirectX::XMFLOAT4X4 _getMtx)
{
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤーの情報を受け取る

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//Y軸
	XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	//カメラ位置計算
	XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42 + 10;//カメラ位置を手前にずらす
	pos.z = mtx._43 + 10;//カメラ位置を前にずらす


	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos;//プレイヤーの位置を追いかけるように作製

	//カメラの位置を決める、向きを固定にするため定数をかける
	eye.x = pos.x + zaxis.x * -100;
	eye.y = (pos.y + 90) + zaxis.y * -100;//傾ける角度のために４５足している
	eye.z = pos.z + 1.0f * -40;

	//カメラ上向き、ジンバル無効化のため
	up = yaxis;

	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelay(DirectX::XMFLOAT4X4 _getMtx)
{
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤー情報格納

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y軸
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	static float AAA = 0;
	//static bool BBB = false;
	//if (BBB == false)
	//{
	//	AAA += 0.1;
	//	if (AAA >= 0.9f)
	//	{
	//		BBB = true;
	//	}
	//}
	//else if (BBB == true)
	//{
	//	AAA -= 0.1;
	//	if (AAA <= -0.9f)
	//	{
	//		//AAA = -0.1;
	//		BBB = false;
	//	}
	//}

	//カメラ位置計算
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41 + AAA;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1) {
		svidx = 0;
		firstflag = true;
	}

	//30フレーム前の情報を使用してカメラを作成
	int useidx;
	useidx = svidx - 1;
	if (useidx < 0) {
		if (firstflag == false) {
			useidx = 0;
		}
		else {
			useidx = svidx + zaxis.size() - 1;
		}
	}

	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos[useidx];

	/*eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;*/

	//eye.x = pos[useidx].x + 0.0f * -100;
	//eye.y = (pos[useidx].y + 115) + 0.0f * -100;//傾ける角度のために４５足している
	//eye.z = pos[useidx].z + 1.0f * -100;

	//static float AAA = 0;
	//static bool BBB = false;
	//if (BBB == false)
	//{
	//	AAA += 0.01;
	//	if (AAA >= 0.1f)
	//	{
	//		BBB = true;
	//	}
	//}
	//else if (BBB == true)
	//{
	//	AAA -= 0.01;
	//	if (AAA <= -0.1f)
	//	{
	//		AAA = -0.1;
	//		//BBB = false;
	//	}
	//}
	
	eye.x = pos[useidx].x + 0.0f * -100;
	eye.y = (pos[useidx].y + 115) + 0.0f * -100;//傾ける角度のために４５足している
	eye.z = pos[useidx].z + 1.0f * -100;

	//カメラ上向き
	up = yaxis[useidx];

	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::CameraDelay(PHitEffectFlg _pHitEffectFlg, DirectX::XMFLOAT4X4 _getMtx)
{
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = _getMtx;//プレイヤー情報格納

	//ビュー変換行列
	static int svidx = 0;
	static bool firstflag = false;

	//Z軸取得
	static std::array<XMFLOAT3, 100> zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	//Y軸
	static std::array<XMFLOAT3, 100> yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	//攻撃を受けたとき
	if (_pHitEffectFlg == PHitEffectFlg::HIT)
	{
		switch (m_cameraEffectFlg)
		{
		case CCamera::CameraEffectFlg::ONE: {
			m_HitNum += 0.2f;
			if (m_HitNum >= 1.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::TWO;
			}
		}
			break;
		case CCamera::CameraEffectFlg::TWO: {
			m_HitNum -= 0.2f;
			if (m_HitNum <= -1.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::FINISH;
			}
		}
			break;
		case CCamera::CameraEffectFlg::FINISH:
			m_HitNum = 0;
			m_cameraEffectFlg = CameraEffectFlg::ONE;
			m_sPlayer->SetPHitEffectFlg(PHitEffectFlg::NONE);
			break;
		default:
			break;
		}
	
	}

	//カメラ位置計算
	static std::array<XMFLOAT3, 100> pos;
	pos[svidx].x = mtx._41 + m_HitNum;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1) {
		svidx = 0;
		firstflag = true;
	}

	//30フレーム前の情報を使用してカメラを作成
	int useidx;
	useidx = svidx - 1;
	if (useidx < 0) {
		if (firstflag == false) {
			useidx = 0;
		}
		else {
			useidx = svidx + zaxis.size() - 1;
		}
	}

	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos[useidx];

	//プレイヤーが死んだときの挙動
	if (_pHitEffectFlg == PHitEffectFlg::DEATH)
	{
		switch (m_cameraEffectFlg)
		{
		case CCamera::CameraEffectFlg::ONE: {
			m_DeathNum += 0.002f;
			if (m_DeathNum >= 0.5f)
			{
				m_cameraEffectFlg = CameraEffectFlg::TWO;
			}
		}
		break;
		case CCamera::CameraEffectFlg::TWO: {
			m_DeathNum -= 0.002f;
			if (m_DeathNum <= -0.5f)
			{
				//繰り返す
				m_cameraEffectFlg = CameraEffectFlg::ONE;
			}
		}
		break;
		case CCamera::CameraEffectFlg::FINISH:
			break;
		default:
			break;
		}

	}
	eye.x = pos[useidx].x + m_DeathNum * -100;
	eye.y = (pos[useidx].y + 115) + 0.0f * -100;//傾ける角度のために４５足している
	eye.z = pos[useidx].z + 1.0f * -100;

	//カメラ上向き
	up = yaxis[useidx];

	//ビュー変換行列生成、カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	//上記でカメラクラス内に値を格納、その後値を反映させてセットする
	CCamera::GetInstance()->CreateCameraMatrix();
}

void CCamera::Reset()
{
	m_HitNum = 0;
	m_DeathNum = 0;
	m_cameraEffectFlg = CameraEffectFlg::ONE;
}

