#pragma once
#include <d3d11.h>

class Scene
{
public:
    Scene() noexcept = default;
    virtual ~Scene() noexcept = default;

    // コピー・ムーブコンストラクタ(代入演算子)の禁止
    Scene(const Scene& s) noexcept = delete;
    Scene& operator=(Scene&) noexcept = default;
    Scene(const Scene&& s) noexcept = delete;
    Scene& operator=(Scene&&) noexcept = delete;

public:
    /// <summary>
    /// 初期化処理
    /// </summary>
    virtual void Initialize() = 0;
    /// <summary>
    /// 終了処理
    /// </summary>
    virtual void Finalize() = 0;
    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="elapsed_time">経過時間</param>
    virtual void Update(const float elapsed_time) = 0;
    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="dc">デバイスコンテキスト</param>
    virtual void Render(const ID3D11DeviceContext* dc) = 0;
};