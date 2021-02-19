/*
John Lawler
ECE 2220
March 26th 2019

This is program 5. this program takes in 3 file inputs, including a dictionary,
and prompts the user with a menu on options to play around with the files that
were inputted such as spellcheck, search and replace, and other string
functionalities.
*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

/* function prototypes in order as they apepar under main */
void SearchReplace(char *strArray[], int lineCount, int maxLen);
void saveFile(char *strArray[], const char *outputFile, int lineCount);
int checkSpelling(FILE *dictFile, char *strArray[], char *userDictName,
                    int lineCount, int maxLen, int userDictCheck);
void allocateMEM(const char *inFile, int lineCount, int maxLen, char *strArray[]);
int LineCount(const char *inFile, int *maxLen);
void printHeader();
int printMenu();

//------------------------------------------------------------------------------

int main(int argc, char *argv[]){
  //variable declarations
  int menuSelection = 0, lineCount, maxlen, *maxLen, userDictCheck = 0;
  maxLen = &maxlen;
  const char *inputFile = argv[1]; const char *outputFile = argv[2];
  FILE *dictionary = fopen(argv[3], "rt");
  if(dictionary == NULL){ printf("\nFile Error.\n"); exit(1); }
  system("clear"); printHeader();

  lineCount = LineCount(inputFile, maxLen); //counts the rows of input file
  char *strArray[lineCount+1], userDictName[25];

  //allcoate memory for string array
  allocateMEM(inputFile, lineCount+1, maxlen, strArray);

  //print menu and get user selection, exit if user selects exit
  menuSelection = printMenu();
  if(menuSelection == 4){exit(1);}

  //while loop to continue to select options until user exits
  while(menuSelection != 4){
    if(menuSelection == 1){ //SpellCheck
      userDictCheck = checkSpelling(dictionary, strArray, userDictName,
                                    lineCount, maxlen, userDictCheck);
    }
    if(menuSelection == 2){   // search and replace
      SearchReplace(strArray, lineCount, maxlen);
    }
    if(menuSelection == 3){   // save modified file to output
      saveFile(strArray, outputFile, lineCount);
      printf("\nSaving successful. Enter key to continue: ");
      getchar();
    }
    menuSelection = printMenu();
  }
  fclose(dictionary);
  return 0;
}

//------------------------------------------------------------------------------

void SearchReplace(char *strArray[], int lineCount, int maxLen){
  int newstrLEN, strLEN, rowCount, lineLEN, n, strDIFF, match = 0, counter = 0;
  int lineSearch = 0, startLine, finishLine;
  char userStr[20], newUserStr[20], temp[100], temp2[20];
  //prompt user for string and find length
  printf("Enter a string to search for: ");
  memset(userStr, 0, 20);
  fgets(temp2, 20, stdin);
  strLEN = strlen(temp2)-1;
  strncpy(userStr, temp2, strLEN);
  //go through loop of each row in strArray and each line's length
  rowCount = 0;
  while(rowCount < lineCount && match == 0){
    lineLEN = 0;
    while(lineLEN < maxLen && match == 0){
      n = memcmp(strArray[rowCount]+lineLEN, userStr, strLEN);
      if(n == 0){    //if the cmp is equal to 0, then a match is found
        printf("The first match is at line %d in char position %d.\n",rowCount,lineLEN);
        printf("What would you like1 to replace it with?: ");
        memset(temp2,0,20); memset(newUserStr, 0, 20);
        fgets(temp2, 20, stdin);
        newstrLEN = strlen(temp2)-1;
        strncpy(newUserStr,temp2,newstrLEN);
        // /*
        printf("Would you like to search and replace through a certain set of lines? 1 = yes, 2 = no: ");
        scanf("%d",&lineSearch);
        while(lineSearch != 1 && lineSearch != 2){
          printf("Wrong entry. Please Enter a 1 to chose lines, or a 2 to search the whole file: ");
          scanf("%d",&lineSearch);
        }

        if(lineSearch == 1){
          printf("Please enter the starting line between 0 and %d: ",lineCount-1);
          scanf("%d",&startLine);
          while(startLine < 0 || startLine >= lineCount){
            printf("Wrong entry. Please enter betwee 0 and %d: ", lineCount-1);
            scanf("%d", &startLine);
          }
          printf("\nPlease enter the finishing line between %d and %d: ", startLine+1, lineCount);
          scanf("%d",&finishLine);
          while(finishLine < 0 || finishLine >= lineCount){
            printf("Wrong entry. Please enter betwee %d and %d: ", startLine+1, lineCount);
            scanf("%d", &finishLine);
          }
        }
        // */
        match = 1;
      }
      if(n != 0){lineLEN++; }
    } // end of while
    rowCount++;
  } //end of for rows count
  //loop to go through the strarray and replace the word the to the user gave
  if(match == 1){
    printf("\n===================================================\n");
    if(lineSearch == 2){
      for(rowCount = 0; rowCount < lineCount; rowCount++){
        lineLEN = 0;
        while(lineLEN < maxLen){
          n = memcmp(strArray[rowCount]+lineLEN, userStr, strLEN);
          if(n == 0){
            printf("A match is replaced at line %d in char position %d.\n",rowCount,lineLEN);

            //reallocate space for word and replace it:
            strDIFF = 0; counter++;
            strDIFF = newstrLEN - strLEN;
            strArray[rowCount] = (char *)realloc(strArray[rowCount], maxLen+strDIFF*counter);
            memset(temp, 0, 100);
            strcpy(temp, strArray[rowCount]+lineLEN+strLEN); //save ending of the string
            *(strArray[rowCount]+lineLEN) = '\0';  //null the string at start of word to be replaced
            strcat(strArray[rowCount], newUserStr);  //concentrate new word onto original string
            strcat(strArray[rowCount], temp);        //adds back on the end of the original string
            lineLEN += newstrLEN;
          }
          if(n != 0) { lineLEN++; }
        }
      }
    } else if(lineSearch == 2){
      for(rowCount = startLine; rowCount <= finishLine; rowCount++){
        lineLEN = 0;
        while(lineLEN < maxLen){
          n = memcmp(strArray[rowCount]+lineLEN, userStr, strLEN);
          if(n == 0){
            printf("A match is replaced at line %d in char position %d.\n",rowCount,lineLEN);

            //reallocate space for word and replace it:
            strDIFF = 0; counter++;
            strDIFF = newstrLEN - strLEN;
            strArray[rowCount] = (char *)realloc(strArray[rowCount], maxLen+strDIFF*counter);
            memset(temp, 0, 100);
            strcpy(temp, strArray[rowCount]+lineLEN+strLEN); //save ending of the string
            *(strArray[rowCount]+lineLEN) = '\0';  //null the string at start of word to be replaced
            strcat(strArray[rowCount], newUserStr);  //concentrate new word onto original string
            strcat(strArray[rowCount], temp);        //adds back on the end of the original string
            lineLEN += newstrLEN;
          }
          if(n != 0) { lineLEN++; }
        }
      }
    }
    printf("===================================================\n\n");

  }else{
    printf("\nNo match found. match = %d\n",match);
  }
    printf("Enter any key to continue: "); getchar();
}

//------------------------------------------------------------------------------

/* this function writes the string array back to the output file that was entered
at the command line. */
void saveFile(char *strArray[], const char *outputFile, int lineCount){
  FILE *file;
  int i;
  file = fopen(outputFile, "w");
  for(i = 0; i < lineCount; i++){
    fprintf(file, "%s", strArray[i]);
  }
  fclose(file);


}

//------------------------------------------------------------------------------

/* this function takes in the dictionary file, stringArray containing the input file, the name
for the user dictionary if its been created, the lineCount = # of rows in the input file, and the
check integer for checking to see if the personal dictionary has been created or not. */
int checkSpelling(FILE *dictFile, char *strArray[], char *userDictName,
                    int lineCount, int maxLen, int userDictCheck){
  int rowCount = 0, linePos = 0, check = 0, i, newSize, stringLen, counter = 0;
  char userEntry[2], temp[100];
  char strWord[50], userDictWord[50], dictWord[50], userWord[50], stringAfterWord[50];
  FILE *userDictFile;
  if(userDictCheck == 1){ userDictFile = fopen(userDictName, "a+"); }   //opens personal dictionary if one has been created
  for(rowCount = 0; rowCount < lineCount; rowCount++){
    linePos = 0;
    while (linePos < maxLen-1){
      bool a = *(strArray[rowCount]+linePos) < 'a';
      bool b = *(strArray[rowCount]+linePos) > 'z';
      bool c = *(strArray[rowCount]+linePos) < 'A';
      bool d = *(strArray[rowCount]+linePos) > 'Z';
      //goes to the next word in the string, or the starting word by skipping punctuation
      while((a || b) && (c || d)){
          linePos++;          //sets position of where the word is in the line
          a = *(strArray[rowCount]+linePos) < 'a';
          b = *(strArray[rowCount]+linePos) > 'z';
          c = *(strArray[rowCount]+linePos) < 'A';
          d = *(strArray[rowCount]+linePos) > 'Z';
        }
        if(linePos == maxLen){    //stop overflow of program by adding in a random letter, this is a fix
          break;
        }
        //resets current strWord memory and below for those strings each loop3
        memset(strWord, 0, 50); memset(dictWord, 0, 50); memset(userDictWord, 0, 50);
        sscanf(strArray[rowCount]+linePos, "%s", strWord);  //stores next word in string array
        stringLen = strlen(strWord);
        for(i = 0; i < stringLen; i++){
          strWord[i] = tolower(strWord[i]);
          if(strWord[i] < 'a' || strWord[i] > 'z'){     //removes punctuation in strWord
            strWord[i] = '\0';
            stringLen = strlen(strWord);
         }
        }
        if(strWord[stringLen] == '\n'){ //nulls the end of the string if it takes in a new line character
          strWord[stringLen] = '\0';
        }
        check = 0;
        rewind(dictFile);
        //while loop to check starting dictionary for word
        while(check == 0 && fgets(dictWord, 50, dictFile) != NULL){
          if(strncmp(dictWord, strWord, stringLen) == 0){
            check = 1;
          }
        }
        //while loop to check personal dictionary if its already been created
        if(userDictCheck == 1 && check == 0){
          rewind(userDictFile);   //resets personal dictionary
          while(fgets(userDictWord, 50, userDictFile) != NULL && check == 0){
            if(strncmp(userDictWord, strWord, stringLen) == 0){
              check = 1;
            }
          }
        }
          if(check == 1){ linePos = linePos + stringLen; }
          if(check == 0){
            memset(userEntry, 0, 2);
            memset(temp, 0, 100);
            //check if user wants to replace word or ignore to add to dictionary
            printf("%s is not found. Enter 1 to replace, 2 to ignore: ", strWord);
            fgets(temp, 100, stdin);
            strncpy(userEntry, temp, strlen(temp)-1); //removes the newline char after user input
            while(strcmp(userEntry, "1") !=0 && strcmp(userEntry, "2") != 0){
              printf("\nWrong entry. Please enter a 1 or 2: ");
              memset(userEntry, 0, 2);
              memset(temp, 0, 100);
              fgets(temp, 100, stdin);
              strncpy(userEntry, temp, strlen(temp)-1); //removes the newline char after user input
            }
            //if statement that goes into replacing the word with a user entered word
            if(!strcmp(userEntry, "1")){
              memset(userWord, 0, 50);
              memset(temp, 0, 100);
              memset(stringAfterWord, 0, 50);
              printf("Please enter a string to replace it with: ");
              fgets(temp, 100, stdin);
              strncpy(userWord, temp, strlen(temp)-1); //removes the newline char after user input


              //find size difference, add to max length of line and realloc for replacement word
              newSize = strlen(userWord) - stringLen;
            //  maxLen += newSize;
              //allcoate space for new word
              counter++;
              strArray[rowCount] = (char *)realloc(strArray[rowCount], maxLen+newSize*counter);
              //save the ending of the string before nulling
              strcpy(stringAfterWord, strArray[rowCount]+linePos+stringLen);
              *(strArray[rowCount]+linePos) = '\0';  //null the string at start of word to be replaced
              strcat(strArray[rowCount], userWord); //concentrate new word onto original string
              strcat(strArray[rowCount], stringAfterWord); //adds back on rest of line
              linePos += newSize;   //resets line position to account for word size difference
            } // ends user entry == 1
            if(!strcmp(userEntry, "2")){

              //if block creates the user dictionary
              if(userDictCheck == 0){
                printf("\nEnter a personal dictionary name: ");
                scanf("%s", userDictName);
                while((getchar()) != '\n');      //controled way to flush stdiniput for \n
                strcat(userDictName, ".txt");
                userDictFile = fopen(userDictName, "w+");
                fprintf(userDictFile, "%s\n",strWord);
                userDictCheck = 1;
              }
              else if(userDictCheck == 1){
                  fprintf(userDictFile, "%s\n",strWord);
                //  while((getchar()) != '\n');      //controled way to flush stdiniput for \n

              }
            } // ends userEntry == 2
            linePos += stringLen;     //increment to the next word's position
          } // ends if check == 0, or if no word in dictionary/personal is found
        //  linePos += stringLen; //reset linePos to next word
    } //ends while loop of (linePos < maxLen)
  } // ends for loop counting rows of string array
  if(userDictCheck == 1){ fclose(userDictFile); }
  return userDictCheck;
}

//------------------------------------------------------------------------------

/* this function takes in the string character array as input as well as the row
   and maxline count to allocate the appropriate amount of memory needed */
void allocateMEM(const char *inFile, int lineCount, int maxLen, char *strArray[]){
  FILE *file;
  int i;
  if((file = fopen(inFile, "rt")) == NULL){ printf("\nFile Error.\n"); exit(1); }

  //loop to allocate memory for each line and then place that line into the spot
  //in memory
  for(i = 0; i < lineCount; i++){
    strArray[i] = (char *)malloc(maxLen * sizeof(char));
    fgets(strArray[i], maxLen+1, file);
  }
  fclose(file);
}

//------------------------------------------------------------------------------

/* this function takes in the input file char pointer and finds the row count
   of the input file */
int LineCount(const char *inFile, int *maxLen){
  FILE *file;
  int count= 0, maxlen = 0;
  char temp[256];

  //check if file is not null
  if((file = fopen(inFile, "rt")) == NULL){
    printf("Error opening file.\n"); exit(1);
  }

  //count the number of lines in the file
  while(!feof(file)){
    fgets(temp, 256, file);
    if(strlen(temp) > maxlen){
      maxlen = strlen(temp);
      *maxLen = maxlen;
    }
    count++;
  }
  fclose(file);
  return count;
}

//------------------------------------------------------------------------------

//function prints a nicely formated menu
void printHeader(){
  printf(" =========================");
  printf("=================================================");
  printf("\n >>\tWelcome to John Lawler's program 5 for ECE 2220 program 5");
  printf("\t<<\n -------------------------");
  printf("-------------------------------------------------");
  printf("\n >>\tThis program takes in three cmd line files and prompts ");
  printf("user with\n\t a menu to check spelling, search/replace a word, ");
  printf("and save\n\tthe modified file to output file.\t\t\t\t<<");
  printf("\n =======================");
  printf("===================================================\n\n");
}

//------------------------------------------------------------------------------

/* this function prints the menu for user to select from */
int printMenu(){
  int choice;
  system("clear");
  printHeader();
  printf("\n1. Check the spelling of Input_File\n2. Search and Replace a word\n");
  printf("3. Save the modified file to Output_File\n4. Exit\n\n");
  printf("Please make a selection of between 1- 4: ");
  scanf("%d",&choice);
  getchar();
  while(choice < 1 || choice > 4){
    printf("\nWrong selection. Please select a number 1-4 from the menu: ");
    scanf("%d",&choice);
    getchar();
  }

  return choice;
}

//------------------------------------------------------------------------------
