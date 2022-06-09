/*
 *  This example is used to test the data sent by the Arduino receiving screen.
 *  When the correct data is received, 
 *  important data such as control name or value can be extracted.
 *  
 */

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];

unsigned char* my_name;
int my_status;

void setup()
{
   Serial.begin(115200);
}

void loop() {
  
  serial_receive();
  
  if (receive_over_flage == 1)
    {
      my_name = STONER.widget;
      my_status = STONER.value;
      Serial.print("name:");
      for (int wds=0;wds<STONER.len-1;wds++)
      Serial.write(my_name[wds]);
      Serial.print("\n");
      Serial.print("value:");
      Serial.println(my_status);

      receive_over_flage = 0;
    }
  
}
