#include "log_helper.h"
#include "mqtt_helper.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <settings.h>

String getLogLevelString(LogLevel level) {
  switch (level) {
  case LOG_ERROR:
    return "ERROR";
  case LOG_WARN:
    return "WARN";
  case LOG_INFO:
    return "INFO";
  case LOG_DEBUG:
    return "DEBUG";
  default:
    return "UNKNOWN";
  }
}

int getLogLevelInt(LogLevel level) {
  switch (level) {
  case LOG_ERROR:
    return 1;
  case LOG_WARN:
    return 2;
  case LOG_INFO:
    return 3;
  case LOG_DEBUG:
    return 4;
  default:
    return 0;
  }
}

void logMessage(LogLevel level, const String &message) {
  if (level > CURRENT_LOG_LEVEL)
    return;

  JsonDocument doc;
  doc["level"]   = getLogLevelInt(level);
  doc["message"] = message;

  String payload;
  serializeJson(doc, payload);

#if defined(LOG_TO_MQTT)
  publishMQTT(MQTT_PUB_LOG, payload);
#endif

#if defined(LOG_TO_SERIAL)
  Serial.println("[" + getLogLevelString(level) + "] " + message);
#endif
}
