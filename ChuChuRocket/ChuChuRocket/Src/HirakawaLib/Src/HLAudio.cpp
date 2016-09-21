/****************************************************************************************************
//
//		HLAudio.h
//
//		@brief	HirakawaLib サウンド管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLAudio.h"

//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLAudio::HLAudio()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLAudio::~HLAudio()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLAudio::Init()
{
	bool	ret = true;

	do {

		// オーディオエンジンのメモリ確保
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
		eflags = eflags | AudioEngine_Debug;
#endif
		p_audio_engine_ = new AudioEngine(eflags);
		if (p_audio_engine_ == NULL) {
			ret = false;
			break;
		}

		// オーディオデータリストの先頭を初期化
		p_audio_data_list_top_ = NULL;

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HLAudio::Destroy()
{
	// リストデータ全削除
	LPHLAudioDataList	p_list = p_audio_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"解放されていないオーディオデータがあったので強制解放しました。\n");
		wcscat_s(warning_message, p_delete_list->p_audio_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_DELETE(p_delete_list->p_audio_data);
		SAFE_DELETE(p_delete_list->p_audio_origin);
		SAFE_DELETE(p_delete_list);
	}

	// オーディオエンジンの削除
	SAFE_DELETE(p_audio_engine_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	更新
---------------------------------------------------------------------------------------------*/
void HLAudio::Update()
{
	if (!p_audio_engine_->Update()) {
		if (p_audio_engine_->IsCriticalError()) {

			// ここでオーディオエンジンがロストすることがあるらしい。
			// 要対応検討

		}
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	オーディオデータ作成
//	@param	const wchar_t*		pFilename		：ファイル名
//	@param	LPHLTextureData*	ppTextureData	：テクスチャデータ格納先ポインタ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLAudio::CreateAudioDataFromWave(const wchar_t* pFilename, LPHLAudioData* ppAudioData)
{
	bool				ret = true;
	LPHLAudioDataList	p_list = NULL;

	LPHLAudioData		p_audio_data = NULL;
	LPHLAudioDataList	p_audio_data_list = NULL;

	do {

		// オーディオデータリストにあるかチェック
		bool	found_flag = false;
		p_list = p_audio_data_list_top_;
		while (p_list != NULL) {
			// ファイル名をチェックして見つけたオーディオデータを格納してフラグを立てて抜ける
			if (wcscmp(p_list->p_audio_data->p_file_name, pFilename) == 0) {
				*ppAudioData = p_list->p_audio_data;
				p_list->ref_counter++;
				found_flag = true;
				break;
			}
			p_list = p_list->p_next;
		}

		// すでにデータがあったから抜ける
		if (found_flag) {
			break;
		}

		// オーディオデータリストのメモリ確保
		p_audio_data_list = new HLAudioDataList;
		if (p_audio_data_list == NULL) {
			ret = false;
			break;
		}

		// オーディオデータ作成
		if (wcsstr(pFilename, L".wav")) {
			p_audio_data_list->p_audio_origin = new SoundEffect(p_audio_engine_, pFilename);
		}
		if (p_audio_data_list->p_audio_origin == NULL) {
			ret = false;
			break;
		}

		// オーディオデータのメモリ確保
		p_audio_data = new HLAudioData;
		if (p_audio_data == NULL) {
			ret = false;
			break;
		}

		// オーディオインスタンスデータを受け取る
		p_audio_data->p_audio_instance = p_audio_data_list->p_audio_origin->CreateInstance();
		if (p_audio_data->p_audio_instance == NULL) {
			ret = false;
			break;
		}

		// ファイル名のコピー
		wcscpy_s(p_audio_data->p_file_name, pFilename);

		// テクスチャデータをモデルデータリストに格納
		p_audio_data_list->p_audio_data = p_audio_data;

		// 末尾に繋げる
		if (p_audio_data_list_top_ == NULL) {
			p_audio_data_list_top_ = p_audio_data_list;
		}
		else {
			p_list = p_audio_data_list_top_;
			while (p_list->p_next != NULL) {
				p_list = p_list->p_next;
			}
			p_list->p_next = p_audio_data_list;
		}

		// 仮引数に確保したテクスチャデータを保存
		*ppAudioData = p_audio_data;

	} while (0);

	// エラーが発生した場合の対処
	if (ret == false) {
		SAFE_DELETE(p_audio_data);
		SAFE_DELETE(p_audio_data_list->p_audio_origin);
		SAFE_DELETE(p_audio_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	オーディオデータ解放
//	@param	LPHLAudioData pAudioData	：解放するオーディオデータ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLAudio::ReleaseAudioData(LPHLAudioData pAudioData)
{
	bool	ret = false;

	// テクスチャデータリストにテクスチャデータがあるかチェック
	LPHLAudioDataList	p_list = p_audio_data_list_top_,
						p_prev_list = NULL;
	while (p_list != NULL) {
		// 同名のオーディオデータがあったら参照カウンタをデクリメントして、
		// 参照カウンタが０になったら削除する
		if (wcscmp(p_list->p_audio_data->p_file_name, pAudioData->p_file_name) == 0) {
			p_list->ref_counter--;
			if (p_list->ref_counter == 0) {
				if (p_prev_list) {
					p_prev_list->p_next = p_list->p_next;
				}
				else {
					p_audio_data_list_top_ = p_list->p_next;
				}
				SAFE_DELETE(p_list->p_audio_data);
				SAFE_DELETE(p_list->p_audio_origin);
				SAFE_DELETE(p_list);
			}

			ret = true;
			break;
		}

		p_prev_list = p_list;
		p_list = p_list->p_next;
	}

	return ret;
}



