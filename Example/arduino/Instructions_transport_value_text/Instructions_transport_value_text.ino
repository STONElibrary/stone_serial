/*
 *  This example is used to test the use of commands to control the screen, in the program will be
 *  sent once a second to set the buzzer 500 milliseconds of the command and send
 *  communication detection commands, while maintaining the real-time return data function.
 *  
 */


#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;

int times = 500;
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

void loop() {

    if (timer_flag == 1)   //Two instructions are sent once per second
    {
      sprintf(buzzer_time,"%d",times);
      set_buzzer(buzzer_time); //Set the buzzer alarm duration on the screen
      
      set_sys("sys_hello");   //Check the communication status of the screen
      timer_flag = 0;
    }
    
    serial_receive();   //Prints the received data
}

ISR(TIMER1_COMPA_vect){   //Timer 1 interrupt function

  timer_flag = 1;
}
