#pragma once

#include "log_helper.h"
#include <WiFi.h>
#include <settings.h>

/**
 * @brief Wi-Fiの接続状態を監視し、切断時に自動再接続を試行します。
 * @return 最終的なWi-Fi接続ステータス (wl_status_t)
 *         - WL_CONNECTED: 接続成功
 *         - それ以外: 接続失敗または試行中
 *
 * 現在の接続ステータスを確認し、切断されている場合は設定された資格情報
 * （WIFI_SSID, WIFI_PASS）を使用して再接続を開始します。
 * 接続成功時にはIPアドレス、SSID、MACアドレスをログに出力します。
 *
 * 【動作仕様】
 * - タイムアウト: 再接続試行開始から最大10秒間、接続を待機します。
 * - ブロッキング: 接続待ちの間、100msごとのポーリングを行い、最大10秒間処理をブロックします。
 * - 失敗時の挙動: 10秒以内に接続できない場合でも、システムの再起動（ESP.restart）は行わず、
 *                呼び出し元へ処理を戻します。
 *
 * @note  loop() 内で定期的に呼び出すことで、ネットワークの継続性を維持することを想定しています。
 * @see   logMessage, WIFI_SSID, WIFI_PASS
 */
wl_status_t maintainWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    logMessage(LOG_WARN, "WiFi未接続。再接続を試行中...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long timeout = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - timeout < 10000) {
      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
      logMessage(LOG_INFO, "WiFi接続成功 IP: " + WiFi.localIP().toString() +
                               " SSID: " + WiFi.SSID() +
                               " MAC: " + WiFi.macAddress());

    } else {
      logMessage(LOG_WARN, "WiFi未接続継続。再起動せずループ維持。");
      // ESP.restart();
    }
  }

  return WiFi.status();
}
