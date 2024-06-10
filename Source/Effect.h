#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

// エフェクト[10]
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    // 再生[10]
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    // 停止[10]
    void Stop(Effekseer::Handle handle);

    // 座標設定[10]
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    // スケール設定[10]
    void SetScale(Effekseer::Handle, const DirectX::XMFLOAT3& scale);

private:
    // [10]
    Effekseer::EffectRef effekseerEffect;
};

