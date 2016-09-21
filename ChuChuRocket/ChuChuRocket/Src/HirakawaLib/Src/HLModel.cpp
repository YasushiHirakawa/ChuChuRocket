/****************************************************************************************************
//
//		HLModel.cpp
//
//		@brief	KoudoLib ���f���`��Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLModel.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
HLModel::HLModel()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
HLModel::~HLModel()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	������
//	@param	ID3D11Device* pDevice	�F�f�o�C�X
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLModel::Init(ID3D11Device* pDevice)
{
	bool ret = true;

	do {

		// �G�t�F�N�g�t�@�N�g���[�̍쐬
		p_effect_factory = new EffectFactory(pDevice);
		if (p_effect_factory == NULL) {
			ret = false;
			break;
		}

		// ���f���f�[�^���X�g�̐擪�|�C���^��������
		p_model_data_list_top_ = NULL;

	} while (0);

	p_model_data_list_top_ = NULL;

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void HLModel::Destroy()
{
	// ���X�g�f�[�^�S�폜
	LPHLModelDataList	p_list = p_model_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"�������Ă��Ȃ����f�����������̂ŋ����J�����܂����B\n");
		wcscat_s(warning_message, p_delete_list->p_model_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_DELETE(p_delete_list->p_model_data);
		SAFE_DELETE(p_delete_list);
	}

	// �G�t�F�N�g�t�@�N�g���[�폜
	SAFE_DELETE(p_effect_factory);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�V
---------------------------------------------------------------------------------------------*/
void HLModel::Update()
{
	// �X�V�t���O�������Ă��Ȃ���΍X�V���Ȃ�
	if (update_flag_ == false) {
		return;
	}

	// ���C�g���ƃt�H�O���̍X�V
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
//	@brief	���f���f�[�^�쐬
//	@param	ID3D11Device*	pDevice		�F�f�o�C�X
//	@param	const wchar_t*	pFilename	�F�t�@�C����
//	@param	LPHLModelData*	ppModelData	�F���f���f�[�^�i�[��|�C���^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLModel::CreateModelData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLModelData* ppModelData)
{
	bool				ret = true;
	LPHLModelDataList	p_list = NULL;
	
	LPHLModelData		p_model_data = NULL;
	LPHLModelDataList	p_model_data_list = NULL;

	do {

		// ���f���f�[�^���X�g�ɂ��邩�`�F�b�N
		bool	found_flag = false;
		p_list = p_model_data_list_top_;
		while (p_list != NULL) {
			// �t�@�C�������`�F�b�N���Č������烂�f���f�[�^���i�[���ăt���O�𗧂ĂĔ�����
			if (wcscmp(p_list->p_model_data->p_file_name, pFilename) == 0) {
				*ppModelData = p_list->p_model_data;
				p_list->ref_counter++;
				found_flag = true;
				break;
			}
			p_list = p_list->p_next;
		}

		// ���łɃf�[�^�����������甲����
		if (found_flag) {
			break;
		}

		// ���f���f�[�^�̃������m��
		p_model_data = new HLModelData;
		if (p_model_data == NULL) {
			ret = false;
			break;
		}

		// ���f���f�[�^�쐬
		if (wcsstr(pFilename, L".sdkmesh")) {
			p_model_data->p_model = Model::CreateFromSDKMESH(pDevice, pFilename, *p_effect_factory, true);
		}
		else if (wcsstr(pFilename, L".cmo")) {
			p_model_data->p_model = Model::CreateFromCMO(pDevice, pFilename, *p_effect_factory, true);
		}
		else {
			wchar_t	warning_message[kStrMax];
			wcscpy_s(warning_message, L"�ǂݍ��߂Ȃ��t�@�C���`���ł��B\n");
			wcscat_s(warning_message, pFilename);
			MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
		}
		if (p_model_data->p_model == NULL) {
			ret = false;
			break;
		}

		// �t�@�C�����̃R�s�[
		wcscpy_s(p_model_data->p_file_name, pFilename);

		// ���f���f�[�^���X�g�̃������m��
		p_model_data_list = new HLModelDataList;
		if (p_model_data_list == NULL) {
			ret = false;
			break;
		}

		// ���f���f�[�^�����f���f�[�^���X�g�Ɋi�[
		p_model_data_list->p_model_data = p_model_data;

		// �����Ɍq����
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

		// ���C�g���f�t�H���g�Ő؂�
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


		// �������Ƀ��f���f�[�^���i�[����
		*ppModelData = p_model_data;

	} while (0);

	// �G���[�����������ꍇ�̑Ώ�
	if (ret == false) {
		SAFE_DELETE(p_model_data);
		SAFE_DELETE(p_model_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^���
//	@param	LPHLModelData	pModelData	�F������郂�f���f�[�^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLModel::ReleaseModelData(LPHLModelData pModelData)
{
	bool	ret = false;

	// ���f���f�[�^���X�g�Ƀ��f���f�[�^�����邩�`�F�b�N
	LPHLModelDataList	p_list = p_model_data_list_top_, 
						p_prev_list = NULL;
	while (p_list != NULL) {
		// �����̃��f���f�[�^����������Q�ƃJ�E���^���f�N�������g���āA
		// �Q�ƃJ�E���^���O�ɂȂ�����폜����
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


