/****************************************************************************************************
//
//		HLTexture.h
//
//		@brief	KoudoLib テクスチャ管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLTexture.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLTexture::HLTexture()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLTexture::~HLTexture()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLTexture::Init()
{
	bool ret = true;

	do {

		// テクスチャデータリストの先頭を初期化
		p_texture_data_list_top_ = NULL;

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HLTexture::Destroy()
{
	// リストデータ全削除
	LPHLTextureDataList	p_list = p_texture_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"解放されていないテクスチャがあったので強制解放しました。\n");
		wcscat_s(warning_message, p_delete_list->p_texture_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_RELEASE(p_delete_list->p_texture_data->p_texture);
		SAFE_DELETE(p_delete_list->p_texture_data);
		SAFE_DELETE(p_delete_list);
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ作成
//	@param	ID3D11Device*		pDevice			：デバイス
//	@param	const wchar_t*		pFilename		：ファイル名
//	@param	LPHLTextureData*	ppTextureData	：テクスチャデータ格納先ポインタ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLTexture::CreateTextureData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLTextureData* ppTextureData)
{
	bool				ret = true;
	LPHLTextureDataList	p_list = NULL;

	LPHLTextureData		p_texture_data = NULL;
	LPHLTextureDataList	p_texture_data_list = NULL;

	do {

		// テクスチャデータリストにあるかチェック
		bool	found_flag = false;
		p_list = p_texture_data_list_top_;
		while (p_list != NULL) {
			// ファイル名をチェックして見つけたらテクスチャデータを格納してフラグを立てて抜ける
			if (wcscmp(p_list->p_texture_data->p_file_name, pFilename) == 0) {
				*ppTextureData = p_list->p_texture_data;
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

		// テクスチャデータのメモリ確保
		p_texture_data = new HLTextureData;
		if (p_texture_data == NULL) {
			ret = false;
			break;
		}

		// テクスチャデータ作成
		if (wcsstr(pFilename, L".dds")) {
			CreateDDSTextureFromFile(pDevice, pFilename, NULL, &p_texture_data->p_texture);
		}
		else {
			CreateWICTextureFromFile(pDevice, pFilename, NULL, &p_texture_data->p_texture);
		}
		if (p_texture_data->p_texture == NULL) {
			ret = false;
			break;
		}

		// ファイル名のコピー
		wcscpy_s(p_texture_data->p_file_name, pFilename);

		// テクスチャデータリストのメモリ確保
		p_texture_data_list = new HLTextureDataList;
		if (p_texture_data_list == NULL) {
			ret = false;
			break;
		}

		// テクスチャデータをモデルデータリストに格納
		p_texture_data_list->p_texture_data = p_texture_data;

		// 末尾に繋げる
		if (p_texture_data_list_top_ == NULL) {
			p_texture_data_list_top_ = p_texture_data_list;
		}
		else {
			p_list = p_texture_data_list_top_;
			while (p_list->p_next != NULL) {
				p_list = p_list->p_next;
			}
			p_list->p_next = p_texture_data_list;
		}

		// 仮引数に確保したテクスチャデータを保存
		*ppTextureData = p_texture_data;

	} while (0);

	// エラーが発生した場合の対処
	if (ret == false) {
		SAFE_RELEASE(p_texture_data->p_texture);
		SAFE_DELETE(p_texture_data);
		SAFE_DELETE(p_texture_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	テクスチャデータ解放
//	@param	LPHLTextureData	pTextureData	：解放するテクスチャデータ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLTexture::ReleaseTextureData(LPHLTextureData pTextureData)
{
	bool	ret = false;

	// テクスチャデータリストにテクスチャデータがあるかチェック
	LPHLTextureDataList	p_list = p_texture_data_list_top_,
						p_prev_list = NULL;
	while (p_list != NULL) {
		// 同名のテクスチャデータがあったら参照カウンタをデクリメントして、
		// 参照カウンタが０になったら削除する
		if (wcscmp(p_list->p_texture_data->p_file_name, pTextureData->p_file_name) == 0) {
			p_list->ref_counter--;
			if (p_list->ref_counter == 0) {
				if (p_prev_list) {
					p_prev_list->p_next = p_list->p_next;
				}
				else {
					p_texture_data_list_top_ = p_list->p_next;
				}
				SAFE_RELEASE(p_list->p_texture_data->p_texture);
				SAFE_DELETE(p_list->p_texture_data);
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

