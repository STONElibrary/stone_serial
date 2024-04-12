# stone_serial  
# Website: https://www.stoneitech.com/
# Arduino library: https://github.com/STONElibrary/arduino_lib/
![Image text](stone-display-hmi.png)

![Image text](stone-gui-software.png)


                            /* STONE serial library v1.1.2 Usage instructions */

This library is written in C language. Before using it, please check whether there is an Example of the same chip platform you are using under the example directory.
If the example is correct, then you can directly copy the stone_config.h,stone.h,stone_receive.c,stone_receive_group.c,stone_transport.c, above five files under the project to your project.
 
If the example does not meet the requirements, you need to manually modify the stone_config.h configuration file before copying it.
The stone_config.h file currently defines five initialization interfaces for the development board：
#define MCU_STM32                  0
#define MCU_APM32                  0
#define MCU_ARDUINO                1
#define MCU_ESP                    0
#define MCU_Raspberry_Pi           0

Each macro definition switch collectively renameds its chip's serial port receive and send functions to the built-in interface name of the stone library,
Such as:
#define stone_uart_get_flag(USER_UART)   (USER_UART.Instance == USER_USART_BASE)   //
#define stone_uart_read_byte(USER_UART)  HAL_UART_Receive_IT(&USER_UART,&STONE_RX_BUF[STONE_RX_CNT],1)
#define stone_uart_read_frame(USER_UART)  HAL_UARTEx_ReceiveToIdle_IT(&USER_UART, STONE_RX_BUF+STONE_RX_CNT2, RX_LEN)

Therefore, if you do not have the function interface of your chip in this file when you migrate, you can also refer to the macro STM32 or the macro ARDUINO definition to write your own interface such as "stone_uart_read_byte"

In addition, according to the previous user feedback, there are often customers who use other serial ports of arduino mega2560 or hardware serial ports of ESP32, 
but it does not work after the library is ported. The reason is that the macro MCU_ARDUINO and macro MCU_ESP use the default serial port 0 as the communication serial port of the library. 
So when you want to use another serial port, please remember to change the serial port definition at the following location in stone_config.h:
For example, when using Serial port 2 of arduino mega2560, change the following Serial to Serial2:

#define STONE_Delay(num)       delayMicroseconds(num*1000)
#define stone_uart_get_flag(USER_UART)   Serial2.available()
#define stone_uart_read(USER_UART)   (STONE_RX_BUF[STONE_RX_CNT]=Serial2.read())

#define transport_unfinished Serial2.availableForWrite()
#define stone_Transmit(BUF, ...); Serial2.println(("%s",BUF));
#define print_with_flash           1//Whether to enable the storage of fixed strings to be printed in Flash (enable saves memory and reduces running speed; disable improves running speed and occupies memory)
#define stone_print   Serial2.print
#define stone_printf(string)     ((print_with_flash) ? Serial2.print(F(string)) : Serial2.print(string))
#define stone_println   Serial2.println
#define stone_printfln(string)     ((print_with_flash) ? Serial2.println(F(string)) : Serial2.println(string))
#define stone_write     Serial2.write
#define stone_writef(string)     ((print_with_flash) ? Serial2.write(F(string)) : Serial2.write(string))

For example, when using HardwareSerial for ESP32, assume that your main program file has the following definition:
HardwareSerial MySerial(1);
Then the configuration should be changed to:

extern HardwareSerial MySerial;
#define STONE_Delay(num)       delayMicroseconds(num*1000)
#define stone_uart_get_flag(USER_UART)   MySerial.available()
#define stone_uart_read(USER_UART)   (STONE_RX_BUF[STONE_RX_CNT]=MySerial.read())

#define transport_unfinished MySerial.availableForWrite()
#define stone_Transmit(BUF, ...); MySerial.println(("%s",BUF));
#define print_with_flash           1//Whether to enable the storage of fixed strings to be printed in Flash (enable saves memory and reduces running speed; disable improves running speed and occupies memory)
#define stone_print   MySerial.print
#define stone_printf(string)     ((print_with_flash) ? MySerial.print(F(string)) : MySerial.print(string))
#define stone_println   MySerial.println
#define stone_printfln(string)     ((print_with_flash) ? MySerial.println(F(string)) : MySerial.println(string))
#define stone_write     MySerial.write
#define stone_writef(string)     ((print_with_flash) ? MySerial.write(F(string)) : MySerial.write(string))


The above method can solve the problem that occurs when you migrate the library.


The following describes some of the more important variables and function interfaces in the library.

void serial_receive();
This function is used for single-byte receive instructions. You need to call it every time the serial single-byte receive interrupt. It sets the value of the variable receive_over_flage to 1 every time it receives a correct instruction frame.
When you use this function to receive data, ensure that it is not blocked by blocking delays such as delay.

void serial_receive_frame_idel(void); and void serial_receive_frame(unsigned short CNT);
The purpose of these two function interfaces is to queue instructions, so that if your program has a higher priority event to handle, it can be called in a lower priority location, and it will not be disturbed by delay.
It increses receive_over_flage by 1 whenever it detects a correct instruction in the buffer.

typedef struct Recive{
    unsigned int        cmd;
    unsigned int        len;
    #if MCU_ARDUINO || MCU_ESP    //Arduino int 2bit
    long                value;
    float               float_value;
    unsigned long long  long_value;
    #else
    int                 value;
    float               float_value;
    unsigned long       long_value;
    #endif
    char       			*data;
    char       			*widget;
    char       			*text;
    int 				data_len;
    int					widget_len;
    int					text_len;

}recive_group;

This struct contains all types of variables in the instructions that result from parsing the instructions,You can see in the example for example:
Serial.print("value:");
Serial.println(STONER.value);

or

Serial.print("widget:");
for (int wds=0;wds<STONER.widget_len;wds++)
Serial.write(STONER.widget[wds]);
Serial.print("\n");

The above usage means that the corresponding member in the structure holds the parsed value in the instruction.

When you want to send instructions, you can refer to a large number of examples within the stone.h file, such as:
/* Command interface for the set value command */
// Call the example: set_value("slider", "slider2", "66");
// Call the example:  set_value("label", "label5", "66.6", "%.2f");
void set_value(char* _type, char* _name, char* _value, ...);


The above is the description of version 1.1.2. If you have any questions during use or bugs found during, please submit issues.