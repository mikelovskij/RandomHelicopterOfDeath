int servo_data[4];//variabili servo (in microsecondi!) 


void leggidati(int nbytes) {
  if(nbytes == 4 * __SIZEOF_INT__){
    for(int i=0;i<4;i++){
      servo_data[i]= Wire.read() << 8;
      servo_data[i] |= Wire.read();
      I2C_PRINTLN(dati[i]);
    }
  }
  else
  {
    I2C_PRINT("Unexpected number of bytes received: ");
    I2C_PRINTLN(nbytes);
  }
}
  