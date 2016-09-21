/****************************************************************************************************
//
//		Game.cpp
//
//		@brief	�Q�[������
//		@author	Yasushi Hirakawa
//		@date	2016/09/20
//
****************************************************************************************************/

#include "Game.h"

/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
Game::Game()
{
	// �֐��|�C���^�̓o�^
	GameFunc[GAME_INIT] = &Game::GameInit;
	GameFunc[GAME_WORK] = &Game::GameWork;
	GameFunc[GAME_DESTROY] = &Game::GameDestroy;

	DrawFunc[DRAW_INIT] = &Game::DrawInit;
	DrawFunc[DRAW_WORK] = &Game::DrawWork;
	DrawFunc[DRAW_DESTROY] = &Game::DrawDestroy;

	// �֐��|�C���^�Ăяo���C���f�b�N�X�̏�����
	game_state_ = GAME_INIT;
	draw_state_ = DRAW_INIT;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
Game::~Game()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�V
//	@retval	�P		�F���[�v�p��
//	@retval	�O		�F���[�v�I��
---------------------------------------------------------------------------------------------*/
int Game::Update()
{
	int ret = (this->*GameFunc[game_state_])();

	if (ret != 0) {
		(this->*DrawFunc[draw_state_])();
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void Game::Destroy()
{
	GameDestroy();		// �Q�[��
	DrawDestroy();		// �`��
}


/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���@������
//	@retval	�P		�F���[�v�p��
//	@retval	�O		�F���[�v�I��
---------------------------------------------------------------------------------------------*/
int Game::GameInit()
{
	HirakawaLib::CreateTextureData(L"Resource/windowslogo.dds", &p_texture_data_);
	HirakawaLib::CreateTextureData(L"Resource/gameloader.png", &p_texture_data_alpha_);
	HirakawaLib::CreateAudioData(L"Resource/bgm_01.wav", &p_audio_data_);

	p_audio_data_->Play();

	_wchdir(L"Resource");
	HirakawaLib::CreateModelData(L"tiny.sdkmesh", &p_model_data_);
	_wchdir(L"../");

	HirakawaLib::SetCameraViewMatrix(Vector3(0.0f, 3.0f, -900.0f), Vector3(0.0f, 0.0f, 0.0f));
	HirakawaLib::SetCameraProjectionMatrix(kWindowWidth, kWindowWidth, 1.0f, 1000.0f);

	position_ = Vector3(0.0f, 0.0f, 0.0f);


	HLLightInfo light_info = HirakawaLib::GetLightInfo();
	light_info.enable = true;
	light_info.diffuse_color = Vector3(0.0f, 0.0f, 0.0f);
	HirakawaLib::SetLightInfo(light_info);


	game_state_ = GAME_WORK;

	return 1;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���@���
//	@retval	�P		�F���[�v�p��
//	@retval	�O		�F���[�v�I��
---------------------------------------------------------------------------------------------*/
int Game::GameWork()
{
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_L_STICK_RIGHT)) {
		position_.x += 10.0f;
		p_audio_data_->Play();
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_L_STICK_LEFT)) {
		position_.x -= 10.0f;
		p_audio_data_->Stop();
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_L_STICK_UP)) {
		position_.y += 10.0f;
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_L_STICK_DOWN)) {
		position_.y -= 10.0f;
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_R_STICK_RIGHT)) {
		position_.x += 10.0f;
		p_audio_data_->Play();
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_R_STICK_LEFT)) {
		position_.x -= 10.0f;
		p_audio_data_->Stop();
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_R_STICK_UP)) {
		position_.y += 10.0f;
	}
	if (HirakawaLib::IsPushGamePad(0, HLGAMEPAD_R_STICK_DOWN)) {
		position_.y -= 10.0f;
	}

	stick_ = HirakawaLib::GetGamePadRightStick(0);
	trigger_ = HirakawaLib::GetGamePadRightTrigger(0);

	mouse_pos_ = HirakawaLib::GetMousePosition();
	wheel_ = HirakawaLib::GetMouseScrollWheel();

	HLLightInfo light_info = HirakawaLib::GetLightInfo();
	light_info.diffuse_color += Vector3(0.005f, 0.005f, 0.005f);
	if (light_info.diffuse_color.x > 1.0f) {
		light_info.diffuse_color = Vector3(0.0f, 0.0f, 0.0f);
	}
	HirakawaLib::SetLightInfo(light_info);

	// �G�X�P�[�v�Ŕj��
	if (HirakawaLib::IsPushKeyboard(Keyboard::Escape)) {
		game_state_ = GAME_DESTROY;
		draw_state_ = DRAW_DESTROY;
	}

	return 1;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���@�j��
//	@retval	�P		�F���[�v�p��
//	@retval	�O		�F���[�v�I��
---------------------------------------------------------------------------------------------*/
int Game::GameDestroy()
{
	HirakawaLib::ReleaseTextureData(p_texture_data_);
	HirakawaLib::ReleaseTextureData(p_texture_data_alpha_);
	HirakawaLib::ReleaseModelData(p_model_data_);
	HirakawaLib::ReleaseAudioData(p_audio_data_);

	return 0;
}


/*---------------------------------------------------------------------------------------------
//	@brief	�`��@������
---------------------------------------------------------------------------------------------*/
void Game::DrawInit()
{

	draw_state_ = DRAW_WORK;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���@���
---------------------------------------------------------------------------------------------*/
void Game::DrawWork()
{
	// �`��J�n
	HirakawaLib::BeginScene();

	Matrix world = HirakawaLib::CreateWorldMatrix(position_, Vector3(270.0f, 180.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	HirakawaLib::DrawModel(p_model_data_, world);

	// �X�v���C�g�`��J�n
	HirakawaLib::BeginSprite();

	RECT rect;
	Vector4 color = { 1.0f, 1.0f, 1.0f, 0.5f };
	rect.left = rect.top = 0L;
	rect.right = rect.bottom = 256L;
	HirakawaLib::DrawSprite(p_texture_data_, Vector2(100.0f, 400.0f), rect, color);

	rect.left = rect.top = 512L;
	rect.right = rect.bottom = 1024L;
	HirakawaLib::DrawSprite(p_texture_data_alpha_, Vector2(50.0f, 300.0f), rect);

	WCHAR	str[kStrMax];
	swprintf_s(str, (L"%s %f %f"), L"Stick  ", stick_.x, stick_.y);
	HirakawaLib::DrawFont(str, Vector2(0.0f, 0.0f));
	swprintf_s(str, (L"%s %f"), L"trigger  ", trigger_);
	HirakawaLib::DrawFont(str, Vector2(0.0f, 50.0f));
	swprintf_s(str, (L"%s %d %d"), L"MousePos  ", mouse_pos_.x, mouse_pos_.y);
	HirakawaLib::DrawFont(str, Vector2(0.0f, 100.0f));
	swprintf_s(str, (L"%s %d"), L"MouseWheel  ", wheel_);
	HirakawaLib::DrawFont(str, Vector2(0.0f, 150.0f));

	// �X�v���C�g�`��I��
	HirakawaLib::EndSprite();

	// �`��I��
	HirakawaLib::EndScene();
}

/*---------------------------------------------------------------------------------------------
//	@brief	�Q�[���@�j��
---------------------------------------------------------------------------------------------*/
void Game::DrawDestroy()
{
}

