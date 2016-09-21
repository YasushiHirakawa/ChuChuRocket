/****************************************************************************************************
//
//		HLAudio.h
//
//		@brief	HirakawaLib �T�E���h�Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLAudio.h"

//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
HLAudio::HLAudio()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
HLAudio::~HLAudio()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	������
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLAudio::Init()
{
	bool	ret = true;

	do {

		// �I�[�f�B�I�G���W���̃������m��
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

		// �I�[�f�B�I�f�[�^���X�g�̐擪��������
		p_audio_data_list_top_ = NULL;

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void HLAudio::Destroy()
{
	// ���X�g�f�[�^�S�폜
	LPHLAudioDataList	p_list = p_audio_data_list_top_,
						p_delete_list = NULL;
	while (p_list != NULL) {
		p_delete_list = p_list;
		p_list = p_list->p_next;

#if _DEBUG
		wchar_t	warning_message[kStrMax];
		wcscpy_s(warning_message, L"�������Ă��Ȃ��I�[�f�B�I�f�[�^���������̂ŋ���������܂����B\n");
		wcscat_s(warning_message, p_delete_list->p_audio_data->p_file_name);
		MessageBox(HLCommon::GetWindowHandle(), warning_message, L"Warning!", MB_OK);
#endif

		SAFE_DELETE(p_delete_list->p_audio_data);
		SAFE_DELETE(p_delete_list->p_audio_origin);
		SAFE_DELETE(p_delete_list);
	}

	// �I�[�f�B�I�G���W���̍폜
	SAFE_DELETE(p_audio_engine_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�V
---------------------------------------------------------------------------------------------*/
void HLAudio::Update()
{
	if (!p_audio_engine_->Update()) {
		if (p_audio_engine_->IsCriticalError()) {

			// �����ŃI�[�f�B�I�G���W�������X�g���邱�Ƃ�����炵���B
			// �v�Ή�����

		}
	}
}

/*---------------------------------------------------------------------------------------------
//	@brief	�I�[�f�B�I�f�[�^�쐬
//	@param	const wchar_t*		pFilename		�F�t�@�C����
//	@param	LPHLTextureData*	ppTextureData	�F�e�N�X�`���f�[�^�i�[��|�C���^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLAudio::CreateAudioDataFromWave(const wchar_t* pFilename, LPHLAudioData* ppAudioData)
{
	bool				ret = true;
	LPHLAudioDataList	p_list = NULL;

	LPHLAudioData		p_audio_data = NULL;
	LPHLAudioDataList	p_audio_data_list = NULL;

	do {

		// �I�[�f�B�I�f�[�^���X�g�ɂ��邩�`�F�b�N
		bool	found_flag = false;
		p_list = p_audio_data_list_top_;
		while (p_list != NULL) {
			// �t�@�C�������`�F�b�N���Č������I�[�f�B�I�f�[�^���i�[���ăt���O�𗧂ĂĔ�����
			if (wcscmp(p_list->p_audio_data->p_file_name, pFilename) == 0) {
				*ppAudioData = p_list->p_audio_data;
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

		// �I�[�f�B�I�f�[�^���X�g�̃������m��
		p_audio_data_list = new HLAudioDataList;
		if (p_audio_data_list == NULL) {
			ret = false;
			break;
		}

		// �I�[�f�B�I�f�[�^�쐬
		if (wcsstr(pFilename, L".wav")) {
			p_audio_data_list->p_audio_origin = new SoundEffect(p_audio_engine_, pFilename);
		}
		if (p_audio_data_list->p_audio_origin == NULL) {
			ret = false;
			break;
		}

		// �I�[�f�B�I�f�[�^�̃������m��
		p_audio_data = new HLAudioData;
		if (p_audio_data == NULL) {
			ret = false;
			break;
		}

		// �I�[�f�B�I�C���X�^���X�f�[�^���󂯎��
		p_audio_data->p_audio_instance = p_audio_data_list->p_audio_origin->CreateInstance();
		if (p_audio_data->p_audio_instance == NULL) {
			ret = false;
			break;
		}

		// �t�@�C�����̃R�s�[
		wcscpy_s(p_audio_data->p_file_name, pFilename);

		// �e�N�X�`���f�[�^�����f���f�[�^���X�g�Ɋi�[
		p_audio_data_list->p_audio_data = p_audio_data;

		// �����Ɍq����
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

		// �������Ɋm�ۂ����e�N�X�`���f�[�^��ۑ�
		*ppAudioData = p_audio_data;

	} while (0);

	// �G���[�����������ꍇ�̑Ώ�
	if (ret == false) {
		SAFE_DELETE(p_audio_data);
		SAFE_DELETE(p_audio_data_list->p_audio_origin);
		SAFE_DELETE(p_audio_data_list);
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�I�[�f�B�I�f�[�^���
//	@param	LPHLAudioData pAudioData	�F�������I�[�f�B�I�f�[�^
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLAudio::ReleaseAudioData(LPHLAudioData pAudioData)
{
	bool	ret = false;

	// �e�N�X�`���f�[�^���X�g�Ƀe�N�X�`���f�[�^�����邩�`�F�b�N
	LPHLAudioDataList	p_list = p_audio_data_list_top_,
						p_prev_list = NULL;
	while (p_list != NULL) {
		// �����̃I�[�f�B�I�f�[�^����������Q�ƃJ�E���^���f�N�������g���āA
		// �Q�ƃJ�E���^���O�ɂȂ�����폜����
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



