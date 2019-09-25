#include <CapacitiveSensor.h>
/*
* CapitiveSense Library Demo Sketch
* Paul Badger 2008
* Uses a high value resistor e.g. 10M between send pin and receive pin
* Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
* Receive pin is the sensor pin - try different amounts of foil/metal on this pin */
CapacitiveSensor cs_0_1=CapacitiveSensor(0,1); //1-10M resistor betweenpins 0&1,pin 1 is sensorpin
CapacitiveSensor cs_0_2=CapacitiveSensor(0,2);//1-10M resistor between pins 0&2,pin 2 is sensorpin
CapacitiveSensor cs_0_23=CapacitiveSensor(0,23);
CapacitiveSensor cs_0_22=CapacitiveSensor(0,22);
CapacitiveSensor cs_0_19=CapacitiveSensor(0,19);
CapacitiveSensor cs_0_18=CapacitiveSensor(0,18);
CapacitiveSensor cs_0_17=CapacitiveSensor(0,17);
void setup() {
  Serial.begin(9600);
  }
void loop() {
  long capacitance0 = cs_0_1.capacitiveSensor(30); // Samples
  long capacitance1 = cs_0_2.capacitiveSensor(30); // Samples
  long capacitance2 = cs_0_23.capacitiveSensor(30);
  long capacitance3 = cs_0_22.capacitiveSensor(30);
  long capacitance4 = cs_0_19.capacitiveSensor(30);
  long capacitance5 = cs_0_18.capacitiveSensor(30);
  long capacitance6 = cs_0_17.capacitiveSensor(30);
  

  Serial.print(capacitance0); // print sensor output 0
  Serial.print(",");
  Serial.print(capacitance1); // print sensor output 1
  Serial.print(",");
  Serial.print(capacitance2);
  Serial.print(",");
  Serial.print(capacitance3);
  Serial.print(",");
  Serial.print(capacitance4);
  Serial.print(",");
  Serial.print(capacitance5);
  Serial.print(",");
  Serial.println(capacitance6);
  delay(10); // arbitrary delay to limit data to serial port
}
