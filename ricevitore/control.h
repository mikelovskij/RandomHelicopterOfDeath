
/***********************************GESTIONE LED*******************************************************/
void inizializza_led(){
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void semaforo(){
  digitalWrite(RED_PIN, !(readyflag==0||readyflag==3||readyflag==4));
  digitalWrite(YELLOW_PIN, !(readyflag==1||readyflag==4));
  digitalWrite(GREEN_PIN, !(readyflag==2||readyflag==3||readyflag==4));
}


/**********************************GESTIONE FLAG*****************************************************/

void redizziamoci(){
  static bool premiemolla;
  if(!B_Start) premiemolla=0;
  if(readyflag==1 && B_Start && !premiemolla){ 
	readyflag++;
	premiemolla=1;
  }
  if((readyflag==2||readyflag==3) && B_Start && !premiemolla){ 
	readyflag=1;
	premiemolla=1;
  }
}
void telecomandiamoci(){
  static bool premiemolla2;
  if(!B_Select) premiemolla2=0;
  if(readyflag==1 && B_Select && !premiemolla2){ 
	tc_total=!tc_total;
	premiemolla2=1;
  }
 /* if((readyflag==2||readyflag==3) && B_Start && !premiemolla){ 
	readyflag=1;
	premiemolla=1;
  }*/
}




void emergency_drop(){
  if((readyflag==2 || readyflag==3) && (alarm_counter > ALARM_LIMIT)){
	readyflag=3;
  }
    if((readyflag==2 || readyflag==3 || readyflag==4) && (alarm_counter > SHUTDOWN_LIMIT)){
	readyflag=4;
  }
  else{
	if((readyflag==3 || readyflag==4)&& (alarm_counter < ALARM_LIMIT)){
	  readyflag--;
	}
  }
}

/***********************************COMUNICAZIONE FRA ARDUINI************************************/

void splittaint(int *input, uint8_t* output){
  for(int i=0;i<4;i++){
    output[2*i] = input[i] >> 8;
    output[2*i+1] = input[i] & 255;
    I2C_PRINT(input[i]);
    I2C_PRINT("\t");
    I2C_PRINT(output[2*i]);
    I2C_PRINT("\t");
    I2C_PRINTLN(output[2*i+1]);
  }
  I2C_PRINTLN("");
  I2C_PRINTLN("");
}

void transmit_data(int *musec){
  uint8_t splitdata[8];
  splittaint(musec, splitdata);
  Wire.beginTransmission(motor_controller_address); 
  Wire.write(splitdata, 8);             
  Wire.endTransmission();
}
  