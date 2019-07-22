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
// uint8_t rF = 5;
// uint8_t rB = 17;
// uint8_t lF = 18;
// uint8_t lB = 19;
byte value = 0;
byte value1 = 0;

volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&mux);
}

int MotorControl::doEncoderRight()
{ 
    static byte pred_r = 0;
    byte r = expander.digitalRead(6);
    if(r != pred_r)
    {
        pred_r = r;
        encoderRightCnt++;
    }
    return encoderRightCnt;
}

int MotorControl::doEncoderLeft()
{ 
    static byte pred_l = 0;
    byte l = expander.digitalRead(7);
    if(l != pred_l)
    {
        pred_l = l;
        encoderLeftCnt++;
    }
    return encoderLeftCnt;
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
    // ledcAttachPin(5,1);
    // ledcAttachPin(18,2);
    // ledcAttachPin(17,3);
    // ledcAttachPin(19,4);
    expander.begin(0x3F);
    expander.pinMode(6, INPUT);
    expander.pinMode(7, INPUT);
    expander.pullUp(6);
    expander.pullUp(7);
}

void MotorControl::goForward(short angle)
{
    ledcWrite(1,85);
    ledcWrite(2,85);
    ledcWrite(3,0);
    ledcWrite(4,0);
    // delay(100);
    // ledcWrite(1,0);
    // ledcWrite(2,0);
    // ledcWrite(3,0);
    // ledcWrite(4,0);
    // delay(100);
}

void MotorControl::goBackward(short angle)
{
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,85);
    ledcWrite(4,85);  
    // delay(100);
    // ledcWrite(1,0);
    // ledcWrite(2,0);
    // ledcWrite(3,0);
    // ledcWrite(4,0);
    // delay(100); 
}

void MotorControl::goRight(short angle)
{ 
    // ledcWrite(1,0);
    // ledcWrite(2,100); 
    // ledcWrite(3,100);
    // ledcWrite(4,0);
    ledcWrite(1,95);
    ledcWrite(2,0); 
    ledcWrite(3,0);
    ledcWrite(4,95);
    delay(80); 
    ledcWrite(1,0);
    ledcWrite(2,0); 
    ledcWrite(3,0);
    ledcWrite(4,0);
    delay(50);   
}

void MotorControl::goLeft(short angle)
{
    // ledcWrite(1,100);
    // ledcWrite(2,0);  
    // ledcWrite(3,0);
    // ledcWrite(4,100);
    ledcWrite(1,0);
    ledcWrite(2,95); 
    ledcWrite(3,95);
    ledcWrite(4,0);
    delay(80);
    ledcWrite(1,0);
    ledcWrite(2,0);  
    ledcWrite(3,0);
    ledcWrite(4,0);
    delay(50);
}
void MotorControl::turnLeft(short angle)
{
   
}

void MotorControl::turnRight(short angle)
{
    
}

void MotorControl::stopMovement()
{
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(4,0);
}


