#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_GPIO GPIO_NUM_3 // D1
#define LED_GPIO GPIO_NUM_10   // D10

static const char *TAG = "BUTTON_TEST";

void app_main(void) {
  // LED設定
  gpio_reset_pin(LED_GPIO);
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(LED_GPIO, 0);

  // ボタン設定
  gpio_reset_pin(BUTTON_GPIO);
  gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

  ESP_LOGI(TAG, "Polling mode - Press button on D1");

  int last_state = 1;
  while (1) {
    int current_state = gpio_get_level(BUTTON_GPIO);

    if (current_state != last_state) {
      ESP_LOGI(TAG, "Button state changed: %d", current_state);

      if (current_state == 0) {
        gpio_set_level(LED_GPIO, 1);
        ESP_LOGI(TAG, "LED ON");
      } else {
        gpio_set_level(LED_GPIO, 0);
        ESP_LOGI(TAG, "LED OFF");
      }

      last_state = current_state;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
