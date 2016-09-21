/****************************************************************************************************
//
//		HLAudio.h
//
//		@brief	KoudoLib サウンド管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLAUDIO_H__
#define __HLAUDIO_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"

//---------------------------------------------------------------------------------------------
//	struct
//--------------------------------------------------------------------------------------------- 
// オーディオデータ構造体
typedef struct HLAudioData
{
	std::unique_ptr<SoundEffectInstance>	p_audio_instance;		// オーディオインスタンスデータ
	wchar_t									p_file_name[kStrMax];	// モデルファイル名

	HLAudioData()
	{
		p_audio_instance = NULL;
		p_file_name[0]	= '\0';
	}

	// 再生
	void Play(bool Loop = false) { p_audio_instance->Play(Loop); }
	// 停止
	inline void Stop() { p_audio_instance->Stop(); }
	// 一時停止
	inline void Pause() { p_audio_instance->Pause(); }
	// ボリューム設定
	inline void SetVolume(float Volume) { p_audio_instance->SetVolume(Volume); }
	// ピッチ設定
	inline void SetPitch(float Pitch) { p_audio_instance->SetPitch(Pitch); }
	// パン設定
	inline void SetPan(float Pan) { p_audio_instance->SetPan(Pan); }

} *LPHLAudioData;

// オーディオデータリスト構造体
typedef struct HLAudioDataList
{
	HLAudioData*			p_audio_data;	// オーディオデータ
	SoundEffect*			p_audio_origin;	// オーディオの元データ
	HLAudioDataList*		p_next;			// リストの次データ
	int						ref_counter;	// 参照カウンタ

	HLAudioDataList()
	{
		p_audio_data	= NULL;
		p_audio_origin	= NULL;
		p_next			= NULL;
		ref_counter		= 1;
	}
} *LPHLAudioDataList;

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLAudio
{
public:
	HLAudio();
	~HLAudio();

	bool Init();				// 初期化
	void Destroy();				// 破棄
	void Update();				// 更新

	// オーディオデータ読み込み
	bool	CreateAudioDataFromWave(const wchar_t* pFilename, LPHLAudioData* ppAudioData);

	// オーディオデータ解放
	bool	ReleaseAudioData(LPHLAudioData pAudioData);

private:
	AudioEngine*		p_audio_engine_;		// オーディオエンジン
	LPHLAudioDataList	p_audio_data_list_top_;	// オーディオデータリスト先頭
};


#endif //__HLAUDIO_H__
