/**************************************************************GENERALI****************************************************************************************/
#define PERIOD 20//periodo (in millisecondi) del loop principale. per ora il minimo effettivo è 6-7 ms ma aggiungendo cose diventerà facilmente di più.
//(pare il minimo essere anche 3-4ms)

/**************************************************************FILTERING****************************************************************************************/
#define FILTERING 4  //numero di elementi da includere in ogni media fatta dall'arduino
#define INITFILTERING 512 //numero di elementi (dopo che sono passati nell' iniziale filtering) usati nelle media iniziale usata per azzerare gli offsets del giroaccelerocoso
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

//#define DEBUG_GYRODATA //questo define abilita invece la trasmissione dei dati del gyro filtrati e di alcune info sull'inizializzazione dell'mpu compatibile con labview
//#define DEBUG_TIMING //stampa via seriale il tempo fra un ciclo e l'altro. rallenta le trasmissioni del debug gyrodata ovviam se sono attivati assieme.
#define DEBUG_SERVO //attiva l'output via seriale delle posizioni (teoriche) dei servo.
//#define DEBUG_TELECOM
#define fb_total 1 //mettere a zero per disattivare i mangiadietro. Evitare di toccare altrimenti.
//#define tc_total 1 //mettere a zero per disattivare il telecomando. Evitare di toccare altrimenti.

/**************************************************************************EMERGENCY PARAMETERS*************************************************************/

const int EMERGENCY_DROP[4]={0,0,0,175}; //sono i valori che gli outputs del telecomando(già convertiti in dof) avranno durante l'atterraggio di emergenza
#define ALARM_LIMIT 1000/PERIOD  //numero di cicli richiesti perchè scatti l'atterraggio di emergenza. ad esempio se period=20 e ALARM_LIMIT=50 allora serve un secondo (n.b. 1000/20=50).
#define SHUTDOWN_LIMIT 3500/PERIOD
#define EMERGENCY_COOLDOWN_SPEED 150/PERIOD //ogni volta che il telecomando riceve un comando valido, il punteggio di allarme retrocede di questa quantità. 



/**************************************************************************UCF (COLLEGAMENTO PINS)*************************************************************/

#define LED_PIN 13
#define RED_PIN 12
#define YELLOW_PIN 11
#define GREEN_PIN 10
const int servo_pin[4]={3,4,5,6};//msx,mdx,mrear,servo
#define RADIOPIN 8
const int servo_init[4]={1250,1250,1250,1470};


/**********************************************************************PARAMECI MANGIADIETRO**********************************************************************/
/*######################################al momento per l'accelerometrio g è circa = +-16000-17000######################################*/
//per il giroscopio dovrebbe aggirarsi sui +-4000 massimo circa, dipebnde dalla velocità di rotaz
#define smorzconst 0.1
#define elasticonst 0.015
#define roll_x -1.5
#define becch_y -1.5
#define rotaz_z 0.75
#define fb_deaccel_z 0.015

#define fb_smorzroll_x smorzconst*roll_x  //questa moltiplicazione, in questo modo viene fatta ogni volta che la variabile vien chiamata. ma tanto nella funzione che la usa viene usata solo una volta per variabile. quindi, siccome se inserissi questo prodotto nella funzione verrebbe effettuato ogni volta che la funz viene chiamata, non cambierebbe nulla. L'unico modo per ottimizzarle di più sarebbe fare il prodotto solo durante l'inizializzazione e poi chiamare i risultati nella funzione. potrebbe essere un'idea se sti prodotti iniziano a diventare troppi.
#define fb_elasticroll_x elasticonst*roll_x
#define fb_smorzbecch_y smorzconst*becch_y
#define fb_elasticbecch_y elasticonst*becch_y
#define fb_smorzrotaz_z smorzconst*rotaz_z  

/****************************************************************PARAMECI MOTORI*************************************************************************/
//Ora i define relativi alle costanti necessarie per la conversione dalle variabili fisice
//a quelle motore, cioè m1,m2,m3,s
/* Per comprensione scrivo il sistema da cui siamo partiti in modo che capisci cosa significano
   queste veriabili P.S. i nomi esosi sono chiaramente colpa di zeno per me erano 
   
   alpha=mc_ruota_s
   beta=mc_ruota_retro
   gamma=mc_beccheggio //no è il rollio cambiato in rollio_dxsx
   mu=mc_beccheggio_s  //cambiato in                rollio_s
   delta=mc_rollio2   //no è il                     beccheggio_dxsx 
   epsilon=mc_rollio //cambiato in                  beccheggio_retro
   eta=mc_sali
   
   wz=alpha*s+beta*m3
   wx=gamma*(m1-m2)+mu*s
   wy=delta*(mi+m2)+epsilon*m3
   az=eta*(m1+m2+m3)
*/



#define mc_ruota_s 0.5
#define mc_ruota_retro 0.02
#define mc_rollio_dxsx 0.3
#define mc_rollio_s 0.02
#define mc_beccheggio_dxsx -0.5 //la metà per le due rispetto al motore solo
#define mc_beccheggio_retro 1
#define mc_sali 0.8

/*NOTA BENE: QUESTI COEFFICIENTI (solo i coeff. mc_) SONO I COEFFICIENTI DELL'INVERSA, QUINDI PIÙ PICCOLI SONO, PIÙ GRANDI LE VARIAZ DEI MOTORI SONO//
#define mc_ruota_s 0.02
#define mc_ruota_retro 0.015
#define mc_rollio_dxsx 0.2
#define mc_rollio_s 1
#define mc_beccheggio_dxsx -0.25 //la metà per le due rispetto al motore solo
#define mc_beccheggio_retro 0.125
#define mc_sali 0.25*/


//coefficienti per il singolo motore:
/*  MISURE OSCOLLOSCOPIO
      0 gradi =  550 microsecondi
     90 gradi = 1470 microsecondi
    180 gradi = 2410 microsecondi

*/
//GLI ESC DOVREBBERO LAVORARE FRA I 1200(min) e i 1800(max) MICROSECONDI. 
/*moltiplicativi*/
#define mc_dx 1
#define mc_sx 1
#define mc_r 1
#define mc_s 1
/*offsets*/
#define mc_offset_dx 1200
#define mc_offset_sx 1200
#define mc_offset_r 1200
#define mc_offset_s 1470

/*********************************************************PARAMECI TELECOMANDORLO*********************************************************************/
//Questi define rappresentano i coefficienti di conversione tra i valori del telecomando ricevuti e le variabili 
//fisiche che vengono portate in giro in questa libreria, cioè omegax,omegay,omegaz,az
#define tc__rollio 300 //visto che è +-1
#define tc__beccheggio 0.75 //sempre se è 0-256---serve un offset per lo zero? sì altrimenti non va indietro. 
#define tc_beccheggio_offset 127 //(per avere lo zero in posizione neutra)
#define tc__routa 300 //visto che è +-1
#define tc__sali 2.5 // se è 0-256 come ricordo. 

