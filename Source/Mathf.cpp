#include <stdlib.h>
#include "Mathf.h"

// ���`���
//float Mathf::Lerp(float a, float b, float t)
//{
//    return a * (1.0f - t) + (b + t);
//}

float Mathf::RandomRange(float min, float max)
{
    // 0.0�`1.0�̒l�����߂�
    float value = static_cast<float>(rand()) / RAND_MAX;

    return min + (max - min) * value;
}
