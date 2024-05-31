#pragma once
#include "Scene.h"

class SceneManager
{
public:
    SceneManager() noexcept = default;
    virtual ~SceneManager() noexcept = default;

    // コピー・ムーブコンストラクタ(代入演算子)の禁止
    SceneManager(const SceneManager& s) noexcept = delete;
    SceneManager& operator=(SceneManager&) noexcept = default;
    SceneManager(const SceneManager&& s) noexcept = delete;
    SceneManager& operator=(SceneManager&&) noexcept = delete;

public:
    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();
    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();
    /// <summary>
    ///  更新処理
    /// </summary>
    /// <param name="elapsed_time">経過時間</param>
    void Update(const float elapsed_time);
    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="dc">デバイスコンテキスト</param>
    void Render(const ID3D11DeviceContext* dc);
    /// <summary>
    /// シーンのクリア処理
    /// </summary>
    void Clear();
    /// <summary>
    /// シーンの切り替え
    /// </summary>
    /// <param name="scene">次のシーンのポインタ</param>
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
};

