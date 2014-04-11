#define        B_C        !Joystick[0]
#define        B_D        !Joystick[1]
#define        B_A        !Joystick[2]
#define        B_B        !Joystick[3]
#define        B_Start    !Joystick[4]
#define        B_Select   !Joystick[5]
#define        B_Bop      !Joystick[6]
#define        End_row    !Joystick[7]



long int alarm_counter=0;
int ADDRESS_reciver;   
bool Joystick[8];
int B_Init;
int B_LR=133;
int B_UD;

void setup_myreciver(int ADDRESS,int receive_pin)
{
    ADDRESS_reciver = ADDRESS;
    //Serial.println("setup");
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	       // Bits per sec
    vw_rx_start();             // Start the receiver PLL running  
	for(int i=0; i<7; i++){
	  Joystick[i]=1;
	}
}

void refresh_recived_commands()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen))
    {   
        if((buf[0]>>5) == ADDRESS_reciver){
		   if(alarm_counter >= EMERGENCY_COOLDOWN_SPEED){
			alarm_counter-=EMERGENCY_COOLDOWN_SPEED;
		   }
		   else alarm_counter=0;
           B_Init=buf[0];
           B_LR=buf[1];
           B_UD=buf[2];
		   int i_wire;
		   for (i_wire = 0; i_wire < 7; i_wire++){
			  Joystick[i_wire]=((buf[3]>>i_wire)%2);
			  TELECOM_PRINT(Joystick[i_wire]);
			  TELECOM_PRINT(", ");
		   }			 
		  }
		  else if((readyflag==2 || readyflag==3||readyflag==4)&& (alarm_counter<MAX_ALARM)) alarm_counter++; 
            
    }
    else if((readyflag==2 || readyflag==3||readyflag==4)&& (alarm_counter<MAX_ALARM)) alarm_counter++;     
    
    TELECOM_PRINT(B_LR);
	TELECOM_PRINT(", ");
	TELECOM_PRINT(B_UD);
	TELECOM_PRINT("\n");
	
}
