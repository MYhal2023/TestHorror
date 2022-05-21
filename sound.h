//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once
#pragma comment(lib, "XAPOFX.lib")

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v
#include "xaudio2fx.h"
#include <XAPOFX.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BGM_VOLUME (0.7f)			//BGM�̉���
#define MAX_VOLUME (1.6f)			//BGM��MAX����
#define MIN_VOLUME (0.0f)			//BGM��MIN����

// �t�F�[�h�̏��
typedef enum
{
	SOUNDFADE_NONE = 0,		// �����Ȃ����
	SOUNDFADE_IN,			// �t�F�[�h�C������
	SOUNDFADE_OUT,			// �t�F�[�h�A�E�g����
} SOUND_FADE;

// �g�p����G�t�F�N�g
typedef enum
{
	SOUND_EFFECT_REVERB = 0,		// ���o�[�u
	SOUND_EFFECT_ECHO,				// �G�R�[
	SOUND_EFFECT_EQ,				// �C�R���C�U�[
	SOUND_EFFECT_MAX,				// �G�t�F�N�g��MAX��
} SOUND_EFFECT;

// �g�p����T�E���h�̎��
enum 
{
	MASTER,
	BGM,
	SE,
	VOICE,
	SOUNDMAX,				// �T�E���h��ނ�MAX��
};

// �T�E���h�t�@�C��
enum 
{
	SOUND_LABEL_SE_HeartBeat,		// SE
	SOUND_LABEL_SE_LighterON,		// SE
	SOUND_LABEL_SE_MatchON,			// SE
	SOUND_LABEL_SE_Breath001,			// SE
	SOUND_LABEL_SE_Breath002,			// SE
	SOUND_LABEL_SE_Breath003,			// SE
	SOUND_LABEL_SE_EnemyVoice,			// SE
	SOUND_LABEL_SE_GetItem,			// SE
	SOUND_LABEL_SE_WoodWalk,			// SE
	SOUND_LABEL_SE_Walk_sound,			// SE
	SOUND_LABEL_SE_Cursol_Move,			// SE
	SOUND_LABEL_SE_Cursol_On,			// SE
	SOUND_LABEL_SE_Door_Close,			// SE
	SOUND_LABEL_SE_Door_Open,			// SE
	SOUND_LABEL_BGM_Normal,			// BGM
	SOUND_LABEL_BGM_Fear,			// BGM
	SOUND_LABEL_BGM_GameOver,			// BGM
	SOUND_LABEL_BGM_HorrorZone,			// BGM
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
void UpdateSound(void);
void SetSourceSoundPanVolume(int label);
void SetSoundFade(int label, SOUND_FADE soundfade, float TargetVolume, float TargetTime);
void SetSourceVolume(int label, float volume);
void SetFrequencyRatio(int label, float Pitch);
void PauseSound(int label);
void ReStartSound(int label);
void PassEffectON(int label, XAUDIO2_FILTER_TYPE type);
void PassEffectOFF(int label);
void SetReverbParam(XAUDIO2FX_REVERB_I3DL2_PARAMETERS type);
void ChangeReverb(float volume);
void SetEchoParam(float wetdrymix, float delay, float feedback);
void SetEQParam(float c0, float c1, float c2, float c3, float b0, float b1, float b2, float b3, float g0, float g1, float g2, float g3);
void MasteringLimit(void);
void EffectSwitch(SOUND_EFFECT effect, BOOL flag);
void SetMasterVoice(float Volume);
void SetVolumeOption(float volume1, float volume2);