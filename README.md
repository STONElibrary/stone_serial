# stone_serial  
# Website: https://www.stoneitech.com/
# Arduino library: https://github.com/STONElibrary/arduino_lib/
/* 
 * 	  
 * 	Some instructions for using the STONE serial library v1.0.8
 * 
 *   This serial library is created to adapt the commands of STONE serial screen, in order to increase the portability of the code is written in C language, for the current mainstream of several MCUs to do test verification.
 * Please read this document before using it for the first time.
 * 
 *  The "stone_config.h" file currently defines four initialization interfaces for the development board：
 * #define MCU_STM32                                0
 * #define MCU_ARDUINO                              1
 * #define MCU_ESP                                  0
 * #define MCU_Raspberry_Pi                         0
 * If you use these four types of development boards, you only need to change the definition following from 0 to 1. If you use other series of development boards, you can refer to the initialization templates of the four types of development boards defined following to adjust themselves.
 * The initialization calls are the development board's serial library and several C language standard libraries, while the delay function in the library are also macro-defined as STONE_Delay, so it is easier to change in the header file.

 *
 *
 * After configuring the "stone_config.h" file, you can start using all the functions in the library for data processing, as described below.
 *
 * The received data is currently received byte by byte and then judged, if it meets the criteria, then continue to receive if not, then discard.
 * The CRC checksum of the two bytes at the end of the command frame is also verified to ensure the integrity of the received data.
 * If you use Arduino IDE, you can see the received data and the legal or illegal indication in the serial monitor.
 * For example, the return data of the button control：53 54 3C 10 1 0 9 62 75 74 74 6F 6E 35 32 1 3E 45 54 63 91 legitimate(This word is used to prompt)
 * 
 * The data will be stored into STONE_RX_BUF buffer with a size of 200 bytes after the reception is completed, after which the user can choose whether to call the command data parsing interface or not.
 * If enabled, the data will be split and parsed for each part of this data, which is convenient for users to interpret and use.
 * The storage structure when parsing the data is as follows.
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
 * 
 * The structure variables are as follows.
 * recive_group STONER;
 * The call format is      STONER.cmd、  STONER.len、    STONER.value、     STONER.float_value、    STONER.long_value
 *            	                     STONER.data、  STONER.widget、 STONER.text
 * 
 * STONE.cmd will save the cmd_code value in the returned command for subsequent parsing, which can be called by users according to their needs.
 * STONE.value will save the len value of the returned command for subsequent parsing, or you can call it according to your needs.

 * When there is a value in the command, the return data will be converted to the corresponding type of value according to the type of value.
 * For example, the return value 0 or 1 of the switch control is stored in STONER.value.
 * The return value 66.66 or 10.00 of the slider control is stored in STONER.float_value.
 * 40% of the percentage value returned by the progress bar control is stored by 4 bytes in the command, so the corresponding long type in the arduino is stored in STONER.long_value

 * 
 * STONER.data creates storage for system-related commands or commands without widgets
 * For example, the data returned when getting the system version number 53 54 3C 0 2 0 10 56 32 2E 30 2E 30 37 2E 32 31 31 31 31 31 39 52 43 3E 45 54 7E 98
 * 56 32 2E 30 2E 30 37 2E 32 31 31 31 31 31 39 52 43 in this data is the hexadecimal format of the version number
 * then the value in STONER.data after parsing is V2.0.07.211119RC

 * 
 * STONER.widget creates storage for commands with widgets
 * For example, the button control named button9 returns the data 53 54 3C 10 1 0 8 62 75 74 74 6F 6E 39 1 3E 45 54 E7 E0
 * The 62 75 74 74 6F 6E in this data is the string "button9" in hexadecimal format
 * The value in STONER.widget is the string "button9".
 * 
 * STONER.text creates storage for commands with text values
 * For example, the data returned by the edit control on the operation screen after entering the text abcd 53 54 3C 10 70 0 C 22 65 64 69 74 32 22 3A 61 62 63 64 3E 45 54 7 D2
 * 61 62 63 64 in this data is the hexadecimal format of the string "abcd".
 * The value in STONER.text is abcd
 * 
 * In order to save memory, the above three pointer variables will request memory space dynamically according to the required length and will not be automatically released after parsing, in consideration of the user's call requirements.
 * The requested memory space will be created when the data is received for the first time, and then the space of the last data request will be released and re-requested after the new data is received
 * The user can manually release the requested space by calling stone_recive_free(); interface, or by calling stone_recive_free("name"); # The name can have the optional values of data, widget, text
 *
 *
 * To send a command there are currently two processing options for the user to choose from.
 * 
 * Option 1. call the packaged send command interface, in the "stone_transport.c" file, the user can see all the existing send command function interfaces, all interfaces are grouped by the cmd_code of the send command, reducing memory usage and complexity.
e.g. set_value(); interface can be used to set the value of edit, set the value of label, set the value of slider, set the value of switch, etc.
 * There are comments and examples on each interface, so please set the parameters you want to pass when calling according to the corresponding examples.

 *
 * For example, to set the value of the label1 widget to 66.66, the original command would be
 * ST<{"cmd_code": "set_value", "type": "label", "widget": "label1", "value":66.66, "format":"%.2f"}>ET
 * Then the call format is set_value("label", "label5", "66.66", "%.2f");
 * Note that the numeric parameters in the send command are also always of string type in the library, such as "66.66" above

 * 
 * Option 2. call the interface STONE_JSON(); This interface is a packet function for sending commands, each call will be a packet, and the packet will be stored in the send buffer when it is finished
 * For example: Set the text of edit1 control to Hello Stone The original command is
 * ST<{"cmd_code":"set_text","type":"edit","widget":"edit1","text":"Hello Stone"}>ET
 * The call format is as follows：
 * 		STONE_JSON("cmd_code", "set_text");
 * 		STONE_JSON("type", "edit");
 * 		STONE_JSON("widget", "edit1");
 * 		STONE_JSON("text", "Hello Stone" , JSON_END);
 * After the above actions are completed, the packet sending command is completed, where the rule is that the first parameter is the name in JSON format, the second parameter is the value in JSON format, both in the form of string.
 * The third parameter is optional, when the user has finished the group packet, JSON_END should be added at the end to be regarded as the completion of the group packet.
 * 
 * The send buffer for the above two solutions is STONE_TX_BUF, with a size of 200 bytes, and the send implementation function has a timeout process to ensure that each command is sent successfully when the user calls the send interface continuously.
 * It is recommended to send using idle send or RTOS subtask to reduce the time taken for sending.
 * 
 * 
*/
