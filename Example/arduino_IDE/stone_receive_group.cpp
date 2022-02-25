/* The protocol sent by serial screen currently adopts hexadecimal format, 
 * which reduces the difficulty of parsing and processing burden of MCU of lower computer. 
 * Multi-byte data adopts the transmission mode of high order before low order after
 * frame header +  CMD   +   LEN   +    DATA            +     terminator  +  CRC
 * ST<   0x1068    0x0004   0x01 0x02 0x03 0x04        >ET        CRC16
 */
 
#include "stone.h"
 
extern unsigned char STONE_RX_BUF[200];
extern unsigned short STONE_RX_CNT; 
extern const unsigned char frame_head[3];
int widget_last_len;
int widget_len;

recive_group STONER;

/* 	Text type data memory allocation function, dynamically created, not automatically freed  */
/*	The stone_recive_free() interface can be called to free memory when the user is finished using it
 *	If the user does not release the memory, the memory space occupied last time will be released before the next use
 */
 /* Create memory space for "data" Pointers */
recive_group datainit(int len){
   if (STONER.data != NULL){
		 memset(STONER.widget, '\0', widget_last_len);		//If it is not empty, release it first
    free(STONER.data);
    STONER.data = NULL;
   }
  STONER.data = (unsigned char*)malloc(len*sizeof(char)+1);
	 widget_last_len = len;
  if (!STONER.data){
    #if print_recive_ALL
      stone_println("Memory request error");
    #endif
  }
  memset(STONER.data, '\0', len);
  return STONER;
}

 /* Create memory space for "widget" Pointers */
recive_group widgetinit(int len){
   if (STONER.widget != NULL){
		memset(STONER.widget, '\0', widget_last_len);
    free(STONER.widget);
    STONER.widget = NULL;
   }
  STONER.widget = (unsigned char*)malloc(len*sizeof(char)+1);
	widget_last_len = len;
  if (!STONER.widget){
    #if print_recive_ALL
      stone_println("Memory request error");
    #endif
  }
  memset(STONER.widget, '\0', len);
  return STONER;
}

 /* Create memory space for "text" Pointers */
recive_group textinit(int len){
   if (STONER.text != NULL){
		 memset(STONER.widget, '\0', widget_last_len);
    free(STONER.text);
    STONER.text = NULL;
   }
  STONER.text = (unsigned char*)malloc(len*sizeof(char)+1);
	 widget_last_len = len;
  if (!STONER.text){
    #if print_recive_ALL
      stone_println("Memory request error");
    #endif
  }
  memset(STONER.text, '\0', len);
  return STONER;
}

/* Manually release memory space */
// sample1：stone_recive_free("data"); //Specifies to free memory for STONE.data
// sample2：stone_recive_free();       //Free up all created memory
void _stone_recive_free(char* name){
  if (name != NULL){
		if (strcmp(name,"data")==0)
		{
			memset(STONER.data, '\0', widget_last_len);
			free(STONER.data);
			STONER.data = NULL;
		}
		else if (strcmp(name,"widget")==0)
		{
			memset(STONER.widget, '\0', widget_last_len);
			free(STONER.widget);
			STONER.widget = NULL;
		}
		else if (strcmp(name,"text")==0)
		{
			memset(STONER.text, '\0', widget_last_len);
			free(STONER.text);
			STONER.text = NULL;
		}
  }
  else{
    if (STONER.data != NULL){
		memset(STONER.data, '\0', widget_last_len);
    free(STONER.data);
    STONER.data = NULL;
   }
  if (STONER.widget != NULL){
		memset(STONER.widget, '\0', widget_last_len);
    free(STONER.widget);
    STONER.widget = NULL;
   }
  if (STONER.text != NULL){
		memset(STONER.text, '\0', widget_last_len);
    free(STONER.text);
    STONER.text = NULL;
   }
  }
}

/* Parsing functions that receive data */
void receive_parse (){

	/* Caches directives represented by CMD */
  STONER.cmd = (unsigned int)STONE_RX_BUF[3];
  STONER.cmd = STONER.cmd << 8 | STONE_RX_BUF[4];

	/* Cache the length LEN represents */
  STONER.len = (unsigned int)STONE_RX_BUF[5];
  STONER.len = STONER.len << 8 | STONE_RX_BUF[6];
  
  unsigned int data_i2 = 0;
  unsigned int temp_value = 0;
  
  switch (STONER.cmd){
    case sys_state: {
      STONER = datainit(STONER.len+1);     //Memory is allocated one byte too much
      sscanf((const char*)STONE_RX_BUF+7,"%[^>ET]",STONER.data);
			
      #if print_recive_sys || print_recive_ALL
      stone_print("type:sys_start;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";value:");
      stone_print(STONER.data[0],HEX);
      stone_println(";");
      #endif
    }break;
    case sys_hello: {
      STONER = datainit(STONER.len+1);
      sscanf((const char*)STONE_RX_BUF+7,"%[^>ET]",STONER.data);
			
      #if print_recive_sys || print_recive_ALL
      stone_print("type:sys_hello;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";value:");
      stone_print(STONER.data[0],HEX);
      stone_println(";");
      #endif
    }break;
    case sys_version: {
      STONER = datainit(STONER.len+1);
			memcpy(STONER.data,STONE_RX_BUF+7,STONER.len);
			
      #if print_recive_sys || print_recive_ALL
      stone_print("type:sys_version;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";value:");
			for (int wds=0;wds<STONER.len;wds++)
			stone_write(STONER.data[wds]);
      stone_println(";");
      #endif
    }break;
    case control_button: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
			STONER.value = STONE_RX_BUF[7+data_i2];
			
      #if print_recive_button || print_recive_ALL
      stone_print("type:button;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_button_u: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
      STONER.value = STONE_RX_BUF[7+data_i2];
			
      #if print_recive_button || print_recive_ALL
      stone_print("type:user_button;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_switch: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
			STONER.value = (int)STONE_RX_BUF[7+STONER.len-1];
			
      #if print_recive_switch || print_recive_ALL
      stone_print("type:switch;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_check_button: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
      STONER.value = STONE_RX_BUF[7+data_i2];
			
      #if print_recive_check || print_recive_ALL
      stone_print("type:check_button;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_radio_button: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
      STONER.value = STONE_RX_BUF[7+data_i2];
			
      #if print_recive_radio || print_recive_ALL
      stone_print("type:radio_button;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_radio_button_s: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-1);
      STONER.value = STONE_RX_BUF[7+data_i2];
			
      #if print_recive_radio || print_recive_ALL
      stone_print("type:radio_button_mcu_get;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-1;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
    }break;
    case control_slider: {
      STONER = widgetinit(STONER.len-3);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-4);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+7+STONER.len-4);
			
      #if print_recive_slider || print_recive_ALL
      stone_print("type:slider;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;
    case control_slider_over: {
      STONER = widgetinit(STONER.len-3);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-4);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+7+STONER.len-4);
			
      #if print_recive_slider || print_recive_ALL
      stone_print("type:slider_over;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;
    case control_progress_bar: {
      STONER = widgetinit(STONER.len-3);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-4);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+7+STONER.len-4);
			
      #if print_recive_progress || print_recive_ALL
      stone_print("type:progress_bar;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;
    case control_progress_bar_p: {
      STONER = widgetinit(STONER.len-3);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-4);
      temp_value = strlen((const char*)STONER.widget);
			
      #if print_recive_progress || print_recive_ALL
      stone_print("type:progress_bar;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";percent:");
      #endif
      for (data_i2=0; data_i2<STONER.len-temp_value; data_i2++)
      {
        if(sizeof(int) == 2){
          STONER.long_value = STONE_RX_BUF[7+temp_value+2+data_i2];
          STONER.long_value = STONER.long_value<<8;
          #if print_recive_progress || print_recive_ALL
          stone_write(STONER.long_value);
          #endif
        }
        else if (sizeof(int) == 4){
          STONER.value = STONE_RX_BUF[7+temp_value+data_i2];
          STONER.value = STONER.value<<8;
          #if print_recive_progress || print_recive_ALL
          stone_write(STONER.value);
          #endif
        }
        }
        #if print_recive_progress || print_recive_ALL
        stone_println(";");
        #endif
    }break;
    case control_label_text: {
      STONER = widgetinit(40);
			widget_len=sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
			temp_value = strlen((const char*)STONER.widget);
			STONER = textinit(STONER.len-temp_value-3);
			memcpy(STONER.text,STONE_RX_BUF+8+temp_value,STONER.len-temp_value-3);
			
      #if print_recive_label || print_recive_ALL
      stone_print("type:label_text;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-temp_value-3;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";text:");
			for (int wds=0;wds<STONER.len-temp_value-3;wds++)
			stone_write(STONER.test[wds]);
      stone_println(";");
      #endif
    }break;
    case control_label_value: {
      STONER = widgetinit(STONER.len-3);
			memcpy(STONER.widget,STONE_RX_BUF+7,STONER.len-4);
			temp_value = strlen((const char*)STONER.widget);
			STONER.float_value = write_hex_to_float(STONE_RX_BUF+7+temp_value);
			
      #if print_recive_label || print_recive_ALL
      stone_print("type:label_value;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;
    case control_edit_text: {
      STONER = widgetinit(40);
			widget_len=sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
			temp_value = strlen((const char*)STONER.widget);
			STONER = textinit(STONER.len-temp_value-3);
			memcpy(STONER.text,STONE_RX_BUF+8+temp_value,STONER.len-temp_value-3);
			
      #if print_recive_label || print_recive_ALL
      stone_print("type:edit_text;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-temp_value-3;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";text:");
			for (int wds=0;wds<STONER.len-temp_value-3;wds++)
			stone_write(STONER.test[wds]);
      stone_println(";");
      #endif
    }break;
    case control_edit_int: {
      STONER = widgetinit(STONER.len-3);
      #if print_recive_edit || print_recive_ALL
      stone_print("type:edit_int;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      #endif
      sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
      temp_value = strlen((const char*)STONER.widget);
      for (; data_i2<temp_value; data_i2++){
        #if print_recive_edit || print_recive_ALL
        stone_write(STONER.widget[data_i2]);
        #endif
      }
      #if print_recive_edit || print_recive_ALL
      stone_print(";int_value:");
      #endif
      for (data_i2=0; data_i2<STONER.len-temp_value-3; data_i2++)
      {
        if(sizeof(int) == 2){
          STONER.long_value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.long_value = STONER.long_value<<8;
          #if print_recive_edit || print_recive_ALL
          stone_write(STONER.long_value);
          #endif
        }
        else if (sizeof(int) == 4){
          STONER.value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.value = STONER.value<<8;
          #if print_recive_edit || print_recive_ALL
          stone_write(STONER.value);
          #endif
        }
        }
        #if print_recive_edit || print_recive_ALL
        stone_println(";");
        #endif
    }break;
    case control_text_selector_text: {
      STONER = widgetinit(40);
      #if print_recive_selector || print_recive_ALL
      stone_print("type:selector_text;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      #endif
      sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
      temp_value = strlen((const char*)STONER.widget);
      for (; data_i2<temp_value; data_i2++){
        #if print_recive_selector || print_recive_ALL
        stone_write(STONER.widget[data_i2]);
        #endif
      }
      STONER = textinit(STONER.len-temp_value-3);
      #if print_recive_selector || print_recive_ALL
      stone_print(";text:");
      #endif
			memcpy(STONER.text,STONE_RX_BUF+8+temp_value,STONER.len-temp_value-3);
        #if print_recive_selector || print_recive_ALL
        stone_println(";");
        #endif
    }break;
    case control_text_selector_value: {
      STONER = widgetinit(STONER.len-3);
      #if print_recive_selector || print_recive_ALL
      stone_print("type:selector_value;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      #endif
      sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
      temp_value = strlen((const char*)STONER.widget);
      for (; data_i2<temp_value; data_i2++){
        #if print_recive_selector || print_recive_ALL
        stone_write(STONER.widget[data_i2]);
        #endif
      }
      #if print_recive_selector || print_recive_ALL
      stone_print(";value:");
      #endif
      for (data_i2=0; data_i2<STONER.len-temp_value-3; data_i2++)
      {
        if(sizeof(int) == 2){
          STONER.long_value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.long_value = STONER.long_value<<8;
          #if print_recive_selector || print_recive_ALL
          stone_write(STONER.long_value);
          #endif
        }
        else if (sizeof(int) == 4){
          STONER.value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.value = STONER.value<<8;
          #if print_recive_selector || print_recive_ALL
          stone_write(STONER.value);
          #endif
        }
        }
        #if print_recive_selector || print_recive_ALL
        stone_println(";");
        #endif
    }break;
    case control_text_selector_num: {
      STONER = widgetinit(STONER.len-3);
      #if print_recive_selector || print_recive_ALL
      stone_print("type:selector_num;");      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      #endif
      sscanf((const char*)STONE_RX_BUF+8,"%[^\"]:",STONER.widget);
      temp_value = strlen((const char*)STONER.widget);
      for (; data_i2<temp_value; data_i2++){
        #if print_recive_selector || print_recive_ALL
        stone_write(STONER.widget[data_i2]);
        #endif
      }
      #if print_recive_selector || print_recive_ALL
      stone_print(";num:");
      #endif
      for (data_i2=0; data_i2<STONER.len-temp_value-3; data_i2++)
      {
        if(sizeof(int) == 2){
          STONER.long_value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.long_value = STONER.long_value<<8;
          #if print_recive_selector || print_recive_ALL
          stone_write(STONER.long_value);
          #endif
        }
        else if (sizeof(int) == 4){
          STONER.value = STONE_RX_BUF[8+temp_value+2+data_i2];
          STONER.value = STONER.value<<8;
          #if print_recive_selector || print_recive_ALL
          stone_write(STONER.value);
          #endif
        }
        }
        #if print_recive_selector || print_recive_ALL
        stone_println(";");
        #endif
    }break;
    case control_image_value: {
      STONER = widgetinit(STONER.len);
			memcpy(STONER.text,STONE_RX_BUF+7,STONER.len-4);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+7+STONER.len-4);
      #if print_recive_image_value || print_recive_ALL
      stone_print("type:image_value;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
			for (int wds=0;wds<STONER.len-4;wds++)
			stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;

  }
  
}

/* Calculator for transfer HEX data into float type */
float write_hex_to_float(unsigned char* buf)
{
unsigned char i = 0;
unsigned short int dat0;
unsigned short int dat1;
#if INT_MAX==32767
  long int temp_value = 0;
#elif INT_MAX==2147483647
  unsigned int temp_value = 0;
#endif
  temp_value = buf[i];
  temp_value <<= 8;
  
  temp_value |= buf[++i];
  temp_value <<= 8;
  
  temp_value |= buf[++i];
  temp_value <<= 8;
  
  temp_value |= buf[++i];
  
  dat0 = temp_value & 0x0000FFFF;
  dat1 = (temp_value >> 16) & 0x0000FFFF;
  HexFloat.buf[0] = dat0 & 0xFF;
  HexFloat.buf[1] = (dat0 >> 8) & 0xFF;
  HexFloat.buf[2] = dat1 & 0xFF;
  HexFloat.buf[3] = (dat1 >> 8) & 0xFF;
  
float fdata = HexFloat.flo;
return fdata;
}
