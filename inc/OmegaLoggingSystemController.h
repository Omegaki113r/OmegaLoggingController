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
 * Last Modified: Monday, 5th August 2024 11:16:02 am
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

#include "OmegaBaseLoggingController.h"

    typedef enum
    {
        eVERBOSE,
        eDEBUG,
        eINFO,
        eWARN,
        eERROR,
    } EventTypes;

#include <sdkconfig.h>

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_EVENT
    void OmegaLoggingSystemController_log_event(EventTypes type, const char *file_name, const char *function_name, const size_t line_number, const char *format, ...);
#define OMEGA_EVENTLOGD(format, ...) OmegaLoggingSystemController_log_event(eDEBUG, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGI(format, ...) OmegaLoggingSystemController_log_event(eINFO, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGW(format, ...) OmegaLoggingSystemController_log_event(eWARN, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGE(format, ...) OmegaLoggingSystemController_log_event(eERROR, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#else
#define OMEGA_EVENTLOGD(format, ...)
#define OMEGA_EVENTLOGI(format, ...)
#define OMEGA_EVENTLOGW(format, ...)
#define OMEGA_EVENTLOGE(format, ...)
#endif

#if CONFIG_OMEGA_LOGGING_SYSTEM_CONTROLLER_DATA
    void OmegaLoggingSystemController_logf_data(const char *, ...);
    void OmegaLoggingSystemController_log_data(const char *, size_t);
#define OMEGA_DATALOGF(format, ...) OmegaLoggingSystemController_logf_data(format, ##__VA_ARGS__)
#define OMEGA_DATALOG(data, data_len) OmegaLoggingSystemController_log_data(data, data_len)
#else
#define OMEGA_DATALOGF(format, ...)
#define OMEGA_DATALOG(data, len)
#endif

#ifdef __cplusplus
}
#endif

#endif