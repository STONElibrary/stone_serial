#ifndef _STONE_CONFIG_H__
#define _STONE_CONFIG_H__

#include "limits.h"

/* Select the development board master type */
#define MCU_STM32                  0
#define MCU_APM32                  0
#define MCU_ARDUINO                0
#define MCU_ESP                    0
#define MCU_Raspberry_Pi           0

#if MCU_STM32
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stm32f0xx_hal.h"     //Modify references to HAL library versions based on series types

#define STONE_Delay 			HAL_Delay
#define USER_UART 				huart1       			//Replace huart1 with the user's own UART handle
#define USER_USART_BASE 	USART1
extern UART_HandleTypeDef USER_UART;

#define stone_uart_get_flag(USER_UART)   (USER_UART.Instance == USER_USART_BASE)   //
#define stone_uart_read(USER_UART)  HAL_UART_Receive_IT(&USER_UART,&STONE_RX_BUF[STONE_RX_CNT],1)


#define transport_unfinished (transport_over_flage == 0)
/* The serial port sends data, here using DMA, which the user adjusts */
#define stone_Transmit(BUF, ...); \
																		memset(BUF+__VA_ARGS__,0,TX_LEN-__VA_ARGS__);\
																		HAL_UART_Transmit_DMA(&USER_UART, (uint8_t *)BUF, __VA_ARGS__);
#endif

#if MCU_APM32
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "apm32f10x_usart.h"


#define STONE_Delay       HAL_Delay
#define USER_UART         USART1            //Replace huart1 with the user's own UART handle

#define stone_uart_get_flag(USER_UART)   (USART_ReadStatusFlag(USER_UART, USART_FLAG_RXBNE) != RESET)   //
#define stone_uart_read(USER_UART)  STONE_RX_BUF[STONE_RX_CNT] = USART_RxData(USER_UART)


#define transport_unfinished (transport_over_flage == 0)
/* The serial port sends data, here using DMA, which the user adjusts */
#define stone_Transmit(BUF, ...); \
                                    memset(BUF+__VA_ARGS__,0,TX_LEN-__VA_ARGS__);\
                                    UART1_Transmit_DMA(__VA_ARGS__);
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
#define print_with_flash           1//Whether to enable the storage of fixed strings to be printed in Flash (enable saves memory and reduces running speed; disable improves running speed and occupies memory)
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

//Print instructions to parse data for Arduino observation******************************************
#define Instruction_parsing        1    //The default open

//Print data, used for Arduino debugging, will occupy memory after enabled********************************
#define print_recive_ALL           0    //Off by default

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
