#include <Arduino.h>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
// #define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
// #define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

uint16_t sAddress = 0x0707;
uint8_t sCommand = 0x02;
uint8_t sRepeats = 0;

void send()
{
  Serial.println();
  Serial.print(F("Send now: address=0x"));
  Serial.print(sAddress, HEX);
  Serial.print(F(", command=0x"));
  Serial.print(sCommand, HEX);
  Serial.print(F(", repeats="));
  Serial.print(sRepeats);
  Serial.println();

  Serial.println(F("Send standard NEC with 8 bit address"));
  Serial.flush();

  IrSender.sendNEC(sAddress, sCommand, sRepeats);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);

  IrSender.begin(DISABLE_LED_FEEDBACK);
}

void loop()
{
  if (Serial.available() > 0)
  {
    sCommand = Serial.read();
    send();
  }

  delay(6); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}
