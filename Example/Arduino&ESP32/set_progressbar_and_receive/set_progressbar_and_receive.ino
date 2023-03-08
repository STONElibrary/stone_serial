/*
 *  This example can control the progress bar, but also receive the correct button press command
 */

#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;
extern unsigned char STONE_RX_BUF[RX_LEN];

extern frame_link * frame_link_p;

String my_name;
float my_status;
int button_status;

int times = 0;
char buzzer_time[10];
char timer_flag = 0;

void setup()
{
  /*  Timer 1 Configuration  */
    TCCR1A = 0;
    TCCR1B = 1 << CS12 | 1 << CS10;   //1024fractional frequency  16000000/1024 = 15625HZ = 64us
    TCCR1B |= 1 << WGM13 | 1 << WGM12;   //MODE:  CTC
    ICR1 = 15625;   //Reloading the value   64*15625 = 1S
    TIMSK1 |= (1 << OCIE1A);  //Enable the capture interrupt of timer 1
    
   Serial.begin(115200);
}

uint8_t x = 0;
uint8_t clears;

int count_press=0;

frame_link * user_link = frame_link_p;

void loop() {

if (timer_flag == 1)   //Two instructions are sent once per second
    {
      times+=100;
  if (times>2000)times = 100;
  sprintf(buzzer_time ,"%d",times);
  set_value("progress_bar", "pb_pwr_1", buzzer_time);
      timer_flag = 0;
    }
    
  serial_receive_frame_idel();
  
  while (receive_over_flage >= 1)
    {
      user_link=user_link->next;
      receive_parse_fifo(user_link->base_offset);
      
      my_name = (char*)STONER.widget;
      
      my_status = STONER.float_value;
      button_status = STONER.value;
//      Serial.print("name:");
//      for (int wds=0;wds<STONER.len-1;wds++)
//      Serial.write(my_name[wds]);
//      Serial.print("\n");
//      Serial.print("int value:");
//      Serial.println(button_status);
//      Serial.print("float value:");
//      Serial.println(my_status);
      
      if(button_status == 1)
      {
      count_press++;
      Serial.print("count_press");
      Serial.println(count_press);
      }

      button_status=STONER.value=0;

      frame_link_p = delElem(frame_link_p);
      if(frame_link_p->frame_count==0 && frame_link_p->next==NULL)
      user_link = frame_link_p;
      receive_over_flage--;
    }
  
}

ISR(TIMER1_COMPA_vect){   //Timer 1 interrupt function

  timer_flag = 1;
}
