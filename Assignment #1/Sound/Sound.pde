
import processing.serial.*;
import processing.sound.*;
SoundFile file0, file1, file2, file3, file4, file5, file6;

// Code written by Valentin Porcellini vp272

Serial myPort;  // Create object from Serial class
String myString = null;
int red, green, blue;

int lastValue0, lastValue1, lastValue2, lastValue3, lastValue4, lastValue5, lastValue6 = 0;


//System initilization Function
void setup() {
  size(640, 360); // Initilize the window dimensions
  background(255); // set background
  printArray(Serial.list());// Print All Serial port names 
  String portName = Serial.list()[3]; // Change the port names to your Teensy Port Name
  myPort = new Serial(this, portName, 9600); //Start the serial com
  
  file0 = new SoundFile(this, "musicalNote.wav");
  file1 = new SoundFile(this, "boing.wav");
  file2 = new SoundFile(this, "kick.wav");
  file3 = new SoundFile(this, "hey.wav");
  file4 = new SoundFile(this, "trombone.wav");
  file5 = new SoundFile(this, "beep.wav");
  file6 = new SoundFile(this, "error.wav");
}

// Update function
void draw() {
  background(red, green, blue);
  updataSerial();
}

// Read data from the serial port 
void updataSerial() {
  while(myPort.available() > 0){
    myString = myPort.readStringUntil(10);    // '\n'(ASCII=10) every number end flag
    if(myString!=null){
      detectTouch();
    }
  }
}
 int passedTime = millis();
// Convert the serial readings into integars. and detect touches using a threthold
void detectTouch(){
  String[] list = split(myString.substring(0, myString.length()-2), ',');
  //If the reading/capacticance is larger than 1000, then it is a touch .
  
  if(Integer.parseInt(list[0]) > 1000){
      red = 255;
      green = 0;
      blue = 0;
    
      if (!file0.isPlaying() && lastValue0 < 1000){
          file0.play();
          lastValue0 = Integer.parseInt(list[0]);
      }      
    }
    else if ( Integer.parseInt(list[1]) > 1000){
      red = 200;
      green = 0;
      blue = 255;
      if (!file1.isPlaying() && lastValue1 < 1000){
          file1.play();
          lastValue1 = Integer.parseInt(list[1]);
      }           
    }
    else if ( Integer.parseInt(list[2]) > 1000){
      red = 0;
      green = 255;
      blue = 255;
      
        if (!file2.isPlaying()  && lastValue2 < 1000){
          file2.play();
          lastValue2 = Integer.parseInt(list[2]);
        }
        }
    else if ( Integer.parseInt(list[3]) > 1000){
      red = 255;
      green = 255;
      blue = 0;
      
      if (!file3.isPlaying()  && lastValue3 < 1000){
          file3.play();
          lastValue3 = Integer.parseInt(list[3]);
      }
    }
    else if ( Integer.parseInt(list[4]) > 1000){
      red = 0;
      green = 200;
      blue = 100;
      
      if (!file4.isPlaying()  && lastValue4 < 1000){
          file4.play();
          lastValue4 = Integer.parseInt(list[4]);
      }
    }
    else if ( Integer.parseInt(list[5]) > 1000){
      red = 100;
      green = 0;
      blue = 255;
      
      if (!file5.isPlaying()  && lastValue5 < 1000){
          file5.play();
          lastValue5 = Integer.parseInt(list[5]);
      }
    }
    else if (Integer.parseInt(list[6]) > 1000){
          
      red = 50;
      green = 100;
      blue = 50;

      if (!file6.isPlaying()  && lastValue6 < 1000){
          file6.play();
          lastValue6 = Integer.parseInt(list[6]);
      }
    }
    else {
      red = 0;
      green = 255;
      blue = 0;
      lastValue0 = Integer.parseInt(list[0]);
      lastValue1 = Integer.parseInt(list[1]);
      lastValue2 = Integer.parseInt(list[2]);
      lastValue3 = Integer.parseInt(list[3]);
      lastValue4 = Integer.parseInt(list[4]);
      lastValue5 = Integer.parseInt(list[5]);
      lastValue6 = Integer.parseInt(list[6]);
      }
  }
