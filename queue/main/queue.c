// 注意
// includeの順序によってはビルドが失敗する。FreeRTOS.hを最初にincludeすること。
#include "freertos/FreeRTOS.h"

#include "freertos/queue.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

// XIAO ESP32-C3 + Expansion Board
#define BUTTON_GPIO GPIO_NUM_3 // D1
#define LED_GPIO GPIO_NUM_10   // D10

#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "GPIO_EXAMPLE";

// キューを作成
static QueueHandle_t gpio_evt_queue = NULL;

// ISRハンドラ (IRAM_ATTRは必須)
static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// GPIO割り込みを処理するタスク ここがメインの仕事をしている
static void gpio_task_example(void *arg) {
  uint32_t io_num;
  TickType_t last_press_time = 0;
  const TickType_t debounce_delay = pdMS_TO_TICKS(50);

  while (1) {
    if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {

      // チャタリング対策
      TickType_t current_time = xTaskGetTickCount();
      if ((current_time - last_press_time) > debounce_delay) {
        // ボタンの状態を読み取る
        int button_state = gpio_get_level(io_num);
        ESP_LOGI(TAG, "Button GPIO[%" PRIu32 "] state: %d", io_num,
                 button_state);

        // ボタンの状態に応じてLEDを制御
        if (button_state == 0) {
          gpio_set_level(LED_GPIO, 1);
          ESP_LOGI(TAG, "LED ON");
        } else {
          gpio_set_level(LED_GPIO, 0);
          ESP_LOGI(TAG, "LED OFF");
        }

        last_press_time = current_time;
      }
    }
  }
}

void app_main(void) {
  // GPIOをリセット
  gpio_reset_pin(LED_GPIO);
  gpio_reset_pin(BUTTON_GPIO);

  gpio_config_t io_conf = {};

  // LED出力ピン設定
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = (1ULL << LED_GPIO);
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  gpio_config(&io_conf);

  // 初期状態: LEDを消灯
  gpio_set_level(LED_GPIO, 0);

  // ボタン入力ピン設定（割り込み付き）
  io_conf.intr_type = GPIO_INTR_ANYEDGE; // 両エッジで割り込み
  io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO);
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pull_up_en = 1; // プルアップ有効
  io_conf.pull_down_en = 0;
  gpio_config(&io_conf);

  // GPIO ISRサービスをインストール
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

  // ボタンGPIOにISRハンドラを追加
  gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, (void *)BUTTON_GPIO);

  // キューを作成
  gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

  // タスクを開始
  xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
}
