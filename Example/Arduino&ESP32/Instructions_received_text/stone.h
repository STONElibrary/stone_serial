/*
 * @file        stone.h
 * 
 * @version     V1.1.0
 * 
 * @date        2022-3-8
 * 
 * @produce   STONE Technologies
 * 
 */

#ifndef _STONE_H__
#define _STONE_H__

#include "stone_config.h"


//Receive relevant*******************************************
#define RX_LEN 200
enum recive_cmd {  sys_state = 0x0000,                         //System states
                  sys_hello = 0x0001,                         //System return code for communication test command (sys_hello)
                  sys_version = 0x0002,                       //System firmware information return for get_version command
                  control_write_hex = 0x0021,                 //The response to the write_hex instruction
                  control_write_line = 0x0023,                //The response to the write_line instruction
                  control_write_text = 0x0025,                //The response to the write_text instruction
                  control_read_hex = 0x0020,                  //The response to the read_hex instruction
                  control_read_line = 0x0022,                 //The response to the read_line instruction
                  control_get_file_size = 0x0024,             //The response to the get_file_size instruction
                  control_delete_file = 0x0026,               //The response to the delete_file instruction
                  control_set_rename = 0x0027,                //The response to the set_rename instruction
                  control_download_file = 0x0028,             //The response to the download_file instruction
                  control_unmount_disk = 0x002A,              //The response to the unmount_disk instruction
                  control_get_xy = 0x0400,                    //The response to the get_xy instruction
                  control_get_wh = 0x0401,                    //The response to the get_wh instruction
                  control_get_min = 0x1160,                   //The response to the get_min instruction
                  control_get_max = 0x1161,                   //The response to the get_max instruction
                  control_button = 0x1001,                    //button status send
                  control_button_u = 0x1002,                  //button key value send (will be released soon)
                  control_switch = 0x1010,                    //switch value changed return
                  control_check_button = 0x1020,              //check button value changed return
                  control_radio_button = 0x1030,              //radio button value changed return initiative
                  control_radio_button_s = 0x1031,            //radio button value changed return passively(by get_checked command from MCU)
                  control_slider = 0x1040,                    //slider value changing return
                  control_slider_over = 0x1041,               //slider value changed return
                  control_progress_bar = 0x1050,              //progress bar value changed return
                  control_progress_bar_p = 0x1051,            //progress bar percentage return passively, by get_percent command from MCU
                  control_label_text = 0x1060,                //label text content return passively (The display will only return the command after received the get_text command from MCU)
                  control_label_value = 0x1062,               //label value return (float type)(while the label was targeted by the set_value function on the button, the code will return once the value is changed)
                  control_edit_text = 0x1070,                 //edit text return (Initiatively or passively. It can be returned after the edit text changed, or returned by get_text command)
                  control_edit_int = 0x1071,                  //edit data return(int type)
                  control_edit_float = 0x1072,                //edit data return(float type)
                  control_text_selector_text = 0x1080,        //text selector text content return passively (by get_text command)
                  control_text_selector_value = 0x1081,       //text selector value return initiatively (int type value, can be read by get_value command from MCU)
                  control_text_selector_num = 0x1082,         //text selector index number return passively (int type value by get_value command from MCU)
                  control_image = 0x1090,                     //Image key Delivers the system key
                  control_image_u = 0x1091,                   //Image key Delivers a user-defined key value
                  control_image_value = 0x1092,               //image_value value return (float type, can be returned initiatively or passively)
                  control_spin_box_text = 0x10A0,             //Spin_box text delivery
                  control_spin_box_int = 0x10A1,              //Spin_box value delivered (int type)
                  control_spin_box_float = 0x10A2,            //Spin_box value delivered (float type)
                  control_combo_box_text = 0x10B0,            //combo_box text delivery
                  control_combo_box_int = 0x10B1,             //combo_box value delivered (int type)
                  control_combo_box_float = 0x10B2,           //combo_box value delivered (float type)
                  control_combo_box_num = 0x10B8,             //combo_box serial number is delivered
                  control_mledit_text = 0x10C0,               //mledit text delivery
                  control_chart_view_value = 0x10D1,          //chart_view value delivered
                  control_chart_view_capacity = 0x10D2,       //chart_view capacity delivery (response to the instruction get_capacity)
                  control_progress_circle_value = 0x10E0,     //progress_circle value delivered (float type)
                  control_progress_circle_p = 0x10E1,         //progress_circle percentage return passively, by get_percent command from MCU
                  control_digit_clock = 0x10F0,               //digit_clock Date + time return (get_date in MCU)
                  control_hscroll_label = 0x1100,             //hscroll_label Text return (the GEt_TEXT instruction is used in the MCU)
                  control_slide_indicator_value = 0x1110,     //gets the current value of the indicator (option)
                  control_slide_view = 0x1120,                //gets the sequence number of the slide_view
                  control_slide_menu = 0x1130,                //gets the sequence number of the slide_menu
                  control_tab_button = 0x1140,                //gets the tab_button value
                  control_tab_view = 0x1150,                  //gets the serial number of the tab_view
                 };

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
    unsigned char       *data;
    unsigned char       *widget;
    unsigned char       *text;

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
                      m_hscroll_label,
                      m_get_min,
                      m_get_max,
                      m_slide_indicator,
                      m_slide_view,
                      m_slide_menu,
                      m_tab_button,
                      m_tab_view,
                      
};

enum Function_Overload {
  RE_DEFINE1 = 0xA0,
  RE_DEFINE2,
  RE_DEFINE3,
  RE_DEFINE4,
  RE_DEFINE5,
  RE_DEFINE6,
  RE_DEFINE7,
  RE_DEFINE8,
  RE_DEFINE9,
};

typedef struct Link {
    int  base_offset;
    int frame_count;
    struct Link *next;
}frame_link;

frame_link * initLink(void);
frame_link * additional(frame_link * p, int base_offset);
frame_link * delElem(frame_link * p);

void case_btn_switch_ckbtn_rdbtn (char _type, char _user);
void case_slider_porgressbar (char _type);
void case_data_text (char _type);
void case_4_byte_data (char _type, char _data);
void case_4_byte_ieee_value (char _type);

void cmd_select(void);
void receive_parse_fifo (int _base_offset);
void receive_parse (void);
void case_file_rw(void);
void print_cmd (void);

void serial_receive(void);
void serial_receive_frame(unsigned short CNT);
void serial_receive_frame_idel(void);

void _stone_recive_free(char* name);
#define DEFARG(name, defval) ((#name[0]) ? #name : defval)
#define stone_recive_free(...) _stone_recive_free(DEFARG(__VA_ARGS__, 0))

float write_hex_to_float(unsigned char* buf);
unsigned short do_crc(unsigned char *ptr, int len);



//Send the related********************************************
#define TX_LEN 200
#define STR_HEAD_CMD          "ST<{"
#define STR_CMD_CODE          "cmd_code"
#define STR_HEAD_CMD2         "ST<{\"cmd_code\":"

#define STR_SYS_REBOOT        "sys_reboot"
#define STR_SYS_HELLO         "sys_hello"
#define STR_SYS_VERSION       "sys_version"
#define STR_SET_              "set_"
#define STR_CLEAR_            "clear_"
#define STR_SLEEP             "sleep"
#define STR_BUZZER            "buzzer"
#define STR_BRIGHTNESS        "brightness"
#define STR_TOUCH_CAL         "touch_cal"
#define STR_TOUCH_TEST        "touch_test"
#define STR_VOL               "vol"
#define STR_VOL_INC           "vol_inc"
#define STR_VOL_DEC           "vol_dec"
#define STR_STEP              "step"
#define STR_MUTE              "mute"
#define STR_TIME              "time"
#define STR_AUDIO             "audio"
#define STR_VIDEO             "video"
#define STR_WRITE_            "write_"
#define STR_HEX               "hex"
#define STR_FILE_NAME         "file_name"
#define STR_DATA              "data"
#define STR_READ_             "read_"
#define STR_OFFSET            "offset"
#define STR_LEN               "len"
#define STR_FILE              "file"
#define STR__SIZE             "_size"
#define STR_DELETE_           "delete_"
#define STR_RENAME            "rename"
#define STR_START_              "start_"
#define STR_NEW_NAME          "new_name"
#define STR_DOWNLOAD_         "download_"
#define STR_UNMOUNT_          "unmount_"
#define STR_DISK              "disk"
#define STR_MODE              "mode"
#define STR_SHOW_             "show_"
#define STR_ENABLE            "enable"
#define STR_VISIBLE           "visible"
#define STR_XY                "xy"
#define STR_WH                "wh"
#define STR_STATE             "state"
#define STR_BORDER_           "border_"
#define STR_WIDTH             "width"
#define STR_FG_               "fg_"
#define STR_BG_               "bg_"
#define STR_FONT              "font"
#define STR_ALIGH_            "aligh_"
#define STR_TAKE_SNAPSHOT     "take_snapshot"
#define STR_OPEN_WIN          "open_win"
#define STR_CLOSE_WIN         "close_win"
#define STR_BACK_WIN          "back_win"
#define STR_BACK_WIN_TO       "back_win_to"
#define STR_BACK_HOME         "back_home"
#define STR_TEXT              "text"
#define STR_TEXT_             "text_"
#define STR_VALUE             "value"
#define STR_GET_              "get_"
#define STR_MAX               "max"
#define STR_SHOW_TEXT         "show_text"
#define STR_PERCNET           "percent"
#define STR_MIN               "min"
#define STR_STEP              "step"
#define STR_PLAY              "play"
#define STR_PAUSE             "pause"
#define STR_STOP              "stop"
#define STR_FORMAT            "format"
#define STR_IMAGE             "image"
#define STR_COLOR             "color"
#define STR_INTERVAL          "interval"
#define STR_FRAME             "frame"
#define STR_LOOP              "loop"
#define STR_YOYO              "yoyo"
#define STR_DIRECTION         "direction"
#define STR_SLIDABLE          "slidable"
#define STR_SNAP_             "snap_"
#define STR_MOVE_             "move_"
#define STR_TO_PAGE           "to_page"
#define STR_SCROLL_           "scroll_"
#define STR_HEIGHT            "height"
#define STR_LIST_VIEW         "list_view"
#define STR_WIDTH             "width"
#define STR_DELTA_            "delta_"
#define STR_LULL              "lull"
#define STR_DURATION          "duration"
#define STR_RANGE             "range"
#define STR_DRAW_TYPE         "draw_type"
#define STR_SCALE             "scale"
#define STR_ROTATION          "rotation"
#define STR_SELECTED          "selected"
#define STR_SPACING           "spacing"
#define STR_DATE              "date"
#define STR_CHECKED           "checked"
#define STR_ANGLE             "angle"
#define STR_TYPE              "type"
#define STR_VIEW              "view"
#define STR_AUTO_PLAY         "auto_play"
#define STR_SIZE              "size"
#define STR_ALIGN_V           "align_v"
#define STR_SYSTEM            "system"
#define STR_WIDGET            "widget"
#define STR_WINDOW            "window"
#define STR_LABEL             "label"
#define STR_EDIT              "edit"
#define STR_SLIDE_            "slide_"
#define STR_MENU              "menu"
#define STR_PORGRESS_BAR      "progress_bar"
#define STR_SLIDER            "slider"
#define STR_IMAGE_ANIMATION   "image_animation"
#define STR_IMAGE_VALUE       "image_value"
#define STR_TEXT_SELECTOR     "text_selector"
#define STR_INDICATOR         "indicator"
#define STR_DIGIT_CLOCK       "digit_clock"
#define STR_SWITCH            "switch"
#define STR_CHECK_BUTTON      "check_button"
#define STR_RADIO_BUTTON      "radio_button"
#define STR_GAUGE             "gauge"
#define STR_GAUGE_POINTER     "gauge_pointer"
#define STR_START_INDEX       "start_index"
#define STR_END_INDEX         "end_index"
#define STR_END               "}>ET"
#define STR_INDEX              "index"
#define STR_MODE_PUSH         "\"mode\":\"push\","
#define STR_SHOW              "show"
#define STR_SMOOTH            "smooth"
#define STR_LINE              "line"
#define STR_AREA              "area"
#define STR_SYMBOL            "symbol"
#define STR_CAPACITY          "capacity"
#define STR_HSCROLL_          "hscroll_"
#define JSON_END              1

#define STR_TYPE_NUM  strcmp(name,STR_SLEEP)==0||\
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
void set_sleep(char* _tf);

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

// Call the example: set_vol("50");
// Call the example: set_vol("+5");
// Call the example: set_vol("-5");
void set_vol(char* _value);

// Call the example: set_mute("true");
// Call the example: set_mute("false");
void set_mute(char* _tf);

// Call the example: set_av_play("audio", "01.wav");
// Call the example: set_av_play("video", "01.mp4");
void set_av_play(char* _av, char* _name);

// Call the example: set_av_pause("audio", "01.wav");
// Call the example: set_av_pause("video", "01.mp4");
void set_av_pause(char* _av, char* _tf);

// Call the example: set_av_stop("audio");
// Call the example: set_av_stop("video");
void set_av_stop(char* _av);

// Call the example: write_hex("config.bin", "[20,40,31,32]");
void write_hex(char* file_name, char* _data);

// Call the example: write_text("config.txt", "stone_text");
void write_text(char* file_name, char* _data, ...);

// Call the example: write_line("config.txt", "stone_text_one");
#define write_line(file_name_, _data_) write_text(file_name_, _data_, RE_DEFINE1)

// Call the example: read_hex("config.bin", "0", "10");
void read_hex(char* file_name, char* _offset, char* _len);

// Call the example: read_line("log.txt", "10");
void read_line(char* file_name, char* _line);

// Call the example: get_file_size("log.txt");
void get_file_size(char* file_name, ...);

// Call the example: delete_file("log.txt");
#define delete_file(file_name_) get_file_size(file_name_, RE_DEFINE1)

// Call the example: set_rename("log.txt", "gol.txt");
void set_rename(char* file_name, char* _new_name);

// Call the example: download_file("log.txt", "u_disk");
void download_file(char* file_name, char* _mode);

// Call the example: unmount_disk();
void unmount_disk(void);

/* Command interface for the command to set the enable state of the widget */
// Call the example: set_enable("switch1", "false");
void set_enable(char* m_name, char* m_tf, ...);

/* Command interface for the command to set the visible state of the widget */ 
// Call the example: set_visible("switch1", "false");
#define set_visible(m_name, m_tf) set_enable(m_name, m_tf, RE_DEFINE1)

/* Command interface for the command to set the coordinate position of the widget */
// Call the example: set_xy("switch1", "100", "200");
void set_xy(char* _name, char* _x, char* _y);

// Call the example: get_xy("switch1");
void get_xy(char* _name, ...);

// Call the example: get_wh("switch1");
#define get_wh(m_name) get_xy(m_name, RE_DEFINE1)

/* Command interface for the command Control state */
// Call the example: set_state("button1", "pressed");
void set_state(char* _name, char* _state);

// Call the example: set_border_type("button1", "normal", "15");
void set_border_type(char* _name, char* _state, char* _value, ...);

// Call the example: set_border_width("button1", "normal", "1");
#define set_border_width(m_name, m_state, m_value) set_border_type(m_name, m_state, m_value, RE_DEFINE1)

// Call the example: set_fg_image("button1", "normal", "guage_bg");
void set_fg_image(char* _name, char* _state, char* _image, ...);

// Call the example: set_bg_image("button1", "pressed", "guage_bg2");
#define set_bg_image(m_name, m_state, m_image) set_fg_image(m_name, m_state, m_image, RE_DEFINE1)

// Call the example: set_font("label2", "normal", "default");
void set_font(char* _name, char* _state, char* _font, ...);

// Call the example: set_font_size("label2", "normal", "18");
#define set_font_size(m_name, m_state, m_font_size) set_font(m_name, m_state, m_font_size, RE_DEFINE1)

// Call the example: set_text_align_h("label2", "normal", "2");
#define set_text_align_h(m_name, m_state, m_align_h) set_font(m_name, m_state, m_align_h, RE_DEFINE2)

// Call the example: set_text_align_v("label2", "normal", "4");
#define set_text_align_v(m_name, m_state, m_align_v) set_font(m_name, m_state, m_align_v, RE_DEFINE3)

/* Command interface for the Set the background color */
// Call the example: set_color("button1", "bg_color", "4278190080");
void set_color(char* _name, char* _object, char* _color);

/* Command interface for the command to open a window */
// Call the example: open_win("window2");
void open_win(char* _name, ...);

// Call the example: take_snapshot("window2");
#define take_snapshot(m_name) open_win(m_name, RE_DEFINE3)

/* Command interface for the command to close the window */
// Call the example: close_win("window1");
#define close_win(m_name) open_win(m_name, RE_DEFINE1)

/* Command interface to return to the previous level of window */
// Call the example: back_win();
void back_win(void);

/* Return to the command interface of the specified window */
// Call the example: back_win_to("window1");
#define back_win_to(m_name) open_win(m_name, RE_DEFINE2)

/* Command interface for returning the main window command */
// Call the example: back_home();
void back_home(void);

/* command interface for Setting the text commands */
// Call the example: set_text("label", "label5", "hello stone");
void set_text(char* _type, char* _name, char* _text);

/* command interface for Setting the text commands */
// Call the example: show_text("progress_bar", "progress_bar1", "true");
// Call the example: show_text("progress_circle", "progress_circle1", "false");
void show_text(char* _type, char* _name, char* _tf);

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
#define set_pause(m_name) set_play(m_name,RE_DEFINE1)

/* Command interface for setting the picture animation stop command */
//// Call the example: set_stop("image_animation1");
#define set_stop(m_name)   set_play(m_name,RE_DEFINE2)

/* Command interface for setting format commands */
// Call the example: set_format("image_value", "iamge_value1", "%02.2f");
// Call the example: set_format("digit_clock", "digit_clock1", "hh:mm:ss");
void set_format(char* _type, char* _name, char* _format);

/* Command interface for setting picture commands */
// Call the example: set_image("image_value", "iamge_value1", "light_on");
void set_image(char* _type, char* _name, char* _image);

/* Command interface for the set picture animation interval command */
// Call the example: set_interval("image_animation1", "100");
void set_interval(char* _name, char* _interval, ...);

// Call the example: set_frame("image_animation1", "1");
#define set_frame(m_name, m_frame) set_interval(m_name, m_frame,RE_DEFINE1)

/* Command interface for setting the loop command */
// Call the example: set_loop("image_animation", "image_animation1", "true");
// Call the example: set_loop("hscroll_label", "hscroll_label2", "true");
void set_loop(char* _type, char* _name, char* _tf, ...);

// Call the example: set_yoyo("image_animation", "image_animation1", "true");
// Call the example: set_yoyo("hscroll_label", "hscroll_label2", "false");
#define set_yoyo(m_type, m_name, m_tf) set_loop(m_type, m_name, m_tf,RE_DEFINE1)

// Call the example: set_direction("image_animation", "image_animation1", "false");
// Call the example: set_direction("hscroll_label", "hscroll_label2", "false");
#define set_direction(m_type, m_name, m_tf) set_loop(m_type, m_name, m_tf,RE_DEFINE2)

// Call the example: set_lull("hscroll_label2", "2000");
#define set_lull(m_name, m_lull) set_loop(STR_HSCROLL_ STR_LABEL, m_name, m_lull,RE_DEFINE3)

// Call the example: set_duration("hscroll_label2", "2000");
#define set_duration(m_name, m_duration) set_loop(STR_HSCROLL_ STR_LABEL, m_name, m_duration,RE_DEFINE4)

/* Command interface for setting range commands */
// Call the example: set_range("image_animation", "image_animation1", "1" , "10");
// Call the example: set_range("x_axis", "x_axis1", "1" , "10");
// Call the example: set_range("y_axis", "y_axis1", "1" , "20");
void set_range(char* _type, char* _name, char* start, char* end);

/* Command interface for the set picture display method command */
// Call the example: set_draw_type("image", "image1" , "2");
void set_draw_type(char* _type, char* _name, char* draw_type);

/* Command interface for the set image scaling command */
// Call the example: set_scale("image", "image2", "0.5" , "1");
void set_scale(char* _type, char* _name, char* _x, char* _y);

/* Command interface for the set picture rotation angle command */
// Call the example: set_rotation("image", "image1", "180");
// Call the example: set_rotation("gif", "gif1", "180");
// Call the example: set_rotation("svg", "svg1", "180");
void set_rotation(char* _type, char* _name, char* _rotation);

/* Command interface for setting the current selector command of the text selector */
// Call the example: set_selected("text_selector","text_selector1", "20");
// Call the example: set_selected("combo_box_ex","combo_box_ex1", "6");
void set_selected(char* _type, char* _name, char* _selected);

/* Command interface to get the current selector command of the text selector */
// Call the example: get_selected("text_selector","text_selector1");
// Call the example: get_selected("combo_box_ex","combo_box_ex1");
void get_selected(char* _type, char* _name);

/* Command interface for the set date command */
// Call the example: set_date("2022-02-22 22:22:22");
void set_date(char* _name, char* _date);

/* Command interface for the get date command */
// Call the example: get_date("clock1");
void get_date(char* _name);

/* Command interface for the get checked command */
// Call the example: get_checked("radio_button1");
void get_checked(char* _name);

// Call the example: set_size("slide_indicator1", "5");
void set_size(char* _name, char* _size);

// Call the example: set_spacing("slide_indicator", "slide_indicator1", "15");
// Call the example: set_spacing("slide_indicator_arc", "slide_indicator_arc1", "10");
void set_spacing(char* _type, char* _name, char* _spacing);

// Call the example: set_view("slide_view1", "2");
void set_view(char* _name, char* _index, ...);

// Call the example: set_auto_play("slide_view2", "1000");
#define set_auto_play(m_name, m_auto_play) set_view(m_name, m_auto_play, RE_DEFINE1)

// Call the example: get_view("slide_view", "slide_view1");
// Call the example: get_view("tab_view", "tab_view2");
void get_view(char* _type, char* _name);

/* Command interface for the set angle command */
// Call the example: set_angle("pointer1", "10");
void set_angle(char* _name, char* _angle, ...);

// Call the example: set_start_angle("pie_slice2", "60");
#define set_start_angle(m_name, m_angle) set_angle(m_name, m_angle, RE_DEFINE1)

// Call the example: set_align_v("slide_view1", "2");
void set_align_v(char* _name, char* _align_v);

// Call the example: set_xslidable("scroll_view2","true");
void set_xslidable(char* _name, char* _value, ...);

// Call the example: set_yslidable("scroll_view2","true");
#define set_yslidable(m_name, m_value) set_xslidable(m_name, m_value,RE_DEFINE1)

// Call the example: set_snap_to_page("scroll_view2","true");
#define set_snap_to_page(m_name, m_value) set_xslidable(m_name, m_value,RE_DEFINE2)

// Call the example: set_move_to_page("scroll_view2","true");
#define set_move_to_page(m_name, m_value) set_xslidable(m_name, m_value,RE_DEFINE3)

// Call the example: set_scroll_to_x("scroll_view2","50");
void set_scroll_to_x(char* _name, char* _offset, ...);

// Call the example: set_scroll_to_y("scroll_view2","50");
#define set_scroll_to_y(m_name, m_offset) set_scroll_to_x(m_name, m_offset,RE_DEFINE1)

// Call the example: set_scroll_delta_to_x("scroll_view2","50");
#define set_scroll_delta_to_x(m_name, m_offset) set_scroll_to_x(m_name, m_offset,RE_DEFINE2)

// Call the example: set_scroll_delta_to_y("scroll_view2","50");
#define set_scroll_delta_to_y(m_name, m_offset) set_scroll_to_x(m_name, m_offset,RE_DEFINE3)

// Call the example: set_scroll_to_xy("scroll_view2","50","50");
void set_scroll_to_xy(char* _name, char* _xoffset, char* _yoffset, ...);

// Call the example: set_scroll_delta_to_xy("scroll_view2","-50","-50");
#define set_scroll_delta_to_xy(m_name, m_xoffset, m_yoffset) set_scroll_to_xy(m_name, m_xoffset, m_yoffset,RE_DEFINE1)

// Call the example: set_height("list_view0","40");
void set_height(char* _name, char* _height, ...);

// Call the example: set_width("list_view_h3","60");
#define set_width(m_name, m_width) set_height(m_name, m_width,RE_DEFINE1)

/* Command interface for the set the curve sequence point data */
// Call the example: chart_set_value_index("line_series", "line_series1", "1", "100");
// Call the example: chart_set_value_index("line_series", "line_series1", "1", "[10,29,69,45,67,34]");
void chart_set_value_index(char* _type, char* _widget, char* _index, char* _value);

/* Command interface for the set the curve sequence point data
The sequence point is set to multiple values in the way of push, 
that is, the data is appended to the end and the previous data is moved forward*/
// Call the example: chart_set_value_push("line_series", "line_series1", "23");
// Call the example: chart_set_value_push("line_series", "line_series1", "[10,29,69,45,67,34]");
void chart_set_value_push(char* _type, char* _widget, char* _value);

/* Command interface for the ets whether the curve sequence boundary is displayed and smooth (line_series only) */
// Call the example: chart_set_line("line_series", "line_series1", "true", "false");
void chart_set_line(char* _type, char* _widget, char* _show, char* _smooth);

/* Command interface for the sets whether the curve sequence boundary is displayed and smooth (line_series only) */
// Call the example: chart_set_area("line_series", "line_series1", "true");
void chart_set_area(char* _type, char* _widget, char* _show);

/* Command interface for the sets whether the curve sequence point marker is displayed (line_series only) */
// Call the example: chart_set_symbol("line_series", "line_series1", "true");
void chart_set_symbol(char* _type, char* _widget, char* _show);

/* Command interface for the set the FIFO capacity of curve sequence points */
// Call the example: chart_set_capacity("line_series", "line_series1", "15");
void chart_set_capacity(char* _type, char* _widget, char* _value);

/* Command interface for the obtain the point data of curve sequence */
// Call the example: chart_get_value("line_series", "line_series1", "4");
void chart_get_value(char* _type, char* _widget, char* _index);

/* Command interface for the gets the cancel sequence point FIFO capacity */
// Call the example: chart_get_capacity("line_series", "line_series1");
void chart_get_capacity(char* _type, char* _widget);

#endif
