#include <JsonUtils.h>

#include "Declares.h"
#include "Defines.h"
#include "BlockchainUtils.h"

JSONVar emptyParamsArray;
JSONVar paramsArray;
// bool connectionClosed = false;
JSONVar received_message;
int nonce;
bool got_nonce = false;
std::string block_hash = "";
const char* chain_head;
bool got_chain_head = false;
const char* parent_block_hash;
bool got_parent_block_hash = false;
JSONVar runtime_info;
bool got_runtime_info = false;

uint32_t getEra() {
    return 0;
}

uint64_t getTip() {
    return 0;
}

std::string getGenesisBlockHash() {
    return "525639f713f397dcf839bd022cd821f367ebcf179de7b9253531f8adbe5436d6";
}

uint32_t getSpecVersion(JSONVar runtimeInfo) {
    int specVersion_ = (int) (runtimeInfo["specVersion"]);
    return (uint32_t) specVersion_;
}

uint32_t getTransactionVersion(JSONVar runtimeInfo) {
    int tx_version_ = (int)(runtimeInfo["transactionVersion"]);
    return (uint32_t) tx_version_;
}

// Get Nonce

uint64_t getNonce() {
    setOnTextCallback(getNonceCallback);
    sendGetAccountNonceMessage();
    while (!got_nonce) {
        websocketLoop();
    }
    Serial.print("Nonce: ");
    Serial.println(nonce);
    return (uint64_t) nonce;
}

void getNonceCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    Serial.println("After parse");
    nonce = (int) (received_message["result"]);
    got_nonce = true;
    Serial.println("After getting nonce from JSON");
}

void sendGetAccountNonceMessage() {
    paramsArray[0] = SS58_ADR;
    String message = createWebsocketMessage("system_accountNextIndex", paramsArray, requestId);
    rpcRequest(message);
}

// Get Block Hash

std::string getBlockHash() {
    setOnTextCallback(getBlockHashCallback);
    sendGetBlockHashMessage();
    while (block_hash == "") {
        websocketLoop();
    }
    Serial.printf("Block hash: %x\n", block_hash);
    return block_hash;
}

void getBlockHashCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    block_hash = (const char*) received_message["result"];
}

void sendGetBlockHashMessage() {
    paramsArray[0] = 0;
    String message = createWebsocketMessage("chain_getBlockHash", paramsArray, requestId);
    rpcRequest(message);
}

// Get Runtime Info

JSONVar getRuntimeInfo() {
    const char* chain_head_local = getChainHead();
    const char* parent_block_local = getParentBlockHash(chain_head_local);
    return getRuntimeInfo(parent_block_local);
}

JSONVar getRuntimeInfo(const char* parentBlockHash) {
    setOnTextCallback(getRuntimeInfoCallback);
    sendGetRuntimeInfoMessage(parentBlockHash);
    while (!got_runtime_info) {
        websocketLoop();
    }
    return runtime_info;
}

void getRuntimeInfoCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    runtime_info = received_message["result"];
    got_runtime_info = true;
}

void sendGetRuntimeInfoMessage(const char* parentBlockHash) {
    paramsArray[0] = parentBlockHash;
    String message = createWebsocketMessage("state_getRuntimeVersion", paramsArray, requestId);
    rpcRequest(message);
}

// Get Chain Head

const char* getChainHead() {
    setOnTextCallback(getChainHeadCallback);
    sendGetChainHeadMessage();
    while (!got_chain_head) {
        websocketLoop();
    }
    Serial.print("Chain head: ");
    Serial.println(chain_head);
    return chain_head;
}

void getChainHeadCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    chain_head = (const char *) (received_message["result"]);
    got_chain_head = true;
}

void sendGetChainHeadMessage() {
    // paramsArray[0] = "";
    String message = createWebsocketMessage("chain_getHead", emptyParamsArray, requestId);
    rpcRequest(message);
}

// Get Parent Block Hash

const char* getParentBlockHash(const char* chainHead) {
    setOnTextCallback(getParentBlockHashCallback);
    sendGetParentBlockHashMessage(chainHead);
    while (!got_parent_block_hash) {
        websocketLoop();
    }
    Serial.print("Chain header: ");
    Serial.println(parent_block_hash);
    return parent_block_hash;
}

void getParentBlockHashCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    parent_block_hash = (const char *) (received_message["result"]["parentHash"]);
    got_parent_block_hash = true;
}

void sendGetParentBlockHashMessage(const char* chainHead) {
    paramsArray[0] = chainHead;
    String message = createWebsocketMessage("chain_getHeader", paramsArray, requestId);
    rpcRequest(message);
}

// Get Metadata

bool got_metadata = false;

void getMetadata() {
    setOnTextCallback(getMetadataCallback);
    sendGetMetadataMessage();
    while (!got_metadata) {
        websocketLoop();
    }
    // Serial.print("Nonce: ");
    // Serial.println(nonce);
    // return (uint64_t) nonce;
}

void getMetadataCallback(uint8_t *payload) {
    // received_message = JSON.parse((char *)payload);
    // Serial.println("After parse");
    // nonce = (int) (received_message["result"]);
    got_metadata = true;
    Serial.println("Got metadata");
    // Serial.println("After getting nonce from JSON");
}

void sendGetMetadataMessage() {
    // paramsArray[0] = SS58_ADR;
    String message = createWebsocketMessage("state_getMetadata", emptyParamsArray, requestId);
    rpcRequest(message);
}