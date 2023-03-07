//=============================================================================
//
// �T�E���h���� [XAudio2.h]
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
	//�^�C�g��


	//�X�e�[�W�Z���N�g


	//�Q�[���V�[��
	SOUND_LABEL_PSHOOT_SE,				//�v���C���[�̒e������SE
	SOUND_LABEL_PHIT_SE,				//�v���C���[���_���[�W���󂯂�SE
	SOUND_LABEL_PDEAD_SE,				//�v���C���[������SE
	SOUND_LABEL_EHIT_SE,				//�G�ɒe��������SE
	SOUND_LABEL_EDEATH_SE,				//�G������SE
	SOUND_LABEL_GAME01_BGM,				//���C���Q�[��BGM01

	//���U���g

	//�I�v�V����
	SOUND_LABEL_OPTION_DECISION_SE,		//�I�v�V����SE����
	SOUND_LABEL_OPTION_BACK_SE,			//�I�v�V����SE�߂�
	SOUND_LABEL_OPTION_CURSOR_SE,		//�I�v�V����SE�J�[�\��
	SOUND_LABEL_NOTE_BGM,				//�m�[�gBGM
};

// �p�����[�^�\����
typedef struct
{
	LPCSTR filename;	// �����t�@�C���܂ł̃p�X��ݒ�
	bool bLoop;			// true�Ń��[�v�B�ʏ�BGM��ture�ASE��false�B
} PARAM;

class Sound :public singleton<Sound>
{
public:
	explicit Sound(token);
	~Sound();
	void SetVolumeAll(float _volume);//�S�̂̉��ʂ�ύX
	void SetScene(Scene_Transition scenename);
	void Play(SOUND label);	 //�����̊J�n(0����)
	void Stop(SOUND label);   //�����̒�~
	void Pause(SOUND label);  //�����̈ꎞ��~
	void Restart(SOUND label);//�����̓r���Đ�
	void SetVolume(SOUND label, float volume);
	void Release(Scene_Transition scenename);//���
private:
	float m_volume = 1.0f;//�T�E���h�̃{�����[���A�S�̂��ꊇ

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

	std::unordered_map<SOUND, WAVEFORMATEXTENSIBLE>g_wfx;			// WAV�t�H�[�}�b�g
	std::unordered_map<SOUND, XAUDIO2_BUFFER>	g_buffer;
	std::unordered_map<SOUND, BYTE*> g_DataBuffer;
};

#endif
