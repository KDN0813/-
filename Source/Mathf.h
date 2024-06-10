#pragma once

// 浮動小数算術
class Mathf
{
public:
    // 線形補間
    static float Lerp(float a, float b, float t)
    {
        return a * (1.0f - t) + (b * t);
    }

    // 指定範囲のランダム値を計算する
    static  float RandomRange(float min, float max);
};

