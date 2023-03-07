//=============================================================================
//
// サウンド処理 [XAudio2.cpp]
//
//=============================================================================

#include "XAudio2.h"


//=============================================================================
// 初期化(ループ：BGM=true,SE=false)
//=============================================================================
Sound::Sound(token)
{
	//タイトル
	//TitleSound = {
	//};
	//ゲームシーン
	GameSceneSound = {
		{ "assets/SE/Game/PShoot01.wav",false },//プレイヤーの弾を撃つSE
		{ "assets/SE/Game/PHit.wav",false },//プレイヤーがダメージを受けるSE
		{ "assets/SE/Game/PDead.wav",false },//プレイヤーが死ぬSE
		{ "assets/SE/Game/EnemyHit.wav",false },//敵に弾が当たるSE
		{ "assets/SE/Game/EnemyDeath.wav",false },//敵が死ぬSE
		{ "assets/SE/Game/GameBGM01.wav",true },//メインゲームBGM01
	};
	//リザルト
	//ResultSound = {
	//};
	//オプション
	OptionSound = {
		{ "assets/SE/Game/PShoot01.wav",false },//プレイヤーの弾を撃つSE
		{ "assets/SE/Game/PHit.wav",false },//プレイヤーがダメージを受けるSE
		{ "assets/SE/Game/PDead.wav",false },//プレイヤーが死ぬSE
		{ "assets/SE/Game/EnemyHit.wav",false },//敵に弾が当たるSE
	};
	//PARAM g_param[SOUND_LABEL_MAX] = {
	//};
	HRESULT      hr;

	// ＣＯＭの初期化
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	hr = XAudio2Create(&g_pXAudio2, 0);		// 第二引数は､動作フラグ デバッグモードの指定(現在は未使用なので0にする)
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);		// 第三引数は、windowsでは無視
	if (FAILED(hr)) {
		//CoUninitialize();
	}

	/**** Create Mastering Voice ****/
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);			// 今回はＰＣのデフォルト設定に任せている
	/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL*/		// 本当６個の引数を持っている
	if (FAILED(hr)) {
		if (g_pXAudio2)	g_pXAudio2->Release();
		//CoUninitialize();
	}

	HANDLE               hFile;
	DWORD                dwChunkSize;
	DWORD                dwChunkPosition;
	DWORD                filetype;


	for (int n = SOUND_LABEL_OPTION_DECISION_SE; n < SOUND_LABEL_NOTE_BGM + 1; n++)
	{
		SOUND i = (SOUND)n;
		int a = n - SOUND_LABEL_OPTION_DECISION_SE;
		memset(&g_wfx[i], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&g_buffer[i], 0, sizeof(XAUDIO2_BUFFER));

		hFile = CreateFileA(OptionSound[a].filename, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);

		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &g_wfx[i], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		g_DataBuffer[i] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, g_DataBuffer[i], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		// 	サブミットボイスで利用するサブミットバッファの設定
		g_buffer[i].AudioBytes = dwChunkSize;
		g_buffer[i].pAudioData = g_DataBuffer[i];
		g_buffer[i].Flags = XAUDIO2_END_OF_STREAM;
		if (OptionSound[a].bLoop)
			g_buffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
		else
			g_buffer[i].LoopCount = 0;

		g_pXAudio2->CreateSourceVoice(&g_pSourceVoice[i], &(g_wfx[i].Format));
	}
	{
	///**** Initalize Sound ****/
	//for (int i = 0; i < SOUND_LABEL_MAX; i++)
	//{
	//	memset(&g_wfx[i], 0, sizeof(WAVEFORMATEXTENSIBLE));
	//	memset(&g_buffer[i], 0, sizeof(XAUDIO2_BUFFER));

	//	hFile = CreateFileA(g_param[i].filename, GENERIC_READ, FILE_SHARE_READ, NULL,
	//		OPEN_EXISTING, 0, NULL);

	//	//check the file type, should be fourccWAVE or 'XWMA'
	//	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	//	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

	//	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	//	ReadChunkData(hFile, &g_wfx[i], dwChunkSize, dwChunkPosition);

	//	//fill out the audio data buffer with the contents of the fourccDATA chunk
	//	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	//	g_DataBuffer[i] = new BYTE[dwChunkSize];
	//	ReadChunkData(hFile, g_DataBuffer[i], dwChunkSize, dwChunkPosition);

	//	CloseHandle(hFile);

	//	// 	サブミットボイスで利用するサブミットバッファの設定
	//	g_buffer[i].AudioBytes = dwChunkSize;
	//	g_buffer[i].pAudioData = g_DataBuffer[i];
	//	g_buffer[i].Flags = XAUDIO2_END_OF_STREAM;
	//	if (g_param[i].bLoop)
	//		g_buffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
	//	else
	//		g_buffer[i].LoopCount = 0;

	//	g_pXAudio2->CreateSourceVoice(&g_pSourceVoice[i], &(g_wfx[i].Format));
	//}
	}
}

Sound::~Sound()
{
	for (int n = 0; n < g_pSourceVoice.size(); n++)
	{
		SOUND i = (SOUND)n;
		if (g_pSourceVoice[i] != NULL)
		{
			g_pSourceVoice[i]->Stop(0);
			g_pSourceVoice[i]->FlushSourceBuffers();
			g_pSourceVoice[i]->DestroyVoice();			// オーディオグラフからソースボイスを削除
			g_pSourceVoice.erase(i);
			delete[]  g_DataBuffer[i];
			g_DataBuffer.erase(i);
		}
	}

	if (g_pMasteringVoice != nullptr)
		g_pMasteringVoice->DestroyVoice();

	if (g_pXAudio2) g_pXAudio2->Release();
}

void Sound::SetVolumeAll(float _volume)
{
	m_volume = _volume;
}


//=============================================================================
// 開放処理
//=============================================================================
void Sound::Release(Scene_Transition scenename)
{
	int start = 0, end = 0;
	switch (scenename)
	{
	case SCENE_ROGO:
		start = 0;
		end = 0;
		break;
	case SCENE_TITLE:
		/*start = SOUND_LABEL_TITLE_BGM;
		end = SOUND_LABEL_TITLE_CURSOR_SE + 1;*/
		break;
	case SCENE_STAGESELECT:
		/*start = SOUND_LABEL_STAGE_MEADOW_BGM;
		end = SOUND_LABEL_STAGE_DECISION_SE + 1;*/
		break;
	case SCENE_GAMESCENE:
		/*start = SOUND_LABEL_STAGECLEAR;
		end = SOUND_LABEL_LILITHBARRIER_SE + 1;*/
		start = SOUND_LABEL_PSHOOT_SE;
		end = SOUND_LABEL_GAME01_BGM + 1;
		break;
	case SCENE_RESULT:
		/*start = SOUND_LABEL_RESULT_WIN_BGM;
		end = SOUND_LABEL_PINKSTAREFFECT_SE + 1;*/
		break;
	default:
		start = 0;
		end = 0;
		break;
	}

	for (int n = start; n < end; n++)
	{
		SOUND i = (SOUND)n;
		if (g_pSourceVoice[i])
		{
			g_pSourceVoice[i]->Stop(0);
			g_pSourceVoice[i]->FlushSourceBuffers();
			g_pSourceVoice[i]->DestroyVoice();			// オーディオグラフからソースボイスを削除
			delete[]  g_DataBuffer[i];
			g_pSourceVoice.erase(i);
			g_DataBuffer.erase(i);
		}
	}
	if (end != 0 && start != 0) {
		g_pMasteringVoice->DestroyVoice();

		if (g_pXAudio2) g_pXAudio2->Release();
	}
	// ＣＯＭの破棄
	//CoUninitialize();
}

//=============================================================================
// 音量設定
//=============================================================================
void Sound::SetVolume(SOUND label, float volume)
{
	//音量設定
	g_pSourceVoice[label]->SetVolume(volume);
}

//=============================================================================
// 再生
//=============================================================================
void Sound::Play(SOUND label)
{
	// ソースボイス作成
	g_pXAudio2->CreateSourceVoice(&(g_pSourceVoice[label]), &(g_wfx[label].Format));
	g_pSourceVoice[label]->SubmitSourceBuffer(&(g_buffer[label]));	// ボイスキューに新しいオーディオバッファーを追加

	// 再生
	g_pSourceVoice[label]->Start(0);

	SetVolume(label, m_volume);

}

//=============================================================================
// 停止
//=============================================================================
void Sound::Stop(SOUND label)
{
	if (g_pSourceVoice[label] == NULL) return;

	XAUDIO2_VOICE_STATE xa2state;
	g_pSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued)
	{
		g_pSourceVoice[label]->Stop(0);
	}
}

//=============================================================================
// 一時停止
//=============================================================================
void Sound::Pause(SOUND label)
{
	g_pSourceVoice[label]->Stop(0);
}

//=============================================================================
// リスタート
//=============================================================================
void Sound::Restart(SOUND label)
{
	g_pSourceVoice[label]->Start(0);
}


void Sound::SetScene(Scene_Transition scenename)
{
	// ＣＯＭの初期化
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XAudio2Create(&g_pXAudio2, 0);
	g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	int start = 0, end = 0;
	std::vector<PARAM> sound;
	switch (scenename)
	{
	case SCENE_ROGO:
		start = 0;
		end = 0;
		break;
	case SCENE_TITLE:
		sound = TitleSound;
		/*start = SOUND_LABEL_TITLE_BGM;
		end = SOUND_LABEL_TITLE_CURSOR_SE + 1;*/
		break;
	case SCENE_STAGESELECT:
		sound = StageSelectSound;
		/*start = SOUND_LABEL_STAGE_MEADOW_BGM;
		end = SOUND_LABEL_STAGE_DECISION_SE + 1;*/
		break;
	case SCENE_GAMESCENE:
		sound = GameSceneSound;
		/*start = SOUND_LABEL_STAGECLEAR;
		end = SOUND_LABEL_LILITHBARRIER_SE + 1;*/
		start = SOUND_LABEL_PSHOOT_SE;
		end = SOUND_LABEL_GAME01_BGM + 1;
		break;
	case SCENE_RESULT:
		sound = ResultSound;
		/*start = SOUND_LABEL_RESULT_WIN_BGM;
		end = SOUND_LABEL_PINKSTAREFFECT_SE + 1;*/
		break;
	default:
		break;
	}


	HANDLE               hFile;
	DWORD                dwChunkSize;
	DWORD                dwChunkPosition;
	DWORD                filetype;
	for (int n = start; n < end; n++)
	{
		SOUND i = (SOUND)n;
		int a = n - start;
		memset(&g_wfx[i], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&g_buffer[i], 0, sizeof(XAUDIO2_BUFFER));

		hFile = CreateFileA(sound[a].filename, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);

		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &g_wfx[i], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		g_DataBuffer[i] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, g_DataBuffer[i], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		// 	サブミットボイスで利用するサブミットバッファの設定
		g_buffer[i].AudioBytes = dwChunkSize;
		g_buffer[i].pAudioData = g_DataBuffer[i];
		g_buffer[i].Flags = XAUDIO2_END_OF_STREAM;
		if (sound[a].bLoop)
			g_buffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
		else
			g_buffer[i].LoopCount = 0;

		g_pXAudio2->CreateSourceVoice(&g_pSourceVoice[i], &(g_wfx[i].Format));
	}

}


//=============================================================================
// ユーティリティ関数群
//=============================================================================
HRESULT Sound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT Sound::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}