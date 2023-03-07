//=============================================================================
//
// �T�E���h���� [XAudio2.cpp]
//
//=============================================================================

#include "XAudio2.h"


//=============================================================================
// ������(���[�v�FBGM=true,SE=false)
//=============================================================================
Sound::Sound(token)
{
	//�^�C�g��
	//TitleSound = {
	//};
	//�Q�[���V�[��
	GameSceneSound = {
		{ "assets/SE/Game/PShoot01.wav",false },//�v���C���[�̒e������SE
		{ "assets/SE/Game/PHit.wav",false },//�v���C���[���_���[�W���󂯂�SE
		{ "assets/SE/Game/PDead.wav",false },//�v���C���[������SE
		{ "assets/SE/Game/EnemyHit.wav",false },//�G�ɒe��������SE
		{ "assets/SE/Game/EnemyDeath.wav",false },//�G������SE
		{ "assets/SE/Game/GameBGM01.wav",true },//���C���Q�[��BGM01
	};
	//���U���g
	//ResultSound = {
	//};
	//�I�v�V����
	OptionSound = {
		{ "assets/SE/Game/PShoot01.wav",false },//�v���C���[�̒e������SE
		{ "assets/SE/Game/PHit.wav",false },//�v���C���[���_���[�W���󂯂�SE
		{ "assets/SE/Game/PDead.wav",false },//�v���C���[������SE
		{ "assets/SE/Game/EnemyHit.wav",false },//�G�ɒe��������SE
	};
	//PARAM g_param[SOUND_LABEL_MAX] = {
	//};
	HRESULT      hr;

	// �b�n�l�̏�����
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	hr = XAudio2Create(&g_pXAudio2, 0);		// �������ͤ����t���O �f�o�b�O���[�h�̎w��(���݂͖��g�p�Ȃ̂�0�ɂ���)
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);		// ��O�����́Awindows�ł͖���
	if (FAILED(hr)) {
		//CoUninitialize();
	}

	/**** Create Mastering Voice ****/
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);			// ����͂o�b�̃f�t�H���g�ݒ�ɔC���Ă���
	/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL*/		// �{���U�̈����������Ă���
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

		// 	�T�u�~�b�g�{�C�X�ŗ��p����T�u�~�b�g�o�b�t�@�̐ݒ�
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

	//	// 	�T�u�~�b�g�{�C�X�ŗ��p����T�u�~�b�g�o�b�t�@�̐ݒ�
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
			g_pSourceVoice[i]->DestroyVoice();			// �I�[�f�B�I�O���t����\�[�X�{�C�X���폜
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
// �J������
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
			g_pSourceVoice[i]->DestroyVoice();			// �I�[�f�B�I�O���t����\�[�X�{�C�X���폜
			delete[]  g_DataBuffer[i];
			g_pSourceVoice.erase(i);
			g_DataBuffer.erase(i);
		}
	}
	if (end != 0 && start != 0) {
		g_pMasteringVoice->DestroyVoice();

		if (g_pXAudio2) g_pXAudio2->Release();
	}
	// �b�n�l�̔j��
	//CoUninitialize();
}

//=============================================================================
// ���ʐݒ�
//=============================================================================
void Sound::SetVolume(SOUND label, float volume)
{
	//���ʐݒ�
	g_pSourceVoice[label]->SetVolume(volume);
}

//=============================================================================
// �Đ�
//=============================================================================
void Sound::Play(SOUND label)
{
	// �\�[�X�{�C�X�쐬
	g_pXAudio2->CreateSourceVoice(&(g_pSourceVoice[label]), &(g_wfx[label].Format));
	g_pSourceVoice[label]->SubmitSourceBuffer(&(g_buffer[label]));	// �{�C�X�L���[�ɐV�����I�[�f�B�I�o�b�t�@�[��ǉ�

	// �Đ�
	g_pSourceVoice[label]->Start(0);

	SetVolume(label, m_volume);

}

//=============================================================================
// ��~
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
// �ꎞ��~
//=============================================================================
void Sound::Pause(SOUND label)
{
	g_pSourceVoice[label]->Stop(0);
}

//=============================================================================
// ���X�^�[�g
//=============================================================================
void Sound::Restart(SOUND label)
{
	g_pSourceVoice[label]->Start(0);
}


void Sound::SetScene(Scene_Transition scenename)
{
	// �b�n�l�̏�����
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

		// 	�T�u�~�b�g�{�C�X�ŗ��p����T�u�~�b�g�o�b�t�@�̐ݒ�
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
// ���[�e�B���e�B�֐��Q
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