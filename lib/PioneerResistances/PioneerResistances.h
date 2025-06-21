#ifndef _PIONEERRESISTANCES_H
#define _PIONEERRESISTANCES_H

/*

    These are definitions of resistance values 
    that are meant to be sent to MCP41050 digipot.

    These values were gathered experimentally and may be different
    for your combination of the audio and digipot you're gonna use

*/

// Band/Escape: 0x00 ~ 0x56
#define PIONEER_CTL_BAND 0x2B

// Volume Down: 0x77 ~ 0xA7
#define PIONEER_CTL_VOL_DOWN 0x8F

// Volume Up: 0xA8 ~ 0xC2
#define PIONEER_CTL_VOL_UP 0xB5

// Previous Track: 0xC3 ~ 0xD5
#define PIONEER_CTL_TRACK_PREV 0xCC

// Next Track: 0xD6 ~ 0xE1
#define PIONEER_CTL_TRACK_NEXT 0xDB

// Mute/Attenuate: 0xE9 ~ 0xF5
#define PIONEER_CTL_ATT 0xEF

// Source: 0xF6 ~ 0xFE
#define PIONEER_CTL_SRC 0xFA

// TODO: measure resistances on a more modern Pioneer model
// to find some more resistances/commands

#endif