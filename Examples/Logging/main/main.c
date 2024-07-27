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
 * Last Modified: Saturday, 27th July 2024 7:33:43 am
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

#include <esp_log.h>
#include <esp_spiffs.h>

#include "OmegaBaseLoggingController.h"
#include "OmegaFileSystemController.h"
#include "OmegaLoggingSystemController.h"

void app_main(void)
{
    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/storage",
        .max_files = 5,
        // .partition_label = "storage",
        .format_if_mount_failed = false,
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffs_config));
    uint64_t i = 0;
    for (;;)
    {
        // OMEGA_LOGI("%llu", i++);
        PROFILE_START(event_logging);
        OMEGA_DATALOGF("%s\r\n", "hello");
        OMEGA_DATALOG("world\r\n", strlen("world\r\n"));
        PROFILE_END();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
