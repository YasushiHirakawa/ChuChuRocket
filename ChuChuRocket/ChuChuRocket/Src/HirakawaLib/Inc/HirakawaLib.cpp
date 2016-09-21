/****************************************************************************************************
//
//		HirakawaLib.cpp
//
//		@brief	HirakawaLib
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HirakawaLib.h"


//---------------------------------------------------------------------------------------------
//	Static Member
//---------------------------------------------------------------------------------------------
HLDirect3D*		HirakawaLib::p_hl_direct3d_;			// Direct3D管理
HLFrameRate*	HirakawaLib::p_hl_framer_rate_;		// フレームレート管理
HLSprite*		HirakawaLib::p_hl_sprite_;				// スプライトデータ管理
HLTexture*		HirakawaLib::p_hl_texture_;			// テクスチャデータ管理
HLModel*		HirakawaLib::p_hl_model_;				// モデルデータ管理
HLInput*		HirakawaLib::p_hl_input_;				// 入力管理
HLAudio*		HirakawaLib::p_hl_audio_;				// オーディオ管理

Matrix			HirakawaLib::camera_view_matrix_;			// カメラのビュー行列
Matrix			HirakawaLib::camera_projection_matrix_;	// カメラの射影行列


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HirakawaLib::HirakawaLib()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HirakawaLib::~HirakawaLib()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@param	HWND hWnd	：ウィンドウハンドル
//	@retval	true		：成功
//	@retval	false		：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::Init(HWND hWnd)
{
	bool ret = true;

	do {

		// Direct3D
		p_hl_direct3d_ = new HLDirect3D;
		if (p_hl_direct3d_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_direct3d_->Init(hWnd) == false) {
			ret = false;
			break;
		}

		// フレームレート
		p_hl_framer_rate_ = new HLFrameRate;
		if (p_hl_framer_rate_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_framer_rate_->Init() == false) {
			ret = false;
			break;
		}

		// スプライト管理
		p_hl_sprite_ = new HLSprite;
		if (p_hl_sprite_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_sprite_->Init(p_hl_direct3d_->GetDevice(), p_hl_direct3d_->GetDeviceContext()) == false) {
			ret = false;
			break;
		}

		// テクスチャデータ管理
		p_hl_texture_ = new HLTexture;
		if (p_hl_texture_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_texture_->Init() == false) {
			ret = false;
			break;
		}

		// モデルデータ管理
		p_hl_model_ = new HLModel;
		if (p_hl_model_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_model_->Init(p_hl_direct3d_->GetDevice()) == false) {
			ret = false;
			break;
		}

		// 入力管理
		p_hl_input_ = new HLInput;
		if (p_hl_input_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_input_->Init() == false) {
			ret = false;
			break;
		}

		// オーディオ管理
		p_hl_audio_ = new HLAudio;
		if (p_hl_audio_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_audio_->Init() == false) {
			ret = false;
			break;
		}

		// ウィンドウハンドルの保存
		HLCommon::SetWindowHandle(hWnd);


		BeginSprite();
		EndSprite();

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HirakawaLib::Destroy()
{
	p_hl_input_->Destroy();			// 入力
	SAFE_DELETE(p_hl_input_);
	p_hl_model_->Destroy();			// モデルデータ
	SAFE_DELETE(p_hl_model_);
	p_hl_texture_->Destroy();		// テクスチャデータ
	SAFE_DELETE(p_hl_texture_);
	p_hl_sprite_->Destroy();		// スプライト
	SAFE_DELETE(p_hl_sprite_);
	p_hl_framer_rate_->Destroy();	// フレームレート
	SAFE_DELETE(p_hl_framer_rate_);
	p_hl_direct3d_->Destroy();		// Direct3D
	SAFE_DELETE(p_hl_direct3d_);
	p_hl_audio_->Destroy();			// オーディオ
	SAFE_DELETE(p_hl_audio_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	更新
//	@retval	true		：成功
//	@retval	false		：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::Update()
{
	bool ret = false;
	
	float fps = (float)p_hl_framer_rate_->GetFPS();

	// 描画フラグが立っていたらゲーム処理へ
	if (p_hl_framer_rate_->GetDrawFlag()) {
#if _DEBUG
		// フレームレート描画
		wchar_t	str[kStrMax];
		swprintf_s(str, (L"%s：%.1f"), HLCommon::GetWindowName(), fps);
		SetWindowText(HLCommon::GetWindowHandle(), str);
#endif
		// 入力更新
		p_hl_input_->Update();

		// オーディオ更新
		p_hl_audio_->Update();

		// モデル更新
		p_hl_model_->Update();
		
		// 戻り値をtrueに変更
		ret = true;
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	カメラのビュー行列の設定
//	@param	Vector3 Position	：カメラ座標
//	@param	Vector3 Lookat		：カメラ注視座標
//	@param	Vector3 Up			：カメラの上方向の向き
//	@retval	Matrix				：カメラのビュー行列
---------------------------------------------------------------------------------------------*/
Matrix HirakawaLib::SetCameraViewMatrix(Vector3 Position, Vector3 Lookat, Vector3 Up)
{
	camera_view_matrix_ = p_hl_direct3d_->SetCameraViewMatrix(Position, Lookat, Up);

	return camera_view_matrix_;
}

/*---------------------------------------------------------------------------------------------
//	@brief	カメラの射影行列の設定
//	@param	unsigned int	Width	：画面サイズ横
//	@param	unsigned int	Height	：画面サイズ縦
//	@param	float			Near	：ニアクリップ距離
//	@param	float			Far		：ファークリップ距離
//	@retval	Matrix					：カメラの射影行列
---------------------------------------------------------------------------------------------*/
Matrix HirakawaLib::SetCameraProjectionMatrix(unsigned int Width, unsigned int Height, float Near, float Far)
{
	camera_projection_matrix_ = p_hl_direct3d_->SetCameraProjectionMatrix(Width, Height, Near, Far);

	return camera_projection_matrix_;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ウィンドウの入力コールバックを設定する
//	@param	hWnd		ウィンドウハンドル
//	@param	msg			メッセージ
//	@param	wParam		メッセージに関する追加情報
//	@param	lParam		メッセージに関する追加情報
---------------------------------------------------------------------------------------------*/
void HirakawaLib::SetWindowInputCallback(HWND hWnd, unsigned int msg, unsigned int wParam, LONG lParam)
{
	p_hl_input_->SetWindowInputCallback(hWnd, msg, wParam, lParam);
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：ボタンが押されたか？(押しっぱなし有効)
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsPressKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：ボタンが押されたか？(押しっぱなし無効)
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsPushKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	キーボード：ボタンが離されたか
//	@param	Keyboard::Keys Key		：キー情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsReleaseKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが押されたか？(押しっぱなし有効)
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsPressMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが押されたか？(押しっぱなし無効)
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsPushMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：ボタンが離されたか
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsReleaseMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：座標取得
//	@retval	MOUSE_POSITION	：マウス座標
---------------------------------------------------------------------------------------------*/
MOUSE_POSITION HirakawaLib::GetMousePosition()
{
	return p_hl_input_->GetMousePosition();
}

/*---------------------------------------------------------------------------------------------
//	@brief	マウス：座標取得
//	@retval	int		：ホイールの値
---------------------------------------------------------------------------------------------*/
int HirakawaLib::GetMouseScrollWheel()
{
	return p_hl_input_->GetMouseScrollWheel();
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：ボタンが押されたか？(押しっぱなし有効)
//	@param	int				Number	：ゲームパッド番号
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsPressGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：ボタンが押されたか？(押しっぱなし無効)
//	@param	int				Number	：ゲームパッド番号
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsReleaseGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：ボタンが離されたか
//	@param	int				Number	：ゲームパッド番号
//	@param	HLMOUSE_BUTTONS Button	：ボタン情報
//	@retval	bool					：入力判定結果
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsPushGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：左スティック取得
//	@param	int			Number	：ゲームパッド番号
//	@retval	Vector2				：スティック情報
---------------------------------------------------------------------------------------------*/
Vector2 HirakawaLib::GetGamePadLeftStick(int Number)
{
	return p_hl_input_->GetGamePadLeftStick(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：右スティック取得
//	@param	int			Number	：ゲームパッド番号
//	@retval	Vector2				：スティック情報
---------------------------------------------------------------------------------------------*/
Vector2 HirakawaLib::GetGamePadRightStick(int Number)
{
	return p_hl_input_->GetGamePadRightStick(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：左トリガー取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	float			：トリガー情報
---------------------------------------------------------------------------------------------*/
float HirakawaLib::GetGamePadLeftTrigger(int Number)
{
	return p_hl_input_->GetGamePadLeftTrigger(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ゲームパッド：右トリガー取得
//	@param	int		Number	：ゲームパッド番号
//	@retval	float			：トリガー情報
---------------------------------------------------------------------------------------------*/
float HirakawaLib::GetGamePadRightTrigger(int Number)
{
	return p_hl_input_->GetGamePadRightTrigger(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	テクスチャデータ作成
//	@param	wchar_t*			pFilename	：ファイル名
//	@param	LPHLTextureData*	pTextureData：テクスチャデータ格納先ポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateTextureData(wchar_t* pFilename, LPHLTextureData* ppTextureData)
{
	return p_hl_texture_->CreateTextureData(p_hl_direct3d_->GetDevice(), pFilename, ppTextureData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	テクスチャデータ解放
//	@param	LPHLTextureData pTextureData：解放したいテクスチャデータポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseTextureData(LPHLTextureData pTextureData)
{
	return p_hl_texture_->ReleaseTextureData(pTextureData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ作成
//	@param	wchar_t*		pFilename	：ファイル名
//	@param	LPHLModelData*	ppModelData	：モデルデータ格納先ポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateModelData(wchar_t* pFilename, LPHLModelData* ppModelData)
{
	return p_hl_model_->CreateModelData(p_hl_direct3d_->GetDevice(), pFilename, ppModelData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ解放
//	@param	LPHLModelData pModelData：解放したいモデルデータポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseModelData(LPHLModelData pModelData)
{
	return p_hl_model_->ReleaseModelData(pModelData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ作成
//	@param	wchar_t*		pFilename	：ファイル名
//	@param	LPHLAudioData*	ppAudioData	：オーディオデータ格納先ポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateAudioData(wchar_t* pFilename, LPHLAudioData* ppAudioData)
{
	return p_hl_audio_->CreateAudioDataFromWave(pFilename, ppAudioData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ解放
//	@param	LPHLAudioData pModelData：解放したいオーディオデータポインタ
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseAudioData(LPHLAudioData pAudioData)
{
	return p_hl_audio_->ReleaseAudioData(pAudioData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	スプライト描画
//	@param	LPHLTextureData pTextureData	：描画するテクスチャデータ
//	@param	Vector2			Position		：描画する座標
//	@param	RECT			Rect			：テクスチャ切り取り範囲
//	@param	Vector4			Color			：テクスチャカラー
//	@param	float			Rotation		：回転
//	@param	float			Scale			：拡大
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawSprite(LPHLTextureData pTextureData, Vector2 Position, RECT Rect, Vector4 Color,
							Vector2 Origin, float Rotation, float Scale)
{
	p_hl_sprite_->DrawSprite(pTextureData, Position, Rect, Color, Origin, Rotation, Scale);
}

/*---------------------------------------------------------------------------------------------
//	@brief	フォント描画
//	@param	wchar_t*	pString		：文字列
//	@param	Vector2		Position	：描画する座標
//	@param	XMVECTOR	Color		：文字の色
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawFont(wchar_t* pString, Vector2 Position, XMVECTOR Color)
{
	p_hl_sprite_->DrawFont(pString, Position, Color);
}

/*---------------------------------------------------------------------------------------------
//	@brief	ワールド行列作成
//	@param	Vector3 Position	：座標
//	@param	Vector3 Rotation	：回転
//	@param	Vector3 Scale		：拡縮
//	@retval	Matrix				：ワールド行列
---------------------------------------------------------------------------------------------*/
Matrix HirakawaLib::CreateWorldMatrix(Vector3 Position, Vector3 Rotation, Vector3 Scale)
{
	Matrix	world = XMMatrixIdentity();

	world *= XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	world *= XMMatrixRotationX(XMConvertToRadians(Rotation.x));
	world *= XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	world *= XMMatrixRotationZ(XMConvertToRadians(Rotation.z));
	world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	return world;
}

/*---------------------------------------------------------------------------------------------
//	@brief	ワールド行列作成
//	@param	LPHLModelData	pModel		：モデルデータ
//	@param	Matrix			WorldMatrix	：ワールド行列
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawModel(LPHLModelData pModel, Matrix WorldMatrix)
 {
	 if (pModel) {
		 pModel->p_model->Draw(p_hl_direct3d_->GetDeviceContext(), *(p_hl_direct3d_->GetCommonStates()),
								 WorldMatrix, camera_view_matrix_, camera_projection_matrix_);
	 }
 }

