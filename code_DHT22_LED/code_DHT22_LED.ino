#include "DHT.h"
#define DHTPIN D4 //ขาไหนนั้นเองต่อที่บอร์ด
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
const int led = D2;    //ขา13เป็นLED
                                                                                                                                                                                                                                            
void setup() {
  // put your setup code here, to run once:
 pinMode(led,OUTPUT);
 Serial.begin(1200);
 Serial.println("DHTxx test");
 dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float h =dht.readHumidity();    //ความชื้น
  float t =dht.readTemperature(); //อุณหภูมิ
  if(isnan(t)||isnan(h)){
     Serial.println("Failed to read from DHT");  
  }
  else{
     Serial.print("Humidity = ");
     Serial.print(h);
     Serial.print("%\t");
     Serial.print("TEmperature = ");
     Serial.print(t);
     Serial.println("*C"); 
   }
   if(t>=25){                       //ไฟให้กระพริบเมือ (t=อุณหภูมิ) มากกว่า 25 ให้ไฟกระพริบ
      digitalWrite(led,HIGH);
      delay(500); 
      digitalWrite(led,LOW);
      delay(500); 
    }
   else {
    digitalWrite(led,LOW);
    delay(500);  
    }
}
