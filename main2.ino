#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <DHT.h>
#include <DHT_U.h>

/*
 * Pin conectors
 * 
 * RFID ( SS_ = 53, SCK = 52, MOSI = 50, irq = '', GND = GND, RST = 9, 3.3 = 3.3) )
 * 
 * servo (servo1 = 20, servo2 = 21)
 * 
 * mq135 ( d0 = 30)
 * 
 * dht (pin pin = 3)
 * 
 * tierra (A0 = a0, d0 = 22)
 */

//globales

#define DHT1PIN 3
#define DHT2PIN 4
int mq = 30;
int d0 = 22;
int humedad = A0;
char l = 'k';
int servo1 = 31;
int servo2 = 32;
int fan1 = 33;
int fan2 = 34;
int ldr = A1;
int dimmer = 6;
int luzL = 24;
int luzC = 25;
int dimmer2 = 5;
int luzC1 = 27;
int boton = 28;
int bomb = 29;


//Rango hum : 0~300.muy humedo # 300~700. intermedio #700~1023.seco
//codigo rfid 35 A2 2E 83

//rfid shit

#define RST_PIN 9
#define SS_PIN 53
MFRC522 mfrc522(SS_PIN, RST_PIN);

//dht 11 shit

#define DHTTYPE DHT11
DHT dht (DHT1PIN, DHTTYPE);
DHT dht2 (DHT2PIN, DHTTYPE);
float temp, hum, temp2, hum2;

//objetos

Servo cochera;
Servo puerta;

void setup() {
  Serial.begin(9600);
  cochera.attach(31,600,1500);
  puerta.attach(32,600,1500);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(mq,INPUT);
  pinMode(servo1, OUTPUT);
  pinMode(servo2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(dimmer, OUTPUT);
  pinMode(luzL, OUTPUT);
  pinMode(luzC, OUTPUT);
  pinMode(dimmer2, OUTPUT);
  pinMode(luzC1, OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(boton, INPUT);
  pinMode(bomb, OUTPUT);
  pinMode(humedad, INPUT);

  //put close

  cochera.write(0);
  cochera.write(0);
  //Initial mode

 digitalWrite(fan1, HIGH);
 digitalWrite(fan2, HIGH);
 digitalWrite(bomb, HIGH);
 digitalWrite(dimmer, LOW);
 digitalWrite(dimmer2, LOW);
  
}

void shit(char a){
  switch (a){
    case 'c':
      cochera.write(70);
      delay(1000);
      cochera.write(0);      
    break;

    case 'T':
      digitalWrite(fan1, LOW);
      delay(1000);
      digitalWrite(fan1, HIGH);
    break;

    case 'H':
      digitalWrite(bomb, LOW);
      delay(1000);
      digitalWrite(bomb, HIGH);
    break;

    case 'q':
      digitalWrite(fan2, LOW);
      delay(1000);
      digitalWrite(fan2, HIGH);
    break;

  }
  return;
}

void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()){  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial() == "35A22E83"){
                  // Enviamos serialemente su UID
//                  Serial.print("Card UID:");
//                  for (byte i = 0; i < mfrc522.uid.size; i++) {
//                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                  
                  l = 'c';
                  shit(l);
            }
                  //Serial.println();
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
             }   
    if( analogRead(humedad) < 700 ){
      l = "H";
      shit(l);
    }

    hum = dht.readHumidity();
    temp = dht.readTemperature();
    
    if(isnan(hum) || isnan (temp)){
      Serial.println("Fallo!");
    }else{
      Serial.print (temp);
      Serial.print (hum);
      l = 'T';
      shit(l);
    }

    hum2 = dht2.readHumidity(); 
    temp2 = dht2.readTemperature();

    if(isnan(hum2) || isnan (temp2)){
      Serial.println("Fallo2!");
    }else{
      Serial.print(temp2);
      Serial.print(hum2);
      l = 'q';
      shit(l);
    }

    boolean humo = digitalRead(mq);
    if (humo){
      Serial.print ("good air");
    }else{
      l = 'q';
      shit(l);
    }

    int luzfuera = analogRead(ldr);

    if (luzfuera < 512){
      analogWrite(dimmer, 1023);
    }else {
      digitalWrite(dimmer, LOW);
    }
    
}
