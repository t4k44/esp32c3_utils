# lib/utils

このライブラリは、ESP32C3プロジェクトで共通して使用されるユーティリティ機能を提供します。WiFi接続管理、MQTT通信、OTAアップデート、ロギング、ブートステータス通知、トーン生成などのヘルパー関数が含まれています。

## セットアップ

1.  **サブモジュールの追加**:
    プロジェクトのルートディレクトリから、以下のコマンドでこのリポジトリをサブモジュールとして追加します。

    ```bash
    git submodule add git@github.com:t4k44/esp32c3_utils.git lib/utils
    ```

2.  **設定ファイルの配置**:
    `settings.h.template`を`include/settings.h`としてコピーし、プロジェクトの要件に合わせて設定をカスタマイズしてください。

    ```bash
    cp lib/utils/settings.h.template include/settings.h
    ```

    `include/settings.h`の例:

    ```cpp
    #pragma once

    // WiFi設定
    #define WIFI_SSID      "MY_SSID"
    #define WIFI_PASS      "SSID_PASS"

    // OTA設定
    #define OTA_HOSTNAME   "node00"  // ホスト名（mDNS用）

    // MQTT設定
    #define MQTT_BROKER    "IP_ADDRESS"
    #define MQTT_PORT      1883
    #define MQTT_CLIENT_ID OTA_HOSTNAME

    // デバイス識別
    #define DEVICE_ID      OTA_HOSTNAME
    #define DEVICE_TYPE    "TYPE_NAME"  // 例：温度センサ、リレー制御など

    // 埋め込み用バージョン識別子
    #ifndef GIT_TAG
    #define GIT_TAG "0.0.0"
    #endif
    #define DEVICE_VERSION GIT_TAG

    // MQTTトピック
    #define MQTT_PUB_STATUS  DEVICE_TYPE "/" DEVICE_ID "/status"
    #define MQTT_PUB_LOG     DEVICE_TYPE "/" DEVICE_ID "/log"
    #define MQTT_SUB_COMMAND DEVICE_TYPE "/" DEVICE_ID "/command"

    #define CURRENT_LOG_LEVEL LOG_DEBUG
    #define LOG_TO_MQTT
    #define LOG_TO_SERIAL
    ```

## 提供される機能

### boot_helper.h / boot_helper.cpp
デバイスの起動ステータス（リセット理由、ファームウェアバージョン、コアID、ヒープ情報、フラッシュサイズなど）をJSON形式で構築し、MQTT_PUB_STATUSトピックにパブリッシュします。

### log_helper.h / log_helper.cpp
ログレベル（ERROR, WARN, INFO, DEBUG）に基づいてメッセージをフィルタリングし、設定に応じてMQTTまたはシリアルポートに出力します。

### mqtt_helper.h / mqtt_helper.cpp
MQTTブローカーへの接続管理、指定されたトピックとペイロードでのメッセージパブリッシュ、受信メッセージの処理、およびデバイスのステータス情報（RSSI、稼働時間、ヒープメモリなど）の定期的なMQTT送信機能を提供します。

### ota_helper.h
OTA (Over-The-Air) アップデート機能のセットアップと処理を行います。ホスト名の設定、OTAの開始/終了/進捗/エラー時のコールバック関数を登録し、OTAサービスを開始・処理します。

### tone_helper.h
指定された周波数と期間でトーンを生成する機能を提供します。成功を示す短いビープ音 (`beepOK()`) やエラーを示すビープ音 (`beepError()`) のユーティリティ関数も含まれています。

### wifi_helper.h
WiFi接続を維持する機能を提供します。WiFiが未接続の場合、再接続を試行し、接続成功時にはIPアドレス、SSID、MACアドレスをログに出力します。接続に失敗した場合でもループを維持します。

## ドキュメント生成

```bash
doxygen Doxyfile
```
