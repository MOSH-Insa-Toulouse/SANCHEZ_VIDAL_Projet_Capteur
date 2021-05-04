# Projet Capteur Graphite SANCHEZ VIDAL

Ce Git contient les ressources nécessaire pour le projet 2021 qui s'inscrit dans l'UF *"Du capteur au banc de Test"* de l'INSA de Toulouse, 4ème année de Génie Physique. De la **conception** jusqu'à la **caractérisation** en passant par le **codage**,  toutes les étapes nécessaires à la fabrication d'une jauge de contrainte à base d'une piste de graphite sont détaillées à continuation.

## Sommaire

---

- [Livrables](##Section1)
- [Description](##Section2)
- [Arduino](##Arduino)
  - [Bibliothèques](###Bibliotèques)
  - [Notre code Arduino](###Notre-Code-Arduino)
- [KICAD](##SectionKICAD)
  - [Placement des composants](###Placement-des-composants)
  - [Sockets KICAD pour les différents composants](###Sockets-KICAD-pour-les-différents-composants)
- [Application Android apk](##Application-Android-apk)
  - [MIT App Inventor](###MIT-App-Inventor)
  - [Notre application](###Notre-Application)
- [Banc de test](##Banc-de-test)
- [Pistes de réflexion](##Pistes-de-réflexion)
- [Information sur les auteurs](##Information-sur-les-auteurs)

## Livrables <a id="Section1"></a>

---

- Shield PCB
- Code Arduino permettant la mesure de la tension (conversion en Ohm), ainsi que le contrôle des fonctionnalités BT, OLED et capteur rotatoire
- Application android APK réalisé avec MIT APP INVENTOR
- Protocole de mesure et le programme Arduino pour le banc de test
- Datasheet du capteur de déformation/ jauge de contrainte

## Description <a id="Section2"></a>

---
Ce projet a pour but le développement d'un **capteur de déformation mécanique** à base de matériaux peu onéreux ainsi qu'écoresponsable. Le principe physique qui fait notre projet possible est le transport des électrons entre les nanoparticules de graphite, qui lui même, est régi par l'effet tunnel. Ce capteur exploite le lien entre la conduction électrique des électrons et l'espace moyen entre les particules d'un réseau granuleux. En effet, l'éloignement et le rapprochement du réseau induit par un effort mécanique fait varier la **conductivité électrique** de la piste de graphite, puisque la distance inter-particule a été affectée. La déformation de cette dernière induit une variation de résistance électrique réversible et exploitable que nous allons utiliser pour créer une jauge de contrainte. Le **conditionneur** de notre projet est un shield comportant un circuit électrique d'amplificateur transimpédance, ainsi que des interface pour un module bluetooth, un capteur rotatoire, une écran OLED et bien sur, l'entrée analogique de la jauge de contrainte. Ce shield a été déssiné sur **KICAD** et des simulations du circuit transimpédance ont été faits sur **LT SPICE**. Le code principal à été réalisé en C++ sur l'**IDE Arduino UNO**. Finalement une application android a été développé sur **MIT App Inventor** pour communiquer et recevoir des données à distance, via bluetooth. Un banc de test a été réalisé avec un rapporteur et les pistes de graphites dessinées sur des feuilles de papier. Un protocole de mesure sera spécifié pour exposer commment nous avons exploité et étudié la réponse de déformation de différents capteurs.

### Code Arduino

- Mesure la tension de sortie du circuit transimpédance
- Calcule la résistance du capteur de graphite connaissant la fonction de transfert du conditionneur
- Envoie la mesure de la tension via Bluetooth sur 1 octet
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

[_Pour télécharger le code_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/99f3a97303d053d3fa053065e3f0f4b805357511/Code%20Arduino/programme_final.ino)

## KICAD <a id="SectionKICAD"></a>

---
Le projet sur le logiciel KICAD permet de:

- Manipuler le PCB sous forme de Shield pour carte Arduino UNO
- Interface du capteur graphite via un circuit transimpédance
- Interface pour un module Bluetooth
- Interface pour un écran oled
- Interface pour un encodeur rotatoire

#### _PCB avec les composants à souder_

![PCB](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/3e0482a74e4f8a15e2c62660e452ac9b22093a1d/Media%20README/WhatsApp%20Image%202021-05-04%20at%204.30.34%20PM.png "imaage PCB avec composants")

Projet KICAD avec fichiers formats pdf et format GERBER à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/tree/main/KICAD)

Fichiers créés pour la conception et la simulation du circuit électrique sur le PCB LT SPICE à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/tree/main/Conception%20PCB)

#### _Apperçu du Shield PCB (empreinte GERBER)_

![PCB pdf](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/pdf%20overview.png "Apperçu du shield PCB en pdf")

### Placement des composants

![alt text](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/emplacement%20des%20composants.png "Placement des composants")

### Sockets KICAD pour les différents composants

- Amplificateur transimpédance

![alt text](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/ampli%20trans.png "Amplificateur trnaimpédance")

- Module Blueetooth

![alt text](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/module%20bluetooth.png "module bluetooth")

- Encodeur rotatoire

![alt text](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/encodeur%20rotatoire.png "encodeur rotqtoire")

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

Notre code de l'application (format .aia) est à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Application%20apk/Appli_Capteur%20(2).aia)

Notre code de l'application (format .apk) est à télécharger [_ici_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Application%20apk/Appli_Capteur.apk)

#### _Apperçu de l'application une fois téléchargée_

![app](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/WhatsApp%20Image%202021-05-04%20at%204.26.40%20PM.png "apperçu de l'application")

## Banc de test

---
Nous avons réalisé un banc de test très "low cost" avec lequel nous avons fait toutes les mesures pour la caractérisation des différents capteurs. Les résultats sont dans notre [_datasheet_](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/raw/main/Datasheet.docx)

Protocole du banc de test/ utilisation optimale et conseillée du capteur (90º/-90º):
Au delà de ces angles la jauge en papier s'abîme et la durée de vie de cette dernière se voit fortement raccourcie.
Fixer les deux extrémités de la jauge en papier a des surfaces rigides et de longueur suffisante pour permettre de lire les angles sur les notations du support (rapporteur dans ce cas).
Pour ce faire nous devons conserver un carré d'environ 1cm de côté comme prolongation de la jauge. Sur ce carré il n'y a pas de piste en graphite.
Fixer la base de la jauge perpendiculairement à l'axe des 0º et la fin de la zone d'accroche des pinces en cuivre à 5mm du centre du rapporteur.
Déplacer l'autre extremité par des pas de 10º en balayant de 90º jusqu'à -90º par rapport à l'axe neutre de la base.

#### _Image du banc de test_

![banc de test](https://github.com/MOSH-Insa-Toulouse/SANCHEZ_VIDAL_Projet_Capteur/blob/77da3085585d26435e430bd8d40fdfa598627142/Media%20README/WhatsApp%20Image%202021-05-04%20at%2011.42.14%20AM.png "banc de test")

## Pistes de réflexion

---

- Améliorer le banc de test parcequ'il est susceptible d'induire plusieurs erreurs aléatoires lors des mesures.

- Implementer la gestion de l'encodeur rotatoire pour le calcule de l'offset et le gain numérique, ains qu'un deboucing (soit analogique ou numérique)

- Tester et caractériser plus de capteurs (plus de pistes en graphites avec des différents types de crayons à papier)

## Information sur les auteurs

---
Santiago SANCHEZ & Marc VIDAL

- sanchezs@etud.insa-toulouse.fr
- vidalbar@etud.insa-toulouse.fr

Étudiants à l'INSA de Toulouse en 4ème année Génie Physique
