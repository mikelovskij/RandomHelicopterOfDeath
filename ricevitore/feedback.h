void mangiadietro(long* azzeratdata, long* mangiafreedom){

  /*output[0]=coeff_telecomando_rollio*B_D-B_B;   //wx
  output[1]=coeff_telecomando_beccheggio*B_LR;    //wy
  output[2]=coeff_telecomando_routa*B_A-B_C;    //wz
  output[3]=coeff_telecomando_sali*B_UD;    //az*/
  //ricordiamo: data=ax,ay,az,gx,gy,gz


  //questo feedback è creato supponendo lineare (linearizzando) l'angolo a cui è ruotato il congegno (per becch e rollio) in funzione dell'acceleraz gravitazionale misurata lungo gli assi x e y.

  /*COSE COPIATE NEL FILE DI PARAMETRI
#define smorzconst 7
#define elasticonst 77
#define roll_x 777
#define becch_y 7777
#define rotaz_z 77777
#define deaccel_z 777777
#define fb_smorzroll_x smorzconst*roll_x
#define fb_elasticroll_x elasticonst*roll_x
#define fb_smorzbecch_y smorzconst*becch_y
#define fb_elasticbecch_y elasticonst*becch_y
#define fb_smorzrotaz_z smorzconst*rotaz_z   
   */
  
  mangiafreedom[0]=filtereddata[3]*fb_smorzroll_x+filtereddata[0]*fb_elasticroll_x
  mangiafreedom[1]=filtereddata[4]*fb_smorzbecch_y+filtereddata[1]*fb_elasticbecch_y
  mangiafreedom[2]=filtereddata[5]*fb_smorzrotaz_z
  mangiafreedom[3]=filtereddata[2]*fb_deaccel_z   //alternativa: in questa componente di feedback, invece di usare az, potrei usare il modulo del vettore g cioè radq(ax^2+ay^2+az^2) ma in teoria è meglio considerare solo az, così in caso di rotazione si ottiene anche un'accelerazione di tutte e tre le eliche in modo da mantenere az costante, e questa serve in quanto una rotazione implica un minore momento applicato dai motori verticalmente.



void azzeratore(long*filtereddata, long* azzeratdata)
