#include "WebsocketUtils.h"

int requestId = 1;

void rpcRequest(String data) {
    Serial.print("Sent to ws: ");
    Serial.println(data);
    websocketSendMessage(data);
    Serial.print("Sent to ws after: ");
    requestId++;
}

String createWebsocketMessage(String method, JSONVar paramsArray, int id) {
    JSONVar messageObject;
    messageObject["jsonrpc"] = "2.0";
    messageObject["method"] = method;
    messageObject["params"] = paramsArray;
    messageObject["id"] = id;
    String messageString = JSON.stringify(messageObject);
    return messageString;
}