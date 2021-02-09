//-----------------------------------------------------------------------+
// File        :  cbi_dsp_i2c                                            |
//                                                                       |
// Description :  Converts an integer of 32 bits to 4 chars              |
//-----------------------------------------------------------------------+

int cbi_dsp_i2c(int iword, char *command) {

  int i,j;
  int fact[4]= {1,256,65536,16777216};
  //-----------------------------------------------------------------------+
  // "iword" is a 32 bit word, with encoded in it 4 ASCII characters. Its  | 
  // least significant byte contains the ASCII code to be decoded to form  |
  // the first character in "command", the next the second and so on.      |
  //-----------------------------------------------------------------------+
  for(i=0; i<4; i++){
    j=((127*fact[i])&iword)/fact[i];
    *command = j;
    command++;
  }
  i=0;
  return i;
  
}

