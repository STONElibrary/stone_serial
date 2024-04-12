

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];
extern frame_link * frame_link_p;
String my_name;
float my_status;
frame_link * stone_user_link = frame_link_p;

void setup()
{
   Serial.begin(115200);
}


char temp_str[40];
char temp_value[10];
void loop() {

  serial_receive();
  
  if (receive_over_flage == 1)
    {

      my_name = (char*)STONER.widget;
      
      if(my_name=="edit1")
      {
        memcpy(temp_str,STONER.text,STONER.text_len);
        delay(500);
        write_line("test15.txt", temp_str);
        delay(10);
        set_text("label", "label1", "write_text_to_file");
        
      }
      
      else if(my_name=="button2")
      {
        download_file("test15.txt", "u_disk");
        delay(10);
        set_text("label", "label1", "download_to_udisk");
      }

      else if(my_name=="button3")
      {
        static int line=0;
        sprintf(temp_value,"%d",line);
        read_line("test15.txt", temp_value);
        (line>9)?(line=0):(line++);
      }

      if(control_read_line == STONER.cmd)
      {
        sprintf(temp_str,"%s",STONER.data);
        set_text("label", "label2", temp_str);
      }
        
      
      memset(temp_str,NULL,40);
      stone_recive_free();
      receive_over_flage = 0;
    }
  
}
