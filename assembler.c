#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */

#define MAX_LINE_LENGTH 255
#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255

enum{
  DONE, OK, EMPTY_LINE
};

typedef struct {
	int address;
	char label[MAX_LABEL_LEN + 1];	/* Question for the reader: Why do we need to add 1? */
} TableEntry;

FILE* infile = NULL;
FILE* outfile = NULL;
char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
TableEntry symbolTable[MAX_SYMBOLS];
int startAddress;


/*Return 0 if opcode, else return -1*/
int isOpcode(char * pStr){
  if(strcmp(pStr, "add") == 0){
    return 0;
  }else if(strcmp(pStr, "and") == 0){
    return 0;
  }else if(strcmp(pStr, "br") == 0){
    return 0;
  }else if(strcmp(pStr, "brn") == 0){
    return 0;
  }else if(strcmp(pStr, "brz") == 0){
    return 0;
  }else if(strcmp(pStr, "brp") == 0){
    return 0;
  }else if(strcmp(pStr, "brnz") == 0){
    return 0;
  }else if(strcmp(pStr, "brnp") == 0){
    return 0;
  }else if(strcmp(pStr, "brzp") == 0){
    return 0;
  }else if(strcmp(pStr, "brnzp") == 0){
    return 0;
  }else if(strcmp(pStr, "jmp") == 0){
    return 0;
  }else if(strcmp(pStr, "jsr") == 0){
    return 0;
  }else if(strcmp(pStr, "jsrr") == 0){
    return 0;
  }else if(strcmp(pStr, "ldb") == 0){
    return 0;
  }else if(strcmp(pStr, "ldw") == 0){
    return 0;
  }else if(strcmp(pStr, "lea") == 0){
    return 0;
  }else if(strcmp(pStr, "not") == 0){
    return 0;
  }else if(strcmp(pStr, "ret") == 0){
    return 0;
  }else if(strcmp(pStr, "rti") == 0){
    return 0;
  }else if(strcmp(pStr, "lshf") == 0){
    return 0;
  }else if(strcmp(pStr, "rshfl") == 0){
    return 0;
  }else if(strcmp(pStr, "rshfa") == 0){
    return 0;
  }else if(strcmp(pStr, "stb") == 0){
    return 0;
  }else if(strcmp(pStr, "stw") == 0){
    return 0;
  }else if(strcmp(pStr, "trap") == 0){
    return 0;
  }else if(strcmp(pStr, "xor") == 0){
    return 0;
  }else if(strcmp(pStr, "halt") == 0){
    return 0;
  }else if(strcmp(pStr, "nop") == 0){
    return 0;
  }else if(strcmp(pStr, ".fill") == 0){
    return 0;
  }else{
    return -1;
  } 
}

/*Convert string to number*/
int toNum( char * pStr ){
   char * t_ptr;
   char * orig_pStr;
   int t_length,k;
   int lNum, lNeg = 0;
   long int lNumLong;

   orig_pStr = pStr;
   if( *pStr == '#' )				/* decimal */
   { 
     pStr++;
     if( *pStr == '-' )				/* dec is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isdigit(*t_ptr))
       {
	 printf("Error: invalid decimal operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNum = atoi(pStr);
     if (lNeg)
       lNum = -lNum;
 
     return lNum;
   }
   else if( *pStr == 'x' )	/* hex     */
   {
     pStr++;
     if( *pStr == '-' )				/* hex is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isxdigit(*t_ptr))
       {
	 printf("Error: invalid hex operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
     lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
     if( lNeg )
       lNum = -lNum;
     return lNum;
   }
   else
   {
	printf( "Error: invalid operand, %s\n", orig_pStr);
	exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
   }
} 


int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4){
	   char * lRet, * lPtr;
	   int i;
	   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
	   for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
	   
           /* convert entire line to lowercase */
	   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	   /* ignore the comments */
	   lPtr = pLine;

	   while( *lPtr != ';' && *lPtr != '\0' && *lPtr != '\n' ) 
		lPtr++;

	   *lPtr = '\0';
	   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
		return( EMPTY_LINE );

	   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
	   {
		*pLabel = lPtr;
		if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   }
	   
           *pOpcode = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   
           *pArg1 = lPtr;
	   
           if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg2 = lPtr;
	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg3 = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg4 = lPtr;

	   return( OK );
}

int isNum(char c){
  if(c >= '0' && c <= '9'){return 1;}
  return 0;
}

/* Create Symbol Table*/
void runFirstPass(FILE* lInfile){
  int i = 0;
  int lRet;
  int origCheck = 0;
  int lineCount = -1;
  int exist = 0;
   do{
       lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
       if( lRet != DONE && lRet != EMPTY_LINE ){
	 if(strcmp(lOpcode, ".end") == 0){return;}
	 if(strcmp(lOpcode, ".orig") != 0 && origCheck == 0){
	   printf("ERROR: Encountered instruction before .ORIG\n");
	   exit(4);
	 }
	 if(strcmp(lOpcode,".orig") == 0 && origCheck == 0){
	   startAddress = toNum(lArg1);
	   if(startAddress % 2 == 1){
	     printf("ERROR: Invalid start location\n");
	     exit(3);
	   }
	   origCheck = 1;
	 }
	 if(strcmp(lLabel,"") != 0){
	   if(isOpcode(lOpcode) == -1){                                       /********* IMPORTANT: THIS SEEMS TO BE BROKEN FOR .FILL ***********/
	     printf("ERROR: '%s' is an invalid opcode\n", lLabel);
	     exit(2);
	   }
	   if(lLabel[0] == 'x'){
	     printf("ERROR: The label '%s' is invalid because it starts with the letter 'x'\n", lLabel);
	     exit(4);
	   }
	   if(isOpcode(lLabel) == 0){
	     printf("ERROR: The label '%s' is an illegal label\n", lLabel);
	     exit(4);
	   }
	   if(isNum(lLabel[0]) == 1){
	     printf("ERROR: The label '%s' is invalid because it starts with a number\n", lLabel);
	     exit(4);
	   }
	   for(int x=0; x < strlen(lLabel); x++){
	     if(!isalnum(lLabel[x])){
	       printf("ERROR: The label '%s' contains non alphanumeric characters\n", lLabel);
	       exit(4);
	     }
	   }
	   exist = 0;
	   for(int j = 0; j < i; j++){
	     if(strcmp(lLabel, symbolTable[j].label) == 0){
	       exist = 1;
	       printf("ERROR: Label '%s' has been set in multiple locations\n", lLabel);
	       exit(4);
	     }
	   }
	   if(exist == 0){
	     strcpy(symbolTable[i].label, lLabel);
	     symbolTable[i].address = startAddress + (lineCount * 2);
	     i++;
	   }
	 }
	 lineCount++;
       }
     } while( lRet != DONE );
}


int getSRTwoVal(char *arg){
  if(strcmp(arg,"r0") == 0){
    return 0;
  }else if(strcmp(arg,"r1") == 0){
    return 1;
  }else if(strcmp(arg,"r2") == 0){
    return 2;
  }else if(strcmp(arg,"r3") == 0){
    return 3;
  }else if(strcmp(arg,"r4") == 0){
    return 4;
  }else if(strcmp(arg,"r5") == 0){
    return 5;
  }else if(strcmp(arg,"r6") == 0){
    return 6;
  }else if(strcmp(arg,"r7") == 0){
    return 7;
  }else{
    printf("ERROR: '%s' is an invalid register\n", arg);
    exit(4);
  }
}


int getSROneVal(char *arg){
  if(strcmp(arg,"r0") == 0){
    return 0;
  }else if(strcmp(arg,"r1") == 0){
    return toNum("x40");
  }else if(strcmp(arg,"r2") == 0){
    return toNum("x80");
  }else if(strcmp(arg,"r3") == 0){
    return toNum("xc0");
  }else if(strcmp(arg,"r4") == 0){
    return toNum("x100");
  }else if(strcmp(arg,"r5") == 0){
    return toNum("x140");
  }else if(strcmp(arg,"r6") == 0){
    return toNum("x180");
  }else if(strcmp(arg,"r7") == 0){
    return toNum("x1c0");
  }else{
    printf("ERROR: '%s' is an invalid register\n", arg);
    exit(4);
  }
}


/* Returns value to add to instruction value. -1 if invalid register */
int getDRVal(char *arg){
  if(strcmp(arg,"r0") == 0){
    return toNum("x0");
  }else if(strcmp(arg,"r1") == 0){
    return toNum("x200");
  }else if(strcmp(arg,"r2") == 0){
    return toNum("x400");
  }else if(strcmp(arg,"r3") == 0){
    return toNum("x600");
  }else if(strcmp(arg,"r4") == 0){
    return toNum("x800");
  }else if(strcmp(arg,"r5") == 0){
    return toNum("xa00");
  }else if(strcmp(arg,"r6") == 0){
    return toNum("xc00");
  }else if(strcmp(arg,"r7") == 0){
    return toNum("xf00");
  }else{
    printf("ERROR: '%s' is an invalid register\n", arg);
    exit(4);
  }
}


int getInst(char *op, char *arg1, char *arg2, char *arg3, char* arg4, int lineNum){

  int inst;
  int imm;
  int k;
  
  if(strcmp(op,"add") == 0){                                                          /* ADD */
    if(strcmp(arg3, "") == 0){
      printf("ERROR: Incorrect number of arguments for ADD instruction\n");
      exit(4);
    }
    
    
    if(arg3[0] == '#' || arg3[0] == 'x'){
      imm = toNum(arg3);
      if(imm < -16 || imm > 15){
	printf("ERROR: '%s' is an invalid constant for ADD\n", arg3);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1f"); /* clear everything before last 5 bits */
      inst = inst | toNum("x20");     /* Set steering bit */
      inst += toNum("x1000");
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
      
      
    }else{
      inst = toNum("x1000");
      inst += getSRTwoVal(arg3);
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
      
    }
    return inst;
  }else if(strcmp(op,"and") == 0){                                                     /* AND */
    
    if(strcmp(arg3, "") == 0){
      printf("ERROR: Incorrect number of arguments for ADD instruction\n");
      exit(4);
    }
    
    if(arg3[0] == '#' || arg3[0] == 'x'){
      imm = toNum(arg3);
      if(imm < -16 || imm > 15){
	printf("ERROR: '%s' is an invalid constant for ADD\n", arg3);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1f"); /* clear everything before last 5 bits */
      inst = inst | toNum("x20");     /* Set steering bit */
      inst += toNum("x5000");
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
    }else{
      inst = toNum("x5000");
      inst += getSRTwoVal(arg3);
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
    }
    return inst;
  }else if(strcmp(op,"br") == 0){                                                         /* BR */
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brn") == 0){                                                   /* BRN */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0800");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0800");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brz") == 0){                                                    /* BRZ */
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0400");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0400");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brp") == 0){                                                    /* BRP */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0200");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0200");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brnz") == 0){                                                    /* BRNZ */
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0c00");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0c00");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brnp") == 0){                                                    /* BRNP */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0a00");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0a00");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brzp") == 0){                                                    /* BRZP */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0600");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256|| imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0600");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"brnzp") == 0){                                                    /* BRNZP */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for BR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += toNum("x0e00");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256 || imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for BR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += toNum("x0e00");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"jmp") == 0){                                                    /* JMP */
    inst = toNum("xc000");
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for JMP instruction\n");
      exit(4);
    }
    inst += getSROneVal(arg1);

    return inst;
  }else if(strcmp(op,"jsr") == 0){                                                    /* JSR */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for JSR instruction\n");
      exit(4);
    }
    if(arg1[0] == 'x' || arg1[0] == '#'){
      imm = toNum(arg1);
      if(imm < -1024|| imm > 1023){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for JSR\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x7ff"); /* clear everything before last 11 bits */
      inst += toNum("x4800");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg1) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -1024 || imm > 1023){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for JSR\n", arg1);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x7ff"); /* clear everything before last 11 bits */
	  inst += toNum("x4800");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
    
  }else if(strcmp(op,"jsrr") == 0){                                                    /* JSRR */
    inst = toNum("x4000");
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for JSRR instruction\n");
      exit(4);
    }
    inst += getSROneVal(arg1);

    return inst;
  }else if(strcmp(op,"ldb") == 0){                                                    /* LDB */
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for LDB instruction\n");
      exit(4);
    }
    imm = toNum(arg3);
    if(imm < -32|| imm > 31){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for LDB\n", arg1);
	exit(3);
      }
    inst = 0;
    inst += imm;
    inst = inst & toNum("x3f");    /* clear everything before last 6 bits */
    inst += getSROneVal(arg2);
    inst += getDRVal(arg1);
    inst += toNum("x2000");

    return inst;
  }else if(strcmp(op,"ldw") == 0){                                                    /* LDW */
    
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for LDB instruction\n");
      exit(4);
    }
    imm = toNum(arg3);
    if(imm < -32|| imm > 31){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for LDB\n", arg1);
	exit(3);
      }
    inst = 0;
    inst += imm;
    inst = inst & toNum("x3f");    /* clear everything before last 6 bits */
    inst += getSROneVal(arg2);
    inst += getDRVal(arg1);
    inst += toNum("x6000");

    return inst;
  }else if(strcmp(op,"lea") == 0){                                                    /* LEA:::::: */
    int curAddress = startAddress + ((lineNum+1) * 2);
    if(strcmp(arg2,"") == 0 || strcmp(arg3,"") != 0){
      printf("ERROR: Incorrect number of operands for LEA instruction\n");
      exit(4);
    }
    if(arg2[0] == 'x' || arg2[0] == '#'){
      imm = toNum(arg2);
      if(imm < -256|| imm > 255){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for LEA\n", arg2);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
      inst += getDRVal(arg1);
      inst += toNum("xe000");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg2) == 0){
	  
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -256 || imm > 255){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for LEA\n", arg2);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("x1ff"); /* clear everything before last 9 bits */
	  inst += getDRVal(arg1);
	  inst += toNum("xe000");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }

    return inst;
  }else if(strcmp(op,"not") == 0){                                                    /* NOT */
    inst = toNum("x9000");
    if(strcmp(arg2,"") == 0 || strcmp(arg3,"") != 0){
      printf("ERROR: Incorrect number of operands for NOT instruction\n");
      exit(4);
    }
    inst += getSROneVal(arg2);
    inst += getDRVal(arg1);
    inst += toNum("x3f");

    return inst;
  }else if(strcmp(op,"ret") == 0){   /* Returns complete instruction */
    return toNum("xc1c0");

    return inst;
  }else if(strcmp(op,"rti") == 0){   /* Returns complete instruction */
    return toNum("x8000");

    return inst;
  }else if(strcmp(op,"lshf") == 0){                                                    /* LSHF */
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for NOT instruction\n");
      exit(4);
    }
    if(arg3[0] == 'x' || arg3[0] == '#'){
      imm = toNum(arg3);
      if(imm < -8|| imm > 7){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for LSHF\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("xf"); /* clear everything before last 4 bits */
      inst += getDRVal(arg1);
      inst += getSROneVal(arg2);
      inst += toNum("xd000");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg3) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -8 || imm > 7){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for LSHF\n", arg3);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("xf"); /* clear everything before last 4 bits */
	  inst += getDRVal(arg1);
	  inst += getSROneVal(arg2);
	  inst += toNum("xd000");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }

    return inst;
  }else if(strcmp(op,"rshfl") == 0){                                                    /* RSHFL */
    
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for NOT instruction\n");
      exit(4);
    }
    if(arg3[0] == 'x' || arg3[0] == '#'){
      imm = toNum(arg3);
      if(imm < -8|| imm > 7){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for RSHLF\n", arg1);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("xf"); /* clear everything before last 4 bits */
      inst += getDRVal(arg1);
      inst += getSROneVal(arg2);
      inst += toNum("xd010");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg3) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -8 || imm > 7){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for RSHFL\n", arg3);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("xf"); /* clear everything before last 4 bits */
	  inst += getDRVal(arg1);
	  inst += getSROneVal(arg2);
	  inst += toNum("xd010");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg1);
      exit(1);
    }
    return inst;
  }else if(strcmp(op,"rshfa") == 0){                                                    /* RSHFA */
    
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for NOT instruction\n");
      exit(4);
    }
    if(arg3[0] == 'x' || arg3[0] == '#'){
      imm = toNum(arg3);
      if(imm < -8|| imm > 7){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for RSHFA\n", arg3);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("xf"); /* clear everything before last 4 bits */
      inst += getDRVal(arg1);
      inst += getSROneVal(arg2);
      inst += toNum("xd030");
    }else{
      inst = 0;
      for(k = 0; k < MAX_SYMBOLS; k++){
	if(strcmp(symbolTable[k].label, arg3) == 0){
	  int curAddress = startAddress + ((lineNum+1) * 2);
	  imm = (symbolTable[k].address - curAddress)/2;
	  if(imm < -8 || imm > 7){                 /* Immediate out of range */
	    printf("ERROR: '%s' is an invalid constant for RSHFA\n", arg3);
	    exit(3);
	  }
	  inst += imm;
	  inst = inst & toNum("xf"); /* clear everything before last 4 bits */
	  inst += getDRVal(arg1);
	  inst += getSROneVal(arg2);
	  inst += toNum("xd030");
	  return inst;
	}
      }
      printf("ERROR: '%s' is an undefined label\n",arg3);
      exit(1);
    }

    return inst;
  }else if(strcmp(op,"stb") == 0){                                                    /* STB */
    
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for STB instruction\n");
      exit(4);
    }
    imm = toNum(arg3);
    if(imm < -32|| imm > 31){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for STB\n", arg3);
	exit(3);
      }
    inst = 0;
    inst += imm;
    inst = inst & toNum("x3f");    /* clear everything before last 6 bits */
    inst += getSROneVal(arg2);
    inst += getDRVal(arg1);
    inst += toNum("x3000");

    return inst;
  }else if(strcmp(op,"stw") == 0){                                                    /* STW */
    
    inst = toNum("x7000");
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for STW instruction\n");
      exit(4);
    }
    imm = toNum(arg3);
    if(imm < -32|| imm > 31){                 /* Immediate out of range */
	printf("ERROR: '%s' is an invalid constant for STW\n", arg3);
	exit(3);
      }
    inst = 0;
    inst += imm;
    inst = inst & toNum("x3f");    /* clear everything before last 6 bits */
    inst += getSROneVal(arg2);
    inst += getDRVal(arg1);
    inst += toNum("x7000");

    return inst;
  }else if(strcmp(op,"trap") == 0){                                                    /* TRAP */
    
    if(strcmp(arg1,"") == 0 || strcmp(arg2,"") != 0){
      printf("ERROR: Incorrect number of operands for TRAP instruction\n");
      exit(4);
    }
    imm = toNum(arg1);
    if(imm < -128 || imm > 127){
      	printf("ERROR: '%s' is an invalid constant for TRAP\n", arg3);
	exit(3);
    }
    inst = 0;
    inst += imm;
    inst = inst & toNum("xff");    /* clear everything before last 8 bits */
    inst += toNum("xf000");

    return inst;
  }else if(strcmp(op,"xor") == 0){                                                    /* XOR */
    
    if(strcmp(arg3,"") == 0 || strcmp(arg4,"") != 0){
      printf("ERROR: Incorrect number of operands for XOR instruction\n");
      exit(4);
    }
    if(arg3[0] == '#'){
      imm = toNum(arg3);
      if(imm < -16 || imm > 15){
	printf("ERROR: '%s' is an invalid constant for XOR\n", arg3);
	exit(3);
      }
      inst = 0;
      inst += imm;
      inst = inst & toNum("x1f"); /* clear everything before last 5 bits */
      inst = inst | toNum("x20");     /* Set steering bit */
      inst += toNum("x9000");
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
      
      
    }else{
      inst = toNum("x9000");
      inst += getSRTwoVal(arg3);
      inst += getSROneVal(arg2);
      inst += getDRVal(arg1);
      
    }

    return inst;
  }else{return -1;}
}

/* Assemble */
void runSecondPass(FILE* lInfile, FILE* lOutfile){
   int lRet;
   int oInst;
   int startFound = 0;
   int location = -1;
   do{
       lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
       if( lRet != DONE && lRet != EMPTY_LINE ){
	 if(strcmp(lOpcode, ".end") == 0){return;}
	 if(strcmp(lOpcode, ".orig") == 0 && startFound == 0){
	   fprintf( lOutfile, "0x%.4X\n", startAddress );
	   startFound = 1;
	 }else{
	   oInst = getInst(lOpcode, lArg1, lArg2, lArg3, lArg4, location);
	   if(oInst == -1){
	     if(strcmp(lOpcode,".fill") == 0){
	       oInst = toNum(lArg1);
	       if(oInst > 32767 || oInst < -32768){
		 printf("ERROR: '%s' is out of range for .FILL\n", lArg1);
		 exit(3);
	       }
	       oInst = oInst & toNum("x0ffff");              /* Clear everything before last word */
	     }else if(strcmp(lOpcode,"halt") == 0){
	       oInst = toNum("xf025");
	     }else if(strcmp(lOpcode,"nop") == 0){
	       oInst = 0;
	     }else{
	       printf("ERROR: '%s' is an invalid opcode\n", lOpcode);
	       exit(2);
	     }
	   }
	   fprintf( lOutfile, "0x%.4X\n", oInst );
	   
	 }
	 location++; 
       }
     } while( lRet != DONE );
}


int main(int argc, char* argv[]) {

     char *iFileName = NULL;
     char *oFileName = NULL;


     if(argc != 3){
       printf("Error: Incorrect number of command line arguments\n");
       exit(4);
     }

     iFileName = argv[0];
     oFileName = argv[1];

     /* open the source file */
     infile = fopen(argv[1], "r");
     outfile = fopen(argv[2], "w");
     
     if (!infile) {
       printf("Error: Cannot open file %s\n", argv[1]);
       exit(4);
		 }
     if (!outfile) {
       printf("Error: Cannot open file %s\n", argv[2]);
       exit(4);
     }

    
     runFirstPass(infile);
     for(int k = 0; k < 5; k++){                                /*FOR TESTING ONLY! REMOVE THIS BEFORE SUBMITTING*/
       printf("Label: %s\n" ,symbolTable[k].label);
       printf("Address: 0x%x\n", symbolTable[k].address);
     }
     rewind(infile);
     runSecondPass(infile, outfile);



     /*Close I/O files*/
     fclose(infile);
     fclose(outfile);
     exit(0);
 }
