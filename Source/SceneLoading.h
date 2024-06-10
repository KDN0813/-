#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>   // [11]

// ローディングシーン[11]
class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading(){}

    // 初期化[11]
    void Initialize() override;

    // 終了化[11]
    void Finalize() override;

    // 更新処理[11]
    void Update(float elapsedTime) override;

    // 描画処理[11]
    void Render() override;

private:
    // ローディングスレッド[11]
    static void LoadingThred(SceneLoading* scene);

private:
    // [11]
    Sprite*         sprite      = nullptr;
    float           angle       = 0.0f;
    Scene*          nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};