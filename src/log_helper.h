#pragma once
#include <Arduino.h>
#include <settings.h>

enum LogLevel { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

/**
 * @brief ログレベルに応じた文字列を取得します。
 *
 * @details 内部のデバッグログ出力時に、指定された LogLevel 列挙型の値を
 * 対応する識別用の文字列（例: "ERROR", "WARN"）に変換するために使用します。
 *
 * @param[in] level 変換対象のログレベル（LogLevel型）
 * @return String 対応するログレベルの文字列（未定義の場合は "UNKNOWN"）
 */
String getLogLevelString(LogLevel level);

/**
 * @brief ログレベルに応じた整数値を取得します。
 *
 * @details ログの優先度比較や、条件分岐の判定用に
 * 指定された LogLevel 列挙型の値を対応する数値へ変換します。
 *
 * @param[in] level 変換対象のログレベル（LogLevel型）
 * @return int 対応するログレベルの数値（未定義の場合は 0）
 */
int getLogLevelInt(LogLevel level);

/**
 * @brief 指定されたログレベルでメッセージを記録・出力します。
 *
 * @details 現在の設定（CURRENT_LOG_LEVEL）と判定し、有効な場合にのみ処理を行います。
 * JSONバッファを生成してログデータを格納し、有効化されているマクロに応じて
 * MQTT送信（LOG_TO_MQTT）やシリアル出力（LOG_TO_SERIAL）を行います。
 *
 * @param[in] level ログメッセージの重要度（LogLevel型）
 * @param[in] message 記録するログ本文の文字列
 */
void logMessage(LogLevel level, const String &message);
