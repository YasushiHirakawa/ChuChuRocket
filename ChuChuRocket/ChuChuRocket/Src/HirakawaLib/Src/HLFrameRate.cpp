/****************************************************************************************************
//
//		HLFrameRate.cpp
//
//		@brief	KoudoLib フレームレート管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/


#include "HLFrameRate.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLFrameRate::HLFrameRate()
: draw_flag_(true)
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLFrameRate::~HLFrameRate()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@retval	true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLFrameRate::Init()
{
	bool ret = true;

	// サンプル数の設定
	diffrent_time_sum_ = 0;
	diffrent_time_save_ = 0;

	// 計測する時計の選択
	LARGE_INTEGER	query_counter;
	QueryPerformanceCounter(&query_counter);
	old_query_count_ = query_counter.QuadPart;	// 生成時の時刻（クロック数）を取得
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);			// 1秒当たりクロック数を取得
	query_frequency_ = (double)freq.QuadPart;
	one_qruery_freq_count_ = freq.QuadPart / kFPS;

	// 計測
	GetFPS();

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
void HLFrameRate::Destroy()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	FPS値を取得
//	@retval double型	：FPS値
---------------------------------------------------------------------------------------------*/
double HLFrameRate::GetFPS()
{
	double Def = GetCurDefTime();
	if (Def == 0) {
		// 計算できないのでを返す
		return 0;
	}

	return UpdateFPS(Def);
}

/*---------------------------------------------------------------------------------------------
//	@brief	現在の差分時刻をミリ秒単位で取得
//	@retval	double型	：前フレームとの差分（ミリ秒）
---------------------------------------------------------------------------------------------*/
double HLFrameRate::GetCurDefTime()
{
	double ret = 0.0;

	// 差分時間を計測
	LARGE_INTEGER	query_counter;
	QueryPerformanceCounter(&query_counter);		// 現在の時刻を取得
	LONGLONG que_count = query_counter.QuadPart;

	// 1フレームの時間に到達していたら...
	if (que_count >= (old_query_count_ + one_qruery_freq_count_)) {
		LONGLONG def_count = que_count - old_query_count_;		// 差分カウント数を算出する。
		double def = (double)def_count;							// 倍精度浮動小数点に変換
		ret = def * 1000 / query_frequency_;					// 差分時間をミリ秒単位で返す

		old_query_count_	= que_count;	// 現在の時刻を保持
		draw_flag_			= true;			// 描画フラグをtrue
	}
	else {
		draw_flag_			= false;		// 到達してないから描画フラグをfalse
	}

	return ret;
}


/*---------------------------------------------------------------------------------------------
//	@brief	FPSを更新
//	@retval	double型	：FPS値
---------------------------------------------------------------------------------------------*/
double HLFrameRate::UpdateFPS(double Def)
{
	diffrent_time_save_ = Def;

	// FPS算出
	double fps;
	double average_def = diffrent_time_sum_ + Def;
	if (average_def != 0) {
		fps = 1000.0 / average_def;
	}

	// 共通加算部分の更新
	diffrent_time_sum_ += Def - diffrent_time_save_;

	return fps;
}
