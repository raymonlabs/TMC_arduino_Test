#include <Arduino.h>
#include "iodef.h"

int x = 0;
int y = 0;

void DRV_EN()
{
    digitalWrite(X_EN, HIGH);
    digitalWrite(Y_EN, HIGH);
    digitalWrite(Z_EN, HIGH);
    digitalWrite(E0_EN, LOW);
    digitalWrite(E1_EN, HIGH);
}


void X_STEP_FWD()
{
    digitalWrite(X_DIR, HIGH);
    //delay(500);
    for( x ; x <250; x++)
    {
        digitalWrite(X_STEP, HIGH);
        delayMicroseconds(50000);
        digitalWrite(X_STEP, LOW);
        delayMicroseconds(50000);
    }
    x=0;
}

void X_STEP_RVS()
{
    digitalWrite(X_DIR, LOW);
    delay(500);
    for( y ; y <250; y++)
    {
        digitalWrite(X_STEP, HIGH);
        delayMicroseconds(50000);
        digitalWrite(X_STEP, LOW);
        delayMicroseconds(50000);
    }
    y=0;
}

void setup()
{
    pinMode(MO1_STAT, INPUT);
    pinMode(MO2_STAT, INPUT);
    pinMode(MO3_STAT, INPUT);
    pinMode(MO4_STAT, INPUT);
    pinMode(MO5_STAT, INPUT);

    pinMode(X_STEP, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Z_STEP, OUTPUT);
    pinMode(E0_STEP, OUTPUT);
    pinMode(E1_STEP, OUTPUT);

    pinMode(X_DIR, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Z_DIR, OUTPUT);
    pinMode(E0_DIR, OUTPUT);
    pinMode(E1_DIR, OUTPUT);

    pinMode(X_EN, OUTPUT);
    pinMode(Y_EN, OUTPUT);
    pinMode(Z_EN, OUTPUT);
    pinMode(E0_EN, OUTPUT);
    pinMode(E1_EN, OUTPUT);

    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    DRV_EN();



// write your initialization code here
}


void loop()
{
    X_STEP_FWD();
    //delay(25);
    //X_STEP_RVS();
}