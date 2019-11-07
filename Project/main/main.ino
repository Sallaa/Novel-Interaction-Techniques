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

float compassAngle;
float destinationAngle;

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

void vibrate(){
    float threshholdNoVibrationmin, threshholdNoVibrationmax;
    float threshholdVibrationmin, threshholdVibrationmax;

    threshholdNoVibrationmin = destinationAngle + 0.05 * destinationAngle;
    threshholdNoVibrationmax = destinationAngle - 0.05 * destinationAngle;

    threshholdVibrationmin = destinationAngle + 0.2 * destinationAngle;
    threshholdVibrationmax = destinationAngle - 0.2 * destinationAngle;

    (threshholdNoVibrationmin < 0) ? (threshholdNoVibrationmin += 2 * PI) : ();
    (threshholdNoVibrationmin > 2 * PI) ? (threshholdNoVibrationmin %= 2 * PI) : ();

    (threshholdNoVibrationmax < 0) ? (threshholdNoVibrationmax += 2 * PI) : ();
    (threshholdNoVibrationmax > 2 * PI) ? (threshholdNoVibrationmax %= 2 * PI) : ();

    (threshholdVibrationmin < 0) ? (threshholdVibrationmin += 2 * PI) : ();
    (threshholdVibrationmin > 2 * PI) ? (threshholdVibrationmin %= 2 * PI) : ();

    (threshholdVibrationmax < 0) ? (threshholdVibrationmax += 2 * PI) : ();
    (threshholdVibrationmax > 2 * PI) ? (threshholdVibrationmax %= 2 * PI) : ();
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

    destinationAngle = getBearing(startLat, startLong, endLat, endLong);
    Serial.println(getBearing(startLat, startLong, endLat, endLong));
}
