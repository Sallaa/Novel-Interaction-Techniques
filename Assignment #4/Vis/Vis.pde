import java.util.*;
import java.lang.*;
import processing.serial.*;
import grafica.*;

// code written by Valentin Porcellini vp272

Serial myPort;  // Create object from Serial class
int locationPlotIMU_X = 100;// The position of the plot in the window
int locationPlotIMU_Y = 100;

int widthPlotIMU = 800;// size of the plot 
int heigthPlotIMU = 600;

int absRangeofY = 15;// The value range of the Y Axis in the plot
int numAxis = 6; 
int winSize = 50;// How many data points are saved in the data array and draw on the screen
int counter = 0;

float upDownThreshold = 4;
float leftRightThreshold = 4;
float rotation_Threshold = 3;// Define your own threshold

//int counterfilter = 0;

GPlot plotIMU[] = new GPlot[numAxis];
long plotIMUIndex[] = new long[numAxis]; // Save the index for GPoints in GPlot 
String myString = null;

//Save the data of the current window in multiple axis
ArrayList<ArrayList> IMUDataArray = new ArrayList();
ArrayList<Float> averageFilter1 = new ArrayList();
ArrayList<Float> averageFilter2 = new ArrayList();
ArrayList<Float> averageFilter3 = new ArrayList();

void setup() {
  size(1200, 900);
  background(255);
  printArray(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);// 
  
  // Initialize Plot Setting 
  plotInitialization();
  
  for(int i=0; i<numAxis; ++i){
   IMUDataArray.add(new ArrayList());
  }
}

void draw() {
  //background(red, green, blue);
  updataSerial();
  draw_plot();
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

void analysisData(String myString){
  String[] list = split(myString.substring(0, myString.length()-2), ',');
  if(list.length == 6){
    float[] imuValue = new float[6]; // imuValue 0-6 : acclx, y, z, gyro x, y, z;
    for(int i = 0; i<6; i++){
      imuValue[i] = Float.parseFloat(list[i]);
       
    }

 // Add the gyroX data into dataArray[0]
    IMUDataArray.get(0).add(imuValue[3]); // gyro x
    IMUDataArray.get(1).add(imuValue[1]); // left right
    IMUDataArray.get(2).add(imuValue[2]); // up down
    IMUDataArray.get(3).add(imuValue[0]);
    IMUDataArray.get(4).add(imuValue[4]);
    IMUDataArray.get(5).add(imuValue[5]);

    averageFilter1.add(imuValue[3]);
    averageFilter2.add(imuValue[1]);
    averageFilter3.add(imuValue[2]);
    counter++;
    

      if (averageFilter1.size() > 10){
         averageFilter1.remove(0);
         averageFilter2.remove(0);
         averageFilter3.remove(0);
      }
     
      // filters  
      setAvgMovingFilter10(averageFilter1,IMUDataArray.get(0));
      setAvgMovingFilter10(averageFilter2,IMUDataArray.get(1));
      setAvgMovingFilter10(averageFilter3,IMUDataArray.get(2));
    

      
    //System.out.println("counter:" + counter1);
    counter = counter % 26;
    
    // Maintain the lenght of the array, If the size of array is larger than winSize, remove the oldest data.
    if(IMUDataArray.get(0).size()> winSize){
      IMUDataArray.get(0).remove(0);
    }
    
    if(IMUDataArray.get(1).size()> winSize){
      IMUDataArray.get(1).remove(0);
    }
    
    if(IMUDataArray.get(2).size()> winSize){
      IMUDataArray.get(2).remove(0);
    }
    
    if(IMUDataArray.get(3).size()> winSize){
      IMUDataArray.get(3).remove(0);
    }
    
    if(IMUDataArray.get(4).size()> winSize){
      IMUDataArray.get(4).remove(0);
    }
    
    if(IMUDataArray.get(5).size()> winSize){
      IMUDataArray.get(5).remove(0);
    }
    
    // process the windows with 50% overlap
    if (counter == 25){
      float maxClockwise = maxValue(IMUDataArray.get(0));
      float maxLeftRight = maxValue(IMUDataArray.get(1));
      float maxUpDown = maxValue(IMUDataArray.get(2));
      
      if (maxClockwise > rotation_Threshold){
        for(int i = 0; i< IMUDataArray.get(0).size(); i++){
          float currentValue = (float) IMUDataArray.get(0).get(i);
          float absoluteValue = Math.abs(currentValue);
          
          if (absoluteValue ==  maxClockwise){
              if (currentValue < 0){
                plotIMU[0].setTitleText("IMU Data AntiClockwise");
              } else {
                plotIMU[0].setTitleText("IMU Data Clockwise");
              }
          }
        }
       
      } else if (maxUpDown > upDownThreshold){
        //System.out.printl("max is " + maxUpDown);
         for(int i = 0; i< IMUDataArray.get(2).size(); i++){
          float currentValue = (float) IMUDataArray.get(2).get(i);
          float absoluteValue = Math.abs(currentValue);
          
          if (absoluteValue ==  maxUpDown){
              if (currentValue < 0){
                plotIMU[0].setTitleText("IMU Data up");
              } else {
                plotIMU[0].setTitleText("IMU Data down");
              }
          }
        }
        
       //plotIMU[0].setTitleText("IMU Data Up");
      } else if (maxLeftRight > leftRightThreshold){
       //plotIMU[0].setTitleText("IMU Data Right");
       
       for(int i = 0; i< IMUDataArray.get(1).size(); i++){
          float currentValue = (float) IMUDataArray.get(1).get(i);
          float absoluteValue = Math.abs(currentValue);
          
          if (absoluteValue ==  maxLeftRight){
              if (currentValue < 0){
                plotIMU[0].setTitleText("IMU Data Left");
              } else {
                plotIMU[0].setTitleText("IMU Data Right");
              }
          }
        }
       
      }
   
      
  }
  }
}


void setAvgMovingFilter10(ArrayList<Float> last10values, ArrayList<Float> dataList){
  float sum = 0;     
  for (int i = 0; i < last10values.size(); i++){
         sum += last10values.get(i);
       }
   float mean = sum / last10values.size();
   dataList.set(dataList.size() - 1, mean);
  return;
}

// get maxvalue of the arraylist
float maxValue(ArrayList<Float> list){
  float maxValue = 0;
  
  for (int i = 0; i < list.size(); i++){
    float absoluteValue = Math.abs(list.get(i));
    if ( absoluteValue > maxValue){
      maxValue = absoluteValue;
    }     
  }
  return maxValue;
}

// Initialization data display
void plotInitialization(){
    // initialization of plotIMU 
    for (int i= 0; i < numAxis; ++i){
        plotIMU[i]= new GPlot(this);
        plotIMU[i].setPos(locationPlotIMU_X, locationPlotIMU_Y);
        plotIMU[i].setDim(widthPlotIMU, heigthPlotIMU);
    }
    plotIMU[0].setTitleText("IMU Data");
    plotIMU[0].getXAxis().setAxisLabelText("Time (t)");
    plotIMU[0].getYAxis().setAxisLabelText("y axis");
}

void draw_plot(){
  // cover the interface
    background(color(150));
    // Display plotIMU 
    // add points
    ArrayList<ArrayList> copy = new ArrayList();
    copy.addAll(IMUDataArray); //<>//
    
    
       for (int i= 0; i < numAxis; ++i){
         for (int j=0; j < copy.get(i).size(); j++){
           plotIMU[i].addPoint(new GPoint(plotIMUIndex[i]++, (Float)copy.get(i).get(j)));
          //System.out.println("size2:" + IMUDataArray.get(0).size());
         }
          
        }
        copy.clear();
   
    
    // plot background and axis 
    plotIMU[0].beginDraw();
    plotIMU[0].drawBackground();
    plotIMU[0].drawXAxis();
    plotIMU[0].drawYAxis();
    plotIMU[0].drawTitle();
    plotIMU[0].endDraw();
    
    // plot lines
    for (int i= 0; i < numAxis; ++i){   
      plotIMU[i].beginDraw();
      plotIMU[i].setXLim(plotIMUIndex[i]-winSize, plotIMUIndex[i]);
      plotIMU[i].setYLim(-absRangeofY, absRangeofY);
      plotIMU[i].drawLines();
      plotIMU[i].endDraw();
    }
  
}
