
/***********************************GESTIONE LEDS*******************************************************/
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


/**********************************GESTIONE FLAGS*****************************************************/

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