#pragma once

#include "log_helper.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <settings.h>

// Global client instances (defined in mqtt_helper.cpp)
extern WiFiClient espClient;
extern PubSubClient* mqttClient;

/**
 * @brief MQTTブローカーへの接続を試行します。
 *
 * 接続が確立されていない場合にのみ再接続を試み、
 * 成功時にはMQTTサブスクリプションを設定します。
 */
void connectMQTT();

/**
 * @brief 指定されたトピックとペイロードでMQTTメッセージをパブリッシュします。
 * @param topic パブリッシュするMQTTトピック。
 * @param payload パブリッシュするメッセージペイロード。
 */
void publishMQTT(const String &topic, const String &payload);

/**
 * @brief MQTTクライアントのループ処理を実行します。
 *
 * 受信メッセージの処理や接続維持のために定期的に呼び出す必要があります。
 */
void handleMQTT();

/**
 * @brief 受信したMQTTメッセージを処理します。
 *
 * 現在は "smart_speaker/speak" トピックのメッセージを処理します。
 * @param topic 受信したメッセージのトピック。
 * @param payload 受信したメッセージのペイロード。
 */
void handleReceivedMessage(const char *topic, const char *payload);

/**
 * @brief デバイスのステータス情報をMQTTでパブリッシュします。
 *
 * RSSI、稼働時間、ヒープメモリ情報などを含むJSON形式のペイロードを構築し、
 * MQTT_PUB_LOGトピックに送信します。
 */
void sendDeviceStatus();
