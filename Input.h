//------------------------------------------------------------------
//! @file  Input.h
//! @brief キーボード・マウスとコントローラーの入力処理
//! @autor Noriyuki Iwamoto
//! @date  7/1～
//------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <dinput.h>
#include <wrl/client.h>
#include "CWindow.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"xinput.lib")
#pragma comment (lib,"dxguid.lib")
#pragma warning(disable : 4995)

using Microsoft::WRL::ComPtr;

/// <summary>
/// 入力クラス
/// </summary>
class Input {
public:
	enum class Vibration {
		NONE,		//振動なし
		SOON,		//普通
		GRADUALLY,  //徐々に
		WAVE_UP,	//波打ち(上昇)
		WAVE_DOWN,	//波打ち(下降)
	};
private:
	/*************************************************************
	* コントローラー用
	*************************************************************/
	_XINPUT_STATE		 x_state;						//今の状態
	WORD			     x_statebuffer;					//現在使用しておらず
	_XINPUT_STATE		 x_buffer;						//過去の状態(1フレーム前)
	Vibration			 x_vibstate;					//今の振動ステータス
	_XINPUT_VIBRATION	 x_vibration;					//振動
	float ThresholdL = 65535 / 4; //しきい値、プラスとマイナスの幅を足したものを４で割っている
	float ThresholdR = 15000; //しきい値、方向向かせるよう

	/*************************************************************
	* キーボード・マウス用
	*************************************************************/
	ComPtr<IDirectInput8>		m_dinput;
	ComPtr<IDirectInputDevice8>	m_dikeyboard;
	ComPtr<IDirectInputDevice8>	m_dimouse;
	char					m_keybuffer[256];		// キーボードバッファ
	char					m_oldkeybuffer[256];	// 前回の入力キーボードバッファ
	DIMOUSESTATE2			m_MouseState;			// マウスの状態
	DIMOUSESTATE2			m_MouseBuffer;			// 過去のマウスの状態
	DIMOUSESTATE2			m_MouseStateTrigger;	// マウスの状態(Trigger)
	DIMOUSESTATE2			m_MouseStateRelease;	// マウスの状態(Release)
	POINT					m_MousePoint;			// マウス座標
	int						m_width;				// マウスのＸ座標最大
	int						m_height;				// マウスのＹ座標最大
	HWND					m_hwnd;
	Input() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr) {}
public:
	Input(const Input&) = delete;//中身無し
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;//中身無し
	Input& operator=(Input&&) = delete;
	static Input& GetInstance() {
		static Input Instance;
		return Instance;
	}
	~Input() { XInputEnable(false); }

	/*************************************************************
	* 両方
	*************************************************************/
	//初期処理、２つを統合
	void Init(HINSTANCE, HWND, int, int);		
	//キーボード・マウス初期化
	bool KeyBoardInit(HINSTANCE, HWND, int, int);	
	//コントローラー初期化
	bool ControllerInit();	
	//更新処理
	void Update();				

	/*************************************************************
	* コントローラー用
	*************************************************************/
	//コントローラー接続確認
	bool CheckControllerConnection();	
	//コントローラー状態更新
	void ControllerState();
	//右スティック取得
	XMFLOAT2 GetRStick();
	//右スティック取得
	XMFLOAT2 GetRStickPlayer();
	//左スティック取得
	XMFLOAT2 GetLStick();
	//左スティック取得
	XMFLOAT2 GetLStickPlayer();
	//右トリガー情報取得
	float GetRTrigger();						
	//左トリガー情報取得
	float GetLTrigger();							
	//コントローラー入力押下(継続)
	bool CheckButtonPress(WORD);						
	//コントローラー入力押下(瞬間)
	bool CheckButtonTrigger(WORD);						
	//コントローラー入力押上(瞬間)
	bool CheckButtonRelease(WORD);
	//どれかのボタンが押されたら真を返す処理
	bool CheckButtonAll();
	//振動更新処理
	void VibrationUpdate();
	//振動開始
	void SetVibration(float,float,Vibration);		
	//振動停止
	void StopVibration();

	/*************************************************************
	* キーボード・マウス用
	*************************************************************/
	//マウス状態取得
	void MouseState();
	//マウス座標取得
	XMINT2 GetMousePos()const;
	//マウスホイール取得
	int GetMouseWheel();
	//マウス左押下(継続)
	bool CheckMouseLButtonPress()const;
	//マウス右押下(継続)
	bool CheckMouseRButtonPress()const;		
	//マウス中央押下(継続)
	bool CheckMouseCButtonPress()const;	
	//マウス左押下(瞬間)
	bool CheckMouseLButtonTrigger()const;	
	//マウス右押下(瞬間)
	bool CheckMouseRButtonTrigger()const;		
	//マウス中央押下(瞬間)
	bool CheckMouseCButtonTrigger()const;	
	//マウス左押上(瞬間)
	bool CheckMouseLButtonRelease()const;		
	//マウス右押上(瞬間)
	bool CheckMouseRButtonRelease()const;
	//マウス中央押上(瞬間)
	bool CheckMouseCButtonRelease()const;
	//キーボード更新
	void KeyBoardState();
	//キー押下(継続)
	bool CheckKeyPress(int);
	//キー押下(瞬間)
	bool CheckKeyTrigger(int);				
	//キー押上(瞬間)
	bool CheckKeyRelease(int);
	//どれかのキーが押されたら真を返す処理
	bool CheckKeyAll();
};