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

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void displaySensorDetails(void)
{
    sensor_t sensor;
    mag.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println(" uT");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println(" uT");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println(" uT");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

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
//    Serial.println(atan2(dLong, dPhi));
    return toRadians(fmod((toDegrees(atan2(dLong, dPhi))), maxDegrees));
}
 
void vibrate(){
    float threshholdNoVibrationmin, threshholdNoVibrationmax;
    float threshholdVibrationmin, threshholdVibrationmax;

    threshholdNoVibrationmin = destinationAngle + 0.05 * 2 * PI;
    threshholdNoVibrationmax = destinationAngle - 0.05 * 2 * PI;

    threshholdVibrationmin = destinationAngle + 0.2 * 2 * PI;
    threshholdVibrationmax = destinationAngle - 0.2 * 2 * PI;

    if (threshholdNoVibrationmin < 0){
        threshholdNoVibrationmin += 2 * PI;
    }

    if (threshholdNoVibrationmin > 2 * PI){
        threshholdNoVibrationmin -= 2 * PI;
    }

    if (threshholdNoVibrationmax < 0){
        threshholdNoVibrationmax += 2 * PI;
    }

    if (threshholdNoVibrationmax > 2 * PI){
        threshholdNoVibrationmax -= 2 * PI;
    }

    if (threshholdVibrationmin < 0){
        threshholdVibrationmin += 2 * PI;
    }
    
    if (threshholdVibrationmin > 2 * PI){
        threshholdVibrationmin -= 2 * PI;
    }

    if (threshholdVibrationmax < 0){
        threshholdVibrationmax += 2 * PI;
    }

    if (threshholdVibrationmax > 2 * PI){
        threshholdVibrationmax -= 2 * PI;
    }

    Serial.print("threshholdNoVibrationmin:"); Serial.println(threshholdNoVibrationmin);
    Serial.print("threshholdNoVibrationmax:"); Serial.println(threshholdNoVibrationmax);

    // TODO if Statements for destinationAngle
    if (threshholdNoVibrationmax > threshholdNoVibrationmin){
        if (threshholdNoVibrationmax <= destinationAngle && destinationAngle >= threshholdNoVibrationmin)
        {
            // TODO: No vibration
        }
    }
    else if (threshholdNoVibrationmin > threshholdNoVibrationmax){
        if ((threshholdNoVibrationmin <= destinationAngle && destinationAngle <= 2* PI) ||  0 <= destinationAngle && destinationAngle <= threshholdNoVibrationmax)
        {
            // TODO: No vibration
        }
    }
}

void compasMeasure(){
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);

    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: ");
    Serial.print(event.magnetic.x);
    Serial.print("  ");
    Serial.print("Y: ");
    Serial.print(event.magnetic.y);
    Serial.print("  ");
    Serial.print("Z: ");
    Serial.print(event.magnetic.z);
    Serial.print("  ");
    Serial.println("uT");

    /* Note: You can also get the raw (non unified values) for */
    /* the last data sample as follows. The .getEvent call populates */
    /* the raw values used below. */
    // Serial.print("X Raw: "); Serial.print(mag.raw.x); Serial.print("  ");
    // Serial.print("Y Raw: "); Serial.print(mag.raw.y); Serial.print("  ");
    // Serial.print("Z Raw: "); Serial.print(mag.raw.z); Serial.println("");
    Serial.print("atan2: ");
    Serial.println(atan2(event.magnetic.x, event.magnetic.y));
    float angle = toRadians((180 / 3.14) * atan2(event.magnetic.x, event.magnetic.y) + 180);
    Serial.print("Angle: ");
    Serial.println(angle);

    /* Delay before the next sample */
    delay(500);
}

void setup()
{
    #ifndef ESP8266
        while (!Serial)
            ; // will pause Zero, Leonardo, etc until serial console opens
    #endif

    Serial.begin(9600);

    pinMode(pinVibrator50, OUTPUT);
    pinMode(pinVibrator100, OUTPUT);

    Serial.println("Magnetometer Test");
    Serial.println("");

    /* Enable auto-gain */
    mag.enableAutoRange(true);

    /* Initialise the sensor */
    if (!mag.begin())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while (1)
            ;
    }

    /* Display some basic information on this sensor */
    displaySensorDetails();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // digitalWrite(pinVibrator100, LOW);
    // delay(1000);
    // digitalWrite(pinVibrator50, HIGH);
    // delay(1000);
    // digitalWrite(pinVibrator50, LOW);
    // delay(1000);
    // digitalWrite(pinVibrator100, HIGH);
    // delay(1000);
    // digitalWrite(pinVibrator100, LOW);
    // delay(1000);

    destinationAngle = getBearing(startLat, startLong, endLat, endLong);
    vibrate();
    Serial.println(destinationAngle);
    compasMeasure();
}
