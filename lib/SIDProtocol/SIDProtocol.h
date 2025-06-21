#ifndef _SIDPROTOCOL_H
#define _SIDPROTOCOL_H

#include "mcp_can.h"

#define SID_CANRX_BUTTONS 0x290
#define SID_CANRX_ACCESS  0x368

#define SID_CANTX_ACCESS  0x357
#define SID_CANTX_BEEP    0x430   
#define SID_CANTX_TEXT    0x337


#define SID_ACCESS_TEST      0x01
#define SID_ACCESS_EMERGENCY 0x02
#define SID_ACCESS_DRIVER    0x03
#define SID_ACCESS_ECU       0x04
#define SID_ACCESS_TEXT      0x05
#define SID_ACCESS_STOP      0xff

typedef enum {
    BTN_NONE,

    // Steering wheel buttons
    BTN_WHEEL_VOL_DOWN,
    BTN_WHEEL_VOL_UP,
    BTN_WHEEL_SRC,
    BTN_WHEEL_SEEK_RIGHT,
    BTN_WHEEL_SEEK_LEFT,
    BTN_WHEEL_NXT,

    // SID panel buttons
    BTN_SID_CLR,
    BTN_SID_SET,
    BTN_SID_DOWN,
    BTN_SID_UP,
    BTN_SID_NIGHT,
    BTN_SID_CLOCK_PLUS,
    BTN_SID_CLOCK_MINUS

} SID_BUTTON;



SID_BUTTON SID_getWheelButton(char command);
SID_BUTTON SID_getPanelButton(char command);

void SID_beep(MCP_CAN *CAN);
bool SID_processCommand(unsigned char command[], SID_BUTTON *btn);
void SID_sendMessage(MCP_CAN *CAN, char message[]);

#endif