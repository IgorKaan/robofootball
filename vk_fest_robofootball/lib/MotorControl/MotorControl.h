#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>
#include <WEMOS_Motor.h>
#include "math.h" 

class MotorControl
{
    public:
        short rotateAngle;

        float targetAngle;
    
        void setupMotor();

        void driveMotor();

        void goForward(short angle);

        void goBackward(short angle);

        void goLeft(short angle);

        void goRight(short angle);

        void turnLeft(short angle);

        void turnRight(short angle);

        void stopMovement();

        void doEncoderRight();

        void doEncoderLeft();

    private:

        short currentVectorX;

        short currentVectorY;

        short targetVectorX;

        short targetVectorY;

        short x0, x1, x2, y0, y1, y2;

        float scalprod, mod1, mod2;

};

#endif