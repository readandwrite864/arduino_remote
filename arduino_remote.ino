#include <Arduino.h>

// #define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
// #define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
// #define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

uint16_t sAddress = 0x0707;
uint8_t sCommand = 0x02;
uint8_t sRepeats = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);

  IrSender.begin(DISABLE_LED_FEEDBACK);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
  if (Serial.available() > 0)
  {
    sCommand = Serial.read();
    send();
    delay(6);
  }

  if (IrReceiver.decode())
  {
    print();
    delay(6);
    IrReceiver.resume();
  }
}

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

  IrSender.sendNEC(sAddress, sCommand, sRepeats);
}

void print()
{
  IrReceiver.printIRResultShort(&Serial);

  switch (IrReceiver.decodedIRData.command)
  {
  case 0x02:
    Serial.print("KEY_POWER");
    break;
  case 0x60:
    Serial.print("KEY_ARROW_UP");
    break;
  case 0x62:
    Serial.print("KEY_ARROW_RIGHT");
    break;
  case 0x61:
    Serial.print("KEY_ARROW_DOWN");
    break;
  case 0x65:
    Serial.print("KEY_ARROW_LEFT");
    break;
  case 0x68:
    Serial.print("KEY_ENTER");
    break;
  case 0x58:
    Serial.print("KEY_RETURN");
    break;
  case 0x2D:
    Serial.print("KEY_EXIT");
    break;
  case 0x6C:
    Serial.print("KEY_A");
    break;
  case 0x14:
    Serial.print("KEY_B");
    break;
  case 0x15:
    Serial.print("KEY_C");
    break;
  case 0x16:
    Serial.print("KEY_D");
    break;
  default:
    Serial.print("KEY_UNKNOWN");
    break;
  }

  Serial.println();
}
