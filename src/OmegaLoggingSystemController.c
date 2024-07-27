/**
 * @file OmegaLoggingSystemController.c
 * @author Omegaki113r
 * @date Saturday, 29th June 2024 3:45:31 am
 * @copyright Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaLoggingSystemController
 * File Name: OmegaLoggingSystemController.c
 * File Created: Saturday, 29th June 2024 3:45:31 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 27th July 2024 7:16:56 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <esp_spiffs.h>
#include <stdarg.h>
#include <string.h>

#include <cJSON.h>

#include "OmegaFileSystemController.h"
#include "OmegaLoggingSystemController.h"
#include "OmegaUtilityDriver.h"

#include <sdkconfig.h>

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_VERBOSE
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGV(format, ...) OMEGA_LOGV(format, ##__VA_ARGS__)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGV(format, ...) OMEGA_HEX_LOGV(format, ##__VA_ARGS__)
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGV(format, ...)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGV(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_DEBUG
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD(format, ...) OMEGA_LOGD(format, ##__VA_ARGS__)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGD(format, ...) OMEGA_HEX_LOGD(format, ##__VA_ARGS__)
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD(format, ...)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGD(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_INFO
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGI(format, ...) OMEGA_LOGI(format, ##__VA_ARGS__)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGI(format, ...) OMEGA_HEX_LOGI(format, ##__VA_ARGS__)
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGI(format, ...)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGI(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_WARN
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGW(format, ...) OMEGA_LOGW(format, ##__VA_ARGS__)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGW(format, ...) OMEGA_HEX_LOGW(format, ##__VA_ARGS__)
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGW(format, ...)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGW(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_ERROR
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE(format, ...) OMEGA_LOGE(format, ##__VA_ARGS__)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGE(format, ...) OMEGA_HEX_LOGE(format, ##__VA_ARGS__)
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE(format, ...)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_HEX_LOGE(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_START(name) PROFILE_START(name)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_END() PROFILE_END()
#else
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_START(name)
#define OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_END()
#endif

/*
    {"id":xxxx, "time":xxxxxx, "status": 0, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
    {"id":xxxx, "time":xxxxxx, "status": 1, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
    {"id":xxxx, "time":xxxxxx, "status": 2, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
*/
#define PROFILE_FORMAT "{\"id\":%llu,\"time\":%llu,\"status\":%u,\"file\":\"%s\",\"func\":\"%s\",\"line\":%u,\"message\":\"%s\"}\r\n"
#define EVENT_LOG_FORMAT "{\"id\":%llu,\"time\":%llu,\"status\":%u,\"file\":\"%s\",\"func\":\"%s\",\"line\":%u,\"message\":\"%s\"}\r\n"
#define DATA_LOG_FORMAT "%s\r\n"

void OmegaLoggingSystemController_log_event(EventTypes type, const char *file_name, const char *function_name, const size_t line_number, const char *format, ...)
{
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_START(log_event);
    va_list args;
    va_start(args, format);
    size_t message_size = snprintf(NULL, 0, format, args);
    va_end(args);
    char *message_buffer = (char *)omega_malloc(message_size + 1);
    if (message_buffer == NULL)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Event Logger string allocation failed");
        goto ret;
    }
    (void)sprintf(message_buffer, format, args);
    size_t total_size, used_size;
    esp_err_t err = esp_spiffs_info(NULL, &total_size, &used_size);
    if (err != ESP_OK)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("esp_spiffs_info err: %d", err);
        goto clean_message_buffer;
    }
    const float used_percent = (float)used_size * 100.0f / (float)total_size;
    OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD("spiffs info:(%.2f%%) %d/%d", used_percent, used_size, total_size);
    if (used_percent > 90.0f)
    {
        // FileHandle handle = OmegaFileSystemController_open_file("/storage/log.txt", eREADING);
        // UNUSED(OmegaFileSystemController_get_file_size(handle));
        // uint64_t line_count = 0;
        // constexpr size_t buf_len = 500;
        // char buf[buf_len];
        // size_t read_size;
        // while ((read_size = OmegaFileSystemController_read_file(handle, eREAD_LINE, (uint8_t *)buf, buf_len)) > 0)
        //     ++line_count;
        // OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Line count: %llu %s", line_count, buf);
        // UNUSED(OmegaFileSystemController_close_file(handle));
        goto clean_message_buffer;
    }
    static uint64_t id = 0;
    uint64_t current_time_us = esp_timer_get_time();
    message_size = snprintf(NULL, 0, PROFILE_FORMAT, id, current_time_us, type, file_name, function_name, line_number, message_buffer);
    char *logging_buffer = (char *)omega_malloc(message_size + 1);
    if (logging_buffer == NULL)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Event Logger string allocation failed");
        goto clean_message_buffer;
    }
    UNUSED(sprintf(logging_buffer, PROFILE_FORMAT, id++, current_time_us, type, file_name, function_name, line_number, message_buffer));
    OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD("%s", logging_buffer);
    size_t written_length;
    message_size = strlen(logging_buffer);
    FileHandle handle = OmegaFileSystemController_open_file("/storage/log_event.txt", eAPPEND);
    OmegaFileSystemController_write_file(handle, (uint8_t *)logging_buffer, message_size);
    UNUSED(OmegaFileSystemController_close_file(handle));
    omega_free(logging_buffer);
clean_message_buffer:
    omega_free(message_buffer);
ret:
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_END();
}

void OmegaLoggingSystemController_logf_data(const char *format, ...)
{
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_START(logf_data);
    va_list args;
    va_start(args, format);
    size_t message_size = snprintf(NULL, 0, format, args);
    va_end(args);
    char *message_buffer = (char *)omega_malloc(message_size + 1);
    if (message_buffer == NULL)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Event Logger string allocation failed");
        goto ret;
    }
    (void)sprintf(message_buffer, format, args);
    size_t total_size, used_size;
    esp_err_t err = esp_spiffs_info(NULL, &total_size, &used_size);
    if (err != ESP_OK)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("esp_spiffs_info err: %d", err);
        goto clean_message_buffer;
    }
    const float used_percent = (float)used_size * 100.0f / (float)total_size;
    OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD("spiffs info:(%.2f%%) %d/%d", used_percent, used_size, total_size);
    if (used_percent > 90.0f)
    {
        // FileHandle handle = OmegaFileSystemController_open_file("/storage/log_data.txt", eREADING);
        // UNUSED(OmegaFileSystemController_get_file_size(handle));
        // uint64_t line_count = 0;
        // constexpr size_t buf_len = 500;
        // char buf[buf_len];
        // size_t read_size;
        // while ((read_size = OmegaFileSystemController_read_file(handle, eREAD_LINE, (uint8_t *)buf, buf_len)) > 0)
        //     ++line_count;
        // OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Line count: %llu %s", line_count, buf);
        // UNUSED(OmegaFileSystemController_close_file(handle));
        goto clean_message_buffer;
    }
    FileHandle handle = OmegaFileSystemController_open_file("/storage/log.txt", eAPPEND);
    size_t written_length = 0;
    OmegaFileSystemController_write_file(handle, (uint8_t *)message_buffer, message_size);
    UNUSED(OmegaFileSystemController_close_file(handle));
clean_message_buffer:
    omega_free(message_buffer);
ret:
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_END();
}

void OmegaLoggingSystemController_log_data(const char *data, const size_t data_length)
{
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_START(log_data);
    size_t total_size, used_size;
    esp_err_t err = esp_spiffs_info(NULL, &total_size, &used_size);
    if (err != ESP_OK)
    {
        OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("esp_spiffs_info err: %d", err);
        goto ret;
    }
    const float used_percent = (float)used_size * 100.0f / (float)total_size;
    OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGD("spiffs info:(%.2f%%) %d/%d", used_percent, used_size, total_size);
    if (used_percent > 90.0f)
    {
        // FileHandle handle = OmegaFileSystemController_open_file("/storage/log_data.txt", eREADING);
        // UNUSED(OmegaFileSystemController_get_file_size(handle));
        // uint64_t line_count = 0;
        // constexpr size_t buf_len = 500;
        // char buf[buf_len];
        // size_t read_size;
        // while ((read_size = OmegaFileSystemController_read_file(handle, eREAD_LINE, (uint8_t *)buf, buf_len)) > 0)
        //     ++line_count;
        // OMEGA_LOGGING_SYSTEM_CONTROLLER_LOGE("Line count: %llu %s", line_count, buf);
        // UNUSED(OmegaFileSystemController_close_file(handle));
        goto ret;
    }
    FileHandle handle = OmegaFileSystemController_open_file("/storage/log.txt", eAPPEND);
    size_t written_length = 0;
    OmegaFileSystemController_write_file(handle, (uint8_t *)data, data_length);
    UNUSED(OmegaFileSystemController_close_file(handle));
ret:
    OMEGA_LOGGING_SYSTEM_CONTROLLER_PROFILE_END();
}