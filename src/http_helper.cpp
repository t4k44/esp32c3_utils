#include <HTTPClient.h>
#include <http_helper.h>

/**
 * @brief HTTP経由で指定されたURLから画像データを取得し、バッファに格納します。
 * * @param[in] url 画像取得元のURL
 * * @param[in] dst_buf 画像データを格納するためのバッファ（事前に確保済みであること）
 * * @param[in] max_bytes 期待するデータサイズ（バイト数）
 * @return uint32_t 実際に受信したデータのバイト数（失敗時は0）
 * * @note 期待するデータサイズ（max_bytes）と異なる場合でも、タイムアウトまたは
 * データ終了まで受信を試みます。データ受信が継続している間は
 * タイムアウトカウンタ（5000ms）がリセットされます。
 */
uint32_t fetchImage(const char* url, uint8_t* dst_buf, uint32_t max_bytes) {
    const char* func = "fetchImage";
    // DEBUG_LOG(func, "START");
    // DEBUG_LOG(func, url);

    if (dst_buf == nullptr) {
        // Serial.printf("[DEBUG][%s] ERROR: destination buffer is null\n", func);
        return 0;
    }

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    // DEBUG_VAL(func, "httpCode", httpCode);

    if (httpCode != HTTP_CODE_OK) {
        // DEBUG_LOG(func, "ERROR: HTTP GET failed");
        http.end();
        return 0;
    }

    int contentLen = http.getSize();
    // DEBUG_VAL(func, "contentLen", contentLen);

    if (contentLen != (int)max_bytes) {
        Serial.printf("[DEBUG][%s] WARNING: expected %u bytes, got %d\n",
                      func, max_bytes, contentLen);
        // サイズ不一致でも受け取れるだけ受け取る
    }

    WiFiClient* stream = http.getStreamPtr();
    uint32_t received  = 0;
    uint32_t timeout_ms = 5000;
    uint32_t t0 = millis();

    while (received < max_bytes && (millis() - t0) < timeout_ms) {
        if (stream->available()) {
            int n = stream->readBytes(image_buf + received,
                                      max_bytes - received);
            received += n;
            t0 = millis(); // データが来ている間はタイムアウトをリセット
        }
        yield();
    }

    http.end();
    // DEBUG_VAL(func, "received_bytes", received);
    // DEBUG_LOG(func, received == max_bytes ? "SUCCESS" : "INCOMPLETE");
    return received;
}
