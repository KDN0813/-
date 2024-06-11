#pragma once
#include "Scene.h"
#include <thread>

class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading(){}

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsedTime) override;
    void Render() override;

private:
    static void LoadingThred(SceneLoading* scene);

private:
    Scene*          nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};