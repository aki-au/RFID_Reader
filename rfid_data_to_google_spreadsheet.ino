
/* Arduino Code which sends data to google spreadsheet */
#include <SPI.h>
#include <MFRC522.h>
#include "ESP8266WiFi.h"
#define SS_PIN 4 //FOR RFID SS PIN BECASUSE WE ARE USING BOTH ETHERNET SHIELD AND RS-522
#define RST_PIN 9
#define No_Of_Card 3
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "api.pushingbox.com";   //YOUR SERVER
  
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
const char* MY_SSID = "Alakarthika";
const char* MY_PWD =  "alakarthika";
byte i;
int j=0;


// the setup function runs once when you press reset or power the board
void setup(){
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    for(byte i=0;i<6;i++)
    {
      key.keyByte[i]=0xFF;
    }
    delay(1000);
     SPI.begin();       // Init SPI bus
  
rfid.PCD_Init(); // Init MFRC522
Serial.println("RFID reading UID");
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  Serial.println("going into wl connect");

  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}
    


// the loop function runs over and over again forever
void loop(){
 
    int m=0;
    if(!rfid.PICC_IsNewCardPresent())
    return;
    if(!rfid.PICC_ReadCardSerial())
    return;
    
     for(i=0;i<No_Of_Card;i++)
    {
                                Serial.print("your card no :");
                    for(int s=0;s<4;s++)
                    {
                      Serial.print(rfid.uid.uidByte[s]);
                      Serial.print(" ");
                     
                    }
                    Serial.println("\nValid Person");
                    Sending_To_spreadsheet();
                    j=0;
                              
                              rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();   return; 
                 
    
    
       // Halt PICC
    rfid.PICC_HaltA();
  
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
 }
}

 void Sending_To_spreadsheet()   //CONNECTING WITH MYSQL
 {WiFiClient client;
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /pushingbox?devid=vA1D5A8687AE4CD2&allowed_members=");     //YOUR URL
        
    client.print("&Member_ID=");
    for(int s=0;s<4;s++)
                  {
                    client.print(rfid.uid.uidByte[s]);
                                  
                  }
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }

 
