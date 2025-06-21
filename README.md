# Firmware for SAAB CAN Adapter for Pioneer audio
Made for SAPRO's CAN adapter with all its components and pinouts but you're free to adapt this code for your hardware and needs.

# Made for
- SAPRO's CAN adapter for Saab CAN bus
- Pioneer car audio with Pioneer's standard W/R TRS port
- Saab 9-5 '98-'02 models (probably will work with 9-3 OG)

# Hardware
- Arduino Pro Mini board with Optiboot or Arduino-compatible bootloader
- MCP41050 digital potentiometer
- MCP2515-based CAN BUS adapter Arduino shield
- CPC135N optical relay

# Software
This project is made primarily for PlatformIO. Everything is tuned for ATMEGA168PA MCU with 16 MHz clock. Yes, the `board` in `platformio.ini` says `168pa8m`; don't let that `8m` confuse you and look at `board_build.f_cpu = 16000000L` line.

To let MCP2515 run on a Saab-specific speed of 47.619 kbps I've modified the mcp_can library by adding a CAN_47KBPS for both 8 MHz and 16 MHz crystals. The modified version can be found [here](https://github.com/namikiri/SAAB_CAN). This dependency is already set in `platformio.ini`

# Experiments
The values in [PioneerResistances.h](lib/PioneerResistances/PioneerResistances.h) are found experimentally by brute-forcing all possible options from `0x00` to `0xFF`. The audio unit responds not to exact resistance but to a range of them so we just take the mean value as correct one. If you use something other than MCP41050, these values will be different. If you've found the values for another chip, feel free to make a PR or tell me about them and I'll add them to this code.

# TODOs/Wishes/Dreams
- better & longer test-drives, more feedback
- use NXT button to answer the call over BT - needs some investigation w/modern Pioneer
- auto speed-dependent volume control
- handle long presses of NXT/SRC buttons to do something (enable/disable auto volume?)
- show text messages on SID for better feedback
- something else that I didn't think about yet?

# Inspirations
- [SAPRO's CAN adapters](http://saab.sapro.ru/can-adapter/) - hope the author won't get mad at me for this
- [Decoded I-BUS messages](https://pikkupossu.1g.fi/tomi/projects/i-bus/i-bus.html#disc)
- [mbaum0/saab-canbus](https://github.com/mbaum0/saab-canbus) - decoding SID key messages over CAN
- [mbaum0/CAN_BUS_Shield](https://github.com/mbaum0/CAN_BUS_Shield) - to find out that `CAN_47KBPS` isn't standard speed