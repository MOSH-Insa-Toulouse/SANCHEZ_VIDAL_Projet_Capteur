
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include<SPI.h>
#include<Wire.h>
#include <stdio.h>
#include <SoftwareSerial.h>
#define rxPin 10 //Broche 11 en tant que RX, � raccorder sur TX du HC-05
#define txPin 11 //Broche 10 en tant que RX, � raccorder sur TX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //D�finition du software serial

int sensorPin = A0; // analog read of the sensor
int sensorValue = 0;
int newSensor =0;
static int VCC=5;
// definition of resistors' values
static long R5= 1e4;
static long R1= 1e5;
static long R3= 1e5;
static int  Rcal= 1000;
float res =0;
byte val;// created variable for 1024-255 conversion

//encoder pins and variables
boolean menu = true;//rotatory encoder menu
#define PinCLK 4 //CLK OUTPUT
#define PinDT 3 //DT OUTPUT
#define PinSW 2 // switch connections
volatile boolean TurnDetected;
volatile boolean up;
static long virtualPosition=0;    // without STATIC it does not count correctly!!!
static long virtualPositionG=0; 

//OLED definitions
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELAY 2
#if(SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// rotatory encoder turn detection
void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
 if (digitalRead(PinCLK))
   up = digitalRead(PinDT);
 else
   up = !digitalRead(PinDT);
 TurnDetected = true;
}

 
void setup() {
  time1=millis();
  // Bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  //data communication   
  mySerial.begin(baudrate);
  Serial.begin(baudrate);
  
  //pins for rotatory encoder
 pinMode(PinCLK,INPUT);
 pinMode(PinDT,INPUT);  
 pinMode(PinSW,INPUT);
 attachInterrupt (0,isr,RISING); 

//OLED initialization 
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}


void loop() {
  
  //Bluetooth
  char someChar[32]; 
  sensorValue = analogRead(sensorPin);
  //offset and gains determined via the rotatory encoder
  if (virtualPositionG >= 20){
    virtualPositionG=20; }
  else{
    if (virtualPositionG<=-1)
     virtualPositionG=1 ;
    }
  if (virtualPosition >=40){
    virtualPosition=40; }
  else{
    if (virtualPosition<=-40)
     virtualPosition=40 ;
    }
   // Gain and offset calculus
  sensorValue =   sensorValue * (1+virtualPositionG*0.1); 
  sensorValue = sensorValue + virtualPosition*10;
  // gain and offset limitations
  if (sensorValue >= 1020){
    sensorValue=1020; }
  else{
    if (sensorValue<=1)
     sensorValue=1 ;
    }
  //conversion 1024-255
  byte val =map (sensorValue,0,1024,0,255); 
  // data sending to serial port
  Serial.print(val);
  Serial.print(" \n");
  // data sending via Bluetooth
  mySerial.println(val);
  mySerial.print(" \n");
  // delay between two analog readings
  delay(50); 
  // data receiving
  while (mySerial.available()) {
  Serial.print((char)mySerial.read());
  }

  //Encodeur Rotatoire
 
 if (!(digitalRead(PinSW))) {      // check if pushbutton is pressed
   //delay between menu action, works as a debounce function
   if ((time1+3000)<(millis())){ 
   menu= !menu;
   time1=millis();
   }
 }
   
   if (TurnDetected)  { // do this only if rotation was detected

     if (menu){ 
      //Offset value change
      if (up  )
        virtualPosition++;
      else
        virtualPosition--;
      
      }
      else {
        //Gain value change
        if (up)
        virtualPositionG++;
      else
        virtualPositionG--;
      }
    TurnDetected = false; // do NOT repeat IF loop until new rotation detected
    //Delay work as a debounce and prevents changing too fast
    delay(100);
    
   }
   
  //OLED display value change
  //Settings Definition
  int num = 1.5;
  display.setTextSize(num);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor (XPOS, YPOS );
  display.print("R= ");
  // analog sensor values conversion into resistance
  res= ( 1 + (R3/Rcal));
  res= res * R1* VCC;
  res= res /((sensorValue*5)*1000000/1024);
  res= res -(R1 +  R5 )/1000000;
  // If the value is superior to 5 Volts the value cannot be treated and it sends 0 OHMS and 5 Volts
  if (val > 250) {
  display.println("0 OMHS");  
  display.print("U= ");
  display.println("5 V");
 }
 else{
  //Oled display of the reistance and tension values
  display.print(res);
  display.println(".e6 OMHS");
  
  display.print("U= ");   
  display.print(float(val)*5.0/255.0);
  display.println(" V");
   
 }
   //Oled display of the offset and Gain(in percentage) values
   //A moving arrow represents the menu cursor
  display.print("Offset= ");
  if (menu){
  display.print(virtualPosition);  
  display.println("<-");
  }
  else {
  display.println(virtualPosition); 
  }
  display.print("Gain= ");
  display.print(int ((virtualPositionG*0.1 + 1)*100) );
  display.print("%");
  if (!menu){
  display.print("<-");
  }
  display.display();
 }
