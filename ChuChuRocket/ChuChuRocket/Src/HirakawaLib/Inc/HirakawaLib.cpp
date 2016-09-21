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
HLDirect3D*		HirakawaLib::p_hl_direct3d_;			// Direct3D�Ǘ�
HLFrameRate*	HirakawaLib::p_hl_framer_rate_;		// �t���[�����[�g�Ǘ�
HLSprite*		HirakawaLib::p_hl_sprite_;				// �X�v���C�g�f�[�^�Ǘ�
HLTexture*		HirakawaLib::p_hl_texture_;			// �e�N�X�`���f�[�^�Ǘ�
HLModel*		HirakawaLib::p_hl_model_;				// ���f���f�[�^�Ǘ�
HLInput*		HirakawaLib::p_hl_input_;				// ���͊Ǘ�
HLAudio*		HirakawaLib::p_hl_audio_;				// �I�[�f�B�I�Ǘ�

Matrix			HirakawaLib::camera_view_matrix_;			// �J�����̃r���[�s��
Matrix			HirakawaLib::camera_projection_matrix_;	// �J�����̎ˉe�s��


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
HirakawaLib::HirakawaLib()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
HirakawaLib::~HirakawaLib()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	������
//	@param	HWND hWnd	�F�E�B���h�E�n���h��
//	@retval	true		�F����
//	@retval	false		�F���s
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

		// �t���[�����[�g
		p_hl_framer_rate_ = new HLFrameRate;
		if (p_hl_framer_rate_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_framer_rate_->Init() == false) {
			ret = false;
			break;
		}

		// �X�v���C�g�Ǘ�
		p_hl_sprite_ = new HLSprite;
		if (p_hl_sprite_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_sprite_->Init(p_hl_direct3d_->GetDevice(), p_hl_direct3d_->GetDeviceContext()) == false) {
			ret = false;
			break;
		}

		// �e�N�X�`���f�[�^�Ǘ�
		p_hl_texture_ = new HLTexture;
		if (p_hl_texture_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_texture_->Init() == false) {
			ret = false;
			break;
		}

		// ���f���f�[�^�Ǘ�
		p_hl_model_ = new HLModel;
		if (p_hl_model_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_model_->Init(p_hl_direct3d_->GetDevice()) == false) {
			ret = false;
			break;
		}

		// ���͊Ǘ�
		p_hl_input_ = new HLInput;
		if (p_hl_input_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_input_->Init() == false) {
			ret = false;
			break;
		}

		// �I�[�f�B�I�Ǘ�
		p_hl_audio_ = new HLAudio;
		if (p_hl_audio_ == NULL) {
			ret = false;
			break;
		}
		if (p_hl_audio_->Init() == false) {
			ret = false;
			break;
		}

		// �E�B���h�E�n���h���̕ۑ�
		HLCommon::SetWindowHandle(hWnd);


		BeginSprite();
		EndSprite();

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void HirakawaLib::Destroy()
{
	p_hl_input_->Destroy();			// ����
	SAFE_DELETE(p_hl_input_);
	p_hl_model_->Destroy();			// ���f���f�[�^
	SAFE_DELETE(p_hl_model_);
	p_hl_texture_->Destroy();		// �e�N�X�`���f�[�^
	SAFE_DELETE(p_hl_texture_);
	p_hl_sprite_->Destroy();		// �X�v���C�g
	SAFE_DELETE(p_hl_sprite_);
	p_hl_framer_rate_->Destroy();	// �t���[�����[�g
	SAFE_DELETE(p_hl_framer_rate_);
	p_hl_direct3d_->Destroy();		// Direct3D
	SAFE_DELETE(p_hl_direct3d_);
	p_hl_audio_->Destroy();			// �I�[�f�B�I
	SAFE_DELETE(p_hl_audio_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�V
//	@retval	true		�F����
//	@retval	false		�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::Update()
{
	bool ret = false;
	
	float fps = (float)p_hl_framer_rate_->GetFPS();

	// �`��t���O�������Ă�����Q�[��������
	if (p_hl_framer_rate_->GetDrawFlag()) {
#if _DEBUG
		// �t���[�����[�g�`��
		wchar_t	str[kStrMax];
		swprintf_s(str, (L"%s�F%.1f"), HLCommon::GetWindowName(), fps);
		SetWindowText(HLCommon::GetWindowHandle(), str);
#endif
		// ���͍X�V
		p_hl_input_->Update();

		// �I�[�f�B�I�X�V
		p_hl_audio_->Update();

		// ���f���X�V
		p_hl_model_->Update();
		
		// �߂�l��true�ɕύX
		ret = true;
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�J�����̃r���[�s��̐ݒ�
//	@param	Vector3 Position	�F�J�������W
//	@param	Vector3 Lookat		�F�J�����������W
//	@param	Vector3 Up			�F�J�����̏�����̌���
//	@retval	Matrix				�F�J�����̃r���[�s��
---------------------------------------------------------------------------------------------*/
Matrix HirakawaLib::SetCameraViewMatrix(Vector3 Position, Vector3 Lookat, Vector3 Up)
{
	camera_view_matrix_ = p_hl_direct3d_->SetCameraViewMatrix(Position, Lookat, Up);

	return camera_view_matrix_;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�J�����̎ˉe�s��̐ݒ�
//	@param	unsigned int	Width	�F��ʃT�C�Y��
//	@param	unsigned int	Height	�F��ʃT�C�Y�c
//	@param	float			Near	�F�j�A�N���b�v����
//	@param	float			Far		�F�t�@�[�N���b�v����
//	@retval	Matrix					�F�J�����̎ˉe�s��
---------------------------------------------------------------------------------------------*/
Matrix HirakawaLib::SetCameraProjectionMatrix(unsigned int Width, unsigned int Height, float Near, float Far)
{
	camera_projection_matrix_ = p_hl_direct3d_->SetCameraProjectionMatrix(Width, Height, Near, Far);

	return camera_projection_matrix_;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�E�B���h�E�̓��̓R�[���o�b�N��ݒ肷��
//	@param	hWnd		�E�B���h�E�n���h��
//	@param	msg			���b�Z�[�W
//	@param	wParam		���b�Z�[�W�Ɋւ���ǉ����
//	@param	lParam		���b�Z�[�W�Ɋւ���ǉ����
---------------------------------------------------------------------------------------------*/
void HirakawaLib::SetWindowInputCallback(HWND hWnd, unsigned int msg, unsigned int wParam, LONG lParam)
{
	p_hl_input_->SetWindowInputCallback(hWnd, msg, wParam, lParam);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�L�[�{�[�h�F�{�^���������ꂽ���H(�������ςȂ��L��)
//	@param	Keyboard::Keys Key		�F�L�[���
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsPressKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�L�[�{�[�h�F�{�^���������ꂽ���H(�������ςȂ�����)
//	@param	Keyboard::Keys Key		�F�L�[���
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsPushKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�L�[�{�[�h�F�{�^���������ꂽ��
//	@param	Keyboard::Keys Key		�F�L�[���
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseKeyboard(Keyboard::Keys Key)
{
	return p_hl_input_->IsReleaseKeyboard(Key);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�}�E�X�F�{�^���������ꂽ���H(�������ςȂ��L��)
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsPressMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�}�E�X�F�{�^���������ꂽ���H(�������ςȂ�����)
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsPushMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�}�E�X�F�{�^���������ꂽ��
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseMouse(HLMOUSE_BUTTONS Button)
{
	return p_hl_input_->IsReleaseMouse(Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�}�E�X�F���W�擾
//	@retval	MOUSE_POSITION	�F�}�E�X���W
---------------------------------------------------------------------------------------------*/
MOUSE_POSITION HirakawaLib::GetMousePosition()
{
	return p_hl_input_->GetMousePosition();
}

/*---------------------------------------------------------------------------------------------
//	@brief	�}�E�X�F���W�擾
//	@retval	int		�F�z�C�[���̒l
---------------------------------------------------------------------------------------------*/
int HirakawaLib::GetMouseScrollWheel()
{
	return p_hl_input_->GetMouseScrollWheel();
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F�{�^���������ꂽ���H(�������ςȂ��L��)
//	@param	int				Number	�F�Q�[���p�b�h�ԍ�
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPressGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsPressGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F�{�^���������ꂽ���H(�������ςȂ�����)
//	@param	int				Number	�F�Q�[���p�b�h�ԍ�
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsPushGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsReleaseGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F�{�^���������ꂽ��
//	@param	int				Number	�F�Q�[���p�b�h�ԍ�
//	@param	HLMOUSE_BUTTONS Button	�F�{�^�����
//	@retval	bool					�F���͔��茋��
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::IsReleaseGamePad(int Number, HLGAMEPAD_BUTTONS Button)
{
	return p_hl_input_->IsPushGamePad(Number, Button);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F���X�e�B�b�N�擾
//	@param	int			Number	�F�Q�[���p�b�h�ԍ�
//	@retval	Vector2				�F�X�e�B�b�N���
---------------------------------------------------------------------------------------------*/
Vector2 HirakawaLib::GetGamePadLeftStick(int Number)
{
	return p_hl_input_->GetGamePadLeftStick(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F�E�X�e�B�b�N�擾
//	@param	int			Number	�F�Q�[���p�b�h�ԍ�
//	@retval	Vector2				�F�X�e�B�b�N���
---------------------------------------------------------------------------------------------*/
Vector2 HirakawaLib::GetGamePadRightStick(int Number)
{
	return p_hl_input_->GetGamePadRightStick(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F���g���K�[�擾
//	@param	int		Number	�F�Q�[���p�b�h�ԍ�
//	@retval	float			�F�g���K�[���
---------------------------------------------------------------------------------------------*/
float HirakawaLib::GetGamePadLeftTrigger(int Number)
{
	return p_hl_input_->GetGamePadLeftTrigger(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���p�b�h�F�E�g���K�[�擾
//	@param	int		Number	�F�Q�[���p�b�h�ԍ�
//	@retval	float			�F�g���K�[���
---------------------------------------------------------------------------------------------*/
float HirakawaLib::GetGamePadRightTrigger(int Number)
{
	return p_hl_input_->GetGamePadRightTrigger(Number);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�e�N�X�`���f�[�^�쐬
//	@param	wchar_t*			pFilename	�F�t�@�C����
//	@param	LPHLTextureData*	pTextureData�F�e�N�X�`���f�[�^�i�[��|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateTextureData(wchar_t* pFilename, LPHLTextureData* ppTextureData)
{
	return p_hl_texture_->CreateTextureData(p_hl_direct3d_->GetDevice(), pFilename, ppTextureData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�e�N�X�`���f�[�^���
//	@param	LPHLTextureData pTextureData�F����������e�N�X�`���f�[�^�|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseTextureData(LPHLTextureData pTextureData)
{
	return p_hl_texture_->ReleaseTextureData(pTextureData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^�쐬
//	@param	wchar_t*		pFilename	�F�t�@�C����
//	@param	LPHLModelData*	ppModelData	�F���f���f�[�^�i�[��|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateModelData(wchar_t* pFilename, LPHLModelData* ppModelData)
{
	return p_hl_model_->CreateModelData(p_hl_direct3d_->GetDevice(), pFilename, ppModelData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^���
//	@param	LPHLModelData pModelData�F������������f���f�[�^�|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseModelData(LPHLModelData pModelData)
{
	return p_hl_model_->ReleaseModelData(pModelData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^�쐬
//	@param	wchar_t*		pFilename	�F�t�@�C����
//	@param	LPHLAudioData*	ppAudioData	�F�I�[�f�B�I�f�[�^�i�[��|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::CreateAudioData(wchar_t* pFilename, LPHLAudioData* ppAudioData)
{
	return p_hl_audio_->CreateAudioDataFromWave(pFilename, ppAudioData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^���
//	@param	LPHLAudioData pModelData�F����������I�[�f�B�I�f�[�^�|�C���^
//	@retval	true	�F����
//	@retval	false	�F���s
---------------------------------------------------------------------------------------------*/
bool HirakawaLib::ReleaseAudioData(LPHLAudioData pAudioData)
{
	return p_hl_audio_->ReleaseAudioData(pAudioData);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�v���C�g�`��
//	@param	LPHLTextureData pTextureData	�F�`�悷��e�N�X�`���f�[�^
//	@param	Vector2			Position		�F�`�悷����W
//	@param	RECT			Rect			�F�e�N�X�`���؂���͈�
//	@param	Vector4			Color			�F�e�N�X�`���J���[
//	@param	float			Rotation		�F��]
//	@param	float			Scale			�F�g��
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawSprite(LPHLTextureData pTextureData, Vector2 Position, RECT Rect, Vector4 Color,
							Vector2 Origin, float Rotation, float Scale)
{
	p_hl_sprite_->DrawSprite(pTextureData, Position, Rect, Color, Origin, Rotation, Scale);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�t�H���g�`��
//	@param	wchar_t*	pString		�F������
//	@param	Vector2		Position	�F�`�悷����W
//	@param	XMVECTOR	Color		�F�����̐F
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawFont(wchar_t* pString, Vector2 Position, XMVECTOR Color)
{
	p_hl_sprite_->DrawFont(pString, Position, Color);
}

/*---------------------------------------------------------------------------------------------
//	@brief	���[���h�s��쐬
//	@param	Vector3 Position	�F���W
//	@param	Vector3 Rotation	�F��]
//	@param	Vector3 Scale		�F�g�k
//	@retval	Matrix				�F���[���h�s��
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
//	@brief	���[���h�s��쐬
//	@param	LPHLModelData	pModel		�F���f���f�[�^
//	@param	Matrix			WorldMatrix	�F���[���h�s��
---------------------------------------------------------------------------------------------*/
void HirakawaLib::DrawModel(LPHLModelData pModel, Matrix WorldMatrix)
 {
	 if (pModel) {
		 pModel->p_model->Draw(p_hl_direct3d_->GetDeviceContext(), *(p_hl_direct3d_->GetCommonStates()),
								 WorldMatrix, camera_view_matrix_, camera_projection_matrix_);
	 }
 }

