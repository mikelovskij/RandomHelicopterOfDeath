#define        B_B        Joystik[0]
#define        B_Y        Joystik[1]
#define        B_X        Joystik[2]
#define        B_A        Joystik[3]
#define        B_Start    Joystik[4]
#define        B_Select   Joystik[5]
#define        B_Bop      Joystik[6]
#define        End_row    Joystik[7]

int ADDRESS_reciver;   
bool Joystik[8];
int B_Init;
int B_LR;
int B_UD;

void setup_myreciver(int ADDRESS,int receive_pin)
{
    ADDRESS_reciver = ADDRESS;
    //Serial.println("setup");
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	       // Bits per sec
    vw_rx_start();             // Start the receiver PLL running  
}

void refresh_recived_commands()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen))
    {   
        if((buf[0]>>5) == ADDRESS_reciver)
          {
          B_Init=buf[0];
          B_LR=buf[1];
          B_UD=buf[2];
  	int i_wire;
  	for (i_wire = 0; i_wire < 7; i_wire++)
  	 Joystik[i_wire]=((buf[3]>>i_wire)%2);
          }    
    }
}
