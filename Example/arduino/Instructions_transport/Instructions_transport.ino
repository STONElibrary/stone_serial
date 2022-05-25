#include "stone.h"

extern unsigned char receive_over_flage;
extern recive_group STONER;

int times = 500;
char buzzer_time[10];

void setup()
{
   Serial.begin(115200);
   
}

void loop() {

    delay(1000);
    set_sys("sys_hello");
    serial_receive();

    delay(1000);
    set_sys("sys_version");
    serial_receive();

    delay(1000);
    sprintf(buzzer_time,"%d",times);
    set_buzzer(buzzer_time);
    serial_receive();

    

        
}
