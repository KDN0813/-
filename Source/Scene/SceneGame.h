#pragma once
#include "Scene.h"

class SceneGame : public Scene
{
public:
    SceneGame() noexcept = default;
    ~SceneGame() noexcept = default;

public:
    void Initialize() override;
    void Finalize() override;
    void Update(const float elapsed_time) override;
    void Render(const ID3D11DeviceContext* dc) override;
};

