#pragma once
#include "gameobject.h"
#include "CModel.h"
#include "dx11mathutil.h"

class Stand : public GameObject
{
public:
	bool Init();
	void Draw();
	void Draw_transform(float _x,float _y,float _z);//•ÏŒ`‚³‚¹‚é
	void Update();
	void Finalize();

	/*void SetModelStand(CModel* _p)
	{
		m_model = _p;
	}

	CModel* GetModel() {
		return m_model;
	}*/

private:
	//CModel* m_model;						// ‚R‚cƒ‚ƒfƒ‹

};

