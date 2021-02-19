/*
John Lawler
ECE 2220
March 6th 2019

This is program 4. The purpose of this program is to take two files as inputs
via command line arguments and convolve the two text files

*/

//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct declaration to be used for storing file inputs
struct TData {
  int MinIndex, MaxIndex, Points;
  float *pValue;
} Input[2], Output;

//function prototypes
void Convolve();
void printFiles(const char *file1, const char *file2);
void storePoints(const char *fileName, int num);


//-----------------------------------------------------------------------------

int main(int argc, char *argv[]){
  const char *fileName1 = argv[1];
  const char *fileName2 = argv[2];
  FILE * outFile;
  int i, j = 0;
  system("clear");
  //check argc size to make sure only 3 inputs are  taken:
  if(argc != 4){
    printf("Wrong Entry Size. Please Enter only Two files\n");
    exit(1);
  }

  //get file information from both files
  storePoints(fileName1, 0);
  storePoints(fileName2, 1);

  //print files
  printf("\nYour Input Files: \n");
  printFiles(fileName1,fileName2);
  //use to check files were copied correctly
  printf("\nEnter any key to continue: ");
  getchar();
  system("clear");
  //check output was convolved correctly
  printf("\nYour Output values after Convolution: \n");
  Convolve();
  printf("\n>>Writing to File %s\n\n",argv[3]);
  //write data to output file
  outFile = fopen(argv[3], "wt");
  if(outFile == NULL){ printf("File Error"); exit(1); }
  for(i = Output.MinIndex+1; i <= Output.MaxIndex; i++){
    fprintf(outFile,"%4i %10f\n",i,Output.pValue[j++]);
  }
  fclose(outFile);
  return 0;
}

//-----------------------------------------------------------------------------

/* this function convolves the two sets of values and places it in the
  output structure variable
*/
void Convolve(){
  int i, j, k, OutMin, OutMax, outSize;
  float tmp;
  int f1_Min = Input[0].MinIndex, f1_Max = Input[0].MaxIndex;
  int f2_Min = Input[1].MinIndex, f2_Max = Input[1].MaxIndex;
  OutMin = f1_Min + f2_Min;
  OutMax = f1_Max + f2_Max;
  outSize = OutMax - OutMin + 1;

  //allocate memory for output pvalues
  Output.pValue = (float*)calloc(outSize,sizeof(float));

  //loop that goes the size of the output to calculate p values for output
  for(i = 0; i < outSize; i++){
    k = i; tmp = 0;
    for (j = 0; j < Input[1].Points; j++){
      if(k >= 0 && k < Input[0].Points){
        tmp = tmp + (Input[0].pValue[k]*Input[1].pValue[j]);
      }
      k = k - 1;
      Output.pValue[i] = tmp;
    }
    printf("Output Value[%i] = %10f\n", i, tmp);
  }

  //store the local function variables into the structure
  Output.MinIndex = OutMin;
  Output.MaxIndex = OutMax;
  Output.Points = outSize;
}

//-----------------------------------------------------------------------------

/* This function is used to reprint the entered fiels to check they were
    written corectly to the structure.
*/
void printFiles(const char *file1, const char *file2){
  int i=0,k;
  printf("\nFile %s:\n",file1);
  k = Input[0].MinIndex;
  for(i = 0; i<Input[0].Points; i++){
    printf("%5i %8.3f\n", k, Input[0].pValue[i]); k++;
  }

  printf("\nFile %s:\n",file2);
  k = Input[1].MinIndex;
  for(i = 0; i<Input[1].Points; i++){
    printf("%5i %8.3f\n", k, Input[1].pValue[i]); k++;
  }

}

//-----------------------------------------------------------------------------

/* this function retrieves the data from each file when called in the main
    and inputs the text data into the input array in the structure
*/
void storePoints(const char *fileName, int num){
  int index, i, Min, Max;
  int count = Input[num].Points;
  FILE *file;
  float point;
  file = fopen(fileName,"rt");
  //check if file is not empty
  if(file == NULL){ printf("File Error"); exit(1); }

  //set the min index
  fscanf(file, "%i %f", &Input[num].MinIndex, &point);
  Min = Input[num].MinIndex;
  Input[num].Points = 0;

  //do loop to find the max index from counting number of points
  do {
    fscanf(file, "%i %f", &index, &point);
    Input[num].Points++;
  } while (!feof(file));
  Max = Input[num].Points + Min - 1;
  count = Input[num].Points;
  Input[num].MaxIndex = Max;
  fclose(file);

  //dynamic mem allocation for array of storing the Points
  Input[num].pValue = calloc(count, sizeof(float));
  file = fopen(fileName, "rt");
  for(i = 0; i < count; i++){
    fscanf(file, "%i %f", &index, &Input[num].pValue[i]);
  }
  fclose(file);


}

//-----------------------------------------------------------------------------
