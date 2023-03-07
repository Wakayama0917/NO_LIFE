#pragma once
#include <Windows.h>
//#include <windef.h>
#pragma comment (lib, "xinput.lib")
#include <xinput.h>



//struct XKEY_STATE
//{
//	bool XDIK_A;
//	bool XDIK_B;
//	bool XDIK_X;
//	bool XDIK_Y;
//	bool XDIK_UP;//十字キー上
//	bool XDIK_DOWN;//十字キー下
//	bool XDIK_LEFT;//十字キー左
//	bool XDIK_RIGHT;//十字キー右
//	bool XDIK_START;//スタート
//	bool XDIK_BACK;//オプション
//	bool XDIK_LEFT_SHOULDER;//LB
//	bool XDIK_RIGHT_SHOULDER;//RB
//	bool XDIK_LEFT_THUMBT;//L3
//	bool XDIK_RIGHT_THUMB;//R3
//	
//};

class PXinput
{
private:


	//XKEY_STATE KEY;//各ボタンの状態を持ってる奴
	struct XKEY
	{
		bool NOW_KEY[20];
		bool OLD_KEY[20];
		float LX, LY;//左スティックの数値
		float RX, RY;//右スティックの数値
	};
	XKEY m_xkeySts;
public:
	enum XKEY_NUM
	{
		XDIK_A,
		XDIK_B,
		XDIK_X,
		XDIK_Y,
		XDIK_UP,//十字キー上
		XDIK_DOWN,//十字キー下
		XDIK_LEFT,//十字キー左
		XDIK_RIGHT,//十字キー右
		XDIK_START,//スタート
		XDIK_BACK,//オプション
		XDIK_LEFT_SHOULDER,//LB
		XDIK_RIGHT_SHOULDER,//RB
		XDIK_LEFT_THUMBT,//L3
		XDIK_RIGHT_THUMB,//R3
		XDIK_END//最後の番号、for文用
	};
	
	
	void idle();//ボタン入力を更新

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		keyno :　チェックしたいキー番号
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool Trigger(bool keyno);//押された瞬間
	bool Release(bool keyno);//離した瞬間
	bool Check(bool keyno);//押されているか判断、長押し用


	XKEY XKEYreturn(){//数値を見る
		return m_xkeySts;
	}

	//XINPUT_STATE

private:
#define MAX_CONTROLLERS  4  //XInputが認識できるのは4つまで
#define Threshold   65535/4 //しきい値、プラスとマイナスの幅を足したものを４で割っている


	//何番目のコントローラーか判断するよう
	struct CONTROLER_STATE{
		XINPUT_STATE state;
		bool bConnected;
	};
	CONTROLER_STATE m_padNum[MAX_CONTROLLERS];//コントローラーの台数を判別
	
	
	HRESULT UpdateControllerState();//接続を調べる
	
};

