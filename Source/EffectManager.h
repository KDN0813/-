#pragma once
#include <DirectXMath.h>

// エフェクシア実装で使う
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

// エフェクトマネージャー[10]
class EffectManager
{
private:
    EffectManager(){}
    ~EffectManager(){}

public:
    // 唯一のインスタンス取得[10]
    static EffectManager& Instance()
    {
        static EffectManager instance;
        return instance;
    }

    // 初期化[10]
    void Initialize();

    // 終了化[10]
    void Finalize();

    // 更新処理[10]
    void Update(float elapsedTime);

    // 描画処理[10]
    void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // Effeckseerマネージャーの取得[10]
    Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }

private:
    Effekseer::ManagerRef           effekseerManager;
    EffekseerRenderer::RendererRef  effekseerRender;
};