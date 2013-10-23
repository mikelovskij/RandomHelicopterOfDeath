///////////////////////////////////////////////////////////////////VARIABILI///////////////////////////////////////////////////////////////////////////////////
MPU6050 accelgyro;//serve per la classe usata dalle librerie dell'MPU
void mobilefilter(int16_t* input,long* output, int vecchio[4][6]);
int16_t data[6]; //contiene ax,ay,az,gx,gy,gz presi (prefiltrati) dall'mpu (ak è l'accelerazione lungo l'asse k e gk è la velocità angolare ATTORNO all'asse k)
long filtereddata[6]; //contiene i valori presi dal vettore "data" e filtrati da una media scorrevole
int16_t vecchio[FILTERING][6];//registro a scorrimento che contiene gli ultimi "FILTERING" vettori acquisiti, usato per far la media passabassante.
int mobilefilter_counter=0; //contatore (globale purtroppo) che serve alla funzione filtro.

///////////////////////////////////////////////////////////////SETUP///////////////////////////////////////////////////////////////////////////////////////////
void riempizeri(int vecchio[4][6]){
	int i,j;
	for(j=0;j<6;j++){
		for(i=0;i<FILTERING;i++){
			vecchio[i][j]=0;
		}
	}
}


void setupmpu(){	    
	Wire.begin();// join I2C bus (I2Cdev library doesn't do this automatically)
	// initialize device
	GYRODEBUG_PRINTLN("Initializing I2C devices...");
	accelgyro.initialize();
	riempizeri(vecchio);
	// verify connection
	GYRODEBUG_PRINTLN("Testing device connections...");
	GYRODEBUG_PRINTLN(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
	accelgyro.dmpInitialize();
	delay(500);
	accelgyro.setDLPFMode(DLP);
	accelgyro.setDMPEnabled(true);
	GYRODEBUG_PRINTLN("\nSend any character to begin horrid data transmission ");
	while (Serial.available() && Serial.read()); // empty buffer
	GYRODEBUG_ASPETTATASTO();       //SE DEBUG GYRO E'DEFINITA, ASPETTA L'INVIO DI UN CARATTERE VIA SERIALE PRIMA DI PROSEGUIRE.
	while (Serial.available() && Serial.read()); // empty buffer again
	GYRODEBUG_PRINT("Pronto!");
}
///////////////////////////////////////////////////////////////FILTRAGGIO//////////////////////////////////////////////////////////////////////////////////////
void mobilefilter(int16_t* input,long* output, int vecchio[4][6]){
	int j;
	for(j=0;j<6;j++){
		output[j]=output[j]+input[j]-vecchio[mobilefilter_counter][j];
		vecchio[mobilefilter_counter][j]=input[j];
		}	
	if(mobilefilter_counter>=FILTERING-1){
		mobilefilter_counter=0;
	}
	else{
		mobilefilter_counter++;
	}
}

////////////////////////////////////////////////////////////////LETTURA E TRASMISSIONE DATI////////////////////////////////////////////////////////////////////
void prendidati(long* filtereddata){
	accelgyro.getMotion6(&data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
	mobilefilter(data,filtereddata, vecchio);
	GYRODEBUG_TRASMETTIDATI(filtereddata);//da mettere solo in modalità debug?
}