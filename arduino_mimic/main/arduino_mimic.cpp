#include "Arduino.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "esp_task_wdt.h"
#include "freertos/task.h"

TaskHandle_t loopTaskHandle = NULL;

void my_setup();
void my_loop();

bool loopTaskWDTEnabled;
void loopTask(void *pvParameters) {
  my_setup();
  for (;;) {
    if (loopTaskWDTEnabled) {
      esp_task_wdt_reset();
    }
    my_loop();
  }
}

extern "C" void app_main() {
  loopTaskWDTEnabled = false;
  initArduino();

#if CONFIG_FREERTOS_NUMBER_OF_CORES > 1

  xTaskCreateUniversal(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle,
                       CONFIG_ARDUINO_RUNNING_CORE);

#else

  xTaskCreate(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle);

#endif
}

void my_setup() { ESP_LOGI("setup", "Hello"); }
void my_loop() {
  ESP_LOGI("loop", "World");
  delay(500);
}
