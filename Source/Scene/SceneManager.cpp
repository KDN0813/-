#include "SceneManager.h"

// 更新処理[10]
void SceneManager::Update(float elpsedTime)
{
    if (nextScene != nullptr)
    {
        Clear();

        // 新しいシーンを設定
        currentScene = nextScene;
        nextScene = nullptr;

        // シーン初期化処理[11]
        // =(メ)準備が完了していないなら初期化する
        if (!currentScene->IsReady())
        {
            currentScene->Initialize();
        }
    }

    if (currentScene != nullptr)
    {
        currentScene->Update(elpsedTime);
    }
}

// 描画処理[10]
void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

// シーンクリア[10]
void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

// シーン切り替え[10]
void SceneManager::ChangeScene(Scene* scene)
{
    // 新しいシーンを設定
    nextScene = scene;
}
