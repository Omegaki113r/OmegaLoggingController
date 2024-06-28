/**
 * @file main.c
 * @author Omegaki113r
 * @date Saturday, 29th June 2024 3:24:38 am
 * @copyright Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * */
/*
 * Project: Logging
 * File Name: main.c
 * File Created: Saturday, 29th June 2024 3:24:38 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 29th June 2024 3:52:11 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2024 - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "OmegaLoggingSystemController.h"

void app_main(void)
{
    uint64_t i = 0;
    for (;;)
    {
        PROFILE_START();
        OMEGA_LOGI("%llu", i++);
        vTaskDelay(pdMS_TO_TICKS(10));
        PROFILE_END(__func__);
    }
}
