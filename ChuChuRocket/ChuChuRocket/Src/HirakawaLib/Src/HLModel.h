/****************************************************************************************************
//
//		HLModel.h
//
//		@brief	KoudoLib ���f���`��Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLMODEL_H__
#define __HLMODEL_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"
#include "HLDirect3D.h"

//---------------------------------------------------------------------------------------------
//	const
//--------------------------------------------------------------------------------------------- 
const int	kMaxLights = IEffectLights::MaxDirectionalLights;	// ���C�g�̍ő吔

//---------------------------------------------------------------------------------------------
//	struct
//--------------------------------------------------------------------------------------------- 
// ���f���f�[�^�\����
typedef struct HLModelData
{
	std::unique_ptr<Model>	p_model;				// ���f���f�[�^
	wchar_t					p_file_name[kStrMax];	// ���f���t�@�C����

	HLModelData()
	{
		p_model			= NULL;
		p_file_name[0]	= '\0';
	}
} *LPHLModelData;

// ���f���f�[�^���X�g�\����
typedef struct HLModelDataList
{
	HLModelData*			p_model_data;	// ���f���f�[�^
	HLModelDataList*		p_next;			// ���X�g�̎��f�[�^
	int						ref_counter;	// �Q�ƃJ�E���^

	HLModelDataList()
	{
		p_model_data	= NULL;
		p_next			= NULL;
		ref_counter		= 1;
	}
} *LPHLModelDataList;

// ���C�g���
struct HLLightInfo
{
	bool		enable;				// �L���t���O
	Vector3		diffuse_color;		// �f�B�q���[�Y�J���[
	Vector3		ambient_color;		// �A���r�G���g�J���[
	Vector3		specular_color;		// �X�y�L�����[�J���[
	Vector3		direction;			// ����

	HLLightInfo()
	{
		enable			= false;
		diffuse_color	= Vector3(1.0F, 1.0F, 1.0F);
		ambient_color	= Vector3(0.25F, 0.25F, 0.25F);
		specular_color	= Vector3(0.0F, 0.0F, 0.0F);
		direction		= Vector3(0.0F, 0.0F, 1.0F);
	}
};

// �t�H�O���
struct HLFogInfo
{
	bool		enable;		// �L���t���O
	Vector3		color;		// �f�B�q���[�Y�J���[
	float		start;		// �J�n����
	float		end;		// �I������

	HLFogInfo()
	{
		enable	= false;
		color	= Vector3(1.0F, 1.0F, 1.0F);
		start	= 0;
		end		= 100;
	}
};

//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLModel
{
public:
	HLModel();
	~HLModel();

	bool Init(ID3D11Device* pDevice);	// ������
	void Destroy();						// �j��
	void Update();						// �X�V

	// ���f���f�[�^�쐬
	bool CreateModelData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLModelData* ppModelData);
	// ���f���f�[�^���
	bool ReleaseModelData(LPHLModelData pModelData);

	// ���C�g���̎擾�E�ݒ�
	HLLightInfo GetLightInfo() { return light_info_; };
	void SetLightInfo(HLLightInfo LightInfo) { light_info_ = LightInfo; update_flag_ = true; }

	// �t�H�O���̎擾�E�ݒ�
	inline HLFogInfo GetFogInfo() { return fog_info_; }
	inline void SetFogInfo(HLFogInfo FogInfo) { fog_info_ = FogInfo; update_flag_ = true; }

private:
	EffectFactory*		p_effect_factory;			// �G�t�F�N�g�t�@�N�g���[
	LPHLModelDataList	p_model_data_list_top_;		// ���f���f�[�^���X�g�̃g�b�v
	HLLightInfo			light_info_;				// ���C�g���
	HLFogInfo			fog_info_;					// �t�H�O���
	bool				update_flag_;				// �X�V�t���O
};


#endif //__HLMODEL_H__
