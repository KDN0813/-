#pragma once

// シーン[11]
class Scene
{
public:
    Scene(){}
    virtual ~Scene(){}

    // 初期化[11]
    virtual void Initialize() = 0;

    // 終了化[11]
    virtual void Finalize() = 0;

    // 更新処理[11]
    virtual void Update(float elapsedTime) = 0;

    // 描画処理[11]
    virtual void Render() = 0;

    // 準備完了しているか[11]
    bool IsReady() const { return ready; }

    // 準備完了設定[11]
    void SetReady() { ready = true; }

private:
    // [11]
    bool ready = false; // 準備が完了しているか
};