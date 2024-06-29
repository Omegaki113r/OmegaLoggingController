/**
 * @file OmegaLoggingSystemController.h
 * @author Omegaki113r
 * @date Saturday, 29th June 2024 3:45:31 am
 * @copyright Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaLoggingSystemController
 * File Name: OmegaLoggingSystemController.h
 * File Created: Saturday, 29th June 2024 3:45:31 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 29th June 2024 6:23:44 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#ifndef __OMEGA_LOGGING_CONTROLLER_H__
#define __OMEGA_LOGGING_CONTROLLER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

#include <esp_timer.h>

#include "OmegaFileSystemController.h"

#include <sdkconfig.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#if CONFIG_SPIRAM
#define omega_malloc(size) heap_caps_malloc(size, MALLOC_CAP_SPIRAM)
#define omega_realloc(prev_ptr, new_size) heap_caps_realloc(prev_ptr, new_size, MALLOC_CAP_SPIRAM)
#define omega_free(ptr) heap_caps_free(ptr)
#else
#define omega_malloc(size) malloc(size)
#define omega_realloc(prev_ptr, new_size) realloc(prev_ptr, new_size)
#define omega_free(ptr) free(ptr)
#endif

#define START_ESCAPE_LOGGING_PARAMETER "\033["
#define END_ESCAPE_LOGGIN_PARAMETER "\033[0m"
#define DETAILED_BACKGROUND_FOREGROUND_COLOR "48;5;250;38;5;232;1"
#define INFO_BACKGROUND_FOREGROUND_COLOR "48;5;2;38;5;0"
#define INFO_TEXT_COLOR "38;5;2"
#define ERROR_BACKGROUND_FOREGROUND_COLOR "48;5;1;38;5;0"
#define ERROR_TEXT_COLOR "38;5;1"
#define WARN_BACKGROUND_FOREGROUND_COLOR "48;5;202;38;5;0"
#define WARN_TEXT_COLOR "38;5;202"
#define DEBUG_BACKGROUND_FOREGROUND_COLOR "48;5;3;38;5;0"
#define DEBUG_TEXT_COLOR "38;5;3"
#define VERBOSE_BACKGROUND_FOREGROUND_COLOR "48;5;15;38;5;0"
#define VERBOSE_TEXT_COLOR "38;5;15"
#define PROFILE_BACKGROUND_FOREGROUND_COLOR "48;5;207;38;5;0"
#define PROFILE_TEXT_COLOR "38;5;207"
#define END_PARAMETER "m"

#define OMEGA_PLOGV(format, ...) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER VERBOSE_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [V] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER VERBOSE_TEXT_COLOR END_PARAMETER format END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#define OMEGA_PLOGD(format, ...) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER DEBUG_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [D] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER DEBUG_TEXT_COLOR END_PARAMETER format END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#define OMEGA_PLOGI(format, ...) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER INFO_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [I] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER INFO_TEXT_COLOR END_PARAMETER format END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#define OMEGA_PLOGW(format, ...) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER WARN_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [W] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER WARN_TEXT_COLOR END_PARAMETER format END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#define OMEGA_PLOGE(format, ...) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER ERROR_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [E] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER ERROR_TEXT_COLOR END_PARAMETER format END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, ##__VA_ARGS__)

#define PROFILE_START() const uint64_t start_time = esp_timer_get_time()
#define PROFILE_END(name) printf(START_ESCAPE_LOGGING_PARAMETER DETAILED_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " %s >> %s:%d " END_ESCAPE_LOGGIN_PARAMETER "" START_ESCAPE_LOGGING_PARAMETER PROFILE_BACKGROUND_FOREGROUND_COLOR END_PARAMETER " [P] " END_ESCAPE_LOGGIN_PARAMETER " " START_ESCAPE_LOGGING_PARAMETER PROFILE_TEXT_COLOR END_PARAMETER "%s >> %lluus" END_ESCAPE_LOGGIN_PARAMETER "\r\n", __func__, __FILE_NAME__, __LINE__, name, esp_timer_get_time() - start_time)

#define OMEGA_FLOGI(format, ...)                                                                                  \
    do                                                                                                            \
    {                                                                                                             \
        FileHandle handle = OmegaFileSystemController_open_file("/storage/log.h", eREADING | eWRITING | eAPPEND); \
        const size_t sz = strlen(format);                                                                         \
        char *buf = omega_malloc(sz + 1);                                                                         \
        if (buf == NULL)                                                                                          \
            return;                                                                                               \
        sprintf(buf, format, ##__VA_ARGS__);                                                                      \
        OmegaFileSystemController_write_file(handle, (uint8_t *)buf, sz);                                         \
        OmegaFileSystemController_close_file(handle);                                                             \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif