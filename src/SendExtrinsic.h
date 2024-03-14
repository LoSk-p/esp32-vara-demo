#include <vector>
#include <string>
#include <cstring>

// #include <WebSocketsClient.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include <JsonUtils.h>
#include <Call.h>
#include <Extrinsic.h>

// #include "BlockchainUtils.h"
#include "PayloadParamsUtils.h"
#include "Secret.h"
#include "Declares.h"
#include "Defines.h"

uint8_t publicKey_[KEYS_SIZE];
uint8_t privateKey_[KEYS_SIZE];
String extrinsicResult;
bool got_extrinsic_result = false;

void sendPingExstrinsic() {
    Data call = createCall();
    std::vector<uint8_t> vk = hex2bytes(PRIV_KEY);
    std::copy(vk.begin(), vk.end(), privateKey_);

    Ed25519::derivePublicKey(publicKey_, privateKey_);

    uint64_t payloadNonce = getNonce();
    std::string payloadBlockHash = getGenesisBlockHash();
    uint32_t payloadEra = getEra();
    uint64_t payloadTip = getTip();
    JSONVar runtimeInfo = getRuntimeInfo();
    uint32_t payloadSpecVersion = getSpecVersion(runtimeInfo);
    uint32_t payloadTransactionVersion = getTransactionVersion(runtimeInfo);
    Serial.printf("Spec version: %u, tx version: %u, nonce: %u, era: %u, tip: %u\n", payloadSpecVersion, payloadTransactionVersion, payloadNonce, payloadEra, payloadTip);

    Data data_ = createPayload(call, payloadEra, payloadNonce, payloadTip, payloadSpecVersion, payloadTransactionVersion, payloadBlockHash, payloadBlockHash);
    Data signature_ = createSignature(data_, privateKey_, publicKey_);
    std::vector<std::uint8_t> pubKey( reinterpret_cast<std::uint8_t*>(std::begin(publicKey_)), reinterpret_cast<std::uint8_t*>(std::end(publicKey_)));
    Data edata_ = createSignedExtrinsic(signature_, pubKey, payloadEra, payloadNonce, payloadTip, call);
    sendExtrinsic(edata_, requestId);
}

Data createCall() {
    Data call;
    std::vector<uint8_t> callStr = hex2bytes(CALL_ENCODED);
    append(call, callStr);
    Serial.printf("Call size %d : \n", call.size());
    for (int k = 0; k < call.size(); k++) 
        Serial.printf("%02x", call[k]);
    Serial.println("");
    return call;
}

Data createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block) {
    Data data_ = doPayload (call, era, nonce, tip, sv, tv, gen, block);
    Serial.printf("\nPayload size %d : \n", data_.size());
    for (int k = 0; k < data_.size(); k++) 
        Serial.printf("%02x", data_[k]);
    Serial.println("");
    return data_;
}

Data createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]) {
    Data signature_ = doSign (data, privateKey, publicKey);
    Serial.printf("\nSignatured size %d : \n", signature_.size());
    for (int k = 0; k < signature_.size(); k++) 
        Serial.printf("%02x", signature_[k]);
    Serial.println("");
    return signature_;
}

Data createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call) {
    Data edata_ = doEncode (signature, pubKey, era, nonce, tip, call);
    Serial.printf("Extrinsic %s: size %d\n", "PING", edata_.size());
    return edata_;
}

void sendExtrinsic(Data extrinsicData, int requestId) {
    String extrinsicMessage = fillParamsJs(extrinsicData, requestId);
    Serial.println("After to string: ");
    Serial.print(extrinsicMessage);
    rpcRequest(extrinsicMessage);
    getExstrinsicResult();
}

void getExstrinsicResult() {
    setOnTextCallback(getExstrinsicResultCallback);
    while (!got_extrinsic_result) {
        websocketLoop();
    }
    Serial.print("Extrinsic result: ");
    Serial.println(extrinsicResult);
}

void getExstrinsicResultCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    extrinsicResult = JSON.stringify(received_message["result"]);
    got_extrinsic_result = true;
}