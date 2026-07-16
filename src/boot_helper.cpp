#include "boot_helper.h"
#include "mqtt_helper.h"
#include <settings.h>

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

  JsonDocument doc;
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
