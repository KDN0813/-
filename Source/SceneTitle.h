#pragma once

#include "Scene.h"

// タイトルシーン[10]
class SceneTitle : public Scene
{
public:
    SceneTitle(){}
    ~SceneTitle() override{};

    // 初期化[10]
    void Initialize() override;

    // 終了化[10]
    void Finalize() override;

    // 更新処理[10]
    void Update(float elapsedTime) override;

    // 描画処理[10]
    void Render() override;

private:
};

