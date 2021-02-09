//-----------------------------------------------------------------------+
// File         :  cbi_c2i                                               |
//                                                                       |
// Description  :  Converts up to 4 chars to an integer of 32 bits       |
//-----------------------------------------------------------------------+

int cbi_c2i(char command[4]) {

        int iword,i;
        int fact[4]= {1,256,65536,16777216};
	//-----------------------------------------------------------------------+
	// "command" is a 4 character string, which will be encoded into iword,  +
	// which is a 32 bit word. Its least significant byte contains the ASCII +
	// code of the first character in "command".                             +
	//-----------------------------------------------------------------------+
        iword=0;
        for(i=0; i<4; i++){
          iword = iword + command[i] * fact[i];
        }
        return iword;
}
