#pragma once

// ���������Z�p
class Mathf
{
public:
    // ���`���
    static float Lerp(float a, float b, float t)
    {
        return a * (1.0f - t) + (b * t);
    }

    // �w��͈͂̃����_���l���v�Z����
    static  float RandomRange(float min, float max);
};

