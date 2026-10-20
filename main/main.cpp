#include "controller.hpp"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
namespace {
struct DisconnectedOutputs : unlock::Outputs {
  void camera(bool on) override { ESP_LOGI("dry-run", "camera request=%d (no GPIO driver)", on); }
  void light(bool on) override { ESP_LOGI("dry-run", "light request=%d (no GPIO driver)", on); }
};
}
extern "C" void app_main() {
  DisconnectedOutputs outputs;
  unlock::Controller controller(outputs);
  ESP_LOGW("boot", "v0.1 scaffold: DISABLED, no BLE, no recognition, no wired outputs");
  for (;;) {
    controller.tick(static_cast<uint64_t>(esp_timer_get_time() / 1000));
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
