/****************************************************************************************************
//
//		HLSprite.h
//
//		@brief	KoudoLib �X�v���C�g�`��Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLSPRITE_H__
#define __HLSPRITE_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"
#include "HLDirect3D.h"
#include "HLTexture.h"


//---------------------------------------------------------------------------------------------
//	define
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLSprite
{
public:
	HLSprite();
	~HLSprite();

	bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);	// ������
	void Destroy();								// �j��

	void BeginSprite(CommonStates* pStates);	// �`��J�n
	void EndSprite();							// �`��I��

	// �X�v���C�g�`��
	void DrawSprite(LPHLTextureData pTextureData, Vector2 Position, RECT Rect, 
					Vector4 Color = Vector4(1.0F, 1.0F, 1.0F, 1.0F), 
					Vector2 Origin = Vector2(0.0F, 0.0F),
					float Rotation = 0.0F, float Scale = 1.0F);

	// ������`��
	void DrawFont(wchar_t* pString, Vector2 Position, XMVECTOR Color = Colors::White);

private:
	SpriteBatch*	p_sprite_batch_;		// �X�v���C�g�o�b�`
	SpriteFont*		p_sprite_font_;			// �X�v���C�g�t�H���g
};


#endif //__HLSPRITE_H__
