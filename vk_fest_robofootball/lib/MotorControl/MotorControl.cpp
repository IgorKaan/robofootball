#include "MotorControl.h"
#include "math.h"
#include <Arduino.h>
#include <PCF8574.h>

PCF8574 expander;
int encoderRightCnt = 0;
int encoderLeftCnt = 0;
uint8_t rF = 4;
uint8_t rB = 16;
uint8_t lF = 2;
uint8_t lB = 15;
byte value = 0;
byte value1 = 0;

void doEncoderRight()
{ 
    static byte pred_r = 0;
    byte r = expander.digitalRead(6);
    if(r != pred_r)
    {
        pred_r = r;
        encoderRightCnt++;
    }
}

void doEncoderLeft()
{ 
    static byte pred_l = 0;
    byte l = expander.digitalRead(7);
    if(l != pred_l)
    {
        pred_l = l;
        encoderLeftCnt++;
    }
}

void MotorControl::setupMotor()
{
    pinMode(rF, OUTPUT);
    pinMode(lF, OUTPUT);
    ledcSetup(1,500,8);
    ledcSetup(2,500,8);
    ledcSetup(3,500,8);
    ledcSetup(4,500,8);
    ledcAttachPin(16,1);
    ledcAttachPin(15,2);
    ledcAttachPin(4,3);
    ledcAttachPin(2,4);
    expander.begin(0x3F);
    expander.pinMode(6, INPUT);
    expander.pinMode(7, INPUT);
    expander.pullUp(6);
    expander.pullUp(7);
}

void MotorControl::goForward(short angle)
{
    ledcWrite(1,120);
    ledcWrite(2,120);
    ledcWrite(3,0);
    ledcWrite(4,0);
}

void MotorControl::goBackward(short angle)
{
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,120);
    ledcWrite(4,120);    
}

void MotorControl::goLeft(short angle)
{
    ledcWrite(2,120); 
    ledcWrite(1,120-90*-sin(angle/57.2958));  
    ledcWrite(3,0);
    ledcWrite(4,0);      
}

void MotorControl::goRight(short angle)
{
    ledcWrite(1,120); 
    ledcWrite(2,120-90*sin(angle/57.2958));  
    ledcWrite(3,0);
    ledcWrite(4,0);      
}

void MotorControl::turnLeft(short angle)
{
    ledcWrite(2,100);
    ledcWrite(3,100);
    ledcWrite(1,0);
    ledcWrite(4,0);
    delay(100);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(1,0);
    ledcWrite(4,0);
    delay(20);
}

void MotorControl::turnRight(short angle)
{
    ledcWrite(2,0);
    ledcWrite(3,0); 
    ledcWrite(1,100);
    ledcWrite(4,100);  
    delay(100);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(1,0);
    ledcWrite(4,0);
    delay(20);
}

void MotorControl::stopMovement()
{
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(4,0); 
}


