//=============================================================================
//
// �T�E���h���� [sound.cpp]
//
//=============================================================================
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
	BOOL UseFilter;		// �G�t�F�N�g�g����
	int type;			// �T�E���h�̎��
} SOUNDPARAM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
IXAudio2SubmixVoice *g_apDrySubmixVoice;					//�T�u�~�b�N�X�{�C�X(�G�t�F�N�g�Ȃ�)
IXAudio2SubmixVoice *g_apWetSubmixVoice;					//�T�u�~�b�N�X�{�C�X(�G�t�F�N�g����)

//�G�t�F�N�g�֘A
//���o�[�u
IUnknown *g_apXPO_Reverb;
XAUDIO2_EFFECT_DESCRIPTOR g_Descriptior[SOUND_EFFECT_MAX];
XAUDIO2_EFFECT_CHAIN g_Chain;

//���o�[�u�̃p�����[�^
FXECHO_PARAMETERS g_ReverbParam;
XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_i3dl2Param = XAUDIO2FX_I3DL2_PRESET_BATHROOM;
XAUDIO2FX_REVERB_PARAMETERS g_reverbParam;

//�G�R�[
IUnknown *g_apXPO_Echo;

//�G�R�[�̃p�����[�^
FXECHO_PARAMETERS g_EchoParam;

//EQ
IUnknown *g_apXPO_EQ;

//EQ�̃p�����[�^
FXEQ_PARAMETERS g_EQParam;

#ifdef _DEBUG	// �f�o�b�O���̂�
IUnknown *g_apXPO_Volmeter;
XAUDIO2_EFFECT_DESCRIPTOR g_Descriptor_Volmeter;
XAUDIO2_EFFECT_CHAIN g_Chain_Volmeter;
XAUDIO2FX_VOLUMEMETER_LEVELS g_Levels;
float g_PeakLevels[2];			//�s�[�N���x���̎�M�ꏊ
float g_RMSLevels[2];			//RMS���x���̎�M�ꏊ
#endif

// �e���f�ނ̃p�����[�^
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ (char*)"data/BGM/title.wav", -1 },		// �^�C�g��BGM
	{ (char*)"data/BGM/game.wav", -1 },			// �Q�[��BGM1
	{ (char*)"data/BGM/result.wav", -1 },		// ���U���gBGM2
	{ (char*)"data/SE/button.wav", 0 },			// �{�^����
	{ (char*)"data/SE/clock.wav", 0 },			// �v���C���[�Ŏ��v��
	{ (char*)"data/SE/clock_enemy.wav", 0 },	// �G�l�~�[�Ŏ��v��
	{ (char*)"data/SE/coin.wav", 0 },			// �v���C���[�ŃR�C����
	{ (char*)"data/SE/coin_enemy.wav", 0 },		// �G�l�~�[�ŃR�C����
	{ (char*)"data/SE/enemydown.wav", 0 },		// �G�l�~�[������
	{ (char*)"data/SE/hit000.wav", 0 },			// �q�b�g��
};

//Fade�֘A
static BOOL			g_Use;									// TRUE:�g���Ă���  FALSE:���g�p
SOUND_FADE			g_Fade = SOUNDFADE_NONE;				// �t�F�[�h�̏��
int					g_label;
float				g_NowVolume = 1.0f;
float				g_NextVolume;
float				g_FadeRate;

//���ʊǗ�
float				g_VolParam[SOUNDMAX];

//=============================================================================
// ����������
//=============================================================================
BOOL InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return FALSE;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return FALSE;
	}

	//�T�u�~�b�N�X�{�C�X�i�G�t�F�N�g����j�̍쐬
	hr = g_pXAudio2->CreateSubmixVoice(&g_apWetSubmixVoice, 2, 48000);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�T�u�~�b�N�X�i�G�t�F�N�g����j�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return FALSE;
	}

	//�T�u�~�b�N�X�{�C�X�i�G�t�F�N�g�Ȃ��j�̍쐬
	hr = g_pXAudio2->CreateSubmixVoice(&g_apDrySubmixVoice, 2, 48000);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�T�u�~�b�N�X�i�G�t�F�N�g�Ȃ��j�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return FALSE;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	//�G�t�F�N�g�̐���
	{
		//���o�[�u�̐���
		XAudio2CreateReverb(&g_apXPO_Reverb);

		//EFFECT_DESCRIPTOR(���o�[�u)�̏�����
		g_Descriptior[SOUND_EFFECT_REVERB].InitialState = FALSE;			//������Ԃ�
		g_Descriptior[SOUND_EFFECT_REVERB].OutputChannels = 2;				//2ch�̃G�t�F�N�g
		g_Descriptior[SOUND_EFFECT_REVERB].pEffect = g_apXPO_Reverb;		//�G�t�F�N�g�{��

		//�G�R�[�̐���
		CreateFX(_uuidof(FXEcho), &g_apXPO_Echo);

		//EFFECT_DESCRIPTOR(�G�R�[)�̏�����
		g_Descriptior[SOUND_EFFECT_ECHO].InitialState = FALSE;				//������Ԃ�
		g_Descriptior[SOUND_EFFECT_ECHO].OutputChannels = 2;				//2ch�̃G�t�F�N�g
		g_Descriptior[SOUND_EFFECT_ECHO].pEffect = g_apXPO_Echo;			//�G�t�F�N�g�{��

		//EQ�̐���
		CreateFX(_uuidof(FXEQ), &g_apXPO_EQ);

		//EFFECT_DESCRIPTOR(EQ)�̏�����
		g_Descriptior[SOUND_EFFECT_EQ].InitialState = FALSE;				//������Ԃ�
		g_Descriptior[SOUND_EFFECT_EQ].OutputChannels = 2;					//2ch�̃G�t�F�N�g
		g_Descriptior[SOUND_EFFECT_EQ].pEffect = g_apXPO_EQ;				//�G�t�F�N�g�{��

		//EFFECT_CHAIN�̍쐬
		g_Chain.EffectCount = SOUND_EFFECT_MAX;								//�}���̂�SOUND_EFFECT_MAX��
		g_Chain.pEffectDescriptors = g_Descriptior;							//�������̍\���̂��w��

		//�{�C�X��EFFECT_CHAIN��}��
		g_apWetSubmixVoice->SetEffectChain(&g_Chain);

		//Release
		g_apXPO_Reverb->Release();
		g_apXPO_Echo->Release();
		g_apXPO_EQ->Release();
	}

	//�p�����[�^�[�̏�����
	{
		// ���o�[�u
		// I3DL2_REVERB_PARAM �ϊ�
		ReverbConvertI3DL2ToNative(&g_i3dl2Param, &g_reverbParam);

		//�G�t�F�N�g�ɒʒm����
		g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_REVERB, &g_reverbParam, sizeof(g_reverbParam));

		// �G�R�[
		g_EchoParam.WetDryMix = FXECHO_DEFAULT_WETDRYMIX;
		g_EchoParam.Delay = FXECHO_DEFAULT_DELAY;
		g_EchoParam.Feedback = FXECHO_DEFAULT_FEEDBACK;

		//�G�t�F�N�g�ɒʒm����
		g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_ECHO, &g_EchoParam, sizeof(g_EchoParam));

		//EQ
		g_EQParam.FrequencyCenter0 = FXEQ_DEFAULT_FREQUENCY_CENTER_0;
		g_EQParam.FrequencyCenter1 = FXEQ_DEFAULT_FREQUENCY_CENTER_1;
		g_EQParam.FrequencyCenter2 = FXEQ_DEFAULT_FREQUENCY_CENTER_2;
		g_EQParam.FrequencyCenter3 = FXEQ_DEFAULT_FREQUENCY_CENTER_3;

		g_EQParam.Bandwidth0 = FXEQ_DEFAULT_BANDWIDTH;
		g_EQParam.Bandwidth1 = FXEQ_DEFAULT_BANDWIDTH;
		g_EQParam.Bandwidth2 = FXEQ_DEFAULT_BANDWIDTH;
		g_EQParam.Bandwidth3 = FXEQ_DEFAULT_BANDWIDTH;

		g_EQParam.Gain0 = FXEQ_DEFAULT_GAIN;
		g_EQParam.Gain1 = FXEQ_DEFAULT_GAIN;
		g_EQParam.Gain2 = FXEQ_DEFAULT_GAIN;
		g_EQParam.Gain3 = FXEQ_DEFAULT_GAIN;

		//�G�t�F�N�g�ɒʒm����
		g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_EQ, &g_EQParam, sizeof(g_EQParam));
	}

	//���ʂ̏�����
	{
		g_VolParam[MASTER] = 1.0f;
		g_VolParam[BGM] = 0.75f;
		g_VolParam[SE] = 0.8f;
		g_VolParam[VOICE] = 1.6f;
	}

#ifdef _DEBUG	// �f�o�b�O���̂�
	hr = XAudio2CreateVolumeMeter(&g_apXPO_Volmeter);
	g_Descriptor_Volmeter.InitialState = true;
	g_Descriptor_Volmeter.OutputChannels = 2;
	g_Descriptor_Volmeter.pEffect = g_apXPO_Volmeter;

	g_Chain_Volmeter.EffectCount = 1;
	g_Chain_Volmeter.pEffectDescriptors = &g_Descriptor_Volmeter;

	g_pMasteringVoice->SetEffectChain(&g_Chain_Volmeter);

	g_apXPO_Volmeter->Release();
#endif

	// Fade�֘A�̏�����
	g_Use = FALSE;
	g_Fade = SOUNDFADE_NONE;
	g_FadeRate = 0.0f;

	return TRUE;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	// �T�u�~�b�N�X�{�C�X�̔j��
	g_apWetSubmixVoice->DestroyVoice();
	g_apDrySubmixVoice->DestroyVoice();

	if(g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �X�V�����i���Fade�Ŏg�p�j
//=============================================================================
void UpdateSound(void)
{
	if (g_Fade != SOUNDFADE_NONE)
	{// �t�F�[�h������
		if (g_Fade == SOUNDFADE_OUT)
		{// �t�F�[�h�A�E�g����
			g_NowVolume -= g_FadeRate;		// ���Z
			if (g_NowVolume > g_NextVolume)
			{
				g_apSourceVoice[g_label]->SetVolume(g_NowVolume);
			}
			else
			{
				// �t�F�[�h�A�E�g�����I��
				g_apSourceVoice[g_label]->SetVolume(0.0f);
				g_apSourceVoice[g_label]->Stop();
				g_Fade = SOUNDFADE_NONE;
			}
		}

		else if (g_Fade == SOUNDFADE_IN)
		{// �t�F�[�h�C������
			g_NowVolume += g_FadeRate;		// ���Z
			g_apSourceVoice[g_label]->SetVolume(g_NowVolume);

			if (g_NowVolume >= g_NextVolume)
			{
				// �t�F�[�h�C�������I��
				g_Fade = SOUNDFADE_NONE;
			}
		}
	}
	//if (GetOption())
	//{
	//	//���ʒ���
	//	{
	//		XAUDIO2_VOICE_STATE xa2state;

	//		for (int i = 0; i < SOUND_LABEL_MAX; i++)
	//		{
	//			// ��Ԏ擾
	//			g_apSourceVoice[i]->GetState(&xa2state);
	//			if (xa2state.BuffersQueued != 0)
	//			{// �Đ���
	//				// ���ʒ���
	//				g_apSourceVoice[i]->SetVolume(g_VolParam[g_aParam[i].type]);
	//			}
	//		}
	//	}
	//}

}

//=============================================================================
// SoundFade����
//=============================================================================
void SetSoundFade(int label, SOUND_FADE fademode, float TargetVolume, float TargetTime)
{
	g_Fade = fademode;
	g_label = label;
	g_NextVolume = TargetVolume;
	g_FadeRate = fabsf(TargetVolume - g_NowVolume) / (TargetTime * 60);
	return;
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
void PlaySound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	if (g_aParam[label].UseFilter == TRUE)
	{
		//�T�u�~�b�N�X�{�C�X�ɑ��M
		XAUDIO2_SEND_DESCRIPTOR Send[2] = { 0, g_apWetSubmixVoice,
											0, g_apDrySubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 2, Send };
		g_apSourceVoice[label]->SetOutputVoices(&SendList);
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// �}�X�^�[�{�C�X�̉��ʒ���
//=============================================================================
void SetMasterVoice(float volume)
{
	volume *= volume;
	g_pMasteringVoice->SetVolume(volume);
	return;
}


//=============================================================================
// �\�[�X�{�C�X�̉��ʒ���
//=============================================================================
void SetSourceVolume(int label, float volume)
{
	volume *= volume;
	g_apSourceVoice[label]->SetVolume(volume);
	return;
}


//=============================================================================
// �I�v�V�����p���ʒ���
//=============================================================================
void SetVolumeOption(float volume1, float volume2)
{
	volume1 *= volume1;
	volume2 *= volume2;

	g_VolParam[BGM] = volume1;
	g_VolParam[SE] = volume2;
	return;
}


//=============================================================================
// Panning(����)����
//=============================================================================
void SetSourceSoundPanVolume(int label)
{
	int InChannels;
	int OutChannels;

	InChannels = 2;
	OutChannels = 2;

	float Volumes[2] = { 1.0f, 0.0f };
	g_apSourceVoice[label]->SetOutputMatrix(NULL, InChannels, OutChannels, Volumes);
	return;
}


//=============================================================================
// �\�[�X�{�C�X�̃{�C�X�̍Đ��s�b�`����
//=============================================================================
void SetFrequencyRatio(int label, float Pitch)
{
	g_apSourceVoice[label]->SetFrequencyRatio(Pitch);
	return;
}


//=============================================================================
// �\�[�X�{�C�X�̈ꎞ��~
//=============================================================================
void PauseSound(int label)
{
	g_apSourceVoice[label]->Stop(XAUDIO2_PLAY_TAILS);
	return;
}


//=============================================================================
// �\�[�X�{�C�X�̍ĊJ
//=============================================================================
void ReStartSound(int label)
{
	g_apSourceVoice[label]->Start();
	return;
}


//=============================================================================
// ���[�p�X�A�n�C�p�X�t�B���^�[��ON
//=============================================================================
void PassEffectON(int label, XAUDIO2_FILTER_TYPE type)
{
	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = type;
	FilterParams.Frequency = 0.5f;
	FilterParams.OneOverQ = 1.4142f;
	g_apSourceVoice[label]->SetFilterParameters(&FilterParams);
	return;
}


//=============================================================================
// ���[�p�X�A�n�C�p�X�t�B���^�[��OFF
//=============================================================================
void PassEffectOFF(int label)
{
	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
	FilterParams.Frequency = 1.0f;
	FilterParams.OneOverQ = 1.0f;
	g_apSourceVoice[label]->SetFilterParameters(&FilterParams);
	return;
}

//=============================================================================
// ���o�[�u�̃p�����[�^����
//=============================================================================
void SetReverbParam(XAUDIO2FX_REVERB_I3DL2_PARAMETERS type)
{
	//�f�t�H���g�̂������������Ă���
	g_i3dl2Param = type;

	// I3DL2_REVERB_PARAM �ϊ�
	ReverbConvertI3DL2ToNative(&g_i3dl2Param, &g_reverbParam);

	//�G�t�F�N�g�ɒʒm
	g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_REVERB, &g_reverbParam, sizeof(g_reverbParam));

	//�ׂ�������������������
	//XAUDIO2FX_REVERB_PARAMETERS reverbParameters;
	//reverbParameters.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	//reverbParameters.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	//reverbParameters.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	//reverbParameters.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	//reverbParameters.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	//reverbParameters.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	//reverbParameters.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	//reverbParameters.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	//reverbParameters.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	//reverbParameters.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	//reverbParameters.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	//reverbParameters.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	//reverbParameters.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	//reverbParameters.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	//reverbParameters.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	//reverbParameters.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	//reverbParameters.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	//reverbParameters.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	//reverbParameters.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	//reverbParameters.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	//reverbParameters.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	//reverbParameters.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;
	//g_apWetSubmixVoice->SetEffectParameters(0, &reverbParameters, sizeof(reverbParameters));
	return;
}


//=============================================================================
// ���o�[�u�̃p�����[�^�����i�����肮�����j
//=============================================================================
void ChangeReverb(float depth)
{
	float WelVolume = 0.5f * depth * depth;
	if (depth >= 1.0f)
	{
		float WelVolume = 1.0f;
	}
	float DryVolume = 1.0f - WelVolume;

	g_apWetSubmixVoice->SetVolume(WelVolume);
	g_apDrySubmixVoice->SetVolume(DryVolume);

	return;
}


//=============================================================================
// �G�R�[�̃p�����[�^����
//=============================================================================
void SetEchoParam(float wetdrymix, float delay, float feedback)
{
	g_apWetSubmixVoice->EnableEffect(SOUND_EFFECT_ECHO);

	//�p�����[�^�[�̏�����
	g_EchoParam.WetDryMix = wetdrymix;
	g_EchoParam.Delay = delay;
	g_EchoParam.Feedback = feedback;

	//�G�t�F�N�g�ɒʒm����
	g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_ECHO, &g_EchoParam, sizeof(g_EchoParam));
}


//=============================================================================
// EQ�̃p�����[�^����
//=============================================================================
void SetEQParam(float c0, float c1, float c2, float c3, float b0, float b1, float b2, float b3, float g0, float g1, float g2, float g3)
{
	g_EQParam.FrequencyCenter0 = c0;
	g_EQParam.FrequencyCenter1 = c1;
	g_EQParam.FrequencyCenter2 = c2;
	g_EQParam.FrequencyCenter3 = c3;

	g_EQParam.Bandwidth0 = b0;
	g_EQParam.Bandwidth1 = b1;
	g_EQParam.Bandwidth2 = b2;
	g_EQParam.Bandwidth3 = b3;

	g_EQParam.Gain0 = g0;
	g_EQParam.Gain1 = g1;
	g_EQParam.Gain2 = g2;
	g_EQParam.Gain3 = g3;

	//�G�t�F�N�g�ɒʒm����
	g_apWetSubmixVoice->SetEffectParameters(SOUND_EFFECT_EQ, &g_EQParam, sizeof(g_EQParam));
	return;
}


//=============================================================================
// �ő剹�ʂ̒���
//=============================================================================
void MasteringLimit(void)
{
	IUnknown *apXPO_FXMLimit;
	CreateFX(_uuidof(FXMasteringLimiter), &apXPO_FXMLimit);

	XAUDIO2_EFFECT_DESCRIPTOR Descriptor_FXMLimit;
	Descriptor_FXMLimit.InitialState = true;
	Descriptor_FXMLimit.OutputChannels = 2;
	Descriptor_FXMLimit.pEffect = apXPO_FXMLimit;

	XAUDIO2_EFFECT_CHAIN Chain_FXMLimit;
	Chain_FXMLimit.EffectCount = 1;
	Chain_FXMLimit.pEffectDescriptors = &Descriptor_FXMLimit;

	g_pMasteringVoice->SetEffectChain(&Chain_FXMLimit);

	apXPO_FXMLimit->Release();

	FXMASTERINGLIMITER_PARAMETERS FXMLimit;

	FXMLimit.Loudness = FXMASTERINGLIMITER_DEFAULT_LOUDNESS;
	FXMLimit.Release = FXMASTERINGLIMITER_DEFAULT_RELEASE;

	return;
}


//=============================================================================
// �G�t�F�N�g�X�C�b�`
//=============================================================================
void EffectSwitch(SOUND_EFFECT effect, BOOL flag)
{
	if (flag == FALSE)
	{
		g_apWetSubmixVoice->DisableEffect(effect);
	}
	else
	{
		g_apWetSubmixVoice->EnableEffect(effect);
	}
}


#ifdef _DEBUG	// �f�o�b�O����\������
//=============================================================================
// �}�X�^�[���ʂ��擾
//=============================================================================
void GetVolMeter(void)
{
	g_Levels.pPeakLevels = g_PeakLevels;
	g_Levels.pRMSLevels = g_RMSLevels;
	g_Levels.ChannelCount = 2;

	g_pMasteringVoice->GetEffectParameters(0, &g_Levels, sizeof(g_Levels));
	return;
}
#endif
