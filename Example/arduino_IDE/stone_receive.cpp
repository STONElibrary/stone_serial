#include "stone.h"


unsigned char STONE_RX_BUF[RX_LEN];
unsigned short STONE_RX_CNT=0;
const unsigned char frame_head[3]={'S','T','<'};//帧头标志数据
const unsigned char frame_end[3]={'>','E','T'};//帧尾标志数据
unsigned char receive_over_flage = 0;


void serial_receive() {
    if (stone_uart_get_flag(USER_UART))    //判断串口是否有读取到数据
    {
			#if MCU_ARDUINO || MCU_ESP || MCU_Raspberry_Pi
      stone_uart_read(USER_UART);                      //读取一个字节存放到STONE_RX_BUF缓冲区
      stone_print(STONE_RX_BUF[STONE_RX_CNT],HEX);   //以16进制格式打印一个字节数据
      stone_print(" ");                    //打印个空格方便观察
      #endif
      STONE_RX_CNT++;
    }
      #if MCU_ARDUINO || MCU_ESP
    else 
    {
      STONE_Delay(1);   //串口读取超时最大时间1ms
      if (stone_uart_get_flag(USER_UART))return;
    }
    #endif
			if (STONE_RX_CNT==1 && STONE_RX_BUF[STONE_RX_CNT-1] != 'S')illegal_date();//当缓冲区只有1个数据时判断
    else if (STONE_RX_CNT==2 && STONE_RX_BUF[STONE_RX_CNT-2] != 'S' && STONE_RX_BUF[STONE_RX_CNT-1] != 'T')illegal_date();//当缓冲区只有2个数据时判断
    else if (STONE_RX_CNT==3 && memcmp(STONE_RX_BUF,frame_head,3)!=0)illegal_date();//当缓冲区只有3个数据时判断
    else if (STONE_RX_CNT>11 && memcmp(STONE_RX_BUF+STONE_RX_CNT-5,frame_end,3)==0 && do_crc(STONE_RX_BUF, STONE_RX_CNT)==0)     //STONE_RX_CNT>13合法指令长度至少为14，后面两个表达式验证帧尾和CRC校验
      {
        #if MCU_ARDUINO
        stone_println("legitimate");
				#endif
        receive_over_flage = 1;
        #if MCU_ARDUINO && Instruction_parsing
        receive_parse();
        #endif
        STONE_RX_CNT=0;      //数据合法计数器清零
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

unsigned short do_crc(unsigned char *ptr, int len)   //CRC16_MODBUS计算
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
