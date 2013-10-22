/**************************************************************GENERALI****************************************************************************************/
#define PERIOD 10//periodo (in millisecondi) del loop principale. per ora il minimo effettivo è 6-7 ms ma aggiungendo cose diventerà facilmente di più.

/**************************************************************FILTERING****************************************************************************************/
#define FILTERING 4  //numero di elementi da includere in ogni media fatta dall'arduino
#define DLP 6  //frequenza di taglio del digital low pass filter interno all'mpu secondo la seguente tabella
/*
 *          |   ACCELEROMETER    |           GYROSCOPE
 * DLP      | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
 * ---------+-----------+--------+-----------+--------+-------------
 * 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
 * 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
 * 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
 * 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
 * 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
 * 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
 * 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
 * 7        |   -- Reserved --   |   -- Reserved --   | Reserved
*/

/**********************************************************************DEBUG MODES E LEDS DI DEBUG**************************************************************/
//#define DEBUG //modalità di debug inclusa nelle leibrerie dell'mpu.Se attivata racconterà sulla seriale step per step tutte le funzioni dell'mpu... non attivare a meno che non si vogliano un sacco di chiacchere in seriale.

#define DEBUG_GYRODATA //questo define abilita invece la trasmissione dei dati del gyro filtrati e di alcune info sull'inizializzazione dell'mpu compatibile con labview

#define DEBUG_SERVO //attiva l'output via seriale delle posizioni (teoriche) dei servo.
/**************************************************************************UCF (COLLEGAMENTO PINS)*************************************************************/
//#define DEBUG_TIMING //stampa via seriale il tempo fra un ciclo e l'altro. rallenta le trasmissioni del debug gyrodata ovviam se sono attivati assieme.
#define LED_PIN 13
const int servo_pin[4]={3,4,5,6};
#define RADIOPIN 2