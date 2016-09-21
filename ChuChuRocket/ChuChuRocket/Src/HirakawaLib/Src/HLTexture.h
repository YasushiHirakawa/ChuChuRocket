/****************************************************************************************************
//
//		HLTexture.h
//
//		@brief	KoudoLib テクスチャ管理
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
// テクスチャデータ構造体
typedef struct HLTextureData
{
	ID3D11ShaderResourceView*	p_texture;				// テクスチャデータ
	wchar_t						p_file_name[kStrMax];	// モデルファイル名

	HLTextureData()
	{
		p_texture = NULL;
		p_file_name[0] = '\0';
	}
} *LPHLTextureData;

// テクスチャデータリスト構造体
typedef struct HLTextureDataList
{
	HLTextureData*			p_texture_data;	// テクスチャデータ
	HLTextureDataList*		p_next;			// リストの次データ
	int						ref_counter;	// 参照カウンタ

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

	bool Init();			// 初期化
	void Destroy();			// 破棄

	// テクスチャデータ作成
	bool CreateTextureData(ID3D11Device* pDevice, const wchar_t* pFilename, LPHLTextureData* ppTextureData);

	// テクスチャデータ解放
	bool ReleaseTextureData(LPHLTextureData pTextureData);

private:
	LPHLTextureDataList		p_texture_data_list_top_;	// テクスチャデータリストのトップ
};


#endif //__HLTEXTURE_H__
