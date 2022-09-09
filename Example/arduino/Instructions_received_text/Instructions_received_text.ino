/*
 *  This example shows how TO SAVE THE received text DATA into a CUSTOM ARRAY
 *  
 */

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];

String my_name;
char my_status[100];
int name_lum,status_lum;

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
      name_lum = strlen(STONER.widget);
      status_lum = STONER.len-name_lum-3; //The length of the text data is equal to the length of the data minus the length of the control name minus two quotes and a colon
      
      memset(my_status,NULL,100); //clear the last text
      memcpy(my_status,STONER.text,status_lum);
      Serial.print("name:");
      for (int wds=0;wds<name_lum;wds++)
      Serial.write(my_name[wds]);
      Serial.print("\n");
      Serial.print("text:");
      for (int tds=0;tds<status_lum;tds++)
      Serial.write(my_status[tds]);
      Serial.println();
      receive_over_flage = 0;
    }
  
}
