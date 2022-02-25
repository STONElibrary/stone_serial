#include "stone.h"

void setup()
{
   Serial.begin(115200);
   //测试返回数据
//    set_sys("sys_hello");
//    set_sys("sys_version");
//      set_sleep("true");
//      set_sleep("false");
//     set_enable("button1","true");
//    set_visible("button2","false");
//    set_coordinate("button3",40,-50);
    
}

void loop() {
    serial_receive();  
//    delay(1000);
//    set_enable("button1","true");
//    delay(1000);
//    set_enable("button2","false");
}
