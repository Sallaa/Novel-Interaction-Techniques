#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_LSM303.h>

const float startLat = 42.4448796;
const float startLong = -76.4810678;

const float endLat = 42.4451686;
const float endLong = -76.4825014;

float compassAngle;
float destinationAngle;

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

// Bluetooth Start

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define SERVICE_UUID        "0000180D-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//#define SERVICE_UUID        "0000180D-0000-1000-8000-00805f9b34fb"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
};

// Bluetooth End

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

float toRadians(float n)
{
    return n * (PI / 180);
}

float toDegrees(float n)
{
    return n * (180 / PI);
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
    float radVal = toRadians(fmod((toDegrees(atan2(dLong, dPhi))), maxDegrees));
    if (radVal < 0){
        radVal += 2 * PI;
    }
    return radVal;
}

void leftOrRight(float currAngle, float destAngle)
{

    float directionAngle = destAngle - currAngle;
    directionAngle = fmod((directionAngle + PI),2 * PI) - PI;

    if (directionAngle < 0)
    {
        Wire.beginTransmission(4);
        Wire.write("l");
        Wire.write(0);
        delay(500);
        Wire.endTransmission();

        Wire.beginTransmission(4);
        Wire.write("r");
        Wire.write(1);
        delay(500);
        Wire.endTransmission();

        Serial.println("right");
    }
    else
    {
        Wire.beginTransmission(4);
        Wire.write("r");
        Wire.write(0);
        delay(500);
        Wire.endTransmission();

        Wire.beginTransmission(4);
        Wire.write("l");
        Wire.write(1);
        delay(500);
        Wire.endTransmission();

        Serial.println("left");
    }
}

void vibrate()
{
    // Make the vibrators vibrate based on three intensities.
    // Depends on the compassAngle measured

    float threshholdNoVibrationmin, threshholdNoVibrationmax; // threshold 5%

    // Calculation of the thresholds based on the destinationAngle
    threshholdNoVibrationmax = destinationAngle + 0.05 * 2 * PI;
    threshholdNoVibrationmin = destinationAngle - 0.05 * 2 * PI;

    // Get rid of negative values, and get rid of values > 2 * PI with modulo
    if (threshholdNoVibrationmin < 0)
    {
        threshholdNoVibrationmin += 2 * PI;
    }

    if (threshholdNoVibrationmin > 2 * PI)
    {
        threshholdNoVibrationmin -= 2 * PI;
    }

    if (threshholdNoVibrationmax < 0)
    {
        threshholdNoVibrationmax += 2 * PI;
    }

    if (threshholdNoVibrationmax > 2 * PI)
    {
        threshholdNoVibrationmax -= 2 * PI;
    }

    // Serial.println("");
    // Serial.print("threshholdNoVibrationmin: ");
    // Serial.println(threshholdNoVibrationmin);
    // Serial.print("threshholdNoVibrationmax: ");
    // Serial.println(threshholdNoVibrationmax);

    if (compassAngle > threshholdNoVibrationmax)
    {
        leftOrRight(compassAngle, destinationAngle);

        //   TODO digitalWrite(pinVibrator100, HIGH);
    }
    else if (compassAngle < threshholdNoVibrationmin)
    {

        leftOrRight(compassAngle, destinationAngle);

        //   TODO digitalWrite(pinVibrator100, HIGH);
    }
    else
    {
        Serial.print("Current Zone: Within Both Thresholds, No Vibration");

        Wire.beginTransmission(4);
        Wire.write("l");
        Wire.write(0);
        delay(300);
        Wire.endTransmission();

        Wire.beginTransmission(4);
        Wire.write("r");
        Wire.write(0);
        delay(300);
        Wire.endTransmission();
    }
}

float compasMeasure()
{
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);

    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */

    Serial.println("");
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
    float angle = atan2(event.magnetic.x, event.magnetic.y);
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.println("");

    if (angle < 0)
    {
        angle += 2 * PI;
    }

    return angle;
    /* Delay before the next sample */
    delay(50);
}

void setup()
{
#ifndef ESP8266
    while (!Serial)
        ; // will pause Zero, Leonardo, etc until serial console opens
#endif

    Serial.begin(9600);

    Wire.begin();

    Serial.println("Magnetometer Test");
    Serial.println("");

    // Bluetooth Setup Start
    
    // Create the BLE Device
    BLEDevice::init("ESP32");
  
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
  
    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);
  
    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                      );
  
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());
  
    // Start the service
    pService->start();
  
    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
    
    // Bluetooth Setup End

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

       destinationAngle = getBearing(startLat, startLong, endLat, endLong);
       Serial.println("Destination Angle: ");
       Serial.print(destinationAngle);
       compassAngle = compasMeasure();
       Serial.print("Compass Angle: ");
       Serial.print(compassAngle);
       vibrate();

       // Bluetooth Loop Start

        // notify changed value
        if (deviceConnected) {
            pCharacteristic->setValue((uint8_t*)&value, 4);
            pCharacteristic->notify();
            value++;
            delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        }
        
        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            delay(500); // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising(); // restart advertising
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }
        
        // connecting
        if (deviceConnected && !oldDeviceConnected) {
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
        }

       // Bluetooth Loop End
       
       delay(1000);

}
