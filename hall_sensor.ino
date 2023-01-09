#include "a1333.h"
A1333 hal = A1333();

void setup() {
  Serial.begin(115200);
  hal.begin(10);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  //  uint32_t eeprom_val = 0x00;
  //  if (hal.eepromRead(0x18, 0, eeprom_val) == 0x00)
  //  {
  //    Serial.println("Receive value: " + String(eeprom_val));
  //  }
  //  else
  //  {
  //    Serial.println("Read 0x18 value fail");
  //  }
  //
  //  delay(1000);
  //  eeprom_val = 0x00;
  //  if (hal.eepromRead(0x58, 0, eeprom_val) == 0x00)
  //  {
  //    Serial.println("Receive value: " + String(eeprom_val));
  //  }
  //  else
  //  {
  //    Serial.println("Read 0x58 value fail");
  //  }
  //delay(1000);

#if 0
  uint32_t wvalue = 3020427315;
  if (hal.writeEEPROM(0x19, wvalue) != 0x00)
  {
    Serial.println("Write " + String(wvalue) + " to 0x19 fail");
  }
  else
  {
    Serial.println("Write " + String(wvalue) + " to 0x19 success");
  }

  for (int i = 0x18; i <= 0x1F; i++)
  {
    uint32_t value = 0;
    if (hal.readEEPROM(i, value) != 0x00)
    {
      Serial.print("Read "  + String(i) + "Fail");
    }
    else
    {
      Serial.println("Read "  + String(i) + " " + String(value));
    }
  }
#endif

  while (1)
  {
    int16_t angle = hal.readEncoderAngle();
    Serial.println("Read angle: " + String(angle));
    delay(500);
  }
}
