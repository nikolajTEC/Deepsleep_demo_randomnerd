#include <Arduino.h>
#include "driver/rtc_io.h"

// Macro to convert a GPIO number into a 64-bit bitmask (1 shifted left by the GPIO number).
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

// Time ESP32 will go to sleep (in microseconds) -> 30 seconds
#define TIME_TO_SLEEP 30
#define uS_TO_S_FACTOR 1000000ULL  

struct ButtonPinMap {
  gpio_num_t  wakePin;
  uint8_t     outputPin;
  const char* label;
};

static const ButtonPinMap BUTTONS[] = {
  { GPIO_NUM_35, 19, "Meget glad" },
  { GPIO_NUM_34, 21, "Glad"       },
  { GPIO_NUM_39, 22, "Sur"        },
  { GPIO_NUM_36, 23, "Meget sur"  },
};

static const int BUTTON_COUNT = sizeof(BUTTONS) / sizeof(BUTTONS[0]);

uint64_t buildWakeupBitmask() {
  uint64_t mask = 0;
  for (int i = 0; i < BUTTON_COUNT; i++) {
    mask |= BUTTON_PIN_BITMASK(BUTTONS[i].wakePin);
  }
  return mask;
}

// Persists across deep sleep cycles
RTC_DATA_ATTR int bootCount = 0;

void handleWakeupButton() {
  uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();

  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (wakeup_pin_mask & BUTTON_PIN_BITMASK(BUTTONS[i].wakePin)) {
      pinMode(BUTTONS[i].outputPin, OUTPUT);
      digitalWrite(BUTTONS[i].outputPin, HIGH);
      delay(1000);
      digitalWrite(BUTTONS[i].outputPin, LOW);

      Serial.printf("Bedømmelse: %s registreret\n", BUTTONS[i].label);
      return; 
    }
  }
  Serial.println("Wakeup button unknown (non-button wakeup)");
}

void setup() {
  Serial.begin(115200);
  delay(100); // Small delay to let Serial stabilize after waking up
  
  bootCount++;

  // Get the reason why the ESP32 woke up
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT1:
      // Woke up by one of your feedback buttons
      handleWakeupButton();
      break;

    case ESP_SLEEP_WAKEUP_TIMER:
      // Woke up by the 30-second timer
      Serial.println("I'm alive");
      break;

    default:
      // First boot, power-on reset, or brownout
      Serial.printf("Boot number: %d\n", bootCount);
      break;
  }

  // 1. Enable button wakeup (EXT1)
  esp_sleep_enable_ext1_wakeup(buildWakeupBitmask(), ESP_EXT1_WAKEUP_ANY_HIGH);
  
  // 2. Enable timer wakeup (30 seconds)
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Go back to sleep. Whichever triggers first (button or 30s timer) will wake it up.
  Serial.println("Going asleep");
  esp_deep_sleep_start();
}

void loop() {
  // This never runs
}