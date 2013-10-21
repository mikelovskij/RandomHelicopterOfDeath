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
    #define GYRODEBUG_AGGIUSTASTRINGHE(x, y)
	#define GYRODEBUG_ASPETTATASTO()
	#endif

#ifdef DEBUG_TIMING	
	#ifndef DEBUG_BEGIN(x)
		#define DEBUG_BEGIN(x) Serial.begin(x) 
	#endif
	#define TIMING_PRINTLN(x) Serial.print(x)
#else
	#define TIMING_PRINTLN(x)
	#ifndef DEBUG_BEGIN(x) //questo va messo nell'ultimo di questi ifdef che vien chiamato, così viene fatto solo se nessun debug è chiamato.
		#define DEBUG_BEGIN(x)
	#endif
#endif


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

//(da poi spostare nella libreria debug)
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