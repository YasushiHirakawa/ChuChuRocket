/****************************************************************************************************
//
//		HLFrameRate.h
//
//		@brief	KoudoLib フレームレート管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLFRAMERATE_H__
#define __HLFRAMERATE_H__

//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"

//---------------------------------------------------------------------------------------------
//	const
//---------------------------------------------------------------------------------------------
const int	kFPS		= 60;	// FPS

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLFrameRate
{
public:
	HLFrameRate();
	~HLFrameRate();

	bool	Init();									// 初期化
	void	Destroy();								// 破棄
	double	GetFPS();								// FPS値を取得
	bool	GetDrawFlag() { return draw_flag_; }	// 描画フラグの取得

private:
	double	GetCurDefTime();				// 現在の時刻を取得
	double	UpdateFPS(double Def);			// FPSを更新

	double				query_frequency_;		// 1秒当たりクロックカウント数（周波数）
	LONGLONG			old_query_count_;		// 以前のクロックカウント数
	LONGLONG			one_qruery_freq_count_;	// 1フレームあたりのクロック数
	DWORD				tgt_old_count_;			// 以前の時刻（ミリ秒）
	double				diffrent_time_save_;	// 前フレームのＦＰＳ保持
	double				diffrent_time_sum_;		// ＦＰＳの差分合計
	bool				draw_flag_;				// 描画フラグ
};

#endif	//__HLFRAMERATE_H__
