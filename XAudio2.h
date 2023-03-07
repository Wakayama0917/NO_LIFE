//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>
#include"singleton.h"
#include<vector>
#include<unordered_map>
#include"SupervisionScene.h"
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


enum SOUND {
	//タイトル


	//ステージセレクト


	//ゲームシーン
	SOUND_LABEL_PSHOOT_SE,				//プレイヤーの弾を撃つSE
	SOUND_LABEL_PHIT_SE,				//プレイヤーがダメージを受けるSE
	SOUND_LABEL_PDEAD_SE,				//プレイヤーが死ぬSE
	SOUND_LABEL_EHIT_SE,				//敵に弾が当たるSE
	SOUND_LABEL_EDEATH_SE,				//敵が死ぬSE
	SOUND_LABEL_GAME01_BGM,				//メインゲームBGM01

	//リザルト

	//オプション
	SOUND_LABEL_OPTION_DECISION_SE,		//オプションSE決定
	SOUND_LABEL_OPTION_BACK_SE,			//オプションSE戻る
	SOUND_LABEL_OPTION_CURSOR_SE,		//オプションSEカーソル
	SOUND_LABEL_NOTE_BGM,				//ノートBGM
};

// パラメータ構造体
typedef struct
{
	LPCSTR filename;	// 音声ファイルまでのパスを設定
	bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
} PARAM;

class Sound :public singleton<Sound>
{
public:
	explicit Sound(token);
	~Sound();
	void SetVolumeAll(float _volume);//全体の音量を変更
	void SetScene(Scene_Transition scenename);
	void Play(SOUND label);	 //音声の開始(0から)
	void Stop(SOUND label);   //音声の停止
	void Pause(SOUND label);  //音声の一時停止
	void Restart(SOUND label);//音声の途中再生
	void SetVolume(SOUND label, float volume);
	void Release(Scene_Transition scenename);//解放
private:
	float m_volume = 1.0f;//サウンドのボリューム、全体を一括

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

	//PARAM g_param[SOUND_LABEL_MAX];

	IXAudio2* g_pXAudio2 = NULL;
	IXAudio2MasteringVoice* g_pMasteringVoice = NULL;
	std::unordered_map<SOUND, IXAudio2SourceVoice*> g_pSourceVoice;

	std::vector<PARAM> TitleSound;
	std::vector<PARAM> StageSelectSound;
	std::vector<PARAM> GameSceneSound;
	std::vector<PARAM> ResultSound;
	std::vector<PARAM> StorySound;
	std::vector<PARAM> OptionSound;

	std::unordered_map<SOUND, WAVEFORMATEXTENSIBLE>g_wfx;			// WAVフォーマット
	std::unordered_map<SOUND, XAUDIO2_BUFFER>	g_buffer;
	std::unordered_map<SOUND, BYTE*> g_DataBuffer;
};

#endif
