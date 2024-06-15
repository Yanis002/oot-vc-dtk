#ifndef REVOSDK_MATH_MTX44_H
#define REVOSDK_MATH_MTX44_H

#include "dolphin/types.h"

typedef float Mtx44[4][4];

void C_MTXPerspective(Mtx44, float, float, float, float);
void C_MTXFrustum(Mtx44, float, float, float, float, float, float);
void C_MTXOrtho(Mtx44, float, float, float, float, float, float);

#endif