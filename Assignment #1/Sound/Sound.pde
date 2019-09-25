
import processing.serial.*;
import processing.sound.*;
SoundFile file, file2, file3, file4, file5, file6, file7;

// Code written by Valentin Porcellini vp272

Serial myPort;  // Create object from Serial class
String myString = null;
int red, green, blue;



//System initilization Function
void setup() {
  size(640, 360); // Initilize the window dimensions
  background(255); // set background
  printArray(Serial.list());// Print All Serial port names 
  String portName = Serial.list()[3]; // Change the port names to your Teensy Port Name
  myPort = new Serial(this, portName, 9600); //Start the serial com
  
  file = new SoundFile(this, "musicalNote.wav");
  file2 = new SoundFile(this, "boing.wav");
  file3 = new SoundFile(this, "kick.wav");
  file4 = new SoundFile(this, "hey.wav");
  file5 = new SoundFile(this, "trombone.wav");
  file6 = new SoundFile(this, "beep.wav");
  file7 = new SoundFile(this, "error.wav");
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
  //System.out.println("list2 " + list[2]);
  //If the reading/capacticance is larger than 1000, then it is a touch .
  
  if(Integer.parseInt(list[0]) > 1000){
      red = 255;
      green = 0;
      blue = 0;
      //System.out.println(millis() - passedTime);
      //if (millis() - passedTime > 1000*file.duration()){
      //  passedTime = millis();
        //delay(1000);
        if (!file.isPlaying()){
          file.play();
        }
        
        
      //}
      
    }
        else if ( Integer.parseInt(list[1]) > 1000){
      red = 200;
      green = 0;
      blue = 255;
        if (!file2.isPlaying()){
          file2.play();
        }
        
               
    }
    else if ( Integer.parseInt(list[2]) > 1000){
      red = 0;
      green = 255;
      blue = 255;
      
        if (!file3.isPlaying()){
          file3.play();
        }
        }
         else if ( Integer.parseInt(list[3]) > 1000){
      red = 255;
      green = 255;
      blue = 0;
      
        if (!file4.isPlaying()){
          file4.play();
        }
        }
        else if ( Integer.parseInt(list[4]) > 1000){
      red = 0;
      green = 200;
      blue = 100;
      
        if (!file5.isPlaying()){
          file5.play();
        }
        }
        else if ( Integer.parseInt(list[5]) > 1000){
      red = 100;
      green = 0;
      blue = 255;
      
        if (!file6.isPlaying()){
          file6.play();
        }
        }
        else if (Integer.parseInt(list[6]) > 1000){
          //System.out.println(previous6);
          //previous6 = Integer.parseInt(list[6]);
          
      red = 50;
      green = 100;
      blue = 50;
      
        if (!file7.isPlaying()){
          file7.play();
        }
        }
    else{
      red = 0;
      green = 255;
      blue = 0;
      }
      }
