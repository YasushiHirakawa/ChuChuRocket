/****************************************************************************************************
//
//		HLDirect3D.cpp
//
//		@brief	KoudoLib Direct3DDeviceの定義
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#include "HLDirect3D.h"

//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	コンストラクタ
---------------------------------------------------------------------------------------------*/
HLDirect3D::HLDirect3D()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	デストラクタ
---------------------------------------------------------------------------------------------*/
HLDirect3D::~HLDirect3D()
{
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@retval true	：成功
//	@retval false	：失敗
---------------------------------------------------------------------------------------------*/
bool HLDirect3D::Init(HWND hWnd)
{
	bool	ret = true;
	HRESULT hr	= S_OK;

	do {

		unsigned int create_device_flags = 0;	// デバイスフラグ（デバッグ状態かを仕込む）

#ifdef _DEBUG
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// ドライバータイプの列挙
		D3D_DRIVER_TYPE driver_types[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		unsigned int num_driver_types = ARRAYSIZE(driver_types);

		// DirectX対応バージョンの列挙
		D3D_FEATURE_LEVEL feature_levels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		unsigned int num_feature_levels = ARRAYSIZE(feature_levels);

		// スワップチェイン（描画領域）の詳細設定
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
		swap_chain_desc.BufferCount			= 1;
		swap_chain_desc.BufferDesc.Width	= kWindowWidth;
		swap_chain_desc.BufferDesc.Height	= kWindowHeight;
		swap_chain_desc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow		= hWnd;
		swap_chain_desc.SampleDesc.Count	= 1;
		swap_chain_desc.SampleDesc.Quality	= 0;
		swap_chain_desc.Windowed			= true;

		// デバイス、デバイスコンテキスト、スワップチェインの作成
		// ドライバータイプとDirectX対応バージョンが合致すれば作成される。
		D3D_DRIVER_TYPE		driver_type	 = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL	future_level = D3D_FEATURE_LEVEL_11_0;
		bool				device_create_success = false;
		for (unsigned int driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++) {
			driver_type = driver_types[driver_type_index];
			if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, driver_type, NULL, create_device_flags, 
				feature_levels, num_feature_levels, D3D11_SDK_VERSION, &swap_chain_desc, &p_swap_chain_,
				&p_d3d_device_, &future_level, &p_d3d_device_context_))) {
				device_create_success = true;
				break;
			}
		}
		if (device_create_success == false) {
			ret = false;
			break;
		}

		// レンダーターゲットビューの作成
		ID3D11Texture2D* p_back_buffer = NULL;
		if (FAILED(p_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_back_buffer))) {
			ret = false;
			break;
		}
		hr = p_d3d_device_->CreateRenderTargetView(p_back_buffer, NULL, &p_render_target_view_);
		p_back_buffer->Release();
		if (FAILED(hr)) {
			ret = false;
			break;
		}

		// 深度ステンシルテクスチャの作成
		D3D11_TEXTURE2D_DESC ds_texture_desc;
		ZeroMemory(&ds_texture_desc, sizeof(ds_texture_desc));
		ds_texture_desc.Width		= kWindowWidth;
		ds_texture_desc.Height		= kWindowHeight;
		ds_texture_desc.MipLevels	= 1;
		ds_texture_desc.ArraySize	= 1;
		ds_texture_desc.Format		= DXGI_FORMAT_D24_UNORM_S8_UINT;
		ds_texture_desc.SampleDesc.Count = 1;
		ds_texture_desc.SampleDesc.Quality = 0;
		ds_texture_desc.Usage		= D3D11_USAGE_DEFAULT;
		ds_texture_desc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
		ds_texture_desc.CPUAccessFlags = 0;
		ds_texture_desc.MiscFlags	= 0;
		if (FAILED(p_d3d_device_->CreateTexture2D(&ds_texture_desc, NULL, &p_depth_stencil_))) {
			ret = false;
			break;
		}

		// 深度ステンシルビューの作成
		D3D11_DEPTH_STENCIL_VIEW_DESC ds_view_desc;
		ZeroMemory(&ds_view_desc, sizeof(ds_view_desc));
		ds_view_desc.Format				= ds_texture_desc.Format;
		ds_view_desc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		ds_view_desc.Texture2D.MipSlice = 0;
		if (FAILED(p_d3d_device_->CreateDepthStencilView(p_depth_stencil_, &ds_view_desc, &p_depth_stencil_view_))) {
			ret = false;
			break;
		}

		// レンダーターゲットと深度ステンシルを設定
		p_d3d_device_context_->OMSetRenderTargets(1, &p_render_target_view_, p_depth_stencil_view_);

		// ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = (float)kWindowWidth;
		vp.Height = (float)kWindowHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_d3d_device_context_->RSSetViewports(1, &vp);

		// コモンステイタスの作成
		p_common_states_ = new CommonStates(p_d3d_device_);
		if (p_common_states_ == NULL) {
			ret = false;
			break;
		}

		// ベーシックエフェクトの作成
		p_basic_effect_ = new BasicEffect(HLDirect3D::GetDevice());
		if (p_basic_effect_ == NULL) {
			ret = false;
			break;
		}
		p_basic_effect_->SetVertexColorEnabled(true);

		// プリミティブバッチの作成
		p_primitive_batch_ = new PrimitiveBatch<VertexPositionColor>(p_d3d_device_context_);
		if (p_primitive_batch_ == NULL) {
			ret = false;
			break;
		}

		// 頂点入力レイアウトの設定
		void const* shader_byte_code;
		size_t byte_code_length;
		p_basic_effect_->GetVertexShaderBytecode(&shader_byte_code, &byte_code_length);
		if (FAILED(p_d3d_device_->CreateInputLayout(
				VertexPositionColor::InputElements,
				VertexPositionColor::InputElementCount,
				shader_byte_code,
				byte_code_length,
				&p_vertex_input_layout_))) {
			ret = false;
			break;
		}

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄
---------------------------------------------------------------------------------------------*/
void HLDirect3D::Destroy()
{
	SAFE_DELETE(p_common_states_);
	SAFE_RELEASE(p_vertex_input_layout_);
	SAFE_DELETE(p_primitive_batch_);
	SAFE_DELETE(p_basic_effect_);
	SAFE_RELEASE(p_depth_stencil_view_);
	SAFE_RELEASE(p_depth_stencil_);
	SAFE_RELEASE(p_render_target_view_);
	SAFE_RELEASE(p_swap_chain_);
	SAFE_RELEASE(p_d3d_device_context_);
	SAFE_RELEASE(p_d3d_device_);
}

/*---------------------------------------------------------------------------------------------
//	@brief	描画開始
---------------------------------------------------------------------------------------------*/
void HLDirect3D::BeginScene()
{
	p_d3d_device_context_->ClearRenderTargetView(p_render_target_view_, Colors::MidnightBlue);
	p_d3d_device_context_->ClearDepthStencilView(p_depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/*---------------------------------------------------------------------------------------------
//	@brief	描画終了
---------------------------------------------------------------------------------------------*/
void HLDirect3D::EndScene()
{
	p_swap_chain_->Present(0, 0);
}


/*---------------------------------------------------------------------------------------------
//	@brief	カメラのビュー行列の設定
//	@param	Vector3 Position	：カメラ座標
//	@param	Vector3 Lookat		：カメラ注視座標
//	@param	Vector3 Up			：カメラの上方向の向き
//	@retval	Matrix				：カメラのビュー行列
---------------------------------------------------------------------------------------------*/
Matrix HLDirect3D::SetCameraViewMatrix(Vector3 Position, Vector3 Lookat, Vector3 Up)
{
	Matrix	view_mat;
	Vector3	position = XMVectorSet(Position.x, Position.y, Position.z, 0.0F),
			lookat = XMVectorSet(Lookat.x, Lookat.y, Lookat.z, 0.0F),
			up = XMVectorSet(Up.x, Up.y, Up.z, 0.0F);
	view_mat = XMMatrixLookAtLH(position, lookat, up);
	//p_basic_effect_->SetView(view_mat);

	return view_mat;
}

/*---------------------------------------------------------------------------------------------
//	@brief	カメラの射影行列の設定
//	@param	unsigned int	Width	：画面サイズ横
//	@param	unsigned int	Height	：画面サイズ縦
//	@param	float			Near	：ニアクリップ距離
//	@param	float			Far		：ファークリップ距離
//	@retval	Matrix	：カメラの射影行列
---------------------------------------------------------------------------------------------*/
Matrix HLDirect3D::SetCameraProjectionMatrix(unsigned int Width, unsigned int Height, float Near, float Far)
{
	Matrix	projection_mat;
	projection_mat = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)Width / (float)Height, Near, Far);
	//p_basic_effect_->SetProjection(projection_mat);

	return projection_mat;
}


