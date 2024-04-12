/*
 * This example is a comparison of two methods of receiving data. 
 * receive1 is function is to read data from the serial buffer one byte at a time and determine until a frame of instructions is read, It is more suitable for simple situations. 
 * receive2 is used to read one byte of data from the serial buffer at a time until the buffer times out, If the buffer times out, the receive_over_flage flag will be increased by 1 for each successful frame,
 * Therefore, even if the data in STONE_RX_BUF is not preferentially processed in the program, it will not be replaced by new data.
*/

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];

String my_name;
float my_status;

void setup()
{
   Serial.begin(115200);
   get_date("clock1");
}

char tempc[50];

void loop() {

  receive2();
  
}

void receive1(){

  serial_receive();
  
  if (receive_over_flage == 1)
    {
      my_name = (char*)STONER.widget;
   
      my_status = STONER.float_value;

      Serial.print("widget_len:");
      Serial.println(STONER.widget_len);
      Serial.print("widget:");
      for (int wds=0;wds<STONER.widget_len;wds++)
      Serial.write(STONER.widget[wds]);
      Serial.print("\n");

      Serial.print("value:");
      Serial.println(STONER.value);
      
      Serial.print("float value:");
      Serial.println(STONER.float_value);

      Serial.print("long value:");
      Serial.println((int)STONER.long_value);

      Serial.print("data_len:");
      Serial.println(STONER.data_len);
      Serial.print("data:");
      for (int wds=0;wds<STONER.data_len;wds++)
      Serial.write(STONER.data[wds]);
      Serial.print("\n");
      

      Serial.print("text_len:");
      Serial.println(STONER.text_len);
      Serial.print("text:");
      for (int wds=0;wds<STONER.text_len;wds++)
      Serial.write(STONER.text[wds]);
      Serial.print("\n");

      if(my_name=="slider1")
      {
        sprintf(tempc,"%d",(int)STONER.float_value);
        set_brightness(tempc);
      }

      receive_over_flage = 0;
//      stone_recive_free();
    }
}

frame_link * user_link = frame_link_p;
void receive2(){

  serial_receive_frame_idel();
  
  while (receive_over_flage >= 1)
    {
      user_link=user_link->next;
      receive_parse_fifo(user_link->base_offset);
      
      my_name = (char*)STONER.widget;
      my_status = STONER.float_value;

      Serial.print("widget_len:");
      Serial.println(STONER.widget_len);
      Serial.print("widget:");
      for (int wds=0;wds<STONER.widget_len;wds++)
      Serial.write(STONER.widget[wds]);
      Serial.print("\n");

      Serial.print("value:");
      Serial.println(STONER.value);
      
      Serial.print("float value:");
      Serial.println(STONER.float_value);

      Serial.print("long value:");
      Serial.println((int)STONER.long_value);

      Serial.print("data_len:");
      Serial.println(STONER.data_len);
      Serial.print("data:");
      for (int wds=0;wds<STONER.data_len;wds++)
      Serial.write(STONER.data[wds]);
      Serial.print("\n");
      

      Serial.print("text_len:");
      Serial.println(STONER.text_len);
      Serial.print("text:");
      for (int wds=0;wds<STONER.text_len;wds++)
      Serial.write(STONER.text[wds]);
      Serial.print("\n");

      if(my_name=="slider1")
      {
        sprintf(tempc,"%d",(int)STONER.float_value);
        set_brightness(tempc);
      }

      frame_link_p = delElem(frame_link_p);
      if(frame_link_p->frame_count==0 && frame_link_p->next==NULL)
      user_link = frame_link_p;
      receive_over_flage--;
//      stone_recive_free();
    }
}
