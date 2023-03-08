/*
 * @file        stone_receive_group.h
 * 
 * @version     V1.1.0
 * 
 * @date        2022-3-8
 * 
 * @produce   STONE Technologies
 * 
 */
 
 /* The protocol sent by serial screen currently adopts hexadecimal format, 
 * which reduces the difficulty of parsing and processing burden of MCU of lower computer. 
 * Multi-byte data adopts the transmission mode of high order before low order after
 * frame header +  CMD   +   LEN   +    DATA            +     terminator  +  CRC
 * ST<   0x1068    0x0004   0x01 0x02 0x03 0x04        >ET        CRC16
 */
 
#include "stone.h"
                         
extern unsigned char STONE_RX_BUF[RX_LEN];
extern unsigned short STONE_RX_CNT; 
extern const unsigned char frame_head[3];
unsigned char data_init_flag,widget_init_flag,text_init_flag = 0;
int widget_last_len;
int widget_len;
int base_offset;
recive_group STONER = {
  0,
  0,
  0,
  0,
  NULL,
  NULL,
  NULL
};

const char* m_static_str[] = {"button;","user_button;","switch;",
                               "check_button;",
                               "radio_button;",
                               "radio_button_mcu_get;",
                               "slider;",
                               "slider_over;",
                               "progress_bar;",
                               "label_text;",
                               "edit_text;",
                                ";percent:",
                                "edit_int;",
                                ";int_value:",
                                "selector_value;",
                                ";value:",
                                "selector_num;",
                                ";num:",
                                "spin_box_int;",
                                "selector_text;",
                                "spin_box_text;",
                                "label_value;",
                                "image_value;",
                                "spin_box_float;",
                                "combo_box_text;",
                                "combo_box_int;",
                                "combo_box_float;",
                                "combo_box_num;",
                                "mledit_text;",
                                "chart_view;",
                                "capacity;",
                                "progress_circle_value;",
                                "progress_circle_percent;",
                                "digit_clock;",
                                "hscroll_label;",
                                "get_min",
                                "get_max",
                                "slide_indicator",
                                "slide_view",
                                "slide_menu",
                                "m_tab_button",
                                "m_tab_view",
                                 };

/*  Text type data memory allocation function, dynamically created, not automatically freed  */
/*  The stone_recive_free() interface can be called to free memory when the user is finished using it
 *  If the user does not release the memory, the memory space occupied last time will be released before the next use
 */
 /* Create memory space for "data" Pointers */
recive_group datainit(int len){
   if (STONER.data != NULL && data_init_flag == 1){
     memset(STONER.data, NULL, widget_last_len);    //If it is not empty, release it first
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
  memset(STONER.data, NULL, len);
  data_init_flag = 1;
  return STONER;
}

 /* Create memory space for "widget" Pointers */
recive_group widgetinit(int len){
   if (STONER.widget != NULL && widget_init_flag == 1){
    memset(STONER.widget, NULL, widget_last_len);
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
  memset(STONER.widget, NULL, len);
  widget_init_flag = 1;
  return STONER;
}

 /* Create memory space for "text" Pointers */
recive_group textinit(int len){
   if (STONER.text != NULL && text_init_flag == 1){
     memset(STONER.text, NULL, widget_last_len);
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
  memset(STONER.text, NULL, len);
  text_init_flag = 1;
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
void receive_parse (void){

  base_offset=0;
  
  /* Caches directives represented by CMD */
  STONER.cmd = (unsigned int)STONE_RX_BUF[3];
  STONER.cmd = STONER.cmd << 8 | STONE_RX_BUF[4];

  /* Cache the length LEN represents */
  STONER.len = (unsigned int)STONE_RX_BUF[5];
  STONER.len = STONER.len << 8 | STONE_RX_BUF[6];
  cmd_select();
}

/* Parse the data through the queue format */
void receive_parse_fifo (int _base_offset){

  base_offset = _base_offset;

  /* Caches directives represented by CMD */
  STONER.cmd = (unsigned int)STONE_RX_BUF[base_offset+3];
  STONER.cmd = STONER.cmd << 8 | STONE_RX_BUF[base_offset+4];

  /* Cache the length LEN represents */
  STONER.len = (unsigned int)STONE_RX_BUF[base_offset+5];
  STONER.len = STONER.len << 8 | STONE_RX_BUF[base_offset+6];
  cmd_select();
}

void cmd_select(void){

  switch (STONER.cmd){
    case sys_state: {
      STONER = datainit(STONER.len+1);     //Memory is allocated one byte too much
      sscanf((const char*)STONE_RX_BUF+base_offset+7,"%[^>ET]",STONER.data);
      
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
      sscanf((const char*)STONE_RX_BUF+base_offset+7,"%[^>ET]",STONER.data);
      
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
      memcpy(STONER.data,STONE_RX_BUF+base_offset+7,STONER.len);
      
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
      case_btn_switch_ckbtn_rdbtn(m_button, 0);
    }break;
    case control_button_u: {
      case_btn_switch_ckbtn_rdbtn(m_user_button, 0);
    }break;
    case control_switch: {
      case_btn_switch_ckbtn_rdbtn(m_switch, 0);
    }break;
    case control_check_button: {
      case_btn_switch_ckbtn_rdbtn(m_check_button, 0);
    }break;
    case control_radio_button: {
      case_btn_switch_ckbtn_rdbtn(m_radio_button, 0);
    }break;
    case control_radio_button_s: {
      case_btn_switch_ckbtn_rdbtn(m_radio_button_mcu_get, 0);
    }break;
    case control_slider: {
      case_slider_porgressbar(m_slider);
    }break;
    case control_slider_over: {
      case_slider_porgressbar(m_slider_over);
    }break;
    case control_progress_bar: {
      case_slider_porgressbar(m_progress_bar);
    }break;
    case control_progress_bar_p: {
      case_4_byte_data(m_progress_bar,m_percent);
    }break;
    case control_label_text: {
      case_data_text(m_label_text);
    }break;
    case control_label_value: {
      case_4_byte_ieee_value(m_label_value);
    }break;
    case control_edit_text: {
      case_data_text(m_edit_text);
    }break;
    case control_edit_int: {
      case_4_byte_data(m_edit_int,m_int_value);
    }break;
    case control_text_selector_text: {
      case_data_text(m_selector_text);
    }break;
    case control_text_selector_value: {
      case_4_byte_data(m_selector_value,m_value);
    }break;
    case control_text_selector_num: {
      case_4_byte_data(m_selector_num,m_num);
    }break;
    
    case control_image: {
      STONER = widgetinit(STONER.len);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-2);
      STONER.value = STONE_RX_BUF[base_offset+7+STONER.len-2];
      STONER.value = STONER.value<<8 | STONE_RX_BUF[base_offset+7+STONER.len-1];
      #if print_recive_image_value || print_recive_ALL
      stone_print("type:image;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-2;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(("%d",STONER.value));
      stone_println(";");
      #endif
    }break;
    
    case control_image_u: {
      STONER = widgetinit(STONER.len);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-2);
      STONER.value = STONE_RX_BUF[base_offset+7+STONER.len-2];
      STONER.value = STONER.value<<8 | STONE_RX_BUF[base_offset+7+STONER.len-1];
      #if print_recive_image_value || print_recive_ALL
      stone_print("type:image_user;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-2;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(("%d",STONER.value));
      stone_println(";");
      #endif
    }break;
    
    case control_image_value: {
      case_4_byte_ieee_value(m_image_value);
    }break;
    
    case control_spin_box_text: {
    case_data_text(m_spin_box_text);
    }break;
    case control_spin_box_int: {
      case_4_byte_data(m_spin_box_int,m_value);
    }break;
    case control_spin_box_float: {
      case_4_byte_ieee_value(m_spin_box_float);
    }break;
    case control_combo_box_text: {
      case_data_text(m_combo_box_text);
    }break;
    case control_combo_box_int: {
      case_4_byte_data(m_combo_box_int,m_value);
    }break;
    case control_combo_box_float: {
      case_4_byte_ieee_value(m_combo_box_float);
    }break;
    case control_combo_box_num: {
      case_4_byte_data(m_combo_box_num,m_num);
    }break;
    case control_mledit_text: {
      case_data_text(m_mledit_text);
    }break;
    case control_write_hex: {
      case_file_rw();
    }break;
    case control_write_text: {
      case_file_rw();
    }break;
    case control_write_line: {
      case_file_rw();
    }break;
    case control_read_hex: {
      case_file_rw();
    }break;
    case control_read_line: {
      case_file_rw();
    }break;
    case control_get_file_size: {
      case_file_rw();
    }break;
    case control_delete_file: {
      case_file_rw();
    }break;
    case control_set_rename: {
      case_file_rw();
    }break;
    case control_download_file: {
      case_file_rw();
    }break;
    case control_unmount_disk: {
      case_file_rw();
    }break;
    case control_get_xy: {
      STONER = widgetinit(STONER.len-8+1);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-8);
      STONER.long_value = 0;
      for(int xy=0;xy<8;xy++)
      {
        STONER.long_value =STONER.long_value << 8;
        STONER.long_value += STONE_RX_BUF[base_offset+7+STONER.len-8+xy];
      }
    }break;
    case control_get_wh: {
      STONER = widgetinit(STONER.len-8+1);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-8);
      STONER.long_value = 0;
      for(int wh=0;wh<8;wh++)
      {
        STONER.long_value =STONER.long_value << 8;
        STONER.long_value += STONE_RX_BUF[base_offset+7+STONER.len-8+wh];
      }
    }break;
    case control_get_min: {
      case_slider_porgressbar(m_get_min);
    }break;
    case control_get_max: {
      case_slider_porgressbar(m_get_max);
    }break;
    case control_slide_indicator_value: {
      case_4_byte_data(m_slide_indicator,m_num);
    }break;
    case control_slide_view: {
      case_4_byte_data(m_slide_view,m_num);
    }break;
    case control_slide_menu: {
      case_4_byte_data(m_slide_menu,m_num);
    }break;
    case control_tab_button: {
      case_btn_switch_ckbtn_rdbtn(m_tab_button, 0);
    }break;
    case control_tab_view: {
      case_4_byte_data(m_tab_view,m_num);
    }break;
    case control_chart_view_value: {
      STONER = widgetinit(STONER.len-5);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-6);
      STONER.value = STONE_RX_BUF[base_offset+7+STONER.len-6];
      STONER.value = STONER.value<<8 | STONE_RX_BUF[base_offset+7+STONER.len-5];
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+base_offset+7+STONER.len-4);
      #if print_recive_image_value || print_recive_ALL
      stone_print("type:chart_view;");
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-2;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";num:");
      stone_print(("%d,",STONER.value));
      stone_print("float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
    }break;
    case control_progress_circle_value: {
      case_4_byte_ieee_value(m_progress_circle_value);
    }break;
    case control_progress_circle_p: {
      case_4_byte_data(m_progress_circle_p,m_percent);
    }break;
    case control_digit_clock: {
      case_4_byte_data(m_digit_clock,m_value);
    }break;
    case control_hscroll_label: {
      case_4_byte_data(m_hscroll_label,m_value);
    }break;
  }
  
}

void case_file_rw(void){
  STONER = datainit(STONER.len+1);
  memcpy(STONER.data,STONE_RX_BUF+base_offset+7,STONER.len);
}

void case_btn_switch_ckbtn_rdbtn (char _type, char _user){
  
      STONER = widgetinit(STONER.len);
      if (_user==1)
      {
        memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-2);
        STONER.long_value = STONE_RX_BUF[base_offset+7+STONER.len-2];
        STONER.long_value = STONER.long_value << 8;
        STONER.long_value += STONE_RX_BUF[base_offset+7+STONER.len-1];
      }
      else
      {
        memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-1);
        STONER.value = STONE_RX_BUF[base_offset+7+STONER.len-1];
      }
      
      #if print_recive_button || print_recive_ALL
      stone_print("type:");
      stone_print(m_static_str[_type]);
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-1;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";value:");
      stone_print(STONER.value);
      stone_println(";");
      #endif
}

void case_slider_porgressbar (char _type){
  
      STONER = widgetinit(STONER.len-3);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-4);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+base_offset+7+STONER.len-4);
      
      #if print_recive_slider || print_recive_ALL
      stone_print("type:");
      stone_print(m_static_str[_type]);
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-4;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
}

void case_data_text (char _type){
  
      STONER = widgetinit(40);
      sscanf((const char*)STONE_RX_BUF+base_offset+8,"%[^\"]:",STONER.widget);
      widget_len=strlen((const char*)STONER.widget);
      STONER = textinit(STONER.len-widget_len-3);
      memcpy(STONER.text,STONE_RX_BUF+base_offset+8+widget_len+2,STONER.len-widget_len-3);
  
      #if print_recive_label || print_recive_ALL
      stone_print("type:");
      stone_print(m_static_str[_type]);      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-widget_len-3;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";text:");
      for (int wds=0;wds<STONER.len-widget_len-3;wds++)
      stone_write(STONER.test[wds]);
      stone_println(";");
      #endif
}
void case_4_byte_data (char _type, char _data){
  
      STONER = widgetinit(STONER.len-3);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-4);
      STONER.long_value = STONE_RX_BUF[base_offset+7+STONER.len-4];
      STONER.long_value = (STONER.long_value<<8) | STONE_RX_BUF[base_offset+7+STONER.len-3];
      STONER.long_value = (STONER.long_value<<8) | STONE_RX_BUF[base_offset+7+STONER.len-2];
      STONER.long_value = (STONER.long_value<<8) | STONE_RX_BUF[base_offset+7+STONER.len-1];
      #if print_recive_progress || print_recive_ALL
      stone_print("type:");
      stone_print(m_static_str[_type]);
      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-4;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(m_static_str[_data]);
      stone_print(STONER.long_value);
      stone_println(";");
      #endif
}

void case_4_byte_ieee_value (char _type){
  
      STONER = widgetinit(STONER.len-3);
      memcpy(STONER.widget,STONE_RX_BUF+base_offset+7,STONER.len-4);
      widget_len = strlen((const char*)STONER.widget);
      STONER.float_value = write_hex_to_float(STONE_RX_BUF+base_offset+7+widget_len);
      
      #if print_recive_label || print_recive_ALL
      stone_print("type:");
      stone_print(m_static_str[_type]);      stone_print("len:");
      stone_print(STONER.len);
      stone_print(";widget:");
      for (int wds=0;wds<STONER.len-4;wds++)
      stone_write(STONER.widget[wds]);
      stone_print(";float_value:");
      stone_print(("%f",STONER.float_value));
      stone_println(";");
      #endif
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
