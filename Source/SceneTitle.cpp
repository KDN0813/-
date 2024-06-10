#include "Graphics/Graphics.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneGame.h"

// 初期化[11]
void SceneTitle::Initialize()
{
}

// 終了化[11]
void SceneTitle::Finalize()
{
}

// 更新処理[11]
void SceneTitle::Update(float elapsedTime)
{
}

// 描画処理[11]
void SceneTitle::Render()
{
    Graphics&                   graphics    = Graphics::Instance();
    ID3D11DeviceContext*        dc          = graphics.GetDeviceContext();
    ID3D11RenderTargetView*     rtv         = graphics.GetRenderTargetView();
    ID3D11DepthStencilView*     dsv         = graphics.GetDepthStencilView();

    // 画面クリア&レンダーターゲット設定[10]
    FLOAT color[] = { 0.0f,0.0f ,0.5f ,1.0f };      // RGBA(0.0f〜1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2Dスプライト描画
    {
    }
}
