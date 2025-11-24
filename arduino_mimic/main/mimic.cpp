#include "Arduino.h"
#include "esp_task_wdt.h"
#include "freertos/task.h"

namespace arduino_mimic {
void setup();
void loop();
} // namespace arduino_mimic

TaskHandle_t loopTaskHandle = NULL;
bool loopTaskWDTEnabled;

void loopTask(void *pvParameters) {
  arduino_mimic::setup();
  for (;;) {
    if (loopTaskWDTEnabled) {
      esp_task_wdt_reset();
    }
    arduino_mimic::loop();
  }
}

extern "C" void app_main() {
  loopTaskWDTEnabled = false;
  initArduino();
  xTaskCreateUniversal(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle,
                       CONFIG_ARDUINO_RUNNING_CORE);
}
