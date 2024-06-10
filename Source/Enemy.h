#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

// エネミー
class Enemy : public Character
{
public:
    Enemy(){}
    ~Enemy()override{}

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    // デバッグ用GUI描画[org]
    virtual void DrawDebugGUI() = 0;

    // デバックプリミティブ描画[05]
    virtual void DarwDebugPrimitive();

    // 破棄[08]
    void Destroy();

    // エネミーの数[org]
    static int enemuNum;
};