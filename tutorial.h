//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : ���c �E�l
//
//=============================================================================
#pragma once



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FLOOR_BLOCK_SIZE			(100.0f)		// ���u���b�N�T�C�Y


#define	TUTORIALWALL_SIZE_Y		(100.0f)	// ���u���b�N�T�C�Y�c
#define	TUTORIALWALL_SIZE_XZ	(1000.0f)	// ���u���b�N�T�C�Y��
#define	PRISONSIDE_SIZE_XZ		(450.0f)	// ���u���b�N�T�C�Y��
#define DOOR_SIZE				(75.0f)		// �h�A�̉���
#define STAIRS_SIZE_XZ			(250.0f)	// �K�i�̉���
#define ROOM					(4)			// �Б��̕����̐�(�ꕔ�����₷���Ƃ�4�����b�V���E�H�[���g���܂�)

#define BED_SHIFT_X				(400.0f)	// �x�b�h�𕔉��̕ǂ���ǂꂾ��������
#define BED_SHIFT_Z				(125.0f)	// �x�b�h�𕔉��̕ǂ���ǂꂾ��������

#define TOILET_SHIFT_X			(400.0f)	// �g�C���𕔉��̕ǂ���ǂꂾ��������
#define TOILET_SHIFT_Z			(-50.0f)	// �g�C���𕔉��̕ǂ���ǂꂾ��������

#define STAIRS_SHIFT_Y			(45.0f)		// ���̊K�i���ǂꂾ���X���C�h�����邩
#define STAIRS_SHIFT_Z			(50.0f)		// �K�i���ǂꂾ���X���C�h�����邩
#define UPSTAIRS_SHIFT_Z		(75.0f)		// ���̊K�i���ǂꂾ���X���C�h�����邩

//////////////////1F//////////////////
#define CLEARSTAGEWALL_SIZE_Y		(300.0f)
#define CLEARSTAGEWALL_SIZE_X		(1000.0f)
#define CLEARSTAGEWALL_SIZE_Z		(300.0f)
#define CLEARSTAGE_DOOR				(100.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitSetTutorial(void);
void InitSetClearStage(void);