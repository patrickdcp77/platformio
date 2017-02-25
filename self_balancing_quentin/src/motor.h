#ifndef MOTOR_H
#define MOTOR_H

#include "io.h"

void initMotor();
void setDirection(int motor, bool forward);
void setPower(int motor, unsigned int power);

#endif
