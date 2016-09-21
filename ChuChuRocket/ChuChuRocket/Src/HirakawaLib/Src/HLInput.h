/****************************************************************************************************
//
//		HLInput.h
//
//		@brief	KoudoLib ���͊Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLINPUT_H__
#define __HLINPUT_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"
#include "HLDirect3D.h"

//---------------------------------------------------------------------------------------------
//	enum
//---------------------------------------------------------------------------------------------
// �}�E�X�{�^����`
enum HLMOUSE_BUTTONS
{
	HLMOUSE_L = 0,			// ��
	HLMOUSE_R,				// �E
	HLMOUSE_CENTER,			// �^��
	MAX_HLMOUSE_BUTTONS
};

// �Q�[���p�b�h�{�^����`
enum HLGAMEPAD_BUTTONS
{
	HLGAMEPAD_A = 0,		// A
	HLGAMEPAD_B,			// B
	HLGAMEPAD_X,			// X
	HLGAMEPAD_Y,			// Y
	HLGAMEPAD_LB,			// LB
	HLGAMEPAD_RB,			// RB
	HLGAMEPAD_BACK,			// BACK
	HLGAMEPAD_START,		// START
	HLGAMEPAD_D_UP,			// D UP
	HLGAMEPAD_D_DOWN,		// D DOWN
	HLGAMEPAD_D_LEFT,		// D LEFT
	HLGAMEPAD_D_RIGHT,		// D RIGHT
	HLGAMEPAD_L_STICK_BUTTON,// L STICK BUTTON
	HLGAMEPAD_L_STICK_UP,	// L STICK UP
	HLGAMEPAD_L_STICK_DOWN,	// L STICK DOWN
	HLGAMEPAD_L_STICK_LEFT,	// L STICK LEFT
	HLGAMEPAD_L_STICK_RIGHT,// L STICK RIGHT
	HLGAMEPAD_R_STICK_BUTTON,// R STICK BUTTON
	HLGAMEPAD_R_STICK_UP,	// R STICK UP
	HLGAMEPAD_R_STICK_DOWN,	// R STICK DOWN
	HLGAMEPAD_R_STICK_LEFT,	// R STICK LEFT
	HLGAMEPAD_R_STICK_RIGHT,// R STICK RIGHT
	MAX_HLGAMEPAD_BUTTONS
};

//---------------------------------------------------------------------------------------------
//	const
//---------------------------------------------------------------------------------------------
const float kGamePadStickButtonRegion = 0.5F;		// �Q�[���p�b�h�X�e�B�b�N�{�^������̈� 

//---------------------------------------------------------------------------------------------
//	struct
//---------------------------------------------------------------------------------------------
// �}�E�X���W
struct MOUSE_POSITION
{
	int	x, y;
};

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLInput
{
public:
	HLInput();
	~HLInput();

	bool Init();			// ������
	void Destroy();			// �j��
	void Update();			// �X�V

	// �E�B���h�E�̓��̓R�[���o�b�N���󂯎��
	void SetWindowInputCallback(HWND hWnd, unsigned int msg, unsigned int wParam, LONG lParam);

	// �L�[�{�[�h�F�L�[�������ꂽ���H(�������ςȂ��L��)
	bool IsPressKeyboard(Keyboard::Keys Key);
	// �L�[�{�[�h�F�L�[�������ꂽ���H(�������ςȂ�����)
	bool IsPushKeyboard(Keyboard::Keys Key);
	// �L�[�{�[�h�F�L�[�������ꂽ��
	bool IsReleaseKeyboard(Keyboard::Keys Key);
	
	// �}�E�X�F�{�^���������ꂽ���H(�������ςȂ��L��)
	bool IsPressMouse(HLMOUSE_BUTTONS Button);
	// �}�E�X�F�{�^���������ꂽ���H(�������ςȂ�����)
	bool IsPushMouse(HLMOUSE_BUTTONS Button);
	// �}�E�X�F�{�^���������ꂽ��
	bool IsReleaseMouse(HLMOUSE_BUTTONS Button);
	// �}�E�X�F���W�擾
	MOUSE_POSITION GetMousePosition();
	// �}�E�X�F�X�N���[���z�C�[���擾
	int GetMouseScrollWheel();

	// �Q�[���p�b�h�F�{�^���������ꂽ���H(�������ςȂ��L��)
	bool IsPressGamePad(int Number, HLGAMEPAD_BUTTONS Button);
	// �Q�[���p�b�h�F�{�^���������ꂽ���H(�������ςȂ�����)
	bool IsPushGamePad(int Number, HLGAMEPAD_BUTTONS Button);
	// �Q�[���p�b�h�F�{�^���������ꂽ��
	bool IsReleaseGamePad(int Number, HLGAMEPAD_BUTTONS Button);
	// �Q�[���p�b�h�F���X�e�B�b�N�擾
	Vector2 GetGamePadLeftStick(int Number);
	// �Q�[���p�b�h�F�E�X�e�B�b�N�擾
	Vector2 GetGamePadRightStick(int Number);
	// �Q�[���p�b�h�F���g���K�[�擾
	float GetGamePadLeftTrigger(int Number);
	// �Q�[���p�b�h�F�E�g���K�[�擾
	float GetGamePadRightTrigger(int Number);

private:
	Keyboard*			p_keyboard_;			// �L�[�{�[�h
	Keyboard::State		keyboard_state_;		// �L�[�{�[�h�̏��
	Keyboard::State		prev_keyboard_state_;	// �O�̃L�[�{�[�h�̏��

	Mouse*				p_mouse_;				// �}�E�X
	Mouse::State		mouse_state_;			// �}�E�X�̏��
	Mouse::State		prev_mouse_state_;		// �O�̃}�E�X�̏��

	GamePad*			p_game_pad_;			// �Q�[���p�b�h
	GamePad::State		game_pad_state_[GamePad::MAX_PLAYER_COUNT];			// �Q�[���p�b�h�̏��
	GamePad::State		prev_game_pad_state_[GamePad::MAX_PLAYER_COUNT];	// �O�̃Q�[���p�b�h�̏��
};


#endif //__HLINPUT_H__
