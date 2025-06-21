/*

  SAAB CAN Audio adapter firmware
  Made for:
    - SAPRO's CAN adapter for Saab CAN bus
    - Pioneer W/R standard resistive wheel control array
    - Saab 9-5 98-02 models

  Hardware:
    - Arduino Pro Mini board with Optiboot or Arduino-compatible bootloader
    - MCP41050 digital potentiometer
    - MCP2515-based CAN BUS adapter Arduino shield
    - CPC135N optical relay


  Made by Asterleen // Marko Deinterlace
       ~ https://asterleen.com ~


*/

#include <Arduino.h>
#include <SPI.h>

#include <PioneerResistances.h>
#include <SIDProtocol.h>

#include "MCP_POT.h"
#include "mcp_can.h"

#define IBUS_PIN_CS  9
#define IBUS_PIN_INT 2
#define IBUS_SPEED CAN_47KBPS

#define POT_PIN_CS 4
#define POT_PIN_ACTIVATE PIN_A4

#define DEBUG_MODE 1

MCP41050 pot(POT_PIN_CS, 0, 0, &SPI);
MCP_CAN CAN(IBUS_PIN_CS);

// CAN bus buffer variables
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

// Current SID reported button
// We define it globally to use it internally
// like for long presses and special commands
// Probably this gonna be a map of buttons to handle 
// combined presses reported by SID
SID_BUTTON currentButton; 

void setResistance(byte val) {
  pot.setValue(val);

  Serial.write("Setting resistance code to 0x");
  Serial.print(val, HEX);
  Serial.write("\n");
}

// SAPRO's CAN adapter doesn't have the digipot
// connected directly but uses a CPC1135N optical relay.
// This is done to prevent the digipot damage and also
// decreases the chance of random commands being sent
// to the car audio.
// Its control pin is connected to Arduino's A4 pin
// so we set it to HIGH to connect the resistor
void setResistorConnected(bool isConnected) {
  digitalWrite(POT_PIN_ACTIVATE, isConnected ? HIGH : LOW);
}

void setup() {
  pinMode(IBUS_PIN_INT, INPUT);
  pinMode(POT_PIN_ACTIVATE, OUTPUT);
  digitalWrite(POT_PIN_ACTIVATE, 0);

  Serial.begin(9600);

  Serial.println("SAAB CAN Audio adapter open source firmware");
  Serial.println(" Made by Asterleen ~ https://asterleen.com");
  Serial.println("===========================================\n\n");

  SPI.begin();
  pot.begin();

  // TODO: setup hardware defined masks/filters?
  if(CAN.begin(MCP_ANY, IBUS_SPEED, MCP_8MHZ) == CAN_OK)
    Serial.println("CAN bus initialize OK");
  else
    Serial.println("Error initializing CAN BUS");
  
  CAN.setMode(MCP_NORMAL);

}

void loop() {
  if(!digitalRead(IBUS_PIN_INT))
  {
    if (CAN.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK) {

      switch (rxId) {
        case SID_CANRX_BUTTONS:
          bool isStateChanged = SID_processCommand(rxBuf, &currentButton);

          if (isStateChanged) {
            byte resistance = 0x00;

            switch (currentButton) {
              case BTN_WHEEL_VOL_DOWN:
                resistance = PIONEER_CTL_VOL_DOWN;
                break;

              case BTN_WHEEL_VOL_UP:
                resistance = PIONEER_CTL_VOL_UP;
                break;

              case BTN_WHEEL_SRC:
                resistance = PIONEER_CTL_SRC;
                break;

              case BTN_WHEEL_SEEK_RIGHT:
                resistance = PIONEER_CTL_TRACK_NEXT;
                break;
              
              case BTN_WHEEL_SEEK_LEFT:
                resistance = PIONEER_CTL_TRACK_PREV;
                break;

              case BTN_WHEEL_NXT:
                resistance = PIONEER_CTL_ATT; // todo: "call pick up" command?
                break;

              case BTN_NONE:
                Serial.print("SID wheel buttons are released: ");
                Serial.println(currentButton, DEC);

                setResistorConnected(false); // stop reporting button press to the audio
                break;

              default:
                break;
            };

            if (resistance != 0x00) { // we've detected some SID wheel button; it can't be 0x00; see PioneerResistances.h
              Serial.print("SID wheel button pressed: ");
              Serial.println(currentButton, DEC);   
              
              setResistance(resistance);
              setResistorConnected(true); // this will held until SID reports BTN_NONE (all buttons are released)
            }
          };

          break;
      }
    }
  }
}
