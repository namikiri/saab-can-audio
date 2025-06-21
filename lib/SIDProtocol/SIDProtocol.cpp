#include "SIDProtocol.h"

SID_BUTTON SID_getWheelButton(char command) {
    if ((command >> 7) & 1)
        return BTN_WHEEL_VOL_DOWN;
    else if ((command >> 6) & 1)
        return BTN_WHEEL_VOL_UP;
    else if ((command >> 5) & 1)
        return BTN_WHEEL_SRC;
    else if ((command >> 4) & 1)
        return BTN_WHEEL_SEEK_RIGHT;
    else if ((command >> 3) & 1)
        return BTN_WHEEL_SEEK_LEFT;
    else if ((command >> 2) & 1)
        return BTN_WHEEL_NXT;
    else
        return BTN_NONE;
};

SID_BUTTON SID_getPanelButton(char command) {
    if ((command >> 7) & 1)
        return BTN_SID_CLR;
    else if ((command >> 6) & 1)
        return BTN_SID_SET;
    else if ((command >> 5) & 1)
        return BTN_SID_DOWN;
    else if ((command >> 4) & 1)
        return BTN_SID_UP;
    else if ((command >> 3) & 1)
        return BTN_SID_NIGHT;
    else if ((command >> 2) & 1)
        return BTN_SID_CLOCK_PLUS;
    else if ((command >> 1) & 1)
        return BTN_SID_CLOCK_MINUS;
    else
        return BTN_NONE;
};

bool SID_processCommand(unsigned char command[], SID_BUTTON *btn) {
    if (command[0]) { // state changed
        *btn = SID_getWheelButton(command[2]);

        if (*btn == BTN_NONE) {
            *btn = SID_getPanelButton(command[3]);
        }
    }

    return command[0];
}

// Makes SID to beep once, like when you set ACC mode
void SID_beep(MCP_CAN *CAN) {
    byte data[8] = {0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN->sendMsgBuf(SID_CANTX_BEEP, 8, data);
}

// WIP/TODO
void SID_requestAccess(MCP_CAN *CAN, char accessCode) {

    unsigned char request[8] = {
        0x1f,
        0x02,
        accessCode,
        0x12,
        0x00,
        0x00,
        0x00,
        0x00
    };

    CAN->sendMsgBuf(SID_CANTX_ACCESS, 8, request);
}

// WIP/TODO
void SID_sendMessage(MCP_CAN *CAN, char message[]) {
    byte _messageBuff1[8] = {0x42, 0x96, 0x2, 0, 0, 0, 0, 0};
    byte _messageBuff2[8] = {0x1, 0x96, 0x2, 0, 0, 0, 0, 0};
    byte _messageBuff3[8] = {0x0, 0x96, 0x2, 0, 0, 0, 0, 0};

    byte spaRequest[8] = {0x1F, 0x01, 0x05, 0x12, 0x00, 0x00, 0x00, 0x00};

    memcpy(&_messageBuff1[3], &message[0], 5);
    memcpy(&_messageBuff2[3], &message[5], 5);
    memcpy(&_messageBuff3[3], &message[10], 2);

    CAN->sendMsgBuf(0x357, 8, spaRequest);

    CAN->sendMsgBuf(SID_CANTX_TEXT, 8, _messageBuff1);
    delay(10);
    CAN->sendMsgBuf(SID_CANTX_TEXT, 8, _messageBuff2);
    delay(10);
    CAN->sendMsgBuf(SID_CANTX_TEXT, 8, _messageBuff3);
}