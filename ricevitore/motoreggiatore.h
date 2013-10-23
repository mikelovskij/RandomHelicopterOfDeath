/*  MISURE OSCOLLOSCOPIO
      0 gradi =  550 microsecondi
     90 gradi = 1470 microsecondi
    180 gradi = 2410 microsecondi

*/
Servo blervo[4];


void inizializza_servo(const int *servo_pin/*, Servo *blervo*/){
	for(int i; i<4; i++)
		if(i!=3){
			blervo[i].attach(servo_pin[i], 30 , 900); //inizializzazione dei 3 esc
		}
		else{
			blervo[i].attach(servo_pin[i]);//l'ultimo servo è veramente un servo quindi useremo limiti diversi/nessun limite
		}
}

void servo_write (int *angolo/*, Servo *servo*/){
  for(int i=0; i<4; i++){
      blervo[i].write(angolo[i]);
      DEBUG_PRINT_SERVO(angolo[i]);
      DEBUG_PRINT_SERVO("\t");
    }
    DEBUG_PRINTLN_SERVO("\t");
}


/*DA QUI IN POI AGGIUNGO FUNZIONI PER PROGRAMMARE GLI ESC SFRUTTANDO DUE BOTTONI*/

void inizializza_bottoni(const int *pin_bottoni){
  for(int j=0; j<2; j++){
        pinMode(pin_bottoni[j], INPUT);
        digitalWrite(pin_bottoni[j], HIGH);        
  }
}

int leggi_bottoni(const int *pin_bottoni){
  int out;
  int stato[2]={0,0};
  for(int i=0; i<2; i++)
    stato[i]=digitalRead(pin_bottoni[i]);
  if(stato[0]==1 && stato[1]==1 )
    out=90;
  else 
    if(stato[0]==0 && stato[1]==1)
      out=15;
    else
      if(stato[0]==1 && stato[1]==0)
        out=165;
  return out;
}
