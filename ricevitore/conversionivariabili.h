////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void telecomando (/*boolean B_A, boolean B_B, boolean B_C, boolean B_D, int B_LR, int B_UD,*/ double *output){
 
 output[0]=tc__rollio*(B_D-B_B);   //wx
 output[1]=tc__beccheggio*(B_LR-tc_beccheggio_offset);    //wy
 output[2]=tc__routa*(B_A-B_C);    //wz
 output[3]=tc__sali*B_UD;    //az
 
}  
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void somma (double *input_telecomando, double *input_fb, double *output){
  int i;
  for(i=0; i<4; i++){
	if(readyflag==2) output[i]=tc_total*input_telecomando[i]+fb_total*input_fb[i];
	else{
	  if(readyflag==3) output[i]=EMERGENCY_DROP[i]+fb_total*input_fb[i];
	  else output[i]=0;
	}
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void converti_fisica_motori (double *input, int *output){
 
  double msx,mdx,mr,s,
         wx,wy,wz,z;
         
  /*double a=mc_ruota_s;
  double b=mc_ruota_retro;
  double c=mc_rollio_dxsx;
  double d=mc_beccheggio_dxsx;
  double e=mc_beccheggio_retro;
  double f=mc_sali;
  double g=mc_rollio_s;*/
  
  double o=mc_ruota_s;
  double q=mc_ruota_retro;
  double n=mc_rollio_dxsx;
  double l=mc_beccheggio_dxsx;
  double m=mc_beccheggio_retro;
  double k=mc_sali;
  double p=mc_rollio_s;
         
  wx=input[0];//rollio
  wy=input[1];//beccheggio
  wz=input[2];//zrotaz
  z=input[3];//sugiù

  /*mr=(g*wy+e*az)/(g*e*(1+f));
  s=(wz-b*mr)/a;
  mdx=0.5*(az/g-mr-(wx-d*s)/c);
  msx=wx/c+mdx-d*s/c;*/
  
  /*msx=wx/(2*c) + (wy*(a*c - b*g))/(2*(a*c*d - a*c*e)) - (z*(a*c*e - b*d*g))/(2*(a*c*d*f - a*c*e*f)) - (g*wz)/(2*a*c);
  mdx=(wy*(a*c + b*g))/(2*(a*c*d - a*c*e)) - wx/(2*c) - (z*(a*c*e + b*d*g))/(2*(a*c*d*f - a*c*e*f)) + (g*wz)/(2*a*c);
  mr=(d*z)/(d*f - e*f) - wy/(d - e);
  s=wz/a + (b*wy)/(a*d - a*e) - (b*d*z)/(a*d*f - a*e*f);*/
  
  mdx=k*z+l*wy-n*wx;
  msx=k*z+l*wy+n*wx;
  mr=k*z+m*wy+q*wz;
  s=o*wz+p*wx;
  
  int i;
  if(readyflag==2||readyflag==3){
  output[0]=mc_sx*msx+mc_offset_sx;
  output[1]=mc_dx*mdx+mc_offset_dx;
  output[2]=mc_r*mr+mc_offset_r;
  output[3]=mc_s*s+mc_offset_s;
  }
  else{
	for(i=0;i<3;i++){
	  output[i]=0;
	 }
   }
	output[3]=mc_offset_s;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mangiadietro(long* input_gyro, double *mangiafreedom){
 //ricordiamo: input_gyro=ax,ay,az,gx,gy,gz


  //questo feedback è creato supponendo lineare (linearizzando) l'angolo a cui è ruotato il congegno (per becch e rollio) in funzione dell'acceleraz gravitazionale misurata lungo gli assi x e y.
  
  mangiafreedom[0]=input_gyro[3]*fb_smorzroll_x+input_gyro[0]*fb_elasticroll_x;
  mangiafreedom[1]=input_gyro[4]*fb_smorzbecch_y+input_gyro[1]*fb_elasticbecch_y;
  mangiafreedom[2]=input_gyro[5]*fb_smorzrotaz_z;
  mangiafreedom[3]=-input_gyro[2]*fb_deaccel_z;   //alternativa: in questa componente di feedback, invece di usare az, potrei usare il modulo del vettore g cioè radq(ax^2+ay^2+az^2) ma in teoria è meglio considerare solo az, così in caso di rotazione si ottiene anche un'accelerazione di tutte e tre le eliche in modo da mantenere az costante, e questa serve in quanto una rotazione implica un minore momento applicato dai motori verticalmente.
  
}

