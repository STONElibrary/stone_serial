/*
 *  This example shows setting text with the SET_value and SET_text directives
 *  
 */


#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;

int times = 60;
char label_time[10];
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

void loop() {

    if (timer_flag == 1)   //Two instructions are sent once per second
    {
      sprintf(label_time,"%d",times);
      set_value("label", "label1", label_time, "%.2f"); //Sets the label in numeric format
      set_text("label", "label2", label_time);  //Sets the label in text format
      timer_flag = 0;
      (times==0)?times=60:times--;
    }
    
    serial_receive();   //Prints the received data
}

ISR(TIMER1_COMPA_vect){   //Timer 1 interrupt function

  timer_flag=1;
}
