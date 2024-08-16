#ifndef REVOSDK_MATH_VEC_H
#define REVOSDK_MATH_VEC_H

#include "dolphin/types.h"

struct Vec {
    float x;
    float y;
    float z;
};

void PSVECNormalize(const struct Vec*, struct Vec*);
float PSVECSquareDistance(const struct Vec*, const struct Vec*);
float PSVECMag(const struct Vec*);
void PSVECCrossProduct(const struct Vec*, const struct Vec*, struct Vec*);

#endif