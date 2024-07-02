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
 * Last Modified: Tuesday, 2nd July 2024 1:40:25 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <esp_spiffs.h>

#include <cJSON.h>

#include "OmegaLoggingSystemController.h"
#include "OmegaUtilityDriver.h"

/*
    {"id":xxxx, "time":xxxxxx, "status": 0, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
    {"id":xxxx, "time":xxxxxx, "status": 1, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
    {"id":xxxx, "time":xxxxxx, "status": 2, "file":"ssss", "func":"ssss", "line":xxx, "message":"ssss"}\r\n
*/
#define PROFILE_FORMAT "{\"id\":%llu,\"time\":%llu,\"status\":%u,\"file\":\"%s\",\"func\":\"%s\",\"line\":%u,\"message\":\"%s\"}\r\n"

void OmegaLoggingSystemController_log_event(EventTypes type, const char *file_name, const char *function_name, const size_t line_number, const char *format, ...)
{
    PROFILE_START(__func__);
    va_list args;
    va_start(args, format);
    size_t message_size = snprintf(NULL, 0, format, args);
    va_end(args);
    char *message_buffer = (char *)omega_malloc(message_size + 1);
    if (message_buffer == NULL)
    {
        OMEGA_LOGE("Event Logger string allocation failed");
        return;
    }
    (void)sprintf(message_buffer, format, args);
    size_t total_size, used_size;
    esp_err_t err = esp_spiffs_info(NULL, &total_size, &used_size);
    if (err != ESP_OK)
    {
        OMEGA_LOGE("esp_spiffs_info err: %d", err);
        return;
    }
    const float used_percent = (float)used_size * 100.0f / (float)total_size;
    OMEGA_LOGD("spiffs info:(%.2f%%) %d/%d", used_percent, used_size, total_size);
    if (used_percent > 10.0f)
    {
        FileHandle handle = OmegaFileSystemController_open_file("/storage/log.txt", eREADING);
        UNUSED(OmegaFileSystemController_get_file_size(handle));
        uint64_t line_count = 0;
        constexpr size_t buf_len = 500;
        char buf[buf_len];
        size_t read_size;
        while ((read_size = OmegaFileSystemController_read_file(handle, eREAD_LINE, (uint8_t *)buf, buf_len)) > 0)
            ++line_count;
        OMEGA_LOGE("Line count: %llu %s", line_count, buf);
        UNUSED(OmegaFileSystemController_close_file(handle));
    }
    static uint64_t id = 0;
    uint64_t current_time_us = esp_timer_get_time();
    message_size = snprintf(NULL, 0, PROFILE_FORMAT, id, current_time_us, type, file_name, function_name, line_number, message_buffer);
    char *logging_buffer = (char *)omega_malloc(message_size + 1);
    if (logging_buffer == NULL)
    {
        OMEGA_LOGE("Event Logger string allocation failed");
        return;
    }
    UNUSED(sprintf(logging_buffer, PROFILE_FORMAT, id++, current_time_us, type, file_name, function_name, line_number, message_buffer));
    OMEGA_LOGD("%s", logging_buffer);
    size_t written_length;
    message_size = strlen(logging_buffer);
    FileHandle handle = OmegaFileSystemController_open_file("/storage/log.txt", eWRITING | eAPPEND);
    if ((written_length = OmegaFileSystemController_write_file(handle, (uint8_t *)logging_buffer, message_size)) != message_size)
        OMEGA_LOGW("Expected to write: %u, Actually written: %u", strlen(logging_buffer), written_length);
    UNUSED(OmegaFileSystemController_close_file(handle));
    omega_free(logging_buffer);
    omega_free(message_buffer);
    PROFILE_END(__func__);
}