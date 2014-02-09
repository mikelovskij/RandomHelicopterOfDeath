/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "Parametri.h"
#include "Wire.h"
#include "Arduino.h"
#include <VirtualWire.h>
#include "Debug.h" 
#include "Ricevitore_lib.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h"  //inutile se c'è già incluso "MPU6050_6Axis_MotionApps20.h"
#include "prendidati.h"
#include "conversionivariabili.h"
#include "ServoTimer2.h"
#include "motoreggiatore.h"





//////////////////////////////////////////////////////////////////DATA////////////////////////////////////////////////////////////////////////////////////////
long inputdata[6]; //misure fatte dalla libreria che prende i dati dall'arduino e filtra e li sputa. le 6 componenti corrispondono ad ax,ay,az,gx,gy,gz

double fb_data[4];//output del modulo feedback
double tc_data[4];//output del telecomando
double ph_data[4];//variabili fusiche

int servo_data[4];//variabili servo (in microsecondi!) 

//////////////////////////////////////////////////////////////////SINCRONIZZAZIONE DEL LOOP////////////////////////////////////////////////////////////////////
unsigned long prev_loop = 0; //buffer che contiene l'ultimo valore di millis usato dal ciclo di sincronizzazione
unsigned long actual_loop; //valore attuale confrontato con prev (volendo si può rimuovere ma servirebbe richiamare la funzione millis() una volta in più.


//////////////////////////////////////////////////////////////ALTRE VARIABILI//////////////////////////////////////////////////////////////////////////////////
bool blinkState = false;


///////////////////////////////////////////////////////////SETUP///////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
	DEBUG_BEGIN(115200);//inizializza la seriale. più è veloce più ciuccia risorse credo, ma in compenso perde meno tempo a trasmettere
	setupmpu();
	setup_myreciver(5,RADIOPIN);
	inizializza_servo(servo_pin, servo_init);
	pinMode(LED_PIN, OUTPUT);
	delay(500);
}

void loop() {
	actual_loop = millis();
	if(actual_loop-prev_loop >=PERIOD){
		TIMING_PRINTLN(actual_loop-prev_loop); 
		prev_loop=actual_loop;    
		prendidati(inputdata);
		refresh_recived_commands();
		mangiadietro(inputdata, fb_data);
		telecomando(tc_data);
		somma(tc_data,fb_data,ph_data);
		converti_fisica_motori(ph_data, servo_data);
		servo_write(servo_data);
		blinkState = !blinkState;
		digitalWrite(LED_PIN, blinkState);
	}
	
}