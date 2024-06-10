#include "Graphics/Graphics.h"
#include "EffectManager.h"

// 初期化[10]
void EffectManager::Initialize()
{
    // (メ)グラフィックスクラス[おそらく描画エンジンで作ってるやつ]
    // (メ)描画するためのデータ詰まってる
    Graphics& graphics = Graphics::Instance();

    // Effekserrレンダラ生成[10]
    effekseerRender = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    // Effekseerマネージャー生成[10]
    effekseerManager = Effekseer::Manager::Create(2048);    // 引数は最大インスタンス数

    // (メ)一旦pdf通りに書く

    // Effekseerレンダラの各所設定(特別なカスタマイズをしない場合は定型方に以下の設定でOK)
    effekseerManager->SetSpriteRenderer(effekseerRender->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRender->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRender->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRender->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRender->CreateModelRenderer());
    // エフェクト内でのローダーの設定(特別なカスタマイズをしない場合は定型方に以下の設定でOK)
    effekseerManager->SetTextureLoader(effekseerRender->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRender->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRender->CreateMaterialLoader());

    // Effekseeerを左て座標系で計算する
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// 終了化[10]
void EffectManager::Finalize()
{
    // EffekseerManagerなどはスマートポインタによって破棄されるので何もしない
}

// 更新処理[10]
void EffectManager::Update(float elapsedTime)
{
    // エフェクト更新処理(引数にはframeの経過時間を渡す)
    effekseerManager->Update(elapsedTime * 60.0f);
}

// 描画処理[10]
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{ 
    // ビュー&プロジェクション行列をEffekseerレンダラに設定
    // reinterpret_cast:ポインターを他のポインター型に変換する
    effekseerRender->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRender->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    // Effekseer描画開始
    effekseerRender->BeginRendering();

    // Effekseer描画実行
    // マネージャー単位で描画するので描画順番を制御する場合はマネージャーを複数個作成し、
    // Draw()関数を実行する順序で制御できそう
    effekseerManager->Draw();

    // Effekseer描画終了
    effekseerRender->EndRendering();
}
