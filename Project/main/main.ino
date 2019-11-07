#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_LSM303.h>

const int pinVibrator50 = 12; // use a 100Ohm resistor
const int pinVibrator100 = 13;

const float startLat = 42.4448796;
const float startLong = -76.4810678;

const float endLat = 42.4451686;
const float endLong = -76.4825014;


float toRadians (float n){
    return n * (PI / 180);
}

float toDegrees(float n){
    return n *(180 / PI);
}

float getBearing(float startLat, float startLong, float endLat, float endLong)
{
    startLat = radians(startLat);
    startLong = radians(startLong);
    endLat = radians(endLat);
    endLong = radians(endLong);

    float dLong = endLong - startLong;

    float dPhi = log(tan(endLat / 2.0 + PI / 4.0) / tan(startLat / 2.0 + PI / 4.0));
    if (abs(dLong) > PI)
    {
        if (dLong > 0.0)
            dLong = -(2.0 * PI - dLong);
        else
            dLong = (2.0 * PI + dLong);
    }
    float maxDegrees = 360;
    Serial.println(atan2(dLong, dPhi));
    return fmod((toDegrees(atan2(dLong, dPhi)) + maxDegrees), maxDegrees);
}



void setup()
{
    Serial.begin(9600);
    pinMode(pinVibrator50, OUTPUT);
    pinMode(pinVibrator100, OUTPUT);
}

void loop()
{
    // put your main code here, to run repeatedly:
    digitalWrite(pinVibrator100, LOW);
    delay(1000);
    digitalWrite(pinVibrator50, HIGH);
    delay(1000);
    digitalWrite(pinVibrator50, LOW);
    delay(1000);
    digitalWrite(pinVibrator100, HIGH);
    delay(1000);
    digitalWrite(pinVibrator100, LOW);
    delay(1000);

    Serial.println(getBearing(startLat,startLong, endLat, endLong));
}
