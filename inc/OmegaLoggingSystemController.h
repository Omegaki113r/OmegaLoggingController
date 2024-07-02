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
 * Last Modified: Tuesday, 2nd July 2024 11:49:29 pm
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

    void OmegaLoggingSystemController_log_event(EventTypes type, const char *file_name, const char *function_name, const size_t line_number, const char *format, ...);
#define OMEGA_EVENTLOGD(format, ...) OmegaLoggingSystemController_log_event(eDEBUG, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGI(format, ...) OmegaLoggingSystemController_log_event(eINFO, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGW(format, ...) OmegaLoggingSystemController_log_event(eWARN, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)
#define OMEGA_EVENTLOGE(format, ...) OmegaLoggingSystemController_log_event(eERROR, __FILE_NAME__, __func__, __LINE__, format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif