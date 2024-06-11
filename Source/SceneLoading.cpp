#include "SceneLoading.h"
#include "Misc.h"
#include "Graphics/Graphics.h"
#include "SceneLoading.h"
#include "SceneManager.h"

void SceneLoading::Initialize()
{
    thread = new std::thread(LoadingThred, this);
}

void SceneLoading::Finalize()
{
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
    nextScene = nullptr;
}

void SceneLoading::Update(float elapsedTime)
{
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
    }
}

void SceneLoading::Render()
{
    Graphics& graphics          = Graphics::Instance();
    ID3D11DeviceContext* dc     = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
}

void SceneLoading::LoadingThred(SceneLoading* scene)
{
    HRESULT hr = S_OK;
    hr = CoInitialize(nullptr);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    scene->nextScene->Initialize();

    CoUninitialize();

    scene->nextScene->SetReady();
}
