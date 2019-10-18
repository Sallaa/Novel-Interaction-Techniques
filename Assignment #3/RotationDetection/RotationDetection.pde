import processing.serial.*;

// Code written by Valentin Porcellini vp272

Serial myPort;  // Create object from Serial class
String myString = null;

float upDownThreshold = 4;
float leftRightThreshold = 4;
float rotation_Threshold = 3;// Define your own threshold
int timeOfLastMove = 0;

int red, green, blue;
String textDisplayed ="";

void setup() {
  size(640, 360);
  background(255);
  printArray(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);// 
  
}
void draw() {
  background(red, green, blue);
  textAlign(CENTER, CENTER);
  textSize(52);
  text(textDisplayed,320,180);
  updataSerial();
}
void updataSerial() {
  while(myPort.available() > 0){
    myString = myPort.readStringUntil(10);    // '\n'(ASCII=10) every number end flag
    //print(myString);
    if(myString!=null){
      analysisData(myString);
    }
  }
}

// Print the direction, change the color, change the text, return true if direction detected
boolean printDirection (float value, float threshold, String negativePrint, String positivePrint, int redValueNegative, int blueValueNegative, int greenValueNegative, int redValuePositive, int blueValuePositive, int greenValuePositive){
  if (Math.abs(value) >= threshold){
      if (value < 0){
        
        red = redValueNegative;
        green = greenValueNegative;
        blue = blueValueNegative;
        textDisplayed = negativePrint;
        
        println(negativePrint);
        
      } else {
        red = redValuePositive;
        green = greenValuePositive;
        blue = blueValuePositive;
        textDisplayed = positivePrint;
        
        println(positivePrint);      
      }
        timeOfLastMove = millis();
        return true;
      } else
        return false;
}

void analysisData(String myString){
  String[] list = split(myString.substring(0, myString.length()-2), ',');
  if(list.length == 6){
    float[] imuValue = new float[6]; // imuValue 0-6 : acclx, y, z, gyro x, y, z;
    for(int i = 0; i<6; i++){
      imuValue[i] = Float.parseFloat(list[i]);
    }
        
    
    // Implement your own judgement below
    //Hint 1: abs function to get absolute value
    //Hint 2: Record the last time the event was triggered
    
    // 
    if (millis() - timeOfLastMove > 300){
      
      // if / else statements to give priority
      
      if (printDirection(imuValue[3], rotation_Threshold, "AntiClockwise", "Clockwise", 255, 0, 0, 127,0 ,0)){
        
      } else if (printDirection(imuValue[2], upDownThreshold, "Up", "Down", 0, 255, 0, 100, 100, 100)){

      } else if (printDirection(imuValue[1], leftRightThreshold, "Left", "Right", 0, 0, 255, 0, 255, 255)){

      }
    } 
  }
}
