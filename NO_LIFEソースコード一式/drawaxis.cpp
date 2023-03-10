#include <DirectXMath.h>
#include <vector>
#include "line.h"

using namespace DirectX;

Line g_line;

void drawaxis(XMFLOAT4X4 _mtx,float _length,XMFLOAT3 _pos) {
	// ????????
	std::vector<Line::MyVertex> v;
	v.resize(2);

	static bool first = true;

	if (first) {
		// ??????
		v[0].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
		v[1].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);

		v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	
		g_line.Init(v);

		first = false;
	}

	// X??
	v[0].vertex.x = _pos.x + _mtx._11*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._12*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._13*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._11*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._12*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._13*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	g_line.SetVertex(v);
	g_line.Draw();


	// Y??
	v[0].vertex.x = _pos.x + _mtx._21*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._22*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._23*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._21*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._22*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._23*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	g_line.SetVertex(v);
	g_line.Draw();

	// Z??
	v[0].vertex.x = _pos.x + _mtx._31*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._32*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._33*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._31*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._32*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._33*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_line.SetVertex(v);
	g_line.Draw();

}