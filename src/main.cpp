#include <Arduino.h>

/*  
  Rui Santos & Sara Santos - Random Nerd Tutorials
  https://RandomNerdTutorials.com/learn-esp32-with-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include "driver/rtc_io.h"

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex   
#define WAKEUP_GPIO_36              GPIO_NUM_36
#define WAKEUP_GPIO_39              GPIO_NUM_39
#define WAKEUP_GPIO_34              GPIO_NUM_34
#define WAKEUP_GPIO_35              GPIO_NUM_35 

#define OUTPUT_GPIO_23 23 
#define OUTPUT_GPIO_22 22
#define OUTPUT_GPIO_21 21
#define OUTPUT_GPIO_19 19

uint64_t bitmask = BUTTON_PIN_BITMASK(WAKEUP_GPIO_36) | BUTTON_PIN_BITMASK(WAKEUP_GPIO_39) | BUTTON_PIN_BITMASK(WAKEUP_GPIO_34) | BUTTON_PIN_BITMASK(WAKEUP_GPIO_35);
RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_button() {
  uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
  if (wakeup_pin_mask & BUTTON_PIN_BITMASK(WAKEUP_GPIO_35)) {
     pinMode(OUTPUT_GPIO_19, OUTPUT);       // <-- added
    digitalWrite(OUTPUT_GPIO_19, HIGH);    // <-- added
    delay(1000);
    digitalWrite(OUTPUT_GPIO_19, LOW);
    Serial.println("Woken by Button 1 (GPIO 35)");
  } else if (wakeup_pin_mask & BUTTON_PIN_BITMASK(WAKEUP_GPIO_34)) {
         pinMode(OUTPUT_GPIO_21, OUTPUT);       // <-- added
    digitalWrite(OUTPUT_GPIO_21, HIGH);    // <-- added
    delay(1000);
    digitalWrite(OUTPUT_GPIO_21, LOW);
    Serial.println("Woken by Button 2 (GPIO 34)");
  } else if (wakeup_pin_mask & BUTTON_PIN_BITMASK(WAKEUP_GPIO_39)) {
     pinMode(OUTPUT_GPIO_22, OUTPUT);       // <-- added
    digitalWrite(OUTPUT_GPIO_22, HIGH);    // <-- added
    delay(1000);
    digitalWrite(OUTPUT_GPIO_22, LOW);
    Serial.println("Woken by Button 3 (GPIO 39)");
    } else if (wakeup_pin_mask & BUTTON_PIN_BITMASK(WAKEUP_GPIO_36)) {
           pinMode(OUTPUT_GPIO_23, OUTPUT);       // <-- added
    digitalWrite(OUTPUT_GPIO_23, HIGH);    // <-- added
    delay(1000);
    digitalWrite(OUTPUT_GPIO_23, LOW);
    Serial.println("Woken by Button 4 (GPIO 36)");
  } else {
    Serial.println("Wakeup button unknown (first boot or non-button wakeup)");
  }
}

void setup() {
  Serial.begin(115200);
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_button();

  //Use ext1 as a wake-up source
  esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void loop() {

}