/****************************************************************************************************
//
//		HLTexture.h
//
//		@brief	KoudoLib �e�N�X�`���Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLTexture.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
HLTexture::HLTexture()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
HLTexture::~HLTexture()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	������
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLTexture::Init()
{
	bool ret = true;

	do {

		// �e�N�X�`���f�[�^���X�g�̐擪��������
		p_texture_data_list_top_ = NULL;

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void HLTexture::Destroy()
{
	// ���X�g�f�[�^�S�폜
	LPHLTextureDataList	p_list = p_texture_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"�������Ă��Ȃ��e�N�X�`�����������̂ŋ���������܂����B\n");
		wcscat_s(warning_message, p_delete_list->p_texture_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_RELEASE(p_delete_list->p_texture_data->p_texture);
		SAFE_DELETE(p_delete_list->p_texture_data);
		SAFE_DELETE(p_delete_list);
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	���f���f�[�^�쐬
//	@param	ID3D11Device*		pDevice			�F�f�o�C�X
//	@param	const wchar_t*		pFilename		�F�t�@�C����
//	@param	LPHLTextureData*	ppTextureData	�F�e�N�X�`���f�[�^�i�[��|�C���^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLTexture::CreateTextureData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLTextureData* ppTextureData)
{
	bool				ret = true;
	LPHLTextureDataList	p_list = NULL;

	LPHLTextureData		p_texture_data = NULL;
	LPHLTextureDataList	p_texture_data_list = NULL;

	do {

		// �e�N�X�`���f�[�^���X�g�ɂ��邩�`�F�b�N
		bool	found_flag = false;
		p_list = p_texture_data_list_top_;
		while (p_list != NULL) {
			// �t�@�C�������`�F�b�N���Č�������e�N�X�`���f�[�^���i�[���ăt���O�𗧂ĂĔ�����
			if (wcscmp(p_list->p_texture_data->p_file_name, pFilename) == 0) {
				*ppTextureData = p_list->p_texture_data;
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

		// �e�N�X�`���f�[�^�̃������m��
		p_texture_data = new HLTextureData;
		if (p_texture_data == NULL) {
			ret = false;
			break;
		}

		// �e�N�X�`���f�[�^�쐬
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

		// �t�@�C�����̃R�s�[
		wcscpy_s(p_texture_data->p_file_name, pFilename);

		// �e�N�X�`���f�[�^���X�g�̃������m��
		p_texture_data_list = new HLTextureDataList;
		if (p_texture_data_list == NULL) {
			ret = false;
			break;
		}

		// �e�N�X�`���f�[�^�����f���f�[�^���X�g�Ɋi�[
		p_texture_data_list->p_texture_data = p_texture_data;

		// �����Ɍq����
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

		// �������Ɋm�ۂ����e�N�X�`���f�[�^��ۑ�
		*ppTextureData = p_texture_data;

	} while (0);

	// �G���[�����������ꍇ�̑Ώ�
	if (ret == false) {
		SAFE_RELEASE(p_texture_data->p_texture);
		SAFE_DELETE(p_texture_data);
		SAFE_DELETE(p_texture_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�e�N�X�`���f�[�^���
//	@param	LPHLTextureData	pTextureData	�F�������e�N�X�`���f�[�^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLTexture::ReleaseTextureData(LPHLTextureData pTextureData)
{
	bool	ret = false;

	// �e�N�X�`���f�[�^���X�g�Ƀe�N�X�`���f�[�^�����邩�`�F�b�N
	LPHLTextureDataList	p_list = p_texture_data_list_top_,
						p_prev_list = NULL;
	while (p_list != NULL) {
		// �����̃e�N�X�`���f�[�^����������Q�ƃJ�E���^���f�N�������g���āA
		// �Q�ƃJ�E���^���O�ɂȂ�����폜����
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

