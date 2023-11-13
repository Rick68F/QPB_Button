# QPB_Button (Queued Push Button)

This library is used for manage Push Button in a simple way, entry are put in a queue.
Only click and long click(time pushed is given).

I have designed this library I don't have fund a library that correspond to my use.

Working usage

One or more button are tracked push/release. with a timer (ESP32/RP2040/STM32) tested on only one microcontroleur type of each architecture.
When a pushbutton is down acquire it's duration, when release put the result in a queue.
If time is less the time/trigger --> nothing.
If time greather that trigger --> push assigned value in the queue and number of tics/trigger time

In the programm there is an init phase
one key table and a call
Reading result in a simple way same as Serial.

see Example

Rick

