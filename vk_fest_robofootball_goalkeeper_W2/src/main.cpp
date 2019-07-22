#include <Arduino.h>
#include <Wire.h>
#include <AX12A.h> 
#include "math.h"
#include <I2Cdev.h>
#include <MotorControl.h>
#include <MqttClient.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <HardwareSerial.h>
#include "esp32-hal-ledc.h"
#include <PCF8574.h>
#include "math.h"

std::string receivedData;
std::string sign;
std::string angle;
std::string move;
std::string rotate;
std::string finish;

int encRight;
int encLeft;

short speed = 100;
short correctValue = 0;

uint8_t finishValue = -1;
uint8_t splitindex;
uint8_t rotateValue = -1;
uint8_t moveForwardValue = -1;
uint8_t platformNumber = 218;
uint8_t sensorId;

bool moveSide = true;

// const char* ssid = "SPEECH_405";
// const char* password = "multimodal";
// const char* mqtt_server = "192.168.0.61";

// const char* ssid = "213_Guest";
// const char* password = "11081975";
// const char* mqtt_server = "192.168.1.72";

const char* ssid = "Keenetic-SPIRAS";
const char* password = "1122334455";
const char* mqtt_server = "192.168.1.65";

mqttClient mqtt(ssid, password, mqtt_server);
MotorControl GyroRobot;

void callback(char* topic, byte* message, unsigned int length)
{
    char platformControlTopic[64];

    sprintf(platformControlTopic, "platforms/%d", platformNumber);
    
    if (strcmp(topic, platformControlTopic)==0) {

        receivedData = "";
        sign = "";
        angle = "";
        move = "";
        rotate = "";
        finish = "";
       
        int digit_sign;
        
        for (int i = 0; i < length; i++)
            {
                receivedData += (char)message[i]; 
            } 
            sign = receivedData[0];
            angle = receivedData.substr(1, 3);
            move = receivedData[4];
            rotate = receivedData[5];
            finish = receivedData[6];

            if (sign == "0") {
                digit_sign = -1;
            }
            else {
                digit_sign = 1;
            }

            correctValue = digit_sign * atoi(angle.c_str());
            moveForwardValue = atoi(move.c_str());
            rotateValue = atoi(rotate.c_str());
            finishValue = atoi(finish.c_str());
        } 
} 

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    GyroRobot.setupMotor();
    GyroRobot = MotorControl();
    mqtt.setupWifi();
    mqtt.setCallback(*callback);
    mqtt.subscribe(platformNumber);
}

void loop()
{
    mqtt.initClientLoop();
    if (correctValue <= 90 && correctValue >= -90 && moveForwardValue == 1 && rotateValue == 0 && finishValue == 0) {
        mqtt.initClientLoop();
        GyroRobot.goForward(speed);        
    }
    else if ((correctValue > 90 || correctValue < -90) && moveForwardValue == 1 && rotateValue == 0 && finishValue == 0) {
        mqtt.initClientLoop();
        GyroRobot.goBackward(speed);        
    } 
    else if (finishValue == 1)
    {
        GyroRobot.stopMovement();
    }
    else if (rotateValue == 1 && finishValue == 0) {
        if (correctValue > 90) {
            mqtt.initClientLoop();
            GyroRobot.goLeft(speed);       
        }
        else if (correctValue < -90) {
            mqtt.initClientLoop();
            GyroRobot.goRight(speed);
        }
        else if (correctValue < 90 && correctValue > 0) {
            mqtt.initClientLoop();
            GyroRobot.goRight(speed);
        }
        else if (correctValue > -90 && correctValue < 0) {
            mqtt.initClientLoop();
            GyroRobot.goLeft(speed);
        }
        else if (finishValue == 1)
        {
        GyroRobot.stopMovement();
        }
    }
    // // mqtt.initClientLoop();
    // // if (moveForwardValue == 1 && rotateValue == 0 && finishValue == 0) {
    // //     if ((correctValue<90)&&(correctValue>-90)) {
    // //     mqtt.initClientLoop();
    // //     GyroRobot.goForward(speed); 
    // //     }
    // //     else if ((correctValue>=90)||(correctValue<=-90)) {
    // //     mqtt.initClientLoop();
    // //     GyroRobot.goBackward(speed);    
    // //     }       
    // // }
    // // else if (rotateValue == 1 && finishValue == 0) {
    // //     if (correctValue > 0) {
    // //         mqtt.initClientLoop();
    // //         GyroRobot.goRight(correctValue);       
    // //     }
    // //     else if (correctValue < 0) {
    // //         mqtt.initClientLoop();
    // //         GyroRobot.goLeft(correctValue);
    // //     }
    // // }
    // // else if (finishValue == 1) {
    // //         GyroRobot.stopMovement();
    // //         Serial.println("stop");
    // // }
    
    
}
  

