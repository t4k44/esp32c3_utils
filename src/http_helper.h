#include <HTTPClient.h>

/**
 * @brief HTTP経由で指定されたURLから画像データを取得し、バッファに格納します。
 * * @param[in] url 画像取得元のURL
 * * @param[out] dst_buf 画像データを格納するためのバッファ（事前に確保済みであること）
 * * @param[in] max_bytes 期待するデータサイズ（バイト数）
 * @return uint32_t 実際に受信したデータのバイト数（失敗時は0）
 * * @note 期待するデータサイズ（max_bytes）と異なる場合でも、タイムアウトまたは
 * データ終了まで受信を試みます。データ受信が継続している間は
 * タイムアウトカウンタ（5000ms）がリセットされます。
 */
uint32_t fetchImage(const char* url, uint8_t* dst_buf, uint32_t max_bytes);
