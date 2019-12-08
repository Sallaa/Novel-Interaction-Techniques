#include <Wire.h>
const int pinVibrator100 = 13;
char c;
int x;
void setup()
{
  pinMode(pinVibrator100, OUTPUT);
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

}

void loop()
{
  if (c == 'r') {
    if (x == 0) {
      digitalWrite(pinVibrator100, LOW);
    } else if (x == 1) {
      digitalWrite(pinVibrator100, HIGH);
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) // loop through all but the last
  {
    c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

}
