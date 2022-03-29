#include <Arduino.h>
#include <TMC5160.h>
//#include "iodef.h"
#include "MightyZap.h"

#define TMC_iRun 31
#define TMC_iHold 20

const uint8_t MOTORS_LENGTH = 3;

// TMC5160 SPI CS PIN
#define mot0_CS 2
#define mot1_CS 4
#define mot2_CS 6

// TMC5160 DRIVER ENABLE PIN
#define mot0_DRV_ENN 3
#define mot1_DRV_ENN 5
#define mot2_DRV_ENN 7

//LINEAR ACTUATOR SWITCH INPUT PIN
#define lin0togglePin 31
#define lin1pushPin 37
#define lin2pushPin 41
#define lin3pushPin 45
#define lin4pushPin 49

#define lin0pushPin 33
#define lin1togglePin 35
#define lin2togglePin 39
#define lin3togglePin 43
#define lin4togglePin 47

//STEP MOTOR SWITCH PIN
#define mot0DirSw 22
#define mot1DirSw 24
#define mot2DirSw 26

#define mot0PwSw 28
#define mot1PwSw 30
#define mot2PwSw 32

#define mot0Spd A0
#define mot1Spd A1
#define mot2Spd A2

int lin0ToggleSw = 0;
int lin0pushSw = 0;
int pressValue = 0;

bool mot0PushSw = 1;
bool mot1PushSw = 1;
bool mot2PushSw = 1;
bool mot0ToggleSw = 1;
bool mot1ToggleSw = 1;
bool mot2ToggleSw = 1;
int mot0AnalogVal = 0;
int mot1AnalogVal = 0;
int mot2AnalogVal = 0;

int conv_mot0AnalogVal = 0;
int conv_mot1AnalogVal = 0;
int conv_mot2AnalogVal = 0;

MightyZap m_zap(&Serial2, 8);

TMC5160_SPI motors[MOTORS_LENGTH] =
        {
                TMC5160_SPI(mot0_CS),
                TMC5160_SPI(mot1_CS),
                TMC5160_SPI(mot2_CS)
        };

void setMotorBegin(TMC5160_SPI motors[])
{
    // This sets the motor & driver parameters /!\ run the configWizard for your driver and motor for fine-tuning !
    TMC5160::PowerStageParameters powerStageParams; // defaults.
    TMC5160::MotorParameters motorParams;
    motorParams.globalScaler = 0; // Adapt to your driver and motor (check TMC5160 datasheet - "Selecting sense resistors")
    motorParams.irun = TMC_iRun;
    motorParams.ihold = TMC_iHold;
    for (int i = 0; i < MOTORS_LENGTH; ++i)
    {
        motors[i].begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
    }
}

void setMotorRamp(TMC5160_SPI motors[])
{
    for (int i = 0; i < MOTORS_LENGTH; ++i)
    {
        motors[i].setRampMode(TMC5160::VELOCITY_MODE);
        motors[i].setAcceleration(500);
    }
}

void defaultState()
{
    pinMode(mot0_DRV_ENN, OUTPUT);
    pinMode(mot1_DRV_ENN, OUTPUT);
    pinMode(mot2_DRV_ENN, OUTPUT);

    digitalWrite(mot0_DRV_ENN, LOW); // Active low
    digitalWrite(mot1_DRV_ENN, LOW); // Active low
    digitalWrite(mot2_DRV_ENN, LOW); // Active low
}

void linActuator()
{
    lin0ToggleSw = digitalRead(lin0togglePin);
    lin0pushSw = digitalRead(lin0pushPin);

    if (lin0pushSw == 0)
    {
        if (lin0ToggleSw == 1)
        {
            m_zap.GoalPosition(0, pressValue);
            m_zap.ledOn(0, RED);
            delay(100);
            if (pressValue != 4096) pressValue++;
        } else
        {
            m_zap.GoalPosition(0, pressValue);
            m_zap.ledOn(0, GREEN);
            if (pressValue != 0) pressValue--;
        }
    }
}

void analogRead_Calculate()
{
    mot0AnalogVal = analogRead(mot0Spd);
    mot1AnalogVal = analogRead(mot1Spd);
    mot2AnalogVal = analogRead(mot2Spd);

    conv_mot0AnalogVal = mot0AnalogVal/2;
    conv_mot1AnalogVal = mot1AnalogVal/2;
    conv_mot2AnalogVal = mot2AnalogVal/2;
}

void stepMotorDrive()
{
//    mot0ToggleSw = digitalRead(mot0DirSw);
//    mot1ToggleSw = digitalRead(mot1DirSw);
//    mot2ToggleSw = digitalRead(mot2DirSw);

    mot0PushSw = digitalRead(mot0PwSw);
    mot1PushSw = digitalRead(mot1PwSw);
    mot2PushSw = digitalRead(mot2PwSw);
    delay(100);
    Serial.println(mot0PushSw);

    if (mot0PushSw == 0)
    {
        Serial.println("loop in");
        if (mot0ToggleSw == 1)
        {
            //Serial.println(mot0PushSw);
            for (int i = 0; i < MOTORS_LENGTH; ++i)
            {
                motors[i].setMaxSpeed(300);
            }
        } else
        {
            for (int i = 0; i < MOTORS_LENGTH; ++i)
            {
                motors[i].setMaxSpeed(-300);
            }
        }
    }
    else
    {
        Serial.println("stop loop");
        for (int i = 0; i < MOTORS_LENGTH; ++i)
        {
            motors[i].setMaxSpeed(0);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    m_zap.begin(32);

    pinMode(lin0togglePin, INPUT_PULLUP);
    pinMode(lin0pushPin, INPUT_PULLUP);

    pinMode(mot0PushSw, INPUT_PULLUP);
    pinMode(mot1PushSw, INPUT_PULLUP);
    pinMode(mot2PushSw, INPUT_PULLUP);

    pinMode(mot0ToggleSw, INPUT_PULLUP);
    pinMode(mot1ToggleSw, INPUT_PULLUP);
    pinMode(mot2ToggleSw, INPUT_PULLUP);

    delay(500);
    defaultState();

    setMotorBegin(motors);
    setMotorRamp(motors);

    Serial.println("hello");

    delay(3000); // Standstill for automatic tuning
}

void loop()
{
    //linActuator();
    stepMotorDrive();
}