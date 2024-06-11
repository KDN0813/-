#pragma once

#include "Scene.h"

class SceneManager
{
private:
    SceneManager(){}
    ~SceneManager(){}
public:
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    void Update(float elpsedTime);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene    = nullptr;
};

