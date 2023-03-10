#include <DirectXMath.h>
#include "CCamera.h"

// 角度をラジアンに変更
inline float ToRad(float _angle) {
	return (_angle *  DirectX::XM_PI) / 180.0f;
}

//  
// 球面束縛カメラ(Ｚ軸回転なしバージョン)
//
void UpdateSphereCamera(
	float _radius,						// 半径(入力)
	float _elevation,					// 仰角（入力）
	float _azimuth,						// 方位角（入力）
	const DirectX::XMFLOAT3& _lookat)	// 注視点（入力）
{		

	DirectX::XMFLOAT3 eye;				// 視点

	// 仰角、方位角をラジアンに変換
	_elevation = ToRad(_elevation);
	_azimuth = ToRad(_azimuth);

	// 仰角に応じた半径を計算
	float r = _radius*sinf(_elevation);

	// 視点座標を計算
	eye.x = r*cosf(_azimuth);
	eye.y = _radius*cosf(_elevation);
	eye.z = r*sinf(_azimuth);

	// 上向きベクトルを計算(仰角をを９０度回転させるとできる)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = _elevation + ToRad(90.0f);			// 仰角を９０度足す

	r = _radius * sinf(elevationplus90);	

	up.x = r * cosf(_azimuth);				
	up.y = _radius * cosf(elevationplus90);
	up.z = r * sinf(_azimuth);

	// 視点ベクトルを注視点分平行移動
	eye.x += _lookat.x;
	eye.y += _lookat.y;
	eye.z += _lookat.z;

	// ビュー変換行列を作成する
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(_lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
