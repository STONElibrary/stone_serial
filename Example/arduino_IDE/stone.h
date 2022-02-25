#ifndef _STONE_H__
#define _STONE_H__

#include "stone_config.h"


//接收相关*******************************************
#define RX_LEN 200
enum recive_cmd {sys_state = 0x0000,                    //系统运行状态
                 sys_hello = 0x0001,                    //系统通信探测返回指令
                 sys_version = 0x0002,                  //GUI软件版本号下发
                 control_button = 0x1001,               //button键值主动下发
                 control_button_u = 0x1002,             //button键值主动下发  (稍后开放),用户自定义
                 control_switch = 0x1010,               //switch开关值改变之后主动下发
                 control_check_button = 0x1020,         //check button值改变之后主动下发
                 control_radio_button = 0x1030,         //radio button值改变之后主动下发
                 control_radio_button_s = 0x1031,       //radio button值改变之后被动下发  (MCU使用get_checked指令获取)
                 control_slider = 0x1040,               //slider值改变中主动下发
                 control_slider_over = 0x1041,          //slider值改变完成后主动下发
                 control_progress_bar = 0x1050,         //progress bar值改变主动/被动下发(MCU使用get_value命令获取)
                 control_progress_bar_p = 0x1051,       //progress bar百分比,被动下发(MCU使用get_percent命令获取)
                 control_label_text = 0x1060,           //label文本被动下发  (MCU通过get_text指令获取之后才会下发  lable不主动下发数据)
                 control_label_value = 0x1062,          //label数值下发  (float类型) (与button绑定set_value时间之后数值变化之后主动下发或者使用get_value指令获取数值)
                 control_edit_text = 0x1070,            //edit文本下发    (主动被动下发  可以是edit数据更改之后主动下发  也可以使用get_text主动获取)
                 control_edit_int = 0x1071,             //edit数值下发  (int类型)
                 control_edit_float = 0x1072,           //edit数值下发  (float类型)
                 control_text_selector_text = 0x1080,   //text selector文本下发    (被动下发 MCU使用get_text主动获取)
                 control_text_selector_value = 0x1081,  //text selector数值下发  (int类型 主动/被动下发  被动下发:MCU使用get_value命令获取)
                 control_text_selector_num = 0x1082,    //text selector序号下发  (int类型 被动下发  MCU使用get_select命令获取)
                 control_image_value = 0x1092           //image_value数值下发  (float类型) (主动下发  被动通过get_value获取)
                 };

typedef struct Recive{
    unsigned int     cmd;
    unsigned int     len;
    int              value;
    float            float_value;
    long             long_value;
    unsigned char    *data;
    unsigned char    *widget;
    unsigned char    *text;

}recive_group;

static union {
float flo;
char buf[4];
} HexFloat;

void receive_parse (void);
void print_cmd (void);

void serial_receive(void);

void _stone_recive_free(char* name);
#define DEFARG(name, defval) ((#name[0]) ? #name : defval)
#define stone_recive_free(...) _stone_recive_free(DEFARG(__VA_ARGS__, 0))

unsigned short illegal_date(void);
float write_hex_to_float(unsigned char* buf);
unsigned short do_crc(unsigned char *ptr, int len);



//发送相关********************************************
#define TX_LEN 200
#define STR_HEAD_CMD					"ST<{"
#define STR_CMD_CODE					"cmd_code"
#define STR_HEAD_CMD2					"ST<{\"cmd_code\":"
#define STR_SYS_REBOOT 				"sys_reboot"
#define STR_SYS_HELLO 				"sys_hello"
#define STR_SYS_VERSION				"sys_version"
#define STR_SET_							"set_"
#define	STR_SLEEP							"sleep"
#define	STR_ENABLE						"enable"
#define STR_VISIBLE						"visible"
#define STR_XY								"xy"
#define	STR_OPEN_WIN					"open_win"
#define	STR_CLOSE_WIN        	"close_win"
#define STR_BACK_WIN					"back_win"
#define	STR_BACK_WIN_TO				"back_win_to"
#define	STR_BACK_HOME					"back_home"
#define	STR_TEXT							"text"
#define	STR_VALUE							"value"
#define STR_GET_							"get_"
#define	STR_MAX								"max"
#define	STR_SHOW_TEXT					"show_text"
#define	STR_PERCNET						"percent"
#define	STR_MIN								"min"
#define	STR_STEP							"step"
#define	STR_PLAY							"play"
#define	STR_PAUSE							"pause"
#define	STR_STOP							"stop"
#define	STR_FORMAT						"format"
#define STR_IMAGE							"image"
#define	STR_INTERVAL					"interval"
#define	STR_LOOP							"loop"
#define	STR_RANGE							"range"
#define	STR_DRAW_TYPE					"draw_type"
#define	STR_SCALE							"scale"
#define	STR_ROTATION					"rotation"
#define	STR_SELECTED					"selected"
#define	STR_DATE							"date"
#define	STR_CHECKED						"checked"
#define	STR_ANGLE							"angle"
#define STR_TYPE							"type"
#define STR_SYSTEM						"system"
#define	STR_WIDGET						"widget"
#define	STR_WINDOW						"window"
#define	STR_LABEL							"label"
#define	STR_EDIT							"edit"
#define	STR_PORGRESS_BAR			"progress_bar"
#define	STR_SLIDER						"slider"
#define	STR_IMAGE_ANIMATION		"image_animation"
#define STR_IMAGE_VALUE				"image_value"
#define	STR_TEXT_SELECTOR			"text_selector"
#define	STR_DIGIT_CLOCK				"digit_clock"
#define	STR_SWITCH						"switch"
#define	STR_CHECK_BUTTON			"check_button"
#define	STR_RADIO_BUTTON			"radio_button"
#define	STR_GAUGE							"gauge"
#define STR_GAUGE_POINTER			"gauge_pointer"
#define STR_START_INDEX				"start_index"
#define STR_END_INDEX					"end_index"
#define STR_END								"}>ET"
#define JSON_END							1

#define STR_TYPE_NUM 	strcmp(name,STR_SLEEP)==0||\
											strcmp(name,STR_ENABLE)==0||\
											strcmp(name,STR_VISIBLE)==0||\
											strcmp(name,STR_VALUE)==0||\
											strcmp(name,STR_MAX)==0||\
											strcmp(name,STR_MIN)==0||\
											strcmp(name,STR_STEP)==0||\
											strcmp(name,STR_INTERVAL)==0||\
											strcmp(name,STR_LOOP)==0||\
											strcmp(name,"start_index")==0||\
											strcmp(name,"end_index")==0||\
											strcmp(name,"draw_type")==0||\
											strcmp(name,"scale_x")==0||\
											strcmp(name,"scale_y")==0||\
											strcmp(name,"rotation")==0||\
											strcmp(name,"image_value")==0||\
											strcmp(name,"selected_index")==0||\
											strcmp(name,STR_ANGLE)==0||\
											name[0]=='x'||\
											name[0]=='y'
											

//发送函数********************************************
void STONE_JSON (char* name, char* value, ...);
void tx_create (void);

void set_sys(char* m_cmd);
void set_sleep(char* m_tf);
void set_enable(char* m_name, char* m_tf, ...);
void set_coordinate(char* _name, char* _x, char* _y);

void open_win(char* _name, ...);
void back_win(void);
void back_home(void);

void set_text(char* _type, char* _name, char* _text);
void set_value(char* _type, char* _name, char* _value, ...);

void get_text(char* _type, char* _name);
void get_value(char* _type, char* _name);
void set_max(char* _type, char* _name, char* value);
void show_text(char* _name, char* _tf);
void get_percent(char* _name);

void set_min(char* _type, char* _name, char* value);
void set_step(char* _name, char* value);
void set_play(char* _name,...);
void set_pause(char* _name);
void set_stop(char* _name);
void set_format(char* _type, char* _name, char* _format);
void set_image(char* _type, char* _name, char* _image);
void set_interval(char* _name, char* _interval);
void set_loop(char* _name, char* _tf);
void set_range(char* _name, char* start, char* end);
void set_draw_type(char* _type, char* _name, char* draw_type);

void set_scale(char* _name, char* _x, char* _y);
void set_rotation(char* _name, char* _rotation);
void set_selected(char* _name, char* _selected);
void get_selected(char* _name);
void set_date(char* _name, char* _date);
void get_date(char* _name);
void get_checked(char* _name);
void set_angle(char* _name, char* _angle);



#endif
