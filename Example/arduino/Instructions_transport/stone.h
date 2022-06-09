/*
 * @file        stone.h
 * 
 * @version     V1.0.6
 * 
 * @date        2022-5-30
 * 
 * @produce   STONE Technologies
 * 
 */

#ifndef _STONE_H__
#define _STONE_H__

#include "stone_config.h"


//Receive relevant*******************************************
#define RX_LEN 200
enum recive_cmd {sys_state = 0x0000,                    //System states
                 sys_hello = 0x0001,                    //System return code for communication test command (sys_hello)
                 sys_version = 0x0002,                  //System firmware information return for get_version command
                 control_button = 0x1001,               //button status send
                 control_button_u = 0x1002,             //button key value send (will be released soon)
                 control_switch = 0x1010,               //switch value changed return
                 control_check_button = 0x1020,         //check button value changed return
                 control_radio_button = 0x1030,         //radio button value changed return initiative
                 control_radio_button_s = 0x1031,       //radio button value changed return passively(by get_checked command from MCU)
                 control_slider = 0x1040,               //slider value changing return
                 control_slider_over = 0x1041,          //slider value changed return
                 control_progress_bar = 0x1050,         //progress bar value changed return
                 control_progress_bar_p = 0x1051,       //progress bar percentage return passively, by get_percent command from MCU
                 control_label_text = 0x1060,           //label text content return passively (The display will only return the command after received the get_text command from MCU)
                 control_label_value = 0x1062,          //label value return (float type)(while the label was targeted by the set_value function on the button, the code will return once the value is changed)
                 control_edit_text = 0x1070,            //edit text return (Initiatively or passively. It can be returned after the edit text changed, or returned by get_text command)
                 control_edit_int = 0x1071,             //edit data return(int type)
                 control_edit_float = 0x1072,           //edit data return(float type)
                 control_text_selector_text = 0x1080,   //text selector text content return passively (by get_text command)
                 control_text_selector_value = 0x1081,  //text selector value return initiatively (int type value, can be read by get_value command from MCU)
                 control_text_selector_num = 0x1082,    //text selector index number return passively (int type value by get_value command from MCU)
								 control_image = 0x1090,								//Image key Delivers the system key
								 control_image_u = 0x1091,							//Image key Delivers a user-defined key value
								 control_image_value = 0x1092,          //image_value value return (float type, can be returned initiatively or passively)
								 control_spin_box_text = 0x10A0,				//Spin_box text delivery
								 control_spin_box_int = 0x10A1,					//Spin_box value delivered (int type)
								 control_spin_box_float = 0x10A2,				//Spin_box value delivered (float type)
								 control_combo_box_text = 0x10B0,				//combo_box text delivery
								 control_combo_box_int = 0x10B1,				//combo_box value delivered (int type)
								 control_combo_box_float = 0x10B2,			//combo_box value delivered (float type)
								 control_combo_box_num = 0x10B8,				//combo_box serial number is delivered
								 control_mledit_text = 0x10C0,					//mledit text delivery
								 control_chart_view_value = 0x10D1,			//chart_view value delivered
								 control_chart_view_capacity = 0x10D2,	//chart_view capacity delivery (response to the instruction get_capacity)
								 control_progress_circle_value = 0x10E0,//progress_circle value delivered (float type)
								 control_progress_circle_p = 0x10E1,		//progress_circle percentage return passively, by get_percent command from MCU
								 control_digit_clock = 0x10F0,					//digit_clock Date + time return (get_date in MCU)
								 control_hscroll_label = 0x1100					//hscroll_label Text return (the GEt_TEXT instruction is used in the MCU)
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

enum group_static_str {m_button = 0,
											m_user_button,
											m_switch,
											m_check_button,
											m_radio_button,
											m_radio_button_mcu_get,
											m_slider,
											m_slider_over,
											m_progress_bar,
											m_label_text,
											m_edit_text,
											m_percent,
											m_edit_int,
											m_int_value,
											m_selector_value,
											m_value,
											m_selector_num,
											m_num,
											m_spin_box_int,
											m_selector_text,
											m_spin_box_text,
											m_label_value,
											m_image_value,
											m_spin_box_float,
											m_combo_box_text,
											m_combo_box_int,
											m_combo_box_float,
											m_combo_box_num,
											m_mledit_text,
											m_chart_view,
											m_capacity,
											m_progress_circle_value,
											m_progress_circle_p,
											m_digit_clock,
											m_hscroll_label
};


void case_btn_switch_ckbtn_rdbtn (char _type);
void case_slider_porgressbar (char _type);
void case_data_text (char _type);
void case_4_byte_data (char _type, char _data);
void case_4_byte_ieee_value (char _type);

void receive_parse (void);
void print_cmd (void);

void serial_receive(void);

void _stone_recive_free(char* name);
#define DEFARG(name, defval) ((#name[0]) ? #name : defval)
#define stone_recive_free(...) _stone_recive_free(DEFARG(__VA_ARGS__, 0))

unsigned short illegal_date(void);
float write_hex_to_float(unsigned char* buf);
unsigned short do_crc(unsigned char *ptr, int len);



//Send the related********************************************
#define TX_LEN 200
#define STR_HEAD_CMD					"ST<{"
#define STR_CMD_CODE					"cmd_code"
#define STR_HEAD_CMD2					"ST<{\"cmd_code\":"

#define STR_SYS_REBOOT 				"sys_reboot"
#define STR_SYS_HELLO 				"sys_hello"
#define STR_SYS_VERSION				"sys_version"
#define STR_SET_							"set_"
#define STR_CLEAR_						"clear_"
#define	STR_SLEEP							"sleep"
#define	STR_BUZZER						"buzzer"
#define	STR_BRIGHTNESS				"brightness"
#define	STR_TOUCH_CAL					"touch_cal"
#define	STR_TOUCH_TEST				"touch_test"
#define	STR_TIME							"time"
#define	STR_ENABLE						"enable"
#define STR_VISIBLE						"visible"
#define STR_XY								"xy"
#define STR_STATE							"state"
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
#define STR_COLOR							"color"
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
											

//Send function********************************************
void STONE_JSON (char* name, char* value, ...);
void tx_create (void);

/* command interface for setting the system-related commands */
// Call the example: set_sys("sys_reboot");
void set_sys(char* m_cmd);

/* Command interface for the command to set the screen backlight */
// Call the example: set_sleep("true");
void set_sleep(char* m_tf);

/* Command interface for setting the buzzer time,In milliseconds */
// Call the example: set_buzzer("100");
void set_buzzer(char* _time);

//#define set_brightness(_value) set_buzzer(_value)
/* Command interface for setting backlight brightness, The value ranges from 0 to 100*/
// Call the example: set_brightness("100");
void set_brightness(char* _value);

/* Command interface for setting Touch screen calibration (for resistive screens) */
// Call the example: set_touch_cal();
void set_touch_cal(void);

/* Command interface for remove Touch screen calibration (for resistive screens) */
// Call the example: clear_touch_cal();
void clear_touch_cal(void);

/* Command interface for Touch screen test */
// Call the example: set_touch_test();
void set_touch_test(void);

/* Command interface for the command to set the enable state of the widget */
// Call the example: set_enable("switch1", "false");
void set_enable(char* m_name, char* m_tf, ...);

/* Command interface for the command to set the visible state of the widget */ 
// Call the example: set_visible("switch1", "false");
#define set_visible(m_name, m_tf) set_enable(m_name, m_tf, 1)
/* Command interface for the command to set the coordinate position of the widget */
// Call the example: set_coordinate("switch1", "100", "200");
void set_coordinate(char* _name, char* _x, char* _y);

/* Command interface for the command Control state */
// Call the example: set_state("button1", "pressed");
void set_state(char* _name, char* _state);

/* Command interface for the Set the background image */
// Call the example: set_bg_image("button1", "guage_bg");
void set_bg_image(char* _name, char* _image);

/* Command interface for the Set the background color */
// Call the example: set_color("button1", "bg_color", "guage_bg");
void set_color(char* _name, char* _object, char* _color);

/* Command interface for the command to open a window */
// Call the example: open_win("window2");
void open_win(char* _name, ...);

/* Command interface for the command to close the window */
// Call the example: close_win("window1");
#define close_win(m_name) open_win(m_name, 1)
/* Command interface to return to the previous level of window */
// Call the example: back_win();
void back_win(void);

/* Return to the command interface of the specified window */
// Call the example: back_win_to("window1");
#define back_win_to(m_name) open_win(m_name, 2)
/* Command interface for returning the main window command */
// Call the example: back_home();
void back_home(void);

/* command interface for Setting the text commands */
// Call the example: set_text("label", "label5", "hello stone");
void set_text(char* _type, char* _name, char* _text);

/* Command interface for the set value command */
// Call the example: set_value("slider", "slider2", "66");
// Call the example:  set_value("label", "label5", "66.6", "%.2f");
void set_value(char* _type, char* _name, char* _value, ...);

/* Command interface for getting text commands */
// Call the example: get_text("edit", "edit1");
void get_text(char* _type, char* _name);

/* Command interface for the Get Value command */
// Call the example: get_value("edit", "edit2");
void get_value(char* _type, char* _name);

/* Command interface for the set maximum command */
// Call the example: set_max("slider", "slider2", "500");
void set_max(char* _type, char* _name, char* value);

/* Command interface for displaying text commands */
// Call the example: show_text("progress_bar1", "true");
void show_text(char* _name, char* _tf);

/* Command interface for the Get Progress Bar Percentage command */
// Call the example: get_percent("progress_bar1");
void get_percent(char* _name);

/* Command interface for the set minimum command */
// Call the example: set_min("slider", "slider2", "1");
void set_min(char* _type, char* _name, char* value);

/* Command interface for setting slider step value command */
// Call the example: set_step("slider1", "1");
void set_step(char* _name, char* value);

/* Command interface for setting the picture animation playback command */
// Call the example: set_play("image_animation1");
void set_play(char* _name,...);

/* Command interface for setting the pause command for picture animation playback */
// Call the example: set_pause("image_animation1");
#define set_pause(m_name) set_play(m_name,1)
/* Command interface for setting the picture animation stop command */
//// Call the example: set_stop("image_animation1");
#define set_stop(m_name)   set_play(m_name,2)
/* Command interface for setting format commands */
// Call the example: set_format("image_value", "iamge_value1", "%02.2f");
void set_format(char* _type, char* _name, char* _format);

/* Command interface for setting picture commands */
// Call the example: set_image("image_value", "iamge_value1", "light_on");
void set_image(char* _type, char* _name, char* _image);

/* Command interface for the set picture animation interval command */
// Call the example: set_interval("image_animation1", "100");
void set_interval(char* _name, char* _interval);

/* Command interface for setting the loop command */
// Call the example: set_loop("image_animation1", "true");
void set_loop(char* _name, char* _tf);

/* Command interface for setting range commands */
// Call the example: set_range("image_animation1", "1" , "10");
void set_range(char* _name, char* start, char* end);

/* Command interface for the set picture display method command */
// Call the example: set_draw_type("image", "image1" , "2");
void set_draw_type(char* _type, char* _name, char* draw_type);

/* Command interface for the set image scaling command */
// Call the example: set_scale("image2", "0.5" , "1");
void set_scale(char* _name, char* _x, char* _y);

/* Command interface for the set picture rotation angle command */
// Call the example: set_rotation("image1", "180");
void set_rotation(char* _name, char* _rotation);

/* Command interface for setting the current selector command of the text selector */
// Call the example: set_selected("text_selector1", "20");
void set_selected(char* _name, char* _selected);

/* Command interface to get the current selector command of the text selector */
// Call the example: get_selected("text_selector1");
void get_selected(char* _name);

/* Command interface for the set date command */
// Call the example: set_date("2022-02-22 22:22:22");
void set_date(char* _name, char* _date);

/* Command interface for the get date command */
// Call the example: get_date("clock1");
void get_date(char* _name);

/* Command interface for the get checked command */
// Call the example: get_checked("check1");
void get_checked(char* _name);

/* Command interface for the set angle command */
// Call the example: set_angle("pointer1", "10");
void set_angle(char* _name, char* _angle);



#endif
