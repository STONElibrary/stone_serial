/*
 *  This example shows how to receive a user-defined button value and save the received value into a custom variable
 */

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];

String my_name;
long my_status;

void setup()
{
   Serial.begin(115200);
}

uint8_t x = 0;
uint8_t clears;
void loop() {

  serial_receive();
  
  if (receive_over_flage == 1)
    {
      my_name = (char*)STONER.widget;
      
      my_status = STONER.long_value;
      Serial.print("name:");
      for (int wds=0;wds<STONER.len-1;wds++)
      Serial.write(my_name[wds]);
      Serial.print("\n");
      Serial.print("long value:");
      Serial.println(my_status);
      receive_over_flage = 0;
    }
  
}
