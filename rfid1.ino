#include <SPI.h>
#include <MFRC522.h>
#include "ESP8266WiFi.h"

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vA1D5A8687AE4CD2"; //device ID from Pushingbox

const char* MY_SSID = "Alakarthika";
const char* MY_PWD =  "alakarthika";
#define SS_PIN D4
#define RST_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
void setup() {
  Serial.begin(9600);
  SPI.begin();       // Init SPI bus
  
mfrc522.PCD_Init(); // Init MFRC522
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


void loop() {
  byte arr[ mfrc522.uid.size];
  WiFiClient client;
  if ( mfrc522.PICC_IsNewCardPresent())
  {
    if ( mfrc522.PICC_ReadCardSerial())
    {
      Serial.print("Tag UID:");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        arr[i] = mfrc522.uid.uidByte[i];
      }

      

      if (client.connect(WEBSITE, 80))
      {
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          client.print("GET /pushingbox?devid=" + devid
                       + "RFIDval" + (String) arr[i]);}
        }
      }
      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: ESP8266/1.0");
      client.println("Connection: close");
      client.println();




      Serial.println();
      mfrc522.PICC_HaltA();
    }
  }
