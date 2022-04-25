#include "stone.h"


unsigned char STONE_RX_BUF[RX_LEN];
unsigned short STONE_RX_CNT=0;
const unsigned char frame_head[3]={'S','T','<'};//Frame header flag data
const unsigned char frame_end[3]={'>','E','T'};//End of frame flag data
unsigned char receive_over_flage = 0;


void serial_receive() {
    if (stone_uart_get_flag(USER_UART))    //Check whether the serial port reads data
    {
			#if MCU_ARDUINO || MCU_ESP || MCU_Raspberry_Pi
      stone_uart_read(USER_UART);                      //Read a byte and store it in the STONE_RX_BUF buffer
      stone_print(STONE_RX_BUF[STONE_RX_CNT],HEX);   //Print a byte of data in hexadecimal format
      stone_print(" ");                    //Print a space for easy viewing
      #endif
      STONE_RX_CNT++;
    }
      #if MCU_ARDUINO || MCU_ESP
    else 
    {
      STONE_Delay(1);   //Maximum timeout for serial reading the data is 1ms
      if (stone_uart_get_flag(USER_UART))return;
    }
    #endif
			if (STONE_RX_CNT==1 && STONE_RX_BUF[STONE_RX_CNT-1] != 'S')illegal_date();//Make a judgment when there is only 1 data in the buffer
    else if (STONE_RX_CNT==2 && STONE_RX_BUF[STONE_RX_CNT-2] != 'S' && STONE_RX_BUF[STONE_RX_CNT-1] != 'T')illegal_date();//Make a judgment when there are only 2 data in the buffer
    else if (STONE_RX_CNT==3 && memcmp(STONE_RX_BUF,frame_head,3)!=0)illegal_date();//Make a judgment when there are only 3 data in the buffer
    else if (STONE_RX_CNT>11 && memcmp(STONE_RX_BUF+STONE_RX_CNT-5,frame_end,3)==0 && do_crc(STONE_RX_BUF, STONE_RX_CNT)==0)     //STONE_RX_CNT>13 The legal instruction length is at least 14, and the last two expressions verify the end of the frame and CRC checksum
      {
        #if MCU_ARDUINO
        stone_println("legitimate");
				#endif
        receive_over_flage = 1;
        #if MCU_ARDUINO && Instruction_parsing
        receive_parse();
        #endif
        STONE_RX_CNT=0;      //Data legal counter is cleared
      }  
}

unsigned short illegal_date (){
  
   #if MCU_ARDUINO
   stone_println("illegal");
	#endif
    receive_over_flage = 0;
    STONE_RX_CNT=0;
    STONE_RX_BUF[0]='\0';
    return 0;
}

unsigned short do_crc(unsigned char *ptr, int len)   //CRC16_MODBUS calculation
{
    unsigned int i;
    unsigned short crc = 0xFFFF;
    unsigned char *crc_idx = ptr+len-2;
    unsigned char CRC_BUF[2];
    len = len-2;
    
    while(len--)
    {
        crc ^= *ptr++;      
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else 
                crc = (crc >> 1);
        }
    }
    CRC_BUF[0]=crc>>8;
    CRC_BUF[1]=crc&0xFF;
    return memcmp(crc_idx,CRC_BUF,2);
}
