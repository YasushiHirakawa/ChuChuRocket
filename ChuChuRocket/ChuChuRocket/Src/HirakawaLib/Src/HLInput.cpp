/****************************************************************************************************
//
//		HLInput.cpp
//
//		@brief	KoudoLib 入力管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLInput.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLInput::HLInput()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLInput::~HLInput()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLInput::Init()
{
	bool ret = true;

	do {

		// キーボードの作成
		p_keyboard_ = new Keyboard();
		if (p_keyboard_ == NULL) {
			ret = false;
			break;
		}
		ZeroMemory(&keyboard_state_, sizeof(Keyboard::State));
		ZeroMemory(&prev_keyboard_state_, sizeof(Keyboard::State));

		// マウスの作成
		p_mouse_ = new Mouse();
		if (p_mouse_ == NULL) {
			ret = false;
			break;
		}
		ZeroMemory(&mouse_state_, sizeof(Mouse::State));
		ZeroMemory(&prev_mouse_state_, sizeof(Mouse::State));

		// ゲームパッドの作成
		p_game_pad_ = new GamePad();
		if (p_game_pad_ == NULL) {
			ret = false;
			break;
		}
		for (int i = 0; i < GamePad::MAX_PLAYER_COUNT; i++) {
			ZeroMemory(&game_pad_state_[i], sizeof(GamePad::State));
			ZeroMemory(&prev_game_pad_state_[i], sizeof(GamePad::State));
		}

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HLInput::Destroy()
{
	SAFE_DELETE(p_game_pad_);
	SAFE_DELETE(p_mouse_);
	SAFE_DELETE(p_keyboard_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	更新
---------------------------------------------------------------------------------------------*/
void HLInput::Update()
{
	// キーボードの状態を更新
	prev_keyboard_state_ = keyboard_state_;
	keyboard_state_ = p_keyboard_->GetState();

	// マウスの状態を更新
	prev_mouse_state_ = mouse_state_;
	mouse_state_ = p_mouse_->GetState();

	// ゲームパッドの状態を更新
	for (int i = 0; i < GamePad::MAX_PLAYER_COUNT; i++) {
		prev_game_pad_state_[i] = game_pad_state_[i];
		game_pad_state_[i] = p_game_pad_->GetState(i);
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	ウィンドウの入力コールバックを設定する
//	@param	hWnd		ウィンドウハンドル
//	@param	msg			メッセージ
//	@param	wParam		メッセージに関する追加情報
//	@param	lParam		メッセージに関する追加情報
---------------------------------------------------------------------------------------------*/
void HLInput::SetWindowInputCallback(HWND hWnd, unsigned int msg, unsigned int wParam, LONG lParam)
{
	switch (msg)
	{
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		Mouse::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(msg, wParam, lParam);
		break;
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：キーが押されたか？(押しっぱなし有効)
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPressKeyboard(Keyboard::Keys Key)
{
	return keyboard_state_.IsKeyDown(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：キーが押されたか？(押しっぱなし無効)
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPushKeyboard(Keyboard::Keys Key)
{
	return (prev_keyboard_state_.IsKeyUp(Key) && keyboard_state_.IsKeyDown(Key));
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：キーが離されたか
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsReleaseKeyboard(Keyboard::Keys Key)
{
	return (prev_keyboard_state_.IsKeyDown(Key) && keyboard_state_.IsKeyUp(Key));
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが押されたか？(押しっぱなし有効)
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPressMouse(HLMOUSE_BUTTONS Button)
{
	bool ret = false;

	switch (Button) {
	case HLMOUSE_L: ret = mouse_state_.leftButton; break;
	case HLMOUSE_R: ret = mouse_state_.rightButton; break;
	case HLMOUSE_CENTER: ret = mouse_state_.middleButton; break;
	}
	
	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが押されたか？(押しっぱなし無効)
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPushMouse(HLMOUSE_BUTTONS Button)
{
	bool ret = false;

	switch (Button) {
	case HLMOUSE_L: ret = (mouse_state_.leftButton && !prev_mouse_state_.leftButton); break;
	case HLMOUSE_R: ret = (mouse_state_.rightButton && !prev_mouse_state_.rightButton); break;
	case HLMOUSE_CENTER: ret = (mouse_state_.middleButton && !prev_mouse_state_.middleButton); break;
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが離されたか
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsReleaseMouse(HLMOUSE_BUTTONS Button)
{
	bool ret = false;

	switch (Button) {
	case HLMOUSE_L: ret = (!mouse_state_.leftButton && prev_mouse_state_.leftButton); break;
	case HLMOUSE_R: ret = (!mouse_state_.rightButton && prev_mouse_state_.rightButton); break;
	case HLMOUSE_CENTER: ret = (!mouse_state_.middleButton && prev_mouse_state_.middleButton); break;
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：座標取得
//	@retval	MOUSE_POSITION	：マウス座標
---------------------------------------------------------------------------------------------*/
MOUSE_POSITION HLInput::GetMousePosition()
{
	MOUSE_POSITION pos;
	pos.x = mouse_state_.x;
	pos.y = mouse_state_.y;
	return pos;
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：座標取得
//	@retval	int		：ホイールの値
---------------------------------------------------------------------------------------------*/
int HLInput::GetMouseScrollWheel()
{
	return mouse_state_.scrollWheelValue;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：ボタンが押されたか？(押しっぱなし有効)
//	@param	int					Number	：ゲームパッド番号
//	@param	HLGAMEPAD_BUTTONS	Button	：ボタン情報
//	@retval	bool						：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPressGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	bool ret = false;

	do {
		if (game_pad_state_[Number].IsConnected() == false) {
			break;
		}

		switch (Button) {
		case HLGAMEPAD_A:		ret = game_pad_state_[Number].buttons.a;		break;
		case HLGAMEPAD_B:		ret = game_pad_state_[Number].buttons.b;		break;
		case HLGAMEPAD_X:		ret = game_pad_state_[Number].buttons.x;		break;
		case HLGAMEPAD_Y:		ret = game_pad_state_[Number].buttons.y;		break;
		case HLGAMEPAD_LB:		ret = game_pad_state_[Number].buttons.leftShoulder;	break;
		case HLGAMEPAD_RB:		ret = game_pad_state_[Number].buttons.rightShoulder; break;
		case HLGAMEPAD_BACK:	ret = game_pad_state_[Number].buttons.back;		break;
		case HLGAMEPAD_START:	ret = game_pad_state_[Number].buttons.start;	break;
		case HLGAMEPAD_D_UP:	ret = game_pad_state_[Number].dpad.up;			break;
		case HLGAMEPAD_D_DOWN:	ret = game_pad_state_[Number].dpad.down;		break;
		case HLGAMEPAD_D_LEFT:	ret = game_pad_state_[Number].dpad.left;		break;
		case HLGAMEPAD_D_RIGHT: ret = game_pad_state_[Number].dpad.right;		break;
		case HLGAMEPAD_L_STICK_BUTTON:	ret = game_pad_state_[Number].buttons.leftStick;	break;
		case HLGAMEPAD_L_STICK_UP:		ret = (game_pad_state_[Number].thumbSticks.leftY > 0.5F);	break;
		case HLGAMEPAD_L_STICK_DOWN:	ret = (game_pad_state_[Number].thumbSticks.leftY < -0.5F);	break;
		case HLGAMEPAD_L_STICK_LEFT:	ret = (game_pad_state_[Number].thumbSticks.leftX < -0.5F);	break;
		case HLGAMEPAD_L_STICK_RIGHT:	ret = (game_pad_state_[Number].thumbSticks.leftX > 0.5F);	break;
		case HLGAMEPAD_R_STICK_BUTTON:	ret = game_pad_state_[Number].buttons.rightStick;	break;
		case HLGAMEPAD_R_STICK_UP:		ret = (game_pad_state_[Number].thumbSticks.rightY > 0.5F);	break;
		case HLGAMEPAD_R_STICK_DOWN:	ret = (game_pad_state_[Number].thumbSticks.rightY < -0.5F); break;
		case HLGAMEPAD_R_STICK_LEFT:	ret = (game_pad_state_[Number].thumbSticks.rightX < -0.5F); break;
		case HLGAMEPAD_R_STICK_RIGHT:	ret = (game_pad_state_[Number].thumbSticks.rightX > 0.5F);	break;
		}

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが押されたか？(押しっぱなし無効)
//	@param	int					Number	：ゲームパッド番号
//	@param	HLGAMEPAD_BUTTONS	Button	：ボタン情報
//	@retval	bool						：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsPushGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	bool ret = false;

	do {
		if (game_pad_state_[Number].connected == false) {
			break;
		}

		switch (Button) {
		case HLGAMEPAD_A:
			ret = (!game_pad_state_[Number].buttons.a && prev_game_pad_state_[Number].buttons.a);
			break;
		case HLGAMEPAD_B:
			ret = (!game_pad_state_[Number].buttons.b && prev_game_pad_state_[Number].buttons.b);
			break;
		case HLGAMEPAD_X:
			ret = (!game_pad_state_[Number].buttons.x && prev_game_pad_state_[Number].buttons.x);
			break;
		case HLGAMEPAD_Y:
			ret = (!game_pad_state_[Number].buttons.y && prev_game_pad_state_[Number].buttons.y);
			break;
		case HLGAMEPAD_LB:
			ret = (!game_pad_state_[Number].buttons.leftShoulder && prev_game_pad_state_[Number].buttons.leftShoulder);
			break;
		case HLGAMEPAD_RB:
			ret = (!game_pad_state_[Number].buttons.rightShoulder && prev_game_pad_state_[Number].buttons.rightShoulder);
			break;
		case HLGAMEPAD_BACK:
			ret = (!game_pad_state_[Number].buttons.back && prev_game_pad_state_[Number].buttons.back);
			break;
		case HLGAMEPAD_START:
			ret = (!game_pad_state_[Number].buttons.start && prev_game_pad_state_[Number].buttons.start);
			break;
		case HLGAMEPAD_D_UP:
			ret = (!game_pad_state_[Number].dpad.up && prev_game_pad_state_[Number].dpad.up);
			break;
		case HLGAMEPAD_D_DOWN:
			ret = (!game_pad_state_[Number].dpad.down && prev_game_pad_state_[Number].dpad.down);
			break;
		case HLGAMEPAD_D_LEFT:
			ret = (!game_pad_state_[Number].dpad.left && prev_game_pad_state_[Number].dpad.left);
			break;
		case HLGAMEPAD_D_RIGHT:
			ret = (!game_pad_state_[Number].dpad.right && prev_game_pad_state_[Number].dpad.right);
			break;
		case HLGAMEPAD_L_STICK_BUTTON:
			ret = (!game_pad_state_[Number].buttons.leftStick && prev_game_pad_state_[Number].buttons.leftStick);
			break;
		case HLGAMEPAD_L_STICK_UP:
			ret = (game_pad_state_[Number].thumbSticks.leftY < kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftY > kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_DOWN:
			ret = (game_pad_state_[Number].thumbSticks.leftY > -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftY < -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_LEFT:
			ret = (game_pad_state_[Number].thumbSticks.leftX > -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftX < -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_RIGHT:
			ret = (game_pad_state_[Number].thumbSticks.leftX < kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftX > kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_BUTTON:
			ret = (!game_pad_state_[Number].buttons.rightStick && prev_game_pad_state_[Number].buttons.rightStick);
			break;
		case HLGAMEPAD_R_STICK_UP:
			ret = (game_pad_state_[Number].thumbSticks.rightY < kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightY > kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_DOWN:
			ret = (game_pad_state_[Number].thumbSticks.rightY > -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightY < -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_LEFT:
			ret = (game_pad_state_[Number].thumbSticks.rightX > -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightX < -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_RIGHT:
			ret = (game_pad_state_[Number].thumbSticks.rightX < kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightX > kGamePadStickButtonRegion);
			break;
		}

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：ボタンが離されたか
//	@param	int					Number	：ゲームパッド番号
//	@param	HLGAMEPAD_BUTTONS	Button	：ボタン情報
//	@retval	bool						：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HLInput::IsReleaseGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	bool ret = false;

	do {
		if (game_pad_state_[Number].connected == false) {
			break;
		}

		switch (Button) {
		case HLGAMEPAD_A:
			ret = (game_pad_state_[Number].buttons.a && !prev_game_pad_state_[Number].buttons.a);
			break;
		case HLGAMEPAD_B:
			ret = (game_pad_state_[Number].buttons.b && !prev_game_pad_state_[Number].buttons.b);
			break;
		case HLGAMEPAD_X:
			ret = (game_pad_state_[Number].buttons.x && !prev_game_pad_state_[Number].buttons.x);
			break;
		case HLGAMEPAD_Y:
			ret = (game_pad_state_[Number].buttons.y && !prev_game_pad_state_[Number].buttons.y);
			break;
		case HLGAMEPAD_LB:
			ret = (game_pad_state_[Number].buttons.leftShoulder && !prev_game_pad_state_[Number].buttons.leftShoulder);
			break;
		case HLGAMEPAD_RB:
			ret = (game_pad_state_[Number].buttons.rightShoulder && !prev_game_pad_state_[Number].buttons.rightShoulder);
			break;
		case HLGAMEPAD_BACK:
			ret = (game_pad_state_[Number].buttons.back && !prev_game_pad_state_[Number].buttons.back);
			break;
		case HLGAMEPAD_START:
			ret = (game_pad_state_[Number].buttons.start && !prev_game_pad_state_[Number].buttons.start);
			break;
		case HLGAMEPAD_D_UP:
			ret = (game_pad_state_[Number].dpad.up && !prev_game_pad_state_[Number].dpad.up);
			break;
		case HLGAMEPAD_D_DOWN:
			ret = (game_pad_state_[Number].dpad.down && !prev_game_pad_state_[Number].dpad.down);
			break;
		case HLGAMEPAD_D_LEFT:
			ret = (game_pad_state_[Number].dpad.left && !prev_game_pad_state_[Number].dpad.left);
			break;
		case HLGAMEPAD_D_RIGHT:
			ret = (game_pad_state_[Number].dpad.right && !prev_game_pad_state_[Number].dpad.right);
			break;
		case HLGAMEPAD_L_STICK_BUTTON:
			ret = (!game_pad_state_[Number].buttons.leftStick && prev_game_pad_state_[Number].buttons.leftStick);
			break;
		case HLGAMEPAD_L_STICK_UP:
			ret = (game_pad_state_[Number].thumbSticks.leftY > kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftY < kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_DOWN:
			ret = (game_pad_state_[Number].thumbSticks.leftY < -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftY > -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_LEFT:
			ret = (game_pad_state_[Number].thumbSticks.leftX < -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftX > -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_L_STICK_RIGHT:
			ret = (game_pad_state_[Number].thumbSticks.leftX > kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.leftX < kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_BUTTON:
			ret = (!game_pad_state_[Number].buttons.rightStick && prev_game_pad_state_[Number].buttons.rightStick);
			break;
		case HLGAMEPAD_R_STICK_UP:
			ret = (game_pad_state_[Number].thumbSticks.rightY > kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightY < kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_DOWN:
			ret = (game_pad_state_[Number].thumbSticks.rightY < -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightY > -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_LEFT:
			ret = (game_pad_state_[Number].thumbSticks.rightX < -kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightX > -kGamePadStickButtonRegion);
			break;
		case HLGAMEPAD_R_STICK_RIGHT:
			ret = (game_pad_state_[Number].thumbSticks.rightX > kGamePadStickButtonRegion
					&& prev_game_pad_state_[Number].thumbSticks.rightX < kGamePadStickButtonRegion);
			break;
		}

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：左スティック取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	Vector2			：スティック情報
---------------------------------------------------------------------------------------------*/
Vector2 HLInput::GetGamePadLeftStick(int Number)
{
	Vector2 stick = Vector2(0.0F, 0.0F);

	do {

		if (game_pad_state_[Number].connected == false) {
			break;
		}

		stick.x = game_pad_state_[Number].thumbSticks.leftX;
		stick.y = game_pad_state_[Number].thumbSticks.leftY;

	} while (0);

	return stick;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：右スティック取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	Vector2			：スティック情報
---------------------------------------------------------------------------------------------*/
Vector2 HLInput::GetGamePadRightStick(int Number)
{
	Vector2 stick = Vector2(0.0F, 0.0F);

	do {

		if (game_pad_state_[Number].connected == false) {
			break;
		}

		stick.x = game_pad_state_[Number].thumbSticks.rightX;
		stick.y = game_pad_state_[Number].thumbSticks.rightY;

	} while (0);

	return stick;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：左トリガー取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	float			：トリガー情報
---------------------------------------------------------------------------------------------*/
float HLInput::GetGamePadLeftTrigger(int Number)
{
	float trigger = 0.0F;

	do {

		if (game_pad_state_[Number].connected == false) {
			break;
		}

		trigger = game_pad_state_[Number].triggers.left;

	} while (0);

	return trigger;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：右トリガー取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	float			：トリガー情報
---------------------------------------------------------------------------------------------*/
float HLInput::GetGamePadRightTrigger(int Number)
{
	float trigger = 0.0F;

	do {

		if (game_pad_state_[Number].connected == false) {
			break;
		}

		trigger = game_pad_state_[Number].triggers.right;

	} while (0);

	return trigger;
}

