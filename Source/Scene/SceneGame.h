#pragma once

#include "Scene.h"

class SceneGame : public Scene 
{
public:
	SceneGame() {}
	~SceneGame() override {}

	void Initialize() override;
	void Finalize() override;
	void Update(float elapsedTime) override;
	void Render() override;
};
