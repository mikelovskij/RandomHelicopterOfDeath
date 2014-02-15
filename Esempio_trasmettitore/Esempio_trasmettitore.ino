// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Original Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
// Modified by: Martino Bernard - do whatever 'u wish


#include <VirtualWire.h>

#define  len 4          //Number of uint8_t in pacchettino
#define  address 101    //Check address

const int led_pin = 11;          // Led to flash whenever transmitting
const int transmit_pin = 12;     // Transmitter pin
const int transmit_en_pin = 13;  // Eventual enable (to spare energy) for the transmitter
/*
boolean B_X;
boolean B_Y;
boolean B_A;
boolean B_B;
boolean B_Start;
boolean B_Select;
boolean B_Bop;
int B_LR;
int B_UD;
*/
int i;

void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true);       // Required for DR3100
    vw_setup(2000);                 // Bits per sec
    pinMode(led_pin, OUTPUT);
    // Sets the inputs from the JoyStick
    pinMode(1,INPUT);
    pinMode(2,INPUT);
    pinMode(3,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
}

void loop()
{

  //The data are read
  /*
  B_LR=analogRead(A0);  
  B_UD=analogRead(A1);  
  B_B=digitalRead(2);
  B_Y=digitalRead(3);
  B_X=digitalRead(4);
  B_A=digitalRead(5);
  B_Start=digitalRead(6);
  B_Select=digitalRead(7);
  B_Bop=digitalRead(8);
  */
  
  uint8_t pacchettino[len];
  
  pacchettino[0]=address<<5;          // The address is stored in the firs 3 bits
  pacchettino[1]=analogRead(A0)>>2;   // Writes the 8-bit output of A0(LR) into pacchettino[1]
  pacchettino[2]=analogRead(A1)>>2;   // Writes the 8-bit output of A1(UD) into pacchettino[2]   
  pacchettino[3]=0;                   // Initialising for the 8-bits of the buttons
  for(i=0;i<8;i++)
    {
     pacchettino[3]=pacchettino[3]+(digitalRead(i+2)<<i); // Write & shift the buttons boolean values
    }

  //The data are sent
  digitalWrite(led_pin, HIGH);  // Flash a light to show transmitting
  vw_send(pacchettino, len);    // Transmits the pacchettino
  vw_wait_tx();                 // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);   //
  delay(1);                     // May set a different delay
}
