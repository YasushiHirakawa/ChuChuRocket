/****************************************************************************************************
//
//		Game.h
//
//		@brief	�Q�[������
//		@author	Yasushi Hirakawa
//		@date	2016/09/20
//
****************************************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "../HirakawaLib/Inc/HirakawaLib.h"


//---------------------------------------------------------------------------------------------
//	enum
//---------------------------------------------------------------------------------------------
enum GAME_STATE
{
	GAME_INIT = 0,			// ������
	GAME_WORK,				// ���
	GAME_DESTROY,			// �j��
	MAX_GAME_STATE			// �ő�l
};

enum DRAW_STATE
{
	DRAW_INIT = 0,			// ������
	DRAW_WORK,				// ���
	DRAW_DESTROY,			// �j��
	MAX_DRAW_STATE			// �ő�l
};

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();

	int		Update();	// �X�V
	void	Destroy();	// �j��

private:
	// �Q�[���𐧌䂷��֐��|�C���^�̒�`
	int		(Game::*GameFunc[MAX_GAME_STATE])();
	void	(Game::*DrawFunc[MAX_DRAW_STATE])();

	// �֐��|�C���^�Ăяo���C���f�b�N�X
	GAME_STATE	game_state_;
	DRAW_STATE	draw_state_;

	// �Q�[����Ɗ֐�
	int		GameInit();			// �Q�[�� ������
	int		GameWork();			// �Q�[�� ���
	int		GameDestroy();		// �Q�[�� �j��

	// �`���Ɗ֐�
	void	DrawInit();			// �`��@������
	void	DrawWork();			// �`��@���
	void	DrawDestroy();		// �`��@�j��


	LPHLTextureData		p_texture_data_;
	LPHLTextureData		p_texture_data_alpha_;
	LPHLModelData		p_model_data_;
	LPHLAudioData		p_audio_data_;

	Vector3				position_;

	Vector2				stick_;
	float				trigger_;
	MOUSE_POSITION		mouse_pos_;
	int					wheel_;
};

#endif //__GAME_H__
