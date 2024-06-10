#include "Graphics/Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

// コンストラクタ[10]
Effect::Effect(const char* filename)
{
    // エフェクトを読み込みする前にロックする
    // ※マルチスレッドでEffectを作成するとDeviceContexを同時アクセスして
    // フリーズする可能性があるんで排他的制御する
    std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());
    // デストラクタでロックが解放される

    // Effekseerのリソースを読み込む
    // EffeksserはUTF-16のファイルパス以外は対応していないため文字コード変換が必要
    char16_t utf16Filename[256];    // (メ)変換後のfilename
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    // Effekseer::Managerを取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // Effekseerエフェクトを読み込み
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}

// 再生[10]
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // エフェクト再生
    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y,
        position.z);

    // スケール設定
    effekseerManager->SetScale(handle, scale, scale, scale);

    // ハンドルを返す
    return handle;
}

// 停止[10]
void Effect::Stop(Effekseer::Handle handle)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // エフェクト停止
    effekseerManager->StopEffect(handle);
}

// 座標設定
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // 座標設定
    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

// スケール変更
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    // EffekseerManager取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // スケール設定
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}