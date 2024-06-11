#pragma once

#include "Scene.h"
#include <memory>
#include <thread>

class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* next_scene) : next_scene(next_scene) {}
    ~SceneLoading(){}

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

private:
    static void LoadingThred(SceneLoading* scene);

private:
    Scene* next_scene   = nullptr;
    std::unique_ptr<std::thread> thread;
};