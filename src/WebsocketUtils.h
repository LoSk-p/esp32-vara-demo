#include <WebSocketsClient.h>
#include "Defines.h"
#include "Declares.h"

typedef void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length);
typedef void (*OnTextWebsocketCallback)(uint8_t *payload);

bool websocketConnected = false;
bool connectionClosed = true;

WebSocketsClient webSocket;
OnTextWebsocketCallback onTextCallback;

void websocketLoop() {
    if (!connectionClosed) {
        webSocket.loop();
    }
}

void websocketSendMessage(String text) {
    webSocket.sendTXT(text);
}

void setupWebsocket() {
    webSocket.beginSSL(WS_SERVER_HOST, WS_SERVER_PORT, "/");
    connectWebscoket(defaultOnTextCallback);
}

void defaultOnTextCallback(uint8_t *payload) {
    Serial.println("Default on text callback");
}

void disconnectWebSocket() {
    Serial.println("Disconnected from websocket");
    webSocket.disconnect();
    connectionClosed = true;
}

void connectWebscoket(OnTextWebsocketCallback callback) {
    connectionClosed = false;
    webSocket.onEvent(mainWebsocketCallback);
    while (!websocketConnected) {
        websocketLoop();
    }
    setOnTextCallback(callback);
}

void setOnTextCallback(OnTextWebsocketCallback callback) {
    onTextCallback = callback;
}

void mainWebsocketCallback(WStype_t type, uint8_t *payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            websocketConnected = false;
            break;
        case WStype_CONNECTED: {
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            websocketConnected = true;
        }
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] RESPONSE: %s\n", (char *)payload);
            onTextCallback(payload);
            break;
    }
}