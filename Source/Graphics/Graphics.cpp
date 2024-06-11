#include "System/Misc.h"
#include "Graphics/Graphics.h"


Graphics* Graphics::instance = nullptr;

Graphics::Graphics(HWND hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	// 画面のサイズを取得する。
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screen_width_uint = rc.right - rc.left;
	UINT screen_height_uint = rc.bottom - rc.top;

	this->screen_width = static_cast<float>(screen_width_uint);
	this->screen_height = static_cast<float>(screen_height_uint);

	HRESULT hr = S_OK;

	// デバイス＆スワップチェーンの生成
	{
		UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// スワップチェーンを作成するための設定オプション
		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		{
			swapchain_desc.BufferDesc.Width = screen_width_uint;
			swapchain_desc.BufferDesc.Height = screen_height_uint;
			swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = 1;
			swapchain_desc.OutputWindow = hWnd;
			swapchain_desc.Windowed = TRUE;
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchain_desc.Flags = 0;
		}

		D3D_FEATURE_LEVEL featureLevel;

		// デバイス＆スワップチェーンの生成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			create_device_flags,
			feature_levels,
			ARRAYSIZE(feature_levels),
			D3D11_SDK_VERSION,
			&swapchain_desc,
			this->swapchain.GetAddressOf(),
			this->device.GetAddressOf(),
			&featureLevel,
			this->immediate_context.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// レンダーターゲットビューの生成
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), nullptr, this->render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルビューの生成
	{
		D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc;
		depth_stencil_buffer_desc.Width = screen_width_uint;
		depth_stencil_buffer_desc.Height = screen_height_uint;
		depth_stencil_buffer_desc.MipLevels = 1;
		depth_stencil_buffer_desc.ArraySize = 1;
		depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_buffer_desc.SampleDesc.Count = 1;
		depth_stencil_buffer_desc.SampleDesc.Quality = 0;
		depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_buffer_desc.CPUAccessFlags = 0;
		depth_stencil_buffer_desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depth_stencil_buffer_desc, nullptr, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = device->CreateDepthStencilView(this->depth_stencil_buffer.Get(), nullptr, this->depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ビューポートの設定
	{
		// 画面のどの領域にDirectXで描いた画を表示するかの設定。
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screen_width_uint);
		viewport.Height = static_cast<float>(screen_height_uint);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		immediate_context->RSSetViewports(1, &viewport);
	}
}

Graphics::~Graphics()
{
}
