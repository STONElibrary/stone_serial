#ifndef _STONE_CONFIG_H__
#define _STONE_CONFIG_H__

#include "limits.h"

/* 选择开发板主控类型 */
#define MCU_STM32                  0
#define MCU_ARDUINO                1
#define MCU_ESP                    0
#define MCU_Raspberry_Pi           0

#if MCU_STM32
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stm32f0xx_hal.h"     //根据系列类型修改引用对应的hal库版本

#define STONE_Delay 			HAL_Delay
#define USER_UART 				huart1       			//将huart1替换成用户自己的UART句柄
#define USER_USART_BASE 	USART1
extern UART_HandleTypeDef USER_UART;

#define stone_uart_get_flag(USER_UART)   (USER_UART.Instance == USER_USART_BASE)   //
#define stone_uart_read(USER_UART)  HAL_UART_Receive_IT(&USER_UART,&STONE_RX_BUF[STONE_RX_CNT],1)


#define transport_unfinished (transport_over_flage == 0)
/* 串口发送数据,此处使用的DMA，用户自行调整 */
#define stone_Transmit(BUF, ...); \
																		memset(BUF+CNT,0,TX_LEN-__VA_ARGS__);\
																		HAL_UART_Transmit_DMA(&USER_UART, (uint8_t *)BUF, __VA_ARGS__);
#endif

#if MCU_ARDUINO || MCU_ESP

#include "Arduino.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "HardwareSerial.h"

#define STONE_Delay(num)       delayMicroseconds(num*1000)
#define stone_uart_get_flag(USER_UART)   Serial.available()
#define stone_uart_read(USER_UART)   (STONE_RX_BUF[STONE_RX_CNT]=Serial.read())

#define transport_unfinished Serial.availableForWrite()
#define stone_Transmit(BUF, ...); Serial.println(("%s",BUF));
#define print_with_flash           1//是否开启将要打印的固定字符串存储到flash中(开启后节省内存降低运行速度，关闭后提高运行速度占用内存)
#define stone_print   Serial.print
#define stone_printf(string)     ((print_with_flash) ? Serial.print(F(string)) : Serial.print(string))
#define stone_println   Serial.println
#define stone_printfln(string)     ((print_with_flash) ? Serial.println(F(string)) : Serial.println(string))
#define stone_write     Serial.write
#define stone_writef(string)     ((print_with_flash) ? Serial.write(F(string)) : Serial.write(string))
#endif

#if MCU_Raspberry_Pi
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include<wiringPi.h>
#include<wiringSerial.h>

#define STONE_Delay(num)       delay(num)
#define stone_Transmit(BUF, ...); serialPrintf(fd,STONE_TX_BUF);
#define stone_printf(...); serialPrintf(fd,STONE_TX_BUF);
#define stone_uart_get_flag(USER_UART)   serialDataAvail(fd)
#define stone_uart_read(USER_UART) serialGetchar(fd)
#endif

//打印指令解析数据，Arduino观察用******************************************
#define Instruction_parsing        1    //默认开启

//打印数据，Arduino调试用，开启后会占用内存********************************
#define print_recive_ALL           0    //默认关闭

#define print_recive_sys           0
#define print_recive_button        0
#define print_recive_switch        0
#define print_recive_check         0
#define print_recive_radio         0
#define print_recive_slider        0
#define print_recive_progress      0
#define print_recive_label         0
#define print_recive_edit          0
#define print_recive_selector      0
#define print_recive_image_value   0

#endif
