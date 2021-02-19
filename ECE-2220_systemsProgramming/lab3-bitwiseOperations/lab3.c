/*
John Lawler
ECE 2220
Febuary 14th 2019

This is program 3. The purpose of this program is to take in ASCII input from
user and perform bitwise operations on the input in order to implement error
correcting code.

*/

//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define UCHAR unsigned char

//function prototypes in same order as functions below
int findP16(int num);
int findP8(int num);
int findP4(int num);
int findP2(int num);
int findP1(int num);
int createHamming(int num);
void printOutput(char char1, char char2, char char3);
void printToBin(void *letterAdr, unsigned char len);
int userPrompt(char *ptrInput, int exitInt);
void printMenu();
void printLine();


//------------------------------------------------------------------------------

int main(){

//variable declarations
  char UserInput[10];
  char *ptrUserInput = UserInput;
  unsigned char char1, char2, char3, tempChar, tempChar1,tempChar2;
  int exitPhrase = 0;
  int CharNum;
  int P1, P2, P4, P8, P16;

  //print start Menu
  system("clear");
  printMenu();

  //take user input prompt user to enter two ascii char or exit key phrase
  do{
    exitPhrase = userPrompt(ptrUserInput,exitPhrase);
      //does not execute rest of do loop if exit = 1
      if(!exitPhrase){
        system("clear");
        printMenu();

        //sets individial char values to each set of 8 bits to manipulate
        char1 = UserInput[0];
        char2 = UserInput[1];
        char3 = char2 >> 3;

        //prints first three lines of output and code with P bits
        printOutput(char1,char2,char3);

        //creates a 32 bit integer from the two characters inputted
        CharNum = char2;
        CharNum <<= 8;
        CharNum = CharNum | char1;

        /*uses the 32 bit integer to place the bit positions correctly into
        the hamming code then finds the P values in each respective function*/
        CharNum = createHamming(CharNum);
        P1 = findP1(CharNum);
        P2 = findP2(CharNum);
        P4 = findP4(CharNum);
        P8 = findP8(CharNum);
        P16 = findP16(CharNum);

        //prints value output and hamming code
        printf("Your carry bit values are (far right being P1): \n");
        printf("P1 = %d\nP2 = %d\nP4 = %d\nP8 = %d\nP16 = %d",P1,P2,P4,P8,P16);

        //section that places the P bits into their respective spots in CharNum
        if(P1){
          CharNum = CharNum | 0x01;
        }else{
          CharNum = CharNum & ~0x01;
        }
        if(P2){
          CharNum = CharNum | 0x02;
        }else{
          CharNum = CharNum & ~0x02;
        }
        if(P4){
          CharNum = CharNum | 0x08;
        }else{
          CharNum = CharNum & ~0x08;
        }
        if(P8){
          CharNum = CharNum | 0x80;
        }else{
          CharNum = CharNum & ~0x80;
        }
        if(P16){
          CharNum = CharNum | 0x8000;
        }else{
          CharNum = CharNum & ~0x8000;
        }

        //some final adjustments to fix the addresses to print nicely
        char1 = CharNum;
        tempChar = CharNum >> 8;
        char2 = tempChar;
        tempChar = CharNum >> 16;
        char3 = tempChar;
        tempChar2 = char2 & ~0xF0;
        tempChar1 = char1 & ~0xF0;

        //print final output statement
        printf("\n\nYour New Code Word in Binary and Hex: \n");
        printToBin(&CharNum,sizeof(CharNum));
        printf("\n0x%6s %4X%4X ","00",(char3>>4),char3);
        printf("%4X%4X %4X",(char2>>4),tempChar2, (char1>>4));
        printf("%4X",tempChar1);
        printf(" = 0x00%X%X%X%X",(char3>>4),char3,char2>>4,tempChar2);
        printf("%X%X\n",char1>>4,tempChar1);

      }
   } while(!exitPhrase);




  return 0;
}

//------------------------------------------------------------------------------

//finds the P16 carry bit
int findP16(int num){
  int P16 = 0, i, sum = 0;

  //sums bit positions in 17-21
  for(i = 16; i<21; i++){
    if(((num>>i) & 0x01) == 0x01){
      sum++;
    }
  }
  if((sum % 2) != 0){
    P16 = 1;
  }
  return P16;
}

//------------------------------------------------------------------------------

//finds the P8 carry bit
int findP8(int num){
  int P8 = 0, sum = 0, i;

  //for bit positions 9-15 for P bit position in 8
  for(i = 8; i<15; i++){
    if(((num>>i) & 0x01) == 0x01){
      sum++;
    }
  }
  if((sum % 2) != 0){
    P8 = 1;
  }
  return P8;
}

//------------------------------------------------------------------------------

//finds the P4 carry bit
int findP4(int num){
  int P4 = 0, sum = 0, i;

  //for positions 5, 6, 7
  for(i = 4; i<7; i++){
    if(((num>>i) & 0x01) == 0x01){
      sum++;
    }
  }

  //finds 1's in positions 12, 13, 14, 15
  for(i = 11; i<15; i++){
    if(((num>>i) & 0x01) == 0x01){
      sum++;
    }
  }

  //finds bits in positions 20 and 21
  for(i = 19; i<21; i++){
    if(((num>>i) & 0x01) == 0x01){
      sum++;
    }
  }
  if((sum % 2) != 0){
    P4 = 1;
  }
  return P4;
}

//------------------------------------------------------------------------------

//function takes 32 bit integer storing hamming code and outputs value of P2
int findP2(int num){
  int P2 = 0, sum = 0, i, j, tempNum = 0;
  for(i = 0; i<=1; i++){

    //if i == 0 start at 6 to sum bit pos 6, 10, 14, and 18
    if(i == 0){
      for(j = 5; j<18;){
        tempNum = num >> j;
        if((tempNum & 0x01) == 0x01){
          sum++;
        }
      j += 4;
      }
    // finds the sum of bit positions 3 7 11 15 and 19
    } else {
      for(j = 2; j<19;){
        tempNum = num >> j;
        if((tempNum & 0x01) == 0x01){
          sum++;
        }
        j += 4;
      }
    }
  }
  if((sum % 2) != 0){
    P2 = 1;
  }
  return P2;
}

//------------------------------------------------------------------------------

/*  This function takes in two character inputs and outputs the parity bit of
    P1 in the hamming code */
int findP1(int num){
  int P1 = 0, i, sum = 0, tempNum;

  //loop that sums the odd bit positions in the integer sent to function
  for(i = 20; i>=2;){
    tempNum = num >> i;
    if((tempNum & 0x01) == 0x01){
      sum++;
    }
    i -= 2;
  }
  //checks whether the sum is even or odd, and if its odd P1 = 1
  if((sum % 2) != 0){
    P1 = 1;
  }
  return P1;
}

//------------------------------------------------------------------------------

/* this function turns the integer filled in order with the char values into a
  32 bit integer and creates a hamming bit adress without the filled in P carry
  bits. It ignores the P positions for now */
int  createHamming(int num){
  int hamNum,tempNum;
  unsigned char tempChar;

  tempNum = num >> 11;               //places 5 most sig bits in temp
  hamNum = tempNum;
  hamNum <<= 8;                      //sets 5 most sig bits into pos 16-20
  tempChar = num>>4;                 //selects next set of bits to be positioned
  hamNum = hamNum | tempChar;
  hamNum <<= 8;
  tempChar = num<<3;                 //isolate the group of 3 bits in pos 4,5,6
  hamNum = hamNum | tempChar;
  tempChar = num<<7; tempChar >>= 5; //isolate the final bit into position 3
  hamNum = hamNum | tempChar;        //sets final bit into pos 3

  return hamNum;
}

//------------------------------------------------------------------------------

//prints out output display of the user entry, its hex and binary value
void printOutput(char char1, char char2, char char3){
  printLine();
  int i = 0;
  char tempChar;
  printf("\t \t  %8c %8c|",char2,char1); printf("\t>> User Entry\n");
  printf("0x%6s %8s %8X %8X|","00","00",char2,char1); printf("\t>> Hexadecimal");
  printf("\n-------- -------- ");

  //prints U  and C in binary
  printToBin(&char2,sizeof(char2)); printf(" ");
  printToBin(&char1,sizeof(char1)); printf("|\t>> Binary Representation");

  //beginning of printing the formatted hamming code with P as carry bits
  printf("\nBits Shifted:\t\t\t   |\n-------- ");
  printToBin(&char3,sizeof(char3));
  printf("\b\b\b\b\b\b\b\b---\t\t\b\b\b\b\b\b");
  tempChar = char2 << 4;
  printToBin(&tempChar,sizeof(tempChar)); printf("\b\b\b\b\b\b\b\bP\t\b\b");
  printToBin(&char1, sizeof(char1)); printf("\b\b\b\b P");
  tempChar = char1 << 4;

  printToBin(&tempChar,sizeof(tempChar)); printf("\b\b\b\b\bP");

  tempChar = char1 << 7;
  if((tempChar | 0x00) != 0){ i = 1; } //finds least significant bit of char1
  printf("%dPP|\n",i);
  printf("\n\n");

}

//------------------------------------------------------------------------------

//takes in letter and size as input and prints the binary value from memory
void printToBin(void *letterAdr, unsigned char len){
  char *HexToBin[16] =
  { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
  };
  UCHAR *adr;
  if(len==1){
    for(adr = (UCHAR *)letterAdr + len - 1; adr >= (UCHAR *)letterAdr; adr--){
      printf("%4s", &HexToBin[*adr>>4][0]);
      printf("%4s", &HexToBin[*adr % 16][0]);
    }
  } else{
    for(adr = (UCHAR *)letterAdr + len - 1; adr >= (UCHAR *)letterAdr; adr--){
      printf("%4s", &HexToBin[*adr>>4][0]);
      printf("%4s ", &HexToBin[*adr % 16][0]);
    }
  }

}

//------------------------------------------------------------------------------

//function takes in userInput and returns wheter the user wants to exit or not
int userPrompt(char *ptrInput, int exitInt){
  char UserInput[10];
  printf("\nInput two ASCII characters or exit keyphrase ""~~"" to exit: ");
  scanf("%s",UserInput);

  //check if user input is right size
  while(strlen(UserInput) != 2){
    system("clear");
    printMenu();
    printf("Size error. Please Re-enter only two characters: ");
    scanf("%s",UserInput);
  }
  if(!strcmp(UserInput,"~~")){
    exitInt = 1;
  }

  ptrInput[0] = UserInput[0];
  ptrInput[1] = UserInput[1];
  printf("\n");
  return exitInt;
}

//------------------------------------------------------------------------------

//function prints a nicely formated menu
void printMenu(){
  printf(" =========================");
  printf("=================================================");
  printf("\n >>\tWelcome to John Lawler's program 3 for ECE 2220 program 3");
  printf("\t<<\n -------------------------");
  printf("-------------------------------------------------");
  printf("\n >>\tThis program takes in two ASCII characters at a time ");
  printf("creates\n\t a codeword that allows the user to determine if the ");
  printf("transmitted\n\tdata arrived correctly or not.\t\t\t\t\t<<");
  printf("\n =======================");
  printf("===================================================\n\n");
}

//------------------------------------------------------------------------------

//function prints a line
void printLine(){
  printf("\n====================================\n");
}

//------------------------------------------------------------------------------
