/****************************************************************************************************
//
//		HLSprite.h
//
//		@brief	KoudoLib �X�v���C�g�`��Ǘ�
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLSprite.h"


//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	�R���X�g���N�^
---------------------------------------------------------------------------------------------*/
HLSprite::HLSprite()
	: p_sprite_batch_(NULL)
	, p_sprite_font_(NULL)
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	�f�X�g���N�^
---------------------------------------------------------------------------------------------*/
HLSprite::~HLSprite()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	������
//	@param	ID3D11Device*			pDevice			�F�f�o�C�X
//	@param	ID3D11DeviceContext*	pDeviceContext	�F�f�o�C�X�R���e�L�X�g
//	@retval true	�F����
//	@retval false	�F���s
---------------------------------------------------------------------------------------------*/
bool HLSprite::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	bool ret = true;

	do {

		// �X�v���C�g�o�b�`�̏�����
		p_sprite_batch_ = new SpriteBatch(pDeviceContext);
		if (p_sprite_batch_ == NULL) {
			ret = false;
			break;
		}

		// �X�v���C�g�t�H���g�̏�����
		p_sprite_font_ = new SpriteFont(pDevice, L"Resource/Font/italic.spritefont");
		if (p_sprite_font_ == NULL) {
			ret = false;
			break;
		}

	} while (0);


	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	�j��
---------------------------------------------------------------------------------------------*/
void HLSprite::Destroy()
{
	SAFE_DELETE(p_sprite_batch_);
	SAFE_DELETE(p_sprite_font_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	�`��J�n
//	@param	CommonStates* pStates	�F�R�����X�e�[�^�X
---------------------------------------------------------------------------------------------*/
void HLSprite::BeginSprite(CommonStates* pStates)
{
	p_sprite_batch_->Begin(SpriteSortMode_Deferred, pStates->NonPremultiplied());
}

/*---------------------------------------------------------------------------------------------
//	@brief	�`��I��
---------------------------------------------------------------------------------------------*/
void HLSprite::EndSprite()
{
	p_sprite_batch_->End();
}

/*---------------------------------------------------------------------------------------------
//	@brief	�X�v���C�g�`��
//	@param	LPHLTextureData pTextureData	�F�`�悷��e�N�X�`���f�[�^
//	@param	Vector2			Position		�F�`�悷����W
//	@param	RECT			Rect			�F�e�N�X�`���؂���͈�
//	@param	Vector4			Color			�F�e�N�X�`���J���[
//	@param	Vector2			Origin			�F�e�N�X�`���̒��S���W
//	@param	float			Rotation		�F��]
//	@param	float			Scale			�F�g��
---------------------------------------------------------------------------------------------*/
void HLSprite::DrawSprite(LPHLTextureData pTextureData, Vector2 Position, RECT Rect, Vector4 Color,
							Vector2 Origin, float Rotation, float Scale)
{
	p_sprite_batch_->Draw(pTextureData->p_texture, Position, &Rect, Color, Rotation, Origin, Scale);
}

/*---------------------------------------------------------------------------------------------
//	@brief	������`��
---------------------------------------------------------------------------------------------*/
void HLSprite::DrawFont(wchar_t* pString, Vector2 Position, XMVECTOR Color)
{
	p_sprite_font_->DrawString(p_sprite_batch_, pString, Position, Color);
}


