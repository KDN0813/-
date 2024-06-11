#pragma once

#include "Scene.h"

class SceneTitle : public Scene
{
public:
    SceneTitle(){}
    ~SceneTitle() override{};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;
};