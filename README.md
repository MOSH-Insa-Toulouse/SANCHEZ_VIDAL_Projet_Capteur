# Projet Capteur Graphite SANCHEZ VIDAL

Ce Git contient les ressources nécessaire pour le projet 2021 qui s'inscrit dans l'UF *"Du capteur au banc de Test"* de l'INSA de Toulouse, 4ème année de Génie Physique. De la **conception** jusqu'à la **caractérisation** passant par le **codage**,  toutes les étapes nécessaires d'une jauge de contrainte à base d'une piste de graphite sont détaillé à continuation.

## Sommaire

---

- [Livrables](##Livrables)
- [Description](##Description)
- [Arduino](##Arduino)
  - [Bibliothèques](###Bibliotèques)
  - [Notre code Arduino](###Notre-Code-Arduino)
- [KICAD](##Projet-KICAD)
  - [Placement des composants](###Placement-des-composants)
  - [Sockets KICAD pour les différents composants](###Sockets-KICAD-pour-les-différents-composants)
- [Application Android apk](##Application-Android-apk)
  - [MIT App Inventor](###MIT-App-Inventor)
  - [Notre application](###Notre-Application)
- [Banc de test](##Banc-de-test)
- [Pistes de réflexion](##Pistes-de-réflexion)
- [Information sur les auteurs](##Information-sur-les-auteurs)

## Livrables

---

- Shield PCB
- Code Arduino permettant la mesure de la resistance R et la tension, ainsi que le contrôle des fonctionnalités BT, OLED et capteur rotatoire
- Application android APK réalisé avec MIT APP INVENTOR
- Protocole de mesure et le programme Arduino pour le banc de test
- Datasheet du capteur de déformation/ jauge de contrainte

## Description

---
Ce projet a pour but le développement d'un **capteur de déformation mécanique** à base de matériaux peut onéreux ainsi que écoresponsable. Le principe physique qui fait notre projet possible le transport des électrons entre les nanoparticules de graphite, qui lui même, est régi par l'effet tunnel. Ce capteur exploite le lien entre la conduction électrique des électrons et l'espace moyen entre les particules d'un réseau granuleux. En effet, l'éloignement et le rapprochement du réseau induit par un stress mécanique fait varier la **conductivité électrique** de la piste de graphite, puisque la distance inter-particule a été affectée. La déformation de cette dernière induit une variation de résistance électrique réversible et exploitable pour créer une jauge de contrainte. Le **conditionneur** de notre projet est un shield comportant un circuit électrique d'amplificateur transimpédance, ainsi que des interface pour un module bluetooth, un capteur rotatoire, une écran OLED et bien sur, l'entrée analogique de la jauge de contrainte. Ce shield a été déssiné sur **KICAD** et des simulations du circuit transimpédance ont été faits sur **LT SPICE**. Le code principala été réalisé en C++ sur l'**IDE Arduino UNO**. Finalement une application android a été développé sur **MIT App Inventor** pour communiquer et recevoir des données à distance, via bluetooth. Un banc de test a été réalisé avec un rapporteur et les pistes de graphites dessinées sur des feuilles de papier. Un protocole de mesure sera spécifié pour exposer commment nous avons exploité et étudié la réponse de déformation de différents capteurs.

### Code Arduino

- Mesure la tension de sortie du circuit transimpédance
- Calcule la résistance du capteur de graphite connaissant la fonction de transfert du conditionneur
- Envoie la mesure de la tension et de la resistance via Bluetooth sur 1 octet
- Affiche les mesures de tension, de résistance et possède un menu pour choisir entre un offset et un gain numérique sur un écran OLED
- Gère l'encodeur rotatoire

### KICAD

- PCB sous forme de Shield pour carte Arduino UNO
- Interface du capteur graphite via un circuit transimpédance
- Interface pour un module Bluetooth
- Interface pour un écran oled
- Interface pour un encodeur rotatoire

### Application apk

- Récupère la mesure de tension
- Calcule et affiche la valeur de la résistance du capteur de graphite connaissant la fonction de transfert du conditionneur (en MΩ)
- Trace un graphique qui décrit l'évolution de la mesure de tension au cours du temps
- Mise à l'échelle automatique de l'ordonnée du graphique

## Arduino

---

### Bibliotèques

- Installer la bibliothèque ["Adafruit GFX Library"](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Librairies%20Arduino/Adafruit-GFX-Library-master.zip)
- Installer la bibliothèque ["Adafruit SSD1306"](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Librairies%20Arduino/Adafruit_BusIO-master.zip)
- Installer la bibliothèque ["Adafruit BusIOa"](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Librairies%20Arduino/Adafruit_SSD1306-master.zip)


### Notre code arduino

Notre code Arduino permet de:

- Mesure la tension de sortie du circuit transimpédance
- Calcule la résistance du capteur de graphite connaissant la fonction de transfert du conditionneur
- Envoie la mesure de la tension et de la resistance via Bluetooth sur 1 octet
- Affiche les mesures de tension, de résistance et possède un menu pour choisir entre un offset et un gain numérique sur un écran OLED
- Gère l'encodeur rotatoire

[_Pour télécharger le code_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/tree/main/Code%20Arduino)

```C++
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
#define rxPin 10 //Broche 11 en tant que RX, à raccorder sur TX du HC-05
#define txPin 11 //Broche 10 en tant que RX, à raccorder sur TX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //Definition du software serial

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
  //Oled display of the reistance 
and tension values
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
 ```

## Projet KICAD

---
Le projet sur le logiciel KICAD permet de:

- Manipuler le PCB sous forme de Shield pour carte Arduino UNO
- Interface du capteur graphite via un circuit transimpédance
- Interface pour un module Bluetooth
- Interface pour un écran oled
- Interface pour un encodeur rotatoire

_PCB avec les composants à souder_

![PCB](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/WhatsApp%20Image%202021-05-04%20at%204.30.34%20PM.png?token=ASUGY4ZMZM3VWFMAP3FIMOLASFQT6 "imaage PCB avec composants")

Projet KICAD avec fichiers formats pdf et format GERBER à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/tree/main/KICAD)

Fichiers créés pour la conception et la simulation du circuit électrique sur le PCB LT SPICE à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/tree/main/Code%20Arduino)

_Apperçu du Shield PCB (empreinte GERBER)_

![PCB pdf](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/pdf%20overview.png?token=ASUGY4Z4JQSBNER7TEYWTMLASFQZS "Apperçu du shield PCB en pdf")

### Placement des composants

![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/emplacement%20des%20composants.png?token=ASUGY47PXMTNVC5WTIEAAITASFQ3C "Placement des composants")

### Sockets KICAD pour les différents composants

- Amplificateur transimpédance

![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/ampli%20trans.png?token=ASUGY44XWCJSPJQ3XEAJQALASFQ4A "Amplificateur trnaimpédance")

- Module Blueetooth

![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/module%20bluetooth.png?token=ASUGY4YGH65H2DKNWMGS2E3ASFQ5E "module bluetooth")

- Encodeur rotatoire

![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/encodeur%20rotatoire.png?token=ASUGY47GIK6ORHWN75S6WCTASFQ6I "encodeur rotqtoire")

## Application Android apk

---
L'application android faite sur MIT App Inventor sert à:

- Récupère la mesure de tension
- Calcule et affiche la valeur de la résistance du capteur de graphite connaissant la fonction de transfert du conditionneur (en MΩ)
- Trace un graphique qui décrit l'évolution de la mesure de tension au cours du temps
- Mise à l'échelle automatique de l'ordonnée du graphique

### MIT App inventor

Le site internet de création d'application [_MIT App Inventor_](https://appinventor.mit.edu/)

### Notre application

Notre code de l'application (format .aia) est à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/main/Application%20apk/Appli_Capteur%20(2).aia)

Notre code de l'application (format .apk) est à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/main/Application%20apk/Appli_Capteur.apk)

_Apperçu de l'application_
![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/WhatsApp%20Image%202021-05-04%20at%204.26.40%20PM.png?token=ASUGY45VD2Q6TI62TDKOWPTASFQTW "apperçu de l'application")

## Banc de test

---
Nous avons réalisé un banc de test très "low cost" avec lequel nous avons fait toutes les mesures pour la caractérisation des différents capteurs. Les résultats sont dans notre [_datasheet_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/main/Datasheet.docx)

Protocole du banc de test/ utilisation optimale et conseillée du capteur (90º/-90º):
Fixer les deux extrémités de la jauge en papier a des surfaces rigides et de longueur suffisante pour permettre de lire les angles sur les notations du support (rapporteur dans ce cas).
Pour ce faire nous devons conserver un carré d'environ 1cm de côté comme prolongation de la jauge. Sur ce carré il n'y a pas de piste en graphite.
Fixer la base de la jauge perpendiculairement à l'axe des 0º et la fin de la zone d'accroche des pinces en cuivre à 5mm du centre du rapporteur.
Déplacer l'autre extremité par des pas de 10º en balayant de 90º jusqu'à -90º par rapport à l'axe neutre de la base.

_Image du banc de test_
![alt text](https://raw.githubusercontent.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/main/Media%20README/WhatsApp%20Image%202021-05-04%20at%2011.42.14%20AM.png?token=ASUGY42A42PJPPHJBPNJW4TASFQTQ "banc de test")

## Pistes de réflexion

---

- Améliorer le banc de test parcequ'il est susceptibles à  induire plusieurs erreurs aléatoires lors des mesures.

- Implementer la gestion de l'encodeur rotatoire pour le calcule de l'offset et le gain numérique, ains qu'un deboucing (soit analogique ou numérique)

- Tester et caractériser plus de capteurs (plus de pistes en graphites avec des différents types de crayons à papier)

## Information sur les auteurs

---
Santiago SANCHEZ & Marc VIDAL

- sanchezs@etud.insa-toulouse.fr
- vidalbar@etud.insa-toulouse.fr

Étudiants à l'INSA de Toulouse en 4ème année Génie Physique
