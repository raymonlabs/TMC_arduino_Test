#include <Arduino.h>
#include <TMC5160.h>
#include "iodef.h"
#include "MightyZap.h"

#define TMC_iRun 31
#define TMC_iHold 20

int lin0ToggleSw = 0;
int lin0pushSw = 0;
int pressValue = 1;

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

MightyZap m_zap(&Serial3, 10);

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
    motorParams.globalScaler = 98; // Adapt to your driver and motor (check TMC5160 datasheet - "Selecting sense resistors")
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
        motors[i].setAcceleration(150);
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
    lin0pushSw = digitalRead(lin0pushPin);
    lin0ToggleSw = digitalRead(lin0togglePin);

    if (lin0pushSw == 0)
    {
        if (lin0ToggleSw == 1)
        {
            m_zap.GoalPosition(0, pressValue);
            m_zap.ledOn(0, RED);
            delay(1000);
            //Serial.println("actH");
            if (pressValue != 4096) pressValue++;
        } else
        {
            m_zap.GoalPosition(0, pressValue);
            m_zap.ledOn(0, GREEN);
            delay(1000);
            //Serial.println("actL");
            if (pressValue != 0) pressValue--;
        }
    } else
    {
        delay(1);
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
    mot0PushSw = digitalRead(mot0PwSw);
    mot1PushSw = digitalRead(mot1PwSw);
    mot2PushSw = digitalRead(mot2PwSw);

    mot0ToggleSw = digitalRead(mot0DirSw);
    mot1ToggleSw = digitalRead(mot1DirSw);
    mot2ToggleSw = digitalRead(mot2DirSw);

    delay(10);

    if (mot0PushSw == 0)
    {
        Serial.println("loop in");
        if (mot0ToggleSw == 1)
        {
            //Serial.println(mot0PushSw);
            motors[0].setMaxSpeed(500);
        } else
        {
            motors[0].setMaxSpeed(-500);
        }
    } else
    {
        {
            motors[0].stop();
        }
    }

    if (mot1PushSw == 0)
    {
        Serial.println("loop in");
        if (mot1ToggleSw == 1)
        {
            //Serial.println(mot0PushSw);
            motors[1].setMaxSpeed(-500);
        } else
        {
            motors[1].setMaxSpeed(500);
        }
    } else
    {
        {
            motors[1].stop();
        }
    }

    if (mot2PushSw == 0)
    {
        Serial.println("loop in");
        if (mot2ToggleSw == 1)
        {
            //Serial.println(mot0PushSw);
            motors[2].setMaxSpeed(500);
        } else
        {
            motors[2].setMaxSpeed(-500);
        }
    } else
    {
        {
            motors[2].stop();
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

    pinMode(mot0PwSw, INPUT_PULLUP);
    pinMode(mot1PwSw, INPUT_PULLUP);
    pinMode(mot2PwSw, INPUT_PULLUP);

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
    linActuator();
    stepMotorDrive();
}