#include "QPB_Button.hpp"

// @0 pin or GPIO
// @1 input mode INPUT or INPUT_PULLUP
// @2 logic state value when trigged
// @3 duration 5 = .5 s (number of timer interrupt (.1 s) before a valid Push)
// @4 number of tics or timer interrupt
// @5 value given in the queue for this key when released
//

#if defined(ARDUINO_ARCH_ESP32)
QPB::QPB_Buttons The_Keys_are[] = {
  { 12, INPUT, HIGH, 5, 0, '3' },
  { 13, INPUT, HIGH, 5, 0, '4' },
  { 14, INPUT, HIGH, 5, 0, '2' },
  { 32, INPUT, HIGH, 5, 0, '1' },
  { 35, INPUT, HIGH, 5, 0, '~' },
  { 0, INPUT_PULLUP, LOW, 5, 0, '0' }  // button Boot from ESP32 DEVKIT V1 - DOIT
};
#elif defined(ARDUINO_ARCH_RP2040)
QPB::QPB_Buttons The_Keys_are[] = {
  { 10, INPUT, HIGH, 5, 0, '1' },
  { 11, INPUT, HIGH, 5, 0, '2' },
  { 12, INPUT, HIGH, 5, 0, '3' },
  { 13, INPUT, HIGH, 5, 0, '4' },
  { 24, INPUT_PULLUP, LOW, 5, 0, '0' }  // button USR on RP2040 YD VCC-GND
};
#elif defined(ARDUINO_ARCH_STM32)
QPB::QPB_Buttons The_Keys_are[] = {
  { PA1, INPUT, HIGH, 5, 0, '1' },
  { PA2, INPUT, HIGH, 5, 0, '2' },
  { PA3, INPUT, HIGH, 5, 0, '3' },
  { PA4, INPUT, HIGH, 5, 0, '4' },
  { PA0, INPUT_PULLUP, LOW, 5, 0, '0' }  // button KEY WeActStudio BlackPill STM32F411CEU
};
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(3000);
  Serial.printf("Nbr of Keys %d\n", sizeof(The_Keys_are) / sizeof(The_Keys_are[0]));
  QPB::QPB_init(The_Keys_are, sizeof(The_Keys_are) / sizeof(The_Keys_are[0]));
}

void loop() {
  // put your main code here, to run repeatedly:
  if (QPB::QPBavailable()) {  // is a key in the queue ?
    QPB::QPB_Key Tkey = QPB::QPBread();
    Serial.printf("Value=%c howmanytime=%d\n", Tkey.key, Tkey.howlong);  // how many time the duration
  }
}

