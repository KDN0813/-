#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	static Graphics& Instance() { return *instance; }

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	float GetScreenWidth() const { return screen_width; }
	float GetScreenHeight() const { return screen_height; }
	std::mutex& GetMutex() { return mutex; }

private:
	static Graphics*								instance;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;
	float	screen_width;
	float	screen_height;
	std::mutex										mutex;
};

