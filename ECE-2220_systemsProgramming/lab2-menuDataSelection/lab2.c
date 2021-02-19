/* John Lawler
	ECE 2220 
	January 29 2019

This is the main driver file for Project 2. The purpose of this project is to take
in data from the user and store them in each respective location, all data stored 
is on one politician

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
  char firstName[33];
  char *middleIn;
  char lastName[33];
  char suffix[4];
  int age;
  char sex[2];
  char office[33];
  int yearsServed;
  int polParty;
  char state[3];
  char phoneNum[11];
  char twitter[16];
  float moneyTaken;
  float liesTold;
  float lieRatio;
  } PoliticianInfo;


void printMenu();
void printStatesList();
void clearData(PoliticianInfo person);
void displayData(PoliticianInfo person);

int main(){
  int selectNum,placeHolder;
  char charHolder[2],character;
  char sexChar;
  //initialize structure strings to  "."  to indicate unknown value and integers to 0
  PoliticianInfo person;
  person = (PoliticianInfo){".",".",".",".",0,".",".",0,0,".",".",".",0,0,0};
  
  
  do{
  //menu selection prompt
  
    printMenu();
    printf("Please choose a selection: ");
    scanf("%i",&selectNum);
    printf("\n");
    while((selectNum < 0) || (selectNum > 8)){
      printf("Incorrect selection. Please choose a number from 1-8: ");
      scanf("%i",&selectNum);
    }
  
    //Inputing the politicans first/last name, middle intial and suffix
    if(selectNum == 1){
      
      //first name and entry error check
      printf("Enter your Politician's First Name: ");
      scanf("%s",person.firstName); 
      while(strlen(person.firstName) > 32){
        printf("Entry exceeds size. Please renter first name: ");
        scanf("%s",person.firstName);
      }
      
      //middle initial entry and error check for size and character
      printf("Enter your politician's Middle initial(One Letter");
      printf(" Only; enter - if N/A): ");
      scanf("%s",charHolder);
      while(charHolder[1] != 0){
        printf("Wrong Entry. Re-enter (One Letter Only): ");
        scanf("%s",charHolder);
      } person.middleIn = charHolder;
      
      //last name entry and error check
      printf("Enter your Politician's Last Name: ");
      scanf("%s",person.lastName);
      while(strlen(person.lastName) > 32){
        printf("Entry exceeds size. Please renter last name: ");
        scanf("%s",person.lastName);
      }
      
      //suffix entry and error check 
      printf("Enter your politician's Suffix (Enter - if N/A): ");
      scanf("%s",person.suffix);
      while(strlen(person.suffix) > 3){
        printf("Entry exceeds size. Please renter suffix: ");
        scanf("%s",person.suffix);
      }
      selectNum = 0;
      system("clear");
    }
  
    //inputting the years, respective party affiliation, office position, and state of origin
    if(selectNum == 2){
  
      //years served entry and error check
      printf("Enter the amount in years served (0 - 99): ");
      scanf("%d",&placeHolder);
      while((placeHolder < 0) || (placeHolder > 99)){
        printf("Incorrect input. Please input a two digit number 0 - 99: ");
        scanf("%d",&placeHolder);
      } person.yearsServed = placeHolder;
    
      //pol party entry and error check
      printf("\nEnter the political party of the politician chosen from the list:\n");
      printf(" below:\n1. Constitution\t\t4. Libertarian\n2. Democrat\t\t5. Rep");
      printf("ublican\n3. Green\n\n");
      printf("Selection: ");
      scanf("%d",&placeHolder);
      while((placeHolder < 1) || (placeHolder > 5)){
        printf("Incorrect input. Please choose a selection between 1 and 5: ");
        scanf("%d\n",&placeHolder);
      } person.polParty = placeHolder;
      

      //office position entry 
      printf("\nEnter the office position held:");
      getchar();
      fgets(person.office,32,stdin);
      //scanf("%s",person.office);
      while(strlen(person.office) > 31){
        printf("Entry exceeds size. Please renter office position: ");
        fgets(person.office,32,stdin);
      }
      
      //state entry 
      printStatesList();
      printf("Enter the state abbreviation choosen from the list provided: ");
      scanf("%s",person.state);
      while(strlen(person.state) > 2){
      printf("Entry exceeds size. Please renter the state: ");
      scanf("%s",person.state);
      }
      selectNum = 0;
      system("clear");
    }
    
    
    //inputing sex and age
    if(selectNum == 3){
    
      //age input and wrong entry check
      printf("Enter the politicians age (21 - 99): ");
      scanf("%d",&person.age);
      while((person.age < 21) || (person.age > 99)){
        printf("Wrong entry. Re enter a correct age: ");
        scanf("%d",&person.age);
      }
      
      //sex and error input check
      printf("Enter the sex of the politician (M or F): ");
      scanf(" %c",&sexChar);
      while ((sexChar != 77) && (sexChar != 70)){
        printf("Wrong entry. Re enter M or F: ");
        scanf(" %c",&sexChar);
      } person.sex[0] = sexChar;
      
      selectNum = 0;
      system("clear");
    }
    
    //twitter and phone number entry
    if(selectNum == 4){
    
      //twitter entry
      printf("Enter the politicians twitter username (omit @): ");
      scanf(" %s", person.twitter);
      while(strlen(person.twitter) > 15){
        printf("Entry exceeds size. Please renter the username: ");
        scanf(" %s",person.twitter);
      }
      
      
      //phone # entry
      printf("Enter the politician's phone number (no spaces/dashes): ");
      scanf(" %s",person.phoneNum);
      while(strlen(person.phoneNum) > 10){
        printf("Entry exceeds size. Please renter the number: ");
        scanf(" %s",person.phoneNum);
      }
      
      selectNum = 0;
      system("clear");
    }
    
    //contributions and lies
    if(selectNum == 5){
      
      //money recieved and range check
      printf("enter the dollar amount of contributions this politician recieved: ");
      scanf(" %f",&person.moneyTaken);
      while (person.moneyTaken < 0 || person.moneyTaken > 10E50){
        printf("Amount not within range. Enter Amount from 0 to 1.0x10^50: ");
        scanf(" %f",&person.moneyTaken);
      }
      
      //lies told
      printf("Enter the number of lies this politician has said: ");
      scanf(" %f", &person.liesTold);
      while (person.liesTold < 0 || person.liesTold > 10E100){
        printf("Amount not within range. Enter Amount from 0 to 1.0x10^100: ");
        scanf(" %f",&person.liesTold);
      }
      person.lieRatio = person.moneyTaken / person.liesTold;
      selectNum = 0;
      system("clear");
    }
    
    //Displays the data
    if(selectNum == 6){
      displayData(person);
      printf("Enter any character to continue: ");
      scanf(" %c",&character);
      system("clear");
    }
  
    //clears the data
    if(selectNum == 7){
      person.age = 0;
      memset(person.sex,0,sizeof(person.sex));
      person = (PoliticianInfo){".",".",".",".",0,".",".",0,0,".",".",".",0,0,0};
      selectNum=0;
      system("clear");
    }
  } while (selectNum != 8);
	return 0;
}

//menu function to print menu when called
void printMenu(){
  printf("\n1. Enter Name\n2. Enter Years, Party, Office, and State\n");
  printf("3. Enter Age and Sex\n4. Enter Contacts\n5. Enter Contributions and Lies\n");
  printf("6. Display the Data\n7. Clear all Data\n8. Quit\n");
}


//state list
void printStatesList(){
  system("clear");
  printf("AL - Alabama\nAK - Alaska\nAZ - Arizona\nAR - Arkansas\nCA - California\n");
  printf("CO - Colorado\nCT - Connecticut\nDE - Delaware\nDC - District of Columbia\n");
  printf("FL - Florida\nGA - Georgia\nGU - Guam\nHI - Hawaii\nID - Idaho\n");
  printf("IL - Illinois\nIN - Indiana\nIA - Iowa\nKS - Kansas\nKY - Kentucky\n");
  printf("LA - Louisiana\nME - Maine\nMD - Maryland\nMA - Massachusetts\n");
  printf("MI - Michigan\nMN - Minnesota\nMS - Mississippi\nMO - Missouri\n");
  printf("MT - Montana\nNE - Nebraska\nNV - Nevada\nNH - New Hampshire\n");
  printf("NJ - New Jersey\nNM - New Mexico\nNY - New York\nNC - North Carolina\n");
  printf("ND - North Dakota\nOH - Ohio\nOK - Oklahoma\nOR - Oregon\n");
  printf("PA - Pennsylvania\nRI - Rhode Island\nSC - South Carolina\n");
  printf("SD - South Dakota\nTN - Tennessee\nTX - Texas\nUT - Utah\nVT - Vermont\n");
  printf("VA - Virginia\nWA - Washington\nWV - West Virgnia\nWI - Wisconsin\n");
  printf("WY - Wyoming\n");
}


//function to display the data when called
void displayData(PoliticianInfo person){

  //first name
  printf("First Name: ");
  if (strcmp(person.firstName,".") != 0){
    printf("\t\t%s\n",person.firstName);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //middle initial
  printf("Middle Initial: ");
  if (strcmp(person.middleIn,".") != 0){
    printf("\t%s\n",person.middleIn);
  }
  else{ printf("\tUnknown value\n"); }
  
  //last name
  printf("Last Name: ");
  if (strcmp(person.lastName,".") != 0){
    printf("\t\t%s\n",person.lastName);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //suffix
  printf("Suffix: ");
  if (strcmp(person.suffix,".") != 0){
    printf("\t\t%s\n",person.suffix);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //age
  printf("Age: ");
  if (person.age != 0){
    printf("\t\t\t%i\n",person.age);
  }
  else{ printf("\t\t\tUnknown value\n"); }
  
  //sex
  printf("Sex: ");
  if (strcmp(person.sex,".") != 0){
    printf("\t\t\t%s\n",person.sex);
  }
  else{ printf("\t\t\tUnknown value\n"); }
  
  //office
  printf("Office Held: ");
  if (strcmp(person.office,".") != 0){
    printf("\t\t%s",person.office);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //years served
  printf("Years Served: ");
  if (person.yearsServed != 0){
    printf("\t\t%i\n",person.yearsServed);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //political party
  printf("Political Party: ");
  if (person.polParty != 0){
    if(person.polParty == 1){
        printf("\tConstitution\n");
    }
    if(person.polParty == 2){
        printf("\tDemocrat\n");
    }
    if(person.polParty == 3){
        printf("\tGreen\n");
    }
    if(person.polParty == 4){
        printf("\tLibertarian\n");
    }
    if(person.polParty == 5){
        printf("\tRepublican\n");   
    }    
  }
  else{ printf("\tUnknown value\n"); }
  
  //state
  printf("State: ");
  if (strcmp(person.state,".") != 0){
    printf("\t\t\t%s\n",person.state);
  }
  else{ printf("\t\t\tUnknown value\n"); }
  
  //twitter
  printf("Twitter: ");
  if (strcmp(person.twitter,".") != 0){
    printf("\t\t%s\n",person.twitter);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //phone
  printf("Phone Number: ");
  if (strcmp(person.phoneNum,".") != 0){
    printf("\t\t(%.3s)%s \b\b\b\b\b-%s\n",person.phoneNum,
    person.phoneNum+strlen(person.phoneNum)-7,
    person.phoneNum+strlen(person.phoneNum)-4);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //usd recieved
  printf("US Dollars Recieved: ");
  if (person.moneyTaken != 0){
    printf("\t%0.2f\n",person.moneyTaken);
  }
  else{ printf("\tUnknown value\n"); }
  
  //lies told
  printf("Lies Told: ");
  if (person.liesTold != 0){
    printf("\t\t%0.2f\n",person.liesTold);
  }
  else{ printf("\t\tUnknown value\n"); }
  
  //dollars per lie told ratio
  printf("\nRatio of Lies \ntold to money \nrecieved($ per lie): ");
  if (person.lieRatio != 0){
    printf("\t%0.2f\n\n",person.lieRatio);
  }
  else{ printf("\tUnknown value\n\n"); }
}

  
  

















