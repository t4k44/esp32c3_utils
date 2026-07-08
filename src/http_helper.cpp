#include <HTTPClient.h>
#include <http_helper.h>

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
            int n = stream->readBytes(dst_buf + received,
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
