#pragma once
#include "log_helper.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_system.h>

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
void publishBootStatus();
