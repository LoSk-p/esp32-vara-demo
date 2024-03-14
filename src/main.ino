#include <Arduino.h>
#include <Arduino_JSON.h>

#include <Extrinsic.h>
 
#include <WiFi.h>
#include <WebSocketsClient.h>
 
#include "Secret.h"
// #include "SendExtrinsic.h"
#include "SubscribeStorageChange.h"
#include "Defines.h"

 
void setup() {
    Serial.begin(115200);
 
    Serial.println();
    Serial.println();
    Serial.println();
 
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOTTT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
 
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());

    setupWebsocket();
    sendPingExstrinsic();
    subscribeStorageChange();
}


void loop() {
    websocketLoop();
}
