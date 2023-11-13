/////////////////////////////////////////////////////////////////
/*
  QPB_Button.hpp - Simple Arduino Library for working with buttons.
*/
/////////////////////////////////////////////////////////////////

#pragma once

#ifndef QPB_Button_hpp
#define QPB_Button_hpp

/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <queue>

namespace QPB {  // Queue Push Button

#if defined(ARDUINO_ARCH_ESP32)
hw_timer_t *QPBtimer = NULL;
#elif defined(ARDUINO_ARCH_RP2040)
// nothing to do
#elif defined(ARDUINO_ARCH_STM32)
// nothing to do
#else
#error Hardware not supported now  --> timer
#endif

// @0 pin or GPIO
// @1 input mode INPUT or INPUT_PULLUP
// @2 logic state value when trigged
// @3 duration 5 = .5 s (number of timer interrupt (.1 s) before a valid Push)
// @4 number of tics or timer interrupt
// @5 value given in the queue for this key when released
//
struct QPB_Buttons {
  uint32_t pin;
  uint32_t mode;
  uint8_t logic;
  uint8_t duration;
  uint16_t tics;
  uint8_t value;
};

struct QPB_Key {
  uint8_t key;
  uint8_t howlong;
};

QPB_Buttons *PtrQPB;
uint8_t nbrQPB;
std::queue<QPB_Key> QPBQueue;


uint8_t QPBavailable() {
  return (QPBQueue.size());
}

QPB_Key QPBread() {
  QPB_Key QPB_values = QPBQueue.front();
  QPBQueue.pop();
  return ((QPB_Key)QPB_values);
}

#if defined(ARDUINO_ARCH_ESP32)
void IRAM_ATTR QPBonTimer() {
#elif defined(ARDUINO_ARCH_RP2040)
bool QPBonTimer(repeating_timer_t *) {
#elif defined(ARDUINO_ARCH_STM32)
void QPBonTimer() {
#endif

  QPB_Key k;
  for (uint8_t i; i < nbrQPB; i++) {
    if (digitalRead(QPB::PtrQPB[i].pin) == QPB::PtrQPB[i].logic) QPB::PtrQPB[i].tics++;
    else {
      if (QPB::PtrQPB[i].tics > QPB::PtrQPB[i].duration) {
        k.key = QPB::PtrQPB[i].value;
        k.howlong = QPB::PtrQPB[i].tics / QPB::PtrQPB[i].duration;
        QPBQueue.push(k);
      }
      QPB::PtrQPB[i].tics = 0;
    }
  }
#if defined(ARDUINO_ARCH_RP2040)
  return (true);
#endif
}

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_RP2040)
void QPB_init(QPB_Buttons *keys, uint8_t cpt, uint8_t timerid = 0) {  // timer 0 if not defined 0-3 are available on ESP32
#elif defined(ARDUINO_ARCH_STM32)
void QPB_init(QPB_Buttons *keys, uint8_t cpt, TIM_TypeDef *timerid = TIM1) {  // TIM1 or TIM2  ???
#endif

#if defined(ARDUINO_ARCH_ESP32)
  QPBtimer = timerBegin(timerid, 80, true);
  timerAttachInterrupt(QPBtimer, &QPB::QPBonTimer, true);
  timerAlarmWrite(QPBtimer, 10000, true);  // every 0.1 seconds
  timerAlarmEnable(QPBtimer);
#elif defined(ARDUINO_ARCH_RP2040)  // tim is not needed and ignored
  static repeating_timer_t Timer1ms;
  add_repeating_timer_ms(10, QPB::QPBonTimer, NULL, &Timer1ms);
#elif defined(ARDUINO_ARCH_STM32)
  HardwareTimer *stimer_t = new HardwareTimer(timerid);  // see https://github.com/stm32duino/wiki/wiki/HardwareTimer-library
  stimer_t->setOverflow(10000, MICROSEC_FORMAT);         //
  stimer_t->attachInterrupt(QPBonTimer);
  stimer_t->resume();
#endif

  PtrQPB = keys;
  nbrQPB = cpt;
  for (uint8_t i; i < nbrQPB; i++) {
    pinMode(QPB::PtrQPB[i].pin, QPB::PtrQPB[i].mode);
  }
}
}  // namespace
#endif

//