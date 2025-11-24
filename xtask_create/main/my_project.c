#include <stdio.h>

#include "esp_log.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t loopTaskHandle = NULL;
bool loopTaskWDTEnabled;

void task1(void *pvParameters) {
  while (1) {
    for (int i = 0; i < 50; i++) {
      ESP_LOGI("task1", "Task 1 - Count: %d", i);
      vTaskDelay(1);
    }
  }
}

void task2(void *pvParameters) {
  while (1) {
    for (int i = 0; i < 50; i++) {
      ESP_LOGI("task2", "Task 2 - Count: %d", i);
      vTaskDelay(1);
    }
  }
}

void app_main(void) {
  loopTaskWDTEnabled = false;
  ESP_LOGI("main", "Hello, ESP-IDF!");

  xTaskCreate(task1, "task1", 8192, NULL, 2, NULL);
  xTaskCreate(task2, "task2", 8192, NULL, 1, NULL);
}
