#include "Arduino.h"

namespace arduino_mimic {
void setup() {
  // あなたのsetupコード
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // あなたのloopコード
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
} // namespace arduino_mimic
