#pragma once

#include "Scene.h"

// シーンマネージャー[10]
class SceneManager
{
private:
    SceneManager(){}
    ~SceneManager(){}
public:
    // 唯一のインスタンス取得[10](シングルトン)
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    // 更新処理[10]
    void Update(float elpsedTime);

    // 描画処理[10]
    void Render();

    // シーンクリア[10]
    void Clear();

    // シーン切り替え[10]
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene    = nullptr;
};

