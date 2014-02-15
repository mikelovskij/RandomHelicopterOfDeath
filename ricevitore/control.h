
/***********************************GESTIONE LEDS*******************************************************/
void inizializza_led(){
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void semaforo(){
  digitalWrite(RED_PIN, (readyflag==0||readyflag==3));
  digitalWrite(YELLOW_PIN, readyflag==1);
  digitalWrite(GREEN_PIN, (readyflag==2||readyflag==3));
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



void emergency_drop(){
  if((readyflag==2 || readyflag==3) && (alarm_counter > ALARM_LIMIT)){
	readyflag=3;
  }
  else{
	if(readyflag==3){
	  readyflag--;
	}
  }
}