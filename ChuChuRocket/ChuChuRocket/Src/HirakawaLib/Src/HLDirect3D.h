/****************************************************************************************************
//
//		HLDirect3D.h
//
//		@brief	HirakawaLib Direct3DDeviceの定義
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLDIRECT3D_H__
#define __HLDIRECT3D_H__


//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
#include "HLCommon.h"


//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLDirect3D
{
public:
	HLDirect3D();
	~HLDirect3D();

	bool Init(HWND hWnd);		// 初期化
	void Destroy();				// 破棄
	void BeginScene();			// 描画開始
	void EndScene();			// 描画終了

	// デバイスの取得
	inline ID3D11Device* GetDevice() { return p_d3d_device_; }

	// デバイスコンテキストの取得
	inline ID3D11DeviceContext* GetDeviceContext() { return p_d3d_device_context_; }

	// コモンステイタスの取得
	inline CommonStates* GetCommonStates() { return p_common_states_; }

	// カメラのビュー行列の設定
	Matrix SetCameraViewMatrix(Vector3 Position, Vector3 Lookat, Vector3 Up = Vector3(0.0F, 1.0F, 0.0F));

	// カメラの射影行列の設定
	Matrix SetCameraProjectionMatrix(unsigned int Width, unsigned int Height, float Near = 1.0F, float Far = 1000.0F);

private:
	ID3D11Device*				p_d3d_device_;				// Direct3D Device
	ID3D11DeviceContext*		p_d3d_device_context_;		// Device Context
	IDXGISwapChain*				p_swap_chain_;				// SwapChain

	ID3D11RenderTargetView*		p_render_target_view_;		// Render Target View
	ID3D11Texture2D*			p_depth_stencil_;			// 深度ステンシル
	ID3D11DepthStencilView*		p_depth_stencil_view_;		// 深度ステンシルビュー

	BasicEffect*				p_basic_effect_;			// Basic Effect
	PrimitiveBatch<VertexPositionColor>*	p_primitive_batch_;	// Primitive Batch
	ID3D11InputLayout*			p_vertex_input_layout_;		// 頂点入力レイアウト

	CommonStates*				p_common_states_;			// common states
};


#endif //__HLDIRECT3D_H__