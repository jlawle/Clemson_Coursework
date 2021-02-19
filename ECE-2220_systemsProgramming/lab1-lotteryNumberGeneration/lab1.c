/* John Lawler January 18 2019
ECE2220 Project 1
The purpsoe of this project is to provide user with the ability to enter a
saelect group of lottery numbers and the program returns the lottery ticket
numbers for that user
 

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Choose(int x, int y);
void intSwap(int * x, int * y);
void printA(int a[], int size);
void randomize(int array[], int size);

int main(){
	int count=0, temp, sum=0, i, k, j, array[6]= { 0 };
	int lotNum[6]= { 0 },winLotNum[6]= { 0 };
	
	//while loop to retrieve user input
	while (k == 0) {
		array[0] = Choose(1,5);
		array[1] = Choose(1,10);
		array[2] = Choose(1,10);
		array[3] = Choose(10,12);
		array[4] = Choose(8,15);
		array[5] = Choose(8, 15);
		sum = 0;

		//checks to make sure sum is under 50
		for(i=0;i<6;i++){
			sum += array[i];
		}
		if(sum<=50) {
			k = 1;
		}
		else {
			printf("Sum is > 50. Re enter lottery numbers.");
			k=0;
		}
	}



	//sorting the array
	for(i=0; i<6; i++) {
		for(k=i+1; k<6; k++) {
			if(array[i] > array[k]){
				temp = array[i];
				array[i] = array[k];
				array[k] = temp;
			}
		}
	}
	
	//print array in order
	printf("\nYour delta numbers are:: ");
	printA(array,6);
	
	//randomize delta sequence
	printf("\nYour final delta sequence is: ");
	randomize(array,6);
	printA(array,6);	

	
	k=0;
	for(i=0; i<6; i++){
		k += array[i];
		lotNum[i] = k;
	}
	printf("\nYour lottery numbers are: ");
	printA(lotNum,6);
	printf("\n");
 

 
   
   while (count < 100000000) {

   	//generate winning lottery ticket
     	winLotNum[0] = rand() % 50;
		for (i=1; i<6; i++){
       	temp = rand() % 50;
        	for(j=0;j<i;j++){
        		//check for duplicates
        		if(temp == winLotNum[j]){
					temp = rand() % 50;
				}
			winLotNum[i] = temp;
      	}
		}

		

	//	printf("\n");
	//	for(i=0;i<6;i++){
	//		printf("%i ",winLotNum[i]);
	//	}

		sum = 0;
     	for (i=0; i<6; i++){
         if (lotNum[i] == winLotNum[i]){
         sum += 1;
         }
     	}
     	if (sum > 5){
     		printf("\n");
       	printf("Congratulations! You've won after %i tries.\n\n",count);
       	exit(1);
     	}
     	else {
      	count += 1;
     	}
    }     
	
	printf("\n");
  	printf("\nyou did not win after %i tries.\n\n", count);
		

	
	return 0;

}

void printA(int a[], int size){
	int i;
	for (i=0; i<size; i++){
		printf("%i ",a[i]);
	}
}

//function to recieve lottery numbers from user
int Choose(int x, int y){
	int num,i=0;
	
	while(i==0){
		
		printf("Enter a number from %i to %i, inclusive: ",x,y);
		scanf("%i",&num);
		
		if((x <= num) && (y >= num)){
			i = 1;
		}
		else{
			printf("Wrong input.\n");
		}
	
	}

	return num;
}

//swap function for random assortment
void intSwap(int * x, int * y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

//random reorder array function
void randomize(int array[], int size){
	int i,indexNum;

	//randomize seed value each time function is called
	srand(time(NULL));

	for(i = size-1; i >= 1; i--){
		indexNum = rand() % (i + 1);
		intSwap(&array[i],&array[indexNum]);
	}
}

	



