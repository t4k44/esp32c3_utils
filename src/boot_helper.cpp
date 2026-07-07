#include "boot_helper.h"
#include "mqtt_helper.h"
#include <settings.h>

/**
 * @brief デバイスの起動ステータスおよびシステム診断情報をMQTTで送信します。
 *
 * ESP32のリセット理由、ファームウェア情報、実行コア、メモリ使用状況（ヒープ）、
 * およびフラッシュサイズを含むJSONペイロードを構築し、
 * 指定されたトピック（MQTT_PUB_STATUS）へパブリッシュします。
 *
 * 【送信されるJSONの主要フィールド】
 * - boot       : 起動ステータス ("ok")
 * - version    : ファームウェアのバージョン (DEVICE_VERSION)
 * - reason     : 文字列化されたリセット理由 (例: "POWERON_RESET", "SOFTWARE_RESET")
 * - sdkVersion : ESP-IDFのSDKバージョン
 * - heapFree   : 現在の空きヒープ容量 (バイト)
 * - flashSize  : 搭載されているフラッシュのサイズ
 *
 * @note  この関数を呼び出す前に、MQTTクライアントが接続済みである必要があります。
 * @see   publishMQTT, esp_reset_reason
 */
void publishBootStatus() {
  // String reason = ESP.getResetReason();
  esp_reset_reason_t reasonCode = esp_reset_reason();

  String reasonStr;
  switch (reasonCode) {
  case ESP_RST_POWERON:
    reasonStr = "POWERON_RESET";
    break;
  case ESP_RST_SW:
    reasonStr = "SOFTWARE_RESET";
    break;
  case ESP_RST_DEEPSLEEP:
    reasonStr = "DEEPSLEEP_RESET";
    break;
  case ESP_RST_BROWNOUT:
    reasonStr = "BROWNOUT_RESET";
    break;
  case ESP_RST_EXT:
    reasonStr = "EXT_RESET";
    break;
  default:
    reasonStr = "UNKNOWN";
    break;
  }

  DynamicJsonDocument doc(128);
  doc["boot"]       = "ok";
  doc["version"]    = DEVICE_VERSION;
  doc["reason"]     = reasonStr;
  doc["core"]       = xPortGetCoreID();
  doc["sdkVersion"] = ESP.getSdkVersion();
  doc["heapMax"]    = ESP.getMaxAllocHeap();
  doc["heapFree"]   = ESP.getFreeHeap();
  doc["flashSize"]  = ESP.getFlashChipSize();
  doc["level"]      = LOG_INFO;

  String payload;
  serializeJson(doc, payload);
  publishMQTT(MQTT_PUB_STATUS, payload);
}
