/*****************************COMANDI PREPROCESSORE CHE ATTIVANO I DEBUGS SE NEL MAIN VENGONO DEFINITE LE COSE GIUSTE**************************************/
#ifdef DEBUG_GYRODATA
	#define GYRODEBUG_PRINT(x) Serial.print(x)
	#define GYRODEBUG_PRINTF(x, y) Serial.print(x, y)
	#define GYRODEBUG_PRINTLN(x) Serial.println(x)
    #define GYRODEBUG_PRINTLNF(x, y) Serial.println(x, y)
    #define GYRODEBUG_TRASMETTIDATI(x) trasmettidati(x)
    #define GYRODEBUG_AGGIUSTASTRINGHE(x, y) aggiustastringhe(x, y)
	#define GYRODEBUG_ASPETTATASTO() while(!Serial.available())
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
#else
    #define GYRODEBUG_PRINT(x)
    #define GYRODEBUG_PRINTF(x, y)
    #define GYRODEBUG_PRINTLN(x)
    #define GYRODEBUG_PRINTLNF(x, y)
	#define GYRODEBUG_TRASMETTIDATI 
    #define GYRODEBUG_AGGIUSTASTRINGHE(x, y) "0"
	#define GYRODEBUG_ASPETTATASTO()
#endif
	
#ifdef DEBUG_SERVO
	#define DEBUG_PRINT_SERVO(x)  Serial.print (x)
	#define DEBUG_PRINTLN_SERVO(x)  Serial.println (x)
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
#else
    #define DEBUG_PRINT_SERVO(x)
    #define DEBUG_PRINTLN_SERVO(x)
#endif  

#ifdef DEBUG_TELECOM	
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
	#define TELECOM_PRINTLN(x) Serial.println(x)
	#define TELECOM_PRINT(x) Serial.print(x)
#else
	#define TELECOM_PRINTLN(x)
	#define TELECOM_PRINT(x)
#endif

#ifdef DEBUG_I2C	
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
	#define I2C_PRINTLN(x) Serial.println(x)
	#define I2C_PRINT(x) Serial.print(x)
#else
	#define I2C_PRINTLN(x)
	#define I2C_PRINT(x)
#endif

#ifdef DEBUG_CONVERSION	
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
	#define CONVERSION_PRINTLN(x) Serial.println(x)
	#define CONVERSION_PRINT(x) Serial.print(x)
#else
	#define CONVERSION_PRINTLN(x)
	#define CONVERSION_PRINT(x)
#endif

#ifdef DEBUG_TIMING	
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
	#define TIMING_PRINTLN(x) Serial.println(x)
#else
	#define TIMING_PRINTLN(x)
	#ifndef DEBUG_BEGIN(x) //questo va messo nell'ultimo di questi ifdef che vien chiamato, così viene fatto solo se nessun debug è chiamato.
		#define DEBUG_BEGIN(x)
	#endif
#endif
/************************************************FUNZIONI DI DEBUG E TRASMISSIONE SERIALE DEI DATII*******************************************************/

String aggiustastringhe (int l, String str){
	int lgh=str.length();	
	while(lgh<l){
		if(str.startsWith("-")){
			str = String ("-" + str);
			str.setCharAt(1,'0');
		}
		else{
			str = String("0"+str);
		}
		lgh=str.length();		
	}
	return(str);
}


void trasmettidati(long* filtereddata){
	String sax = String(filtereddata[0]);
	String say = String(filtereddata[1]);
	String saz = String(filtereddata[2]);   
	String sgx = String(filtereddata[3]);
	String sgy = String(filtereddata[4]);
	String sgz = String(filtereddata[5]);
	sax = GYRODEBUG_AGGIUSTASTRINGHE(6,sax);
	say = GYRODEBUG_AGGIUSTASTRINGHE(6,say);
	saz = GYRODEBUG_AGGIUSTASTRINGHE(6,saz);
	sgx = GYRODEBUG_AGGIUSTASTRINGHE(6,sgx);
	sgy = GYRODEBUG_AGGIUSTASTRINGHE(6,sgy);
	sgz = GYRODEBUG_AGGIUSTASTRINGHE(6,sgz);
	String grumo = String(sax +',' + say +',' + saz + ',' +sgx+ ',' + sgy +',' + sgz);
	GYRODEBUG_PRINTLN(grumo);
}
