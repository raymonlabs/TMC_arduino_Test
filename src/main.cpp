#include <Arduino.h>
#include <TMC5160.h>

const uint8_t mot0_CS = 2; // CS pin in SPI mode
const uint8_t mot0_DRV_ENN = 3;  // DRV_ENN pin in SPI mode

const uint8_t mot1_CS = 4; // CS pin in SPI mode
const uint8_t mot1_DRV_ENN = 5;  // DRV_ENN pin in SPI mode

const uint8_t mot2_CS = 6; // CS pin in SPI mode
const uint8_t mot2_DRV_ENN = 7;  // DRV_ENN pin in SPI mode


TMC5160_SPI motor0 = TMC5160_SPI(mot0_CS); //Use default SPI peripheral and SPI settings.
TMC5160_SPI motor1 = TMC5160_SPI(mot1_CS); //Use default SPI peripheral and SPI settings.
TMC5160_SPI motor2 = TMC5160_SPI(mot2_CS); //Use default SPI peripheral and SPI settings.


void setup()
{
    // USB/debug serial coms
    Serial.begin(115200);

    pinMode(mot0_DRV_ENN, OUTPUT);
    digitalWrite(mot0_DRV_ENN, LOW); // Active low

    pinMode(mot1_DRV_ENN, OUTPUT);
    digitalWrite(mot1_DRV_ENN, LOW); // Active low

    pinMode(mot2_DRV_ENN, OUTPUT);
    digitalWrite(mot2_DRV_ENN, LOW); // Active low

    // This sets the motor & driver parameters /!\ run the configWizard for your driver and motor for fine tuning !
    TMC5160::PowerStageParameters powerStageParams; // defaults.
    TMC5160::MotorParameters motorParams;
    motorParams.globalScaler = 98; // Adapt to your driver and motor (check TMC5160 datasheet - "Selecting sense resistors")
    motorParams.irun = 31;
    motorParams.ihold = 16;

    SPI.begin();
    motor0.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
    motor1.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
    motor2.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);

    // ramp definition
    motor0.setRampMode(TMC5160::POSITIONING_MODE);
    motor0.setMaxSpeed(400);
    motor0.setAcceleration(500);

    motor1.setRampMode(TMC5160::POSITIONING_MODE);
    motor1.setMaxSpeed(400);
    motor1.setAcceleration(500);

    motor2.setRampMode(TMC5160::POSITIONING_MODE);
    motor2.setMaxSpeed(400);
    motor2.setAcceleration(500);

    Serial.println("starting up");

    delay(1000); // Standstill for automatic tuning
}

void loop()
{
    uint32_t now = millis();
    static unsigned long t_dirchange, t_echo;
    static bool dir;

    // every n seconds or so...
    if ( now - t_dirchange > 2000 )
    {
        t_dirchange = now;

        // reverse direction
        dir = !dir;
        motor0.setTargetPosition(dir ? 700 : 0);  // 1 full rotation = 200s/rev
        motor1.setTargetPosition(dir ? 700 : 0);  // 1 full rotation = 200s/rev
        motor2.setTargetPosition(dir ? 700 : 0);  // 1 full rotation = 200s/rev
    }

    // print out current position
    if( now - t_echo > 100 )
    {
        t_echo = now;

        // get the current target position
        float xactual = motor0.getCurrentPosition();
        float vactual = motor0.getCurrentSpeed();
        Serial.print("current position : ");
        Serial.print(xactual);
        Serial.print("\tcurrent speed : ");
        Serial.println(vactual);
    }
}