#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <settings.h>

#include "mqtt_helper.h"
#include "log_helper.h"

WiFiClientSecure* secret_client = nullptr;
PubSubClient* mqttClient = nullptr;
static unsigned long lastMqttRetryAttempt = 0;

void connectMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WARN] WiFi未接続のため、MQTT接続をスキップ");
    return;
  }

  if (mqttClient == nullptr) {
    secret_client = new WiFiClientSecure();
    mqttClient = new PubSubClient(*secret_client);
  }
  if (!mqttClient->connected()) {
    unsigned long now = millis();
    // 5秒間隔でのみ再接続を試みる（CPU負荷およびTLS処理のオーバーヘッド防止）
    if (now - lastMqttRetryAttempt < 5000) {
      return;
    }
    lastMqttRetryAttempt = now;

    secret_client->setInsecure(); // 証明書検証を無効化し、メモリ消費を低減
    mqttClient->setServer(MQTT_BROKER, MQTT_PORT);

    Serial.println("[INFO] MQTT再接続を試行中...");
    String clientId = "ESP32C3Client-" + String(random(0xffff), HEX);

    if (mqttClient->connect(clientId.c_str(), MQTT_CLIENT_ID, MQTT_CLIENT_PW)) {
      mqttClient->setBufferSize(4096);
      logMessage(LOG_INFO, "MQTT接続成功");
      mqttClient->subscribe(MQTT_SUB_COMMAND);
      logMessage(LOG_DEBUG, "サブスク登録: " + String(MQTT_SUB_COMMAND));
    } else {
      Serial.println("[ERROR] MQTT接続失敗 RC=" + String(mqttClient->state()));
    }
  }
}

void publishMQTT(const String &topic, const String &payload) {
  static bool inLogging = false; // logMessage() を呼ぶ前にループ保護。
  if (inLogging)
    return;
  inLogging = true;

  if (mqttClient != nullptr) {
    mqttClient->publish(topic.c_str(), payload.c_str());
    logMessage(LOG_DEBUG, "MQTT送信: " + topic + " → " + payload);
  }

  inLogging = false;
}

void handleMQTT() {
  if (WiFi.status() == WL_CONNECTED) {
    connectMQTT();
    if (mqttClient != nullptr) {
      mqttClient->loop();
    }
  }
}

/**
 * @brief デバイスのステータス情報をMQTTでパブリッシュします。
 *
 * RSSI、稼働時間、ヒープメモリ情報などを含むJSON形式のペイロードを構築し、
 * MQTT_PUB_LOGトピックに送信します。
 */
void sendDeviceStatus() {
  JsonDocument doc;
  doc["level"]  = LOG_INFO;
  doc["rssi"]   = WiFi.RSSI();
  doc["uptime"] = millis() / 1000;  // 秒単位
  doc["heap"]   = ESP.getFreeHeap();

  String payload;
  serializeJson(doc, payload);
  publishMQTT(MQTT_PUB_LOG, payload);   // 例：smart_speaker/esp32c3_node01/log
}
