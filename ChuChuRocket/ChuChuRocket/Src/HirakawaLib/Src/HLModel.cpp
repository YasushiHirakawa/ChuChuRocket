/****************************************************************************************************
//
//		HLModel.cpp
//
//		@brief	KoudoLib モデル描画管理
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLModel.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLModel::HLModel()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLModel::~HLModel()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@param	ID3D11Device* pDevice	：デバイス
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLModel::Init(ID3D11Device* pDevice)
{
	bool ret = true;

	do {

		// エフェクトファクトリーの作成
		p_effect_factory = new EffectFactory(pDevice);
		if (p_effect_factory == NULL) {
			ret = false;
			break;
		}

		// モデルデータリストの先頭ポインタを初期化
		p_model_data_list_top_ = NULL;

	} while (0);

	p_model_data_list_top_ = NULL;

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HLModel::Destroy()
{
	// リストデータ全削除
	LPHLModelDataList	p_list = p_model_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"解放されていないモデルがあったので強制開放しました。\n");
		wcscat_s(warning_message, p_delete_list->p_model_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_DELETE(p_delete_list->p_model_data);
		SAFE_DELETE(p_delete_list);
	}

	// エフェクトファクトリー削除
	SAFE_DELETE(p_effect_factory);
}

/*---------------------------------------------------------------------------------------------
//	@brief	更新
---------------------------------------------------------------------------------------------*/
void HLModel::Update()
{
	// 更新フラグが立っていなければ更新しない
	if (update_flag_ == false) {
		return;
	}

	// ライト情報とフォグ情報の更新
	LPHLModelDataList model_list = p_model_data_list_top_;
	while (model_list != NULL) {
		model_list->p_model_data->p_model->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* p_light = dynamic_cast<IEffectLights*>(effect);
			if (p_light) {
				p_light->SetLightEnabled(0, light_info_.enable);
				p_light->SetLightDiffuseColor(0, light_info_.diffuse_color);
				p_light->SetAmbientLightColor(light_info_.ambient_color);
				p_light->SetLightSpecularColor(0, light_info_.specular_color);
				p_light->SetLightDirection(0, light_info_.direction);
			}

			IEffectFog* p_fog = dynamic_cast<IEffectFog*>(effect);
			if (p_fog) {
				p_fog->SetFogEnabled(fog_info_.enable);
				p_fog->SetFogStart(fog_info_.start);
				p_fog->SetFogEnd(fog_info_.end);
				p_fog->SetFogColor(fog_info_.color);
			}
		});
		model_list = model_list->p_next;
	}
	update_flag_ = false;
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ作成
//	@param	ID3D11Device*	pDevice		：デバイス
//	@param	const wchar_t*	pFilename	：ファイル名
//	@param	LPHLModelData*	ppModelData	：モデルデータ格納先ポインタ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLModel::CreateModelData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLModelData* ppModelData)
{
	bool				ret = true;
	LPHLModelDataList	p_list = NULL;
	
	LPHLModelData		p_model_data = NULL;
	LPHLModelDataList	p_model_data_list = NULL;

	do {

		// モデルデータリストにあるかチェック
		bool	found_flag = false;
		p_list = p_model_data_list_top_;
		while (p_list != NULL) {
			// ファイル名をチェックして見つけたらモデルデータを格納してフラグを立てて抜ける
			if (wcscmp(p_list->p_model_data->p_file_name, pFilename) == 0) {
				*ppModelData = p_list->p_model_data;
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

		// モデルデータのメモリ確保
		p_model_data = new HLModelData;
		if (p_model_data == NULL) {
			ret = false;
			break;
		}

		// モデルデータ作成
		if (wcsstr(pFilename, L".sdkmesh")) {
			p_model_data->p_model = Model::CreateFromSDKMESH(pDevice, pFilename, *p_effect_factory, true);
		}
		else if (wcsstr(pFilename, L".cmo")) {
			p_model_data->p_model = Model::CreateFromCMO(pDevice, pFilename, *p_effect_factory, true);
		}
		else {
			wchar_t	warning_message[kStrMax];
			wcscpy_s(warning_message, L"読み込めないファイル形式です。\n");
			wcscat_s(warning_message, pFilename);
			MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
		}
		if (p_model_data->p_model == NULL) {
			ret = false;
			break;
		}

		// ファイル名のコピー
		wcscpy_s(p_model_data->p_file_name, pFilename);

		// モデルデータリストのメモリ確保
		p_model_data_list = new HLModelDataList;
		if (p_model_data_list == NULL) {
			ret = false;
			break;
		}

		// モデルデータをモデルデータリストに格納
		p_model_data_list->p_model_data = p_model_data;

		// 末尾に繋げる
		if (p_model_data_list_top_ == NULL) {
			p_model_data_list_top_ = p_model_data_list;
		}
		else {
			p_list = p_model_data_list_top_;
			while (p_list->p_next != NULL) {
				p_list = p_list->p_next;
			}
			p_list->p_next = p_model_data_list;
		}

		// ライトをデフォルトで切る
		p_model_data->p_model->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* p_light = dynamic_cast<IEffectLights*>(effect);
			if (p_light) {
				for (int i = 0; i < kMaxLights; i++) {
					p_light->SetLightEnabled(i, false);
				}
				p_light->SetLightDiffuseColor(0, light_info_.diffuse_color);
				p_light->SetAmbientLightColor(light_info_.ambient_color);
				p_light->SetLightSpecularColor(0, light_info_.specular_color);
				p_light->SetLightDirection(0, light_info_.direction);
			}

			IEffectFog* p_fog = dynamic_cast<IEffectFog*>(effect);
			if (p_fog) {
				p_fog->SetFogEnabled(false);
				p_fog->SetFogStart(fog_info_.start);
				p_fog->SetFogEnd(fog_info_.end);
				p_fog->SetFogColor(fog_info_.color);
			}
		});


		// 仮引数にモデルデータを格納する
		*ppModelData = p_model_data;

	} while (0);

	// エラーが発生した場合の対処
	if (ret == false) {
		SAFE_DELETE(p_model_data);
		SAFE_DELETE(p_model_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	モデルデータ解放
//	@param	LPHLModelData	pModelData	：解放するモデルデータ
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLModel::ReleaseModelData(LPHLModelData pModelData)
{
	bool	ret = false;

	// モデルデータリストにモデルデータがあるかチェック
	LPHLModelDataList	p_list = p_model_data_list_top_, 
						p_prev_list = NULL;
	while (p_list != NULL) {
		// 同名のモデルデータがあったら参照カウンタをデクリメントして、
		// 参照カウンタが０になったら削除する
		if (wcscmp(p_list->p_model_data->p_file_name, pModelData->p_file_name) == 0) {
			p_list->ref_counter--;
			if (p_list->ref_counter == 0) {
				if (p_prev_list) {
					p_prev_list->p_next = p_list->p_next;
				}
				else {
					p_model_data_list_top_ = p_list->p_next;
				}
				SAFE_DELETE(p_list->p_model_data);
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


