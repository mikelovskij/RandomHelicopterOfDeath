// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//     2012-06-20 - improved FIFO overflow handling and simplified read process
//     2012-06-19 - completely rearranged DMP initialization code and simplification
//     2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//     2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//     2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                - add 3D math helper file to DMP6 example sketch
//                - add Euler output and Yaw/Pitch/Roll output formats
//     2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//     2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//     2012-05-30 - basic DMP initialization working

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

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

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
#define DEBUG_1 6 //AGGIUNTI QUESTI OUTPUTS PER COLLEGARE LEDS DI CONTROLLO DELLE VARIE OPERAZIONI.
#define DEBUG_2 7 //è possibile controllare questi multipli outputs con una singola variabile magari? tipo un numero a 6 bits che controlla un led per bit?
#define DEBUG_3 8//sì, ma solo col solito orribile metodo del bitshifting ecc... putost che'n tost... per ora però forse nonserve
#define DEBUG_4 9
#define DEBUG_5 10//probabilmente alcuni di questi leds andranno tolti a favore di altri output più necessari. 
#define DEBUG_6 11


bool blinkState = false;

//////////////////////////////// MPU control/status vars (QUESTE VARIABILI SERVONO PER COMUNICARE CON L'MPU6050)/////////////////////////////////////////////////////////////
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

//////// orientation/motion vars QUESTE DUE VARUABILI CONTENGONO I DATI LETTI DAL GIROACCELEROMETRO, PRIMA DI ESSERE FILTRATI, e altre variabili utili al filtro////////////////
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 gg;
long aaxpart=0;
long aaypart=0;
long aazpart=0;
int f=0;
#define lp 64 //NUMERO DI NUMERI SU CUI FARE LA MEDIA QUANDO SI PASSABASSA
#define arr 6 // LA POTENZA DI DUE PER LA QUALE VIENE DIVISO IL DATO DOPO LA SOMMA, NEL FAR LA MEDIA. PER ORA è ALTO SOLO PER COMODITà.

///////////////////////////////////////////queste variabili sono invece il risultato del filtraggio e dovrebbero essere usate dai feedbacks ////////////////////////////////////
int aax;
int aay;
int aaz;
int gx;
int gy;
int gz;

///////////////////VARIABILI COMANDO: QUESTE VARIABILI DOVREBBERO ESSERE GESTITE DAL MODULO RADIORICEVITORE/DA QUALCHE PULSANTE MANUALE E GESTIRE LE VARIE FUNZIONI////////////


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ======================================================================================================================================================================
// ===                      INITIAL SETUP INSERITE IN QUESTA SEZIONE EVENTUALI COSE DA FARE ALL'ACCENSIONE DEL DISPOSITIVO, POSSIBILMENTE IN FONDO                  ===
// ======================================================================================================================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);//OVVIAMENTE LA COMUNICAZIONE SERIAL USB NON SERVIRà NEL COSO FINALE, MA PUò ESSERE LASCIATA, MAGARI COMMENTATA, PER EVENTUALE DEBUGGING
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.
    
    pinMode(DEBUG_1, OUTPUT);
    pinMode(DEBUG_2, OUTPUT);
    pinMode(DEBUG_3, OUTPUT);
    pinMode(DEBUG_4, OUTPUT);
    pinMode(DEBUG_5, OUTPUT);
    pinMode(DEBUG_6, OUTPUT);
    
    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    mpu.testConnection() ? digitalWrite(DEBUG_1,true) : digitalWrite(DEBUG_1,false);                                //IL PRIMO LED MOSTRA SE SI RIESCE A COMUNICARE CON L'MPU
    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data                                                  //DA SOSTITUIRE CON UN SEMPLICE DELAY O CON UNA VARIABILE CONTROLLATA DA TELECOM O BOTTONE
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        digitalWrite(DEBUG_2,true);                                                               //IL SECONDO LED MOSTRA L'IIZIALIZZAZIONE DEL DMP
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        digitalWrite(DEBUG_1,false); 
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output AGGIUNTI I LED DI DEBUG PRIMA PERCHè SERVONO PRIMA
    pinMode(LED_PIN, OUTPUT);
   
}



// =======================================================================================================================================
// ===                    MAIN PROGRAM LOOP QUESTA è LA PARTE CHE VIENE INVECE RIPETUTA DI CONTINUO DAL PROCESSORE.                    ===
// =======================================================================================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
       
        // other program behavior stuff here
        // .
        // .QUA VA INSERITO TUTTO IL RESTO DEL PROGRAMMA IN TEORIA, E VIENE ESEGUITO MENTRE IL PROCESSORE NON è OCCUPATO DALLA TRASMISSIONE I2C CON IL GIROSCOPIO
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data

     // for()
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
        digitalWrite(DEBUG_3,true);                                                 //                 IL TERZO LED SI ACCENDE IN CASO DI OVERFLOW DELLA FIFO. 

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        digitalWrite(DEBUG_3,false);            

            mpu.dmpGetGyro(&gg, fifoBuffer) ;  
            mpu.dmpGetAccel(&aa, fifoBuffer);
 /////////////////////////////FILTRAGGIO BASE DELL'ACCELERAZIONE CON UNA BANALE MEDIA. C'E' DECISAMENTE SPAZIO PER MIGLIORARE QUA////////////////////////////////         
   if (f<lp){
     aaxpart=aaxpart+aa.x; 
     aaypart=aaypart+aa.y;
     aazpart=aazpart+aa.z;
     f++;
   }
   else{
     aax=aaxpart>>arr;
     aay=aaypart>>arr;
     aaz=aazpart>>arr;
     aaxpart=0;
     aaypart=0;
     aazpart=0;
     f=0;
   }
 ///trasmissione dei dati cheserve controllare alla seriale///////////////////         
            Serial.print(millis());
            Serial.print("a\t");
            Serial.print(aa.x);
            Serial.print("\t");
            Serial.print(aa.y);
            Serial.print("\t");
            Serial.print(aa.z);
            Serial.print("\t");
            Serial.print(aax);
            Serial.print("\t");
            Serial.print(aay);
            Serial.print("\t");
            Serial.print(aaz);
             Serial.print("\tg\t");
            Serial.print(gg.x);
            Serial.print("\t");
            Serial.print(gg.y);
            Serial.print("\t");
            Serial.println(gg.z);

         // blink LED to indicate activity INUTILE LED LAMPEGGIOSO///////////////
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}
