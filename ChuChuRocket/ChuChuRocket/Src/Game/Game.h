/****************************************************************************************************
//
//		Game.h
//
//		@brief	ゲーム処理
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
	GAME_INIT = 0,			// 初期化
	GAME_WORK,				// 作業
	GAME_DESTROY,			// 破棄
	MAX_GAME_STATE			// 最大値
};

enum DRAW_STATE
{
	DRAW_INIT = 0,			// 初期化
	DRAW_WORK,				// 作業
	DRAW_DESTROY,			// 破棄
	MAX_DRAW_STATE			// 最大値
};

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();

	int		Update();	// 更新
	void	Destroy();	// 破棄

private:
	// ゲームを制御する関数ポインタの定義
	int		(Game::*GameFunc[MAX_GAME_STATE])();
	void	(Game::*DrawFunc[MAX_DRAW_STATE])();

	// 関数ポインタ呼び出しインデックス
	GAME_STATE	game_state_;
	DRAW_STATE	draw_state_;

	// ゲーム作業関数
	int		GameInit();			// ゲーム 初期化
	int		GameWork();			// ゲーム 作業
	int		GameDestroy();		// ゲーム 破棄

	// 描画作業関数
	void	DrawInit();			// 描画　初期化
	void	DrawWork();			// 描画　作業
	void	DrawDestroy();		// 描画　破棄


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
