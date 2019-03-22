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

#define DHTPIN 3
int mq = 30;
int d0 = 22;
int humedad = A0;
char l = 'k';
int servo1 = 31;
int servo2 = 32;
int fan1 = 33;
int ldr = A1;
int luzS = 4;
int luzL = 24;
int luzC = 25;
int luzC2 = 5;
int luzE = 27;


//Rango hum : 0~300.muy humedo # 300~700. intermedio #700~1023.seco
//codigo rfid 35 A2 2E 83

//rfid shit

#define RST_PIN 9
#define SS_PIN 53
MFRC522 mfrc522(SS_PIN, RST_PIN);

//dht 11 shit

#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);
float temp, hum;

//objetos

Servo cochera;
Servo puerta;

void setup() {
  Serial.begin(9600);
  cochera.attach(30,600,1500);
  puerta.attach(31,600,1500);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(mq,INPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);

  //put close

  cochera.write(0);
  cochera.write(0);
  //Initial mode

 digitalWrite(fan1, HIGH);
  
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
      //Start something
    break;
  }
  return;
}

void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()){  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()){
                  // Enviamos serialemente su UID
                  Serial.print("Card UID:");
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                  } 
                  l = 'c';
                  shit(l);
                  Serial.println();
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
             }      
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

    boolean humo = digitalRead(mq);
    if ( mq){
      Serial.print ("good air");
    }else{
      l = 'q';
      shit(l);
    }

}
