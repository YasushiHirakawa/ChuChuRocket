/****************************************************************************************************
//
//		HLTexture.h
//
//		@brief	KoudoLib �e�N�X�`���Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLTEXTURE_H__
#define __HLTEXTURE_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"
#include "HLDirect3D.h"


//---------------------------------------------------------------------------------------------
//	define
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
//	struct
//--------------------------------------------------------------------------------------------- 
// �e�N�X�`���f�[�^�\����
typedef struct HLTextureData
{
	ID3D11ShaderResourceView*	p_texture;				// �e�N�X�`���f�[�^
	wchar_t						p_file_name[kStrMax];	// ���f���t�@�C����

	HLTextureData()
	{
		p_texture = NULL;
		p_file_name[0] = '\0';
	}
} *LPHLTextureData;

// �e�N�X�`���f�[�^���X�g�\����
typedef struct HLTextureDataList
{
	HLTextureData*			p_texture_data;	// �e�N�X�`���f�[�^
	HLTextureDataList*		p_next;			// ���X�g�̎��f�[�^
	int						ref_counter;	// �Q�ƃJ�E���^

	HLTextureDataList()
	{
		p_texture_data	= NULL;
		p_next			= NULL;
		ref_counter		= 1;
	}
} *LPHLTextureDataList;


//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLTexture
{
public:
	HLTexture();
	~HLTexture();

	bool Init();			// ������
	void Destroy();			// �j��

	// �e�N�X�`���f�[�^�쐬
	bool CreateTextureData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLTextureData* ppTextureData);

	// �e�N�X�`���f�[�^���
	bool ReleaseTextureData(LPHLTextureData pTextureData);

private:
	LPHLTextureDataList		p_texture_data_list_top_;	// �e�N�X�`���f�[�^���X�g�̃g�b�v
};


#endif //__HLTEXTURE_H__
