



#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SoftwareSerial.h>

#include "io.h"
#include "motor.h"

#define ACCEL_GAIN 60.0
#define GYRO_GAIN -90.0

const float ANGLE_GAIN = 1.20;

float aa_constant = 0.1;
float ti_constant = 3;
float overallgain;

#define DEBUG_ENABLE_PRINTING 1
#define DEBUG_DISABLE_MOTORS 0

MPU6050 mpu;
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
int16_t gyro[3];
float ypr[3];

float angle_Y, angular_rate_Y, angular_rate_X, angle_X, angle_Z, angular_rate_Z;
float initial_angle_Y;

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

float cur_speed;
float cycle_time = 0.01;

unsigned long STD_LOOP_TIME = 9000;
unsigned long lastLoopTime = STD_LOOP_TIME;
unsigned long lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;
unsigned int skip;

float level = 0;
float Steering;
float SteerValue;
float SteerCorrect;
int Steer = 0;

float initial_angular_rate_Y = 0;
float initial_angular_rate_Y_sum = 0;
float initial_angular_rate_X = 0;
float initial_angular_rate_X_sum = 0;

float gyroangle_dt;
float angle;
float anglerads;
float balance_torque;
float softstart;

float balancetrim = 0;

signed char Motor1percent;
signed char Motor2percent;

int SteerLeft;
int SteerRight;

int balancelForward;
int balancelBackward;
float gv0, gv1, gv2, gv3, gv4, gv5, gv6;

float SG_filter_result;
float x_accdeg;

float gangleraterads;
float gangleratedeg;
float gangleratedeg2;

void read_accel_gyro();
void do_calculations();
void set_motor();
void serialOut_timing();
void serialOut_timing();

void setup() {
    int j;
    initMotor();
    pinMode(forwardPin, INPUT_PULLUP);
    pinMode(backwardPin, INPUT_PULLUP);

    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

    Serial.begin(115200);

    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    delay(2);

    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(239);
    mpu.setYGyroOffset(-44);
    mpu.setZGyroOffset(-50);
    mpu.setZAccelOffset(1788);

    if (devStatus == 0) {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(intPin), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    delay(2000);

    for (j = 0; j < 7; j++) {
        read_accel_gyro();
        initial_angular_rate_Y_sum = (float) initial_angular_rate_Y_sum + angular_rate_Y;
        initial_angular_rate_X_sum = (float) initial_angular_rate_X_sum + angular_rate_X;
        delay(10);
    }

    initial_angular_rate_Y = (float) initial_angular_rate_Y_sum / 7;
    initial_angular_rate_X = (float) initial_angular_rate_X_sum / 7;

    initial_angle_Y = angle_Y;
}

void loop() {
    overallgain = 0;
    cur_speed = 0;
    level = 0;
    Steer = 0;
    angle = 0;
    Steering = 512;
    SteerValue = 512;
    overallgain = 0.3;

    while (1) {

        read_accel_gyro();
        do_calculations();
        set_motor();

        lastLoopUsefulTime = micros() - loopStartTime;

        if (lastLoopUsefulTime < STD_LOOP_TIME) {
            delayMicroseconds(STD_LOOP_TIME - lastLoopUsefulTime);
        }

        lastLoopTime = micros() - loopStartTime;
        loopStartTime = micros();

        serialOut_timing();

        if (overallgain < 0.5) {
            overallgain = (float) overallgain + 0.005;
        }
        if (overallgain > 0.5) {
            overallgain = 0.5;
        }

    }
}

void read_accel_gyro() {

    if (!dmpReady)
        return;
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.print(" fifoCount: ");
        Serial.print(fifoCount);
        Serial.print(" mpuIntStatus: ");
        Serial.print(mpuIntStatus);
        Serial.println(F("FIFO overflow!"));
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize)
            fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGyro(gyro, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        angle_X = ypr[0] * RAD_TO_DEG; // not used...0 is center of gravity offset
        angle_Y = ypr[1] * RAD_TO_DEG; // Accel for Tilt, 0 is center of gravity offset
        angle_Z = ypr[2] * RAD_TO_DEG; // not used...0 is center of gravity offset
        angular_rate_X = ((double) gyro[0] / 131.0); // Gyro for steering, in degs/sec.
        angular_rate_Y = ((double) gyro[1] / 131.0); // Gyro for tilt, in degs/sec.
        angular_rate_Z = ((double) gyro[2] / 131.0); // Gyro for X, in degs/sec.

        angular_rate_X = angular_rate_X * RAD_TO_DEG; // Gyro for steering, in degs/sec.
        angular_rate_Y = angular_rate_Y * RAD_TO_DEG; // Gyro for tilt,
        angular_rate_Z = angular_rate_Z * RAD_TO_DEG; // Gyro for X

    }
}

void do_calculations() {
    SteerLeft = 1;
    SteerRight = 1;

    balancelForward = digitalRead(forwardPin);
    balancelBackward = digitalRead(backwardPin);

    if (balancelForward == 0)
        balancetrim = balancetrim - 0.04;
    if (balancelBackward == 0)
        balancetrim = balancetrim + 0.04;

    if (balancetrim < -30)
        balancetrim = -30;
    if (balancetrim > 30)
        balancetrim = 30;

    gv0 = gv1;
    gv1 = gv2;
    gv2 = gv3;
    gv3 = gv4;
    gv4 = gv5;
    gv5 = gv6;
    gv6 = (float) angle_Y;

    SG_filter_result = (float) ((-2 * gv0) + (3 * gv1) + (6 * gv2) + (7 * gv3) + (6 * gv4) + (3 * gv5) + (-2 * gv6)) / 21;

    gangleratedeg2 = angular_rate_X - initial_angular_rate_X; //IDH subtract curent value from inital value to get delta.

    if (SteerLeft == 1 && SteerRight == 1) {
        SteerCorrect = 0;
        if (gangleratedeg2 > 10 || gangleratedeg2 < -10) {
            SteerCorrect = (float) 0.4 * gangleratedeg2;
        }
        SteerValue = 512;
    } else {
        if (SteerLeft == 0) {
            SteerValue = 612;
        }

        if (SteerRight == 0) {
            SteerValue = 412;
        }
        SteerCorrect = 0;
    }

    //SG_filter_result = (float) SG_filter_result;

    x_accdeg = (float) ((SG_filter_result - (initial_angle_Y + balancetrim)) * (1.0) * ANGLE_GAIN);

    if (x_accdeg < -72)
        x_accdeg = -72;
    if (x_accdeg > 72)
        x_accdeg = 72;

    gangleratedeg = (float) (angular_rate_Y - initial_angular_rate_Y); // IDH
    if (gangleratedeg < -110)
        gangleratedeg = -110;
    if (gangleratedeg > 110)
        gangleratedeg = 110;

    gyroangle_dt = (float) ti_constant * cycle_time * gangleratedeg;

    gangleraterads = (float) gangleratedeg * 0.017453;

    angle = (float) ((1 - aa_constant) * (angle + gyroangle_dt)) + (aa_constant * x_accdeg);

    anglerads = (float) angle * 0.017453;

    balance_torque = (float) (ACCEL_GAIN * anglerads) + (GYRO_GAIN * gangleraterads);

    cur_speed = (float) (cur_speed + (anglerads * 6 * cycle_time)) * 0.999;
    //cur_speed = level * 0.005 + cur_speed;
    level = (float) (balance_torque - cur_speed) * overallgain;

}

void set_motor() {
    unsigned int p;

    level = level * 50.0;
    if (level < -100.0) {
        level = -100.0;
    }
    if (level > 100.0) {
        level = 100.0;
    }

    Steer = (float) SteerValue - SteerCorrect;
    Steer = (Steer - 512) * 0.09;
    Motor1percent = (signed char) level + Steer;
    Motor2percent = (signed char) level - Steer;

    if (Motor1percent > 100)
        Motor1percent = 100;
    if (Motor1percent < -100)
        Motor1percent = -100;
    if (Motor2percent > 100)
        Motor2percent = 100;
    if (Motor2percent < -100)
        Motor2percent = -100;

    if (DEBUG_DISABLE_MOTORS == 1) {
        Motor1percent = 0;
        Motor2percent = 0;
    }

    setDirection(0, Motor1percent > 0);
    setDirection(1, Motor2percent > 0);

    p = abs(Motor1percent) * 2.5;
    setPower(0, p);

    p = abs(Motor2percent) * 2.5;
    setPower(1, p);
}

////////////////////////////////////////////////////////////////////////////////
void serialOut_timing() { //print out to serial port when enabled.
    ////////////////////////////////////////////////////////////////////////////////

    if (DEBUG_ENABLE_PRINTING == 1 && //deadman is pushed
            skip++ == 10) { //display every 200ms (at 5Hz)
        skip = 0;

        Serial.print(" ang_X: ");
        Serial.print(angle_X);
        Serial.print(" ang_Y: ");
        Serial.print(angle_Y);
        Serial.print(" ang_Z: ");
        Serial.print(angle_Z);
        Serial.print(" ang_rate_X: ");
        Serial.print(angular_rate_X);
        Serial.print(" ang_rate_Y: ");
        Serial.print(angular_rate_Y);
        Serial.print(" ang_rate_Z: ");
        Serial.print(angular_rate_Z);
        Serial.print("   Mot1%: ");
        Serial.print(Motor1percent);
        Serial.print(" Mot2%: ");
        Serial.print(Motor2percent);
        Serial.print(" x_accdeg: ");
        Serial.print(x_accdeg);
        //    Serial.print(" gangratedeg: ");
        //    Serial.print(gangleratedeg);
        //    Serial.print(" gyroangle_dt: ");
        //    Serial.print(gyroangle_dt);
        Serial.print(" cur_speed: ");
        Serial.print(cur_speed);
        Serial.print(" trim: ");
        Serial.print(balancetrim);
        Serial.print(" level: ");
        Serial.print(level);
        Serial.println("  ");
    }
}
