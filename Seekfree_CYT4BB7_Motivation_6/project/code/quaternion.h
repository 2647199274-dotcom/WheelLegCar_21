#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "zf_common_headfile.h"
#include "Attitude.h"

typedef struct
{
    float pitch;
    float roll;
    float yaw;
} Euler_angle;

extern Euler_angle angle;

void Quaternion_Init(void);
void Quaternion_Update(void);
void Quaternion_GetEuler(Euler_angle *euler);

#endif
