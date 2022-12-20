#include <Arduino.h>

#define LED1PIN 3 //Jeg definere her LED1 på pin 3
#define LED2PIN 5 //Jeg definere her LED2 på pin 4

void setup() {
  pinMode(LED1PIN, OUTPUT); //Her defineres at det skal være et OUTPUT
  pinMode(LED2PIN, OUTPUT); //Her defineres at det skal være et OUTPUT
  Serial.begin(115200); //starter serial
}

void loop() {
  if (Serial.available() > 0) { //Når serialporten er aktiv kan scriptet begynde

    String command = Serial.readString(); //Her læser jeg inputtet fra Serial monitoren og lagre det i en variabel "Command"

    // Tjek for LED1ON-kommando
    if (command == "LED1ON") { // Jeg definere hermed at hvis inputtet fra serial monitoren er "LED1ON", skal denne sætning køres.
      digitalWrite(LED1PIN, HIGH); //Her tænder jeg LED'en på pin 3
    }
    // Tjek for LED1OFF-kommando
    else if (command == "LED1OFF") {
      digitalWrite(LED1PIN, LOW);
    }
    // Tjek for LED2ON-kommando
    else if (command == "LED2ON") {
      digitalWrite(LED2PIN, HIGH);
    }
    // Tjek for LED2OFF-kommando
    else if (command == "LED2OFF") {
      digitalWrite(LED2PIN, LOW);
    }
    if(command == "A") { // Jeg definere hermed at hvis inputtet fra serial monitoren er "A", skal denne sætning køres.
      int Drejer = analogRead(A0); //Jeg laver en varial "Drejer", hvor værdien fra A0 bliver lagret.
      Serial.println(Drejer); //Printer variablen "Drejer" til serialmonitoren.
      
    }
    if (command == "B") { //Magen til ovenstående kommentar, bortset fra denne sætning læser fra A1.
      int Drejer2 = analogRead(A1);
      Serial.println(Drejer2);
    }
  }
}
