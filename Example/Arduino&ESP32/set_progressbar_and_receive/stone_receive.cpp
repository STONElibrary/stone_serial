/*
 * @file        stone_receive.h
 * 
 * @version     V1.1.2
 * 
 * @date        2024-4-10
 * 
 * @produce   STONE Technologies
 * 
 */
 
#include "stone.h"


unsigned char STONE_RX_BUF[RX_LEN];
unsigned short STONE_RX_CNT,STONE_RX_CNT2;
unsigned char receive_over_flage = 0;
unsigned char frame_head_end[6] = {'S','T','<','>','E','T'};
int frame_base_sub;

frame_link * initLink(void){
    frame_link * temp = (frame_link*)malloc(sizeof(frame_link));
    temp->base_offset = 0;
    temp->frame_count = 0;
    temp->next = NULL;

    return temp;
}

/*If the compiler does not support calling a function in a constant expression, 
call frame_link_p=initLink() in a program startup function;*/
#if (MCU_ARDUINO || MCU_ESP)
frame_link * frame_link_p=initLink();
#else
frame_link * frame_link_p=NULL;
#endif

frame_link * additional(frame_link * p, int base_offset) {
    frame_link * temp = p;
    frame_link * c = (frame_link*)malloc(sizeof(frame_link));
    c->base_offset = base_offset;
    c->next = NULL;
    while(temp->next)
    temp = temp->next;
    temp->next = c;
    p->frame_count++;
    
    return p;
}

frame_link * delElem(frame_link * p) {
    frame_link * temp = p;
    frame_link * del = NULL;
//    while(p->frame_count)
//    {
      del = temp->next;
      temp->next = temp->next->next;
      p->frame_count--;
      free(del);
//    }
    return p;
}

#if (MCU_ARDUINO || MCU_ESP)
unsigned long receive_idel_time;
#define illegal_date(LEN) Serial.println("illegal");\
                          memset(STONE_RX_BUF,'\0',LEN);\
                          receive_over_flage = STONE_RX_CNT = 0;
                          
#define dequeue_date(END) if(END>0)Serial.println("illegal");\
                          STONE_RX_BUF[0] = STONE_RX_BUF[END];\
                          STONE_RX_CNT = 1;
#else
#define illegal_date(LEN) memset(STONE_RX_BUF,'\0',LEN);\
                          receive_over_flage = STONE_RX_CNT = 0;

#define dequeue_date(END) for(int i=0;i<STONE_RX_CNT-END;i++)\
                          STONE_RX_BUF[0] = STONE_RX_BUF[END];\
                          STONE_RX_CNT = 1;
#endif


/**
 * @method serial_receive
 * @export none
 */
void serial_receive() {
    if (stone_uart_get_flag(USER_UART))    //Check whether the serial port reads data
    {
      #if MCU_ARDUINO || MCU_ESP || MCU_Raspberry_Pi
      stone_uart_read(USER_UART);                      //Read a byte and store it in the STONE_RX_BUF buffer
      #endif
      
      STONE_RX_CNT2=++STONE_RX_CNT;
      if(STONE_RX_CNT==3 && STONE_RX_BUF[2] != '<'){dequeue_date(2);}
      if(STONE_RX_CNT==2 && STONE_RX_BUF[1] != 'T'){dequeue_date(1);}
      if(STONE_RX_CNT==1 && STONE_RX_BUF[0] != 'S'){dequeue_date(0);}
      
      #if MCU_ARDUINO || MCU_ESP || MCU_Raspberry_Pi
      Serial.print(STONE_RX_BUF[STONE_RX_CNT-1],HEX);   //Print a byte of data in hexadecimal format
      Serial.print(" ");                    //Print a space for easy viewing
      #endif
    }
      #if MCU_ARDUINO || MCU_ESP
    else 
    {
      STONE_Delay(1);   //Maximum timeout for serial reading the data is 1ms
      if (stone_uart_get_flag(USER_UART))return;
    }
    #endif
         if (STONE_RX_CNT>11 && STONE_RX_BUF[STONE_RX_CNT-5] == '>' 
                             && STONE_RX_BUF[STONE_RX_CNT-4] == 'E' 
                             && STONE_RX_BUF[STONE_RX_CNT-3] == 'T' 
                             && do_crc(STONE_RX_BUF, STONE_RX_CNT)==0)     //STONE_RX_CNT>11 The legal instruction length is at least 14, and the last two expressions verify the end of the frame and CRC checksum
      {
        #if MCU_ARDUINO || MCU_ESP
        Serial.println("legitimate");
        #endif
        receive_over_flage = 1;
        #if (MCU_ARDUINO || MCU_ESP) && Instruction_parsing
        receive_parse();
        #endif
        STONE_RX_CNT=0;      //Data legal counter is cleared
      }
    
    else if  (STONE_RX_CNT>=RX_LEN){illegal_date(RX_LEN);return;} 
}

/**
 * @method serial_receive
 * @export none
 */
int find_frame_ (unsigned char* find_)
{
  int num=0,len,ret=1;
  
  while (num <= STONE_RX_CNT2)
  {
    if (frame_head_end[2] == *(find_-4) && frame_head_end[1] == *(find_-5) && frame_head_end[0] == *(find_-6))
    {

      len = *(find_-1);
      len = len << 8 | *find_;
      
      if ((num+len+3+2) < STONE_RX_CNT2)
      {
        ret = memcmp((unsigned char*)(find_+len+1),(unsigned char*)(frame_head_end+3),3);
        
        if (ret == 0)
        {
            ret = do_crc(find_-6, len+3+2+2+3+2);
            if (ret == 0)
              frame_link_p = additional(frame_link_p, num-6);
        }
      }

    }
    num++;
    find_++;
  }
	STONE_RX_CNT2=0;
  if (frame_link_p->frame_count>0)
  return 0;
  else
  return -1;
}
/**
 * @method serial_receive_frame
 * @export none
 */
void serial_receive_frame(unsigned short CNT){

  STONE_RX_CNT = CNT;
	STONE_RX_CNT2 += CNT;
  
  if (STONE_RX_CNT2 > RX_LEN && STONE_RX_BUF[RX_LEN-3] != 'T')
  {
		STONE_RX_CNT2=0;
    illegal_date(RX_LEN);
    return;
  }
  unsigned char* find_frame = STONE_RX_BUF;
  frame_base_sub = find_frame_(find_frame);


  if (frame_base_sub == 0)
  {
    #if MCU_ARDUINO || MCU_ESP
        Serial.println("legitimate");
        #endif
        receive_over_flage=frame_link_p->frame_count;
        STONE_RX_CNT=0;
    }
  else
  {
    illegal_date(RX_LEN);
  }

}

void serial_receive_frame_idel(void){
  
#if MCU_ARDUINO || MCU_ESP

unsigned long receive_idel_time2;
  if (stone_uart_get_flag(USER_UART))
  {
    receive_idel_time = millis();
    while (1)
    {
          if (stone_uart_get_flag(USER_UART))    //Check whether the serial port reads data
          {
            stone_uart_read(USER_UART);                      //Read a byte and store it in the STONE_RX_BUF buffer
            Serial.print(STONE_RX_BUF[STONE_RX_CNT],HEX);   //Print a byte of data in hexadecimal format
            Serial.print(" ");                    //Print a space for easy viewing
            STONE_RX_CNT2=STONE_RX_CNT++;
          }

          else
          {
            receive_idel_time2 = millis();
            if (receive_idel_time2<=receive_idel_time)return; //overflow
            
            if ((receive_idel_time2 - receive_idel_time) > 10)
            break;
          }

    }
    serial_receive_frame(STONE_RX_CNT);
  }
  
#endif
  
}

/**
 * @method do_crc
 * CRC16_MODBUS
 * @param {unsigned char*}
 * @param {int}
 *
 * @return {unsigned short}
 */
unsigned short do_crc(unsigned char* ptr, int len)
{
    unsigned int i;
    unsigned short crc = 0xFFFF;
  
    unsigned char *crc_idx = ptr+len-2;
    unsigned char swap;
    swap = *crc_idx;
    *crc_idx = *(crc_idx + 1);
    *(crc_idx + 1) = swap;
    
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
    *(crc_idx + 1) = *crc_idx;
    *crc_idx = swap;
    return crc;
}
