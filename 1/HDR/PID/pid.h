#ifndef __PID_H
#define __PID_H

#include "sys.h"

int Velocity_PI(int Speed_measure);
int Turn(int gyro_Z);
int Vertical_PI(int gyro_Z);



void EXTI9_5_IRQHandler(void);

void luxian1(void);
void luxian2(void);
void luxian3(void);
void luxian4(void);
void luxian5(void);
void luxian6(void);
void luxian7(void);
void luxian8(void);


#endif

