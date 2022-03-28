#include <Arduino.h>
#include <TMC5160.h>

const uint8_t mot0_CS = 2; // CS pin in SPI mode
const uint8_t mot0_DRV_ENN = 3;  // DRV_ENN pin in SPI mode

const uint8_t mot1_CS = 4; // CS pin in SPI mode
const uint8_t mot1_DRV_ENN = 5;  // DRV_ENN pin in SPI mode

const uint8_t mot2_CS = 6; // CS pin in SPI mode
const uint8_t mot2_DRV_ENN = 7;  // DRV_ENN pin in SPI mode

const uint8_t MOTORS_LENGTH = 3;

TMC5160_SPI motors[MOTORS_LENGTH] = {
        TMC5160_SPI(mot0_CS),
        TMC5160_SPI(mot1_CS),
        TMC5160_SPI(mot2_CS)
};

void setMotorRamp(TMC5160_SPI motors[]);

void setMotorBegin(TMC5160_SPI motors[]);



void setup()
{
    // USB/debug serial COM PORT
    Serial.begin(115200);

    pinMode(mot0_DRV_ENN, OUTPUT);
    digitalWrite(mot0_DRV_ENN, LOW); // Active low

    pinMode(mot1_DRV_ENN, OUTPUT);
    digitalWrite(mot1_DRV_ENN, LOW); // Active low

    pinMode(mot2_DRV_ENN, OUTPUT);
    digitalWrite(mot2_DRV_ENN, LOW); // Active low


    SPI.begin();
    setMotorBegin(motors);

    // ramp definition
    setMotorRamp(motors);

    Serial.println("starting up");

    delay(1000); // Standstill for automatic tuning
}

void setMotorBegin(TMC5160_SPI motors[]) {
    // This sets the motor & driver parameters /!\ run the configWizard for your driver and motor for fine-tuning !
    TMC5160::PowerStageParameters powerStageParams; // defaults.
    TMC5160::MotorParameters motorParams;
    motorParams.globalScaler = 0; // Adapt to your driver and motor (check TMC5160 datasheet - "Selecting sense resistors")
    motorParams.irun = 31;
    motorParams.ihold = 20;
    for (int i = 0; i < MOTORS_LENGTH; ++i)
    {
        motors[i].begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
    }

}
void setMotorRamp(TMC5160_SPI motors[])
{
    for (int i = 0; i < MOTORS_LENGTH; ++i)
    {
        motors[i].setRampMode(TMC5160::POSITIONING_MODE);
        motors[i].setMaxSpeed(400);
        motors[i].setAcceleration(500);
    }

}

void loop()
{
    uint32_t now = millis();
    static unsigned long tDirChange, tEcho;
    static bool dir;

    // every n seconds or so...
    if (now - tDirChange > 2000)
    {
        tDirChange = now;

        // reverse direction
        dir = !dir;
        for (int i = 0; i < MOTORS_LENGTH; ++i)
        {
            motors[i].setTargetPosition(dir ? 700 : 0);
        }
    }

    // print out current position
    if (now - tEcho > 100)
    {
        tEcho = now;

        // get the current target position
        float xActual = motors[0].getCurrentPosition();
        float vActual = motors[0].getCurrentSpeed();
        Serial.print("current position : ");
        Serial.print(xActual);
        Serial.print("\tcurrent speed : ");
        Serial.println(vActual);
    }
}
