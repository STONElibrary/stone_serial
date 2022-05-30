#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;

char* my_name;
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
      Serial.print("widget:");
      for (int wds=0;wds<STONER.len-1;wds++)
      Serial.write(my_name[wds]);
      Serial.print(";value:");
      Serial.println(my_status);
      
      receive_over_flage = 0;
    }
    
}
