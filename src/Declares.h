#include <string>

// PayloadParamsUtils

uint64_t getNonce();
std::string getGenesisBlockHash();
uint32_t getEra();
uint64_t getTip();
uint32_t getSpecVersion(JSONVar runtimeInfo);
uint32_t getTransactionVersion(JSONVar runtimeInfo);
const char* getChainHead();
const char* getParentBlockHash(const char* chainHead);
JSONVar getRuntimeInfo(const char* parentBlockHash);
JSONVar getRuntimeInfo();
void getMetadata();


String createWebsocketMessage(String method, JSONVar paramsArray, int id);
void rpcRequest(String data);


void sendSubscribeToBlocksMessage();
void sendSubscribeStorageMessage();
void sendGetAccountNonceMessage();
void sendGetPayloadMessage();
void sendGetBlockHashMessage();
void sendGetChainHeadMessage();
void sendGetParentBlockHashMessage(const char* chainHead);
void sendGetRuntimeInfoMessage(const char* parentBlockHash);
void sendGetMetadataMessage();

void getNonceCallback(uint8_t *payload);
void getBlockHashCallback(uint8_t *payload);
void getChainHeadCallback(uint8_t *payload);
void getParentBlockHashCallback(uint8_t *payload);
void getRuntimeInfoCallback(uint8_t *payload);
void getMetadataCallback(uint8_t *payload);

// WebsocketUtils

typedef void (*OnTextWebsocketCallback)(uint8_t *payload);

void websocketLoop();
void websocketSendMessage(String text);
void setupWebsocket();
void defaultOnTextCallback(uint8_t *payload);
void disconnectWebSocket();
void connectWebscoket(OnTextWebsocketCallback callback);
void setOnTextCallback(OnTextWebsocketCallback callback);
void mainWebsocketCallback(WStype_t type, uint8_t *payload, size_t length);

// SendExtrinsic

void sendPingExstrinsic();
Data createCall();
Data createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block);
Data createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]);
Data createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call);
void sendExtrinsic(Data extrinsicData, int requestId);
void getExstrinsicResult();
void getExstrinsicResultCallback(uint8_t *payload);

// SubscribeStorageChange

void subscribeStorageChange();
void sendSubscribeStorageMessage();
void changeStorageCallback(uint8_t *payload);
bool checkPublicKeyInMessage(String *message);
bool checkPONGInMessage(String *message);