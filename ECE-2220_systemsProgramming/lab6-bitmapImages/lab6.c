/*
John Lawler
ECE 2220
April 9th 2019

This is program 6. this program takes in 3 file inputs, including a dictionary,
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

/* structure definitions */
struct HEADER
{  unsigned short int Type;     /* Magic identifier       */
   unsigned int Size;           /* File size in bytes     */
   unsigned short int Reserved1, Reserved2;
   unsigned int Offset;         /* Offset to data (in B)  */
} Header;                       /* -- 14 Bytes --         */

struct INFOHEADER
{  unsigned int Size;           /* Header size in bytes    */
   int Width, Height;           /* Width / Height of image */
   unsigned short int Planes;   /* Number of colour planes */
   unsigned short int Bits;     /* Bits per pixel          */
   unsigned int Compression;    /* Compression type        */
   unsigned int ImageSize;      /* Image size in bytes     */
   int xResolution, yResolution;/* Pixels per meter        */
   unsigned int Colors;         /* Number of colors        */
   unsigned int ImportantColors;/* Important colors        */
} InfoHeader;                   /* -- 40 Bytes --          */


typedef struct tagPIXEL  { unsigned char Red, Green, Blue; } PIXEL;

 struct userRGB {int R, G, B; } rgb;
//------------------------------------------------------------------------------

/* function prototypes */
void edgeDetect(PIXEL ***inBMP,PIXEL ***outBMP, char *outfileName);
void shadeIMAGE(PIXEL ***inBMP,PIXEL ***outBMP, char *outfileName);
void saveUserColor(const char *red, const char *green, const char *blue);

//------------------------------------------------------------------------------

/* this is the nmain function */
int main(int argc, char *argv[]){
  system("clear");
  PIXEL ***inPtrBMP, ***outPtrBMP;
  int row, col;
  FILE *file;
  char outputName[20] = { '\0' };
  const char *inputFile = argv[1];

  //if not all arguments are entered correctly, exit porgram
  if(argc != 6){ printf("Error reading in files. Exiting Program.\n"); exit(1);}

  //places the output filename given by the user into the char array to create new files
  strncpy(outputName, argv[2], strlen(argv[2]) - 4);
  //open bitmap file
  file = fopen(inputFile, "rb");
  if (file == NULL){printf("File error.\n"); exit(1);}

  //fill the headers with the file information
  fread(&Header, 14, 1, file);
  fread(&InfoHeader, 40, 1, file);

  /* intializes the ***pointer size to however many rows (height count) the
    the image contains */
   outPtrBMP = (PIXEL***)calloc(InfoHeader.Height,sizeof(PIXEL**));
   inPtrBMP = (PIXEL***)calloc(InfoHeader.Height,sizeof(PIXEL**));

  /* for loop initializing each row **pointer to the size of however many
    columns (width size) the image has */
  for(row = 0; row < InfoHeader.Height; row++){
    outPtrBMP[row] = (PIXEL**)calloc(InfoHeader.Width,sizeof(PIXEL*));
    inPtrBMP[row] = (PIXEL**)calloc(InfoHeader.Width,sizeof(PIXEL*));

    /* nested for loop that runs to allocate memory for each row **pointer[row] to hold
       the count of width pixels in the bitmap where each [row][col] is referenced
       by a single *pointer to the pixel itself of size struct PIXEL  */
    for(col = 0; col < InfoHeader.Width; col++){
      inPtrBMP[row][col] = (PIXEL *)calloc(1,sizeof(PIXEL));
      outPtrBMP[row][col] = (PIXEL *)calloc(1,sizeof(PIXEL));

      //read each individual pixel into its respective spot in ptr[row][col],
      //pixels are read and written starting at blue becuase of their order in the file
      fread(&(*inPtrBMP[row][col]).Blue, 1, 1, file);
      fread(&(*inPtrBMP[row][col]).Green, 1, 1, file);
      fread(&(*inPtrBMP[row][col]).Red, 1, 1, file);

    } //end of for loop col =0
  } //end of for loop row = 0

  //sets the user color input into the structure
  saveUserColor(argv[3],argv[4],argv[5]);
  //calls edge detect function, sending it the empty allocated pointers to the bitmap
  edgeDetect(inPtrBMP, outPtrBMP, outputName);
  //sends the same pointers and file output name from user
  shadeIMAGE(inPtrBMP, outPtrBMP, outputName);
  fclose(file);

  return 0;
}

//------------------------------------------------------------------------------

/* this function applies the edge filter in the given instructions to present a
    different image on top of another image */
void edgeDetect(PIXEL ***inBMP,PIXEL ***tempBMP, char *outfileName){
  char Matrix[3][3] = { {  0, -1,  0 },
                        { -1,  4, -1 },
                        {  0, -1,  0 } };
  FILE *outputFile;
  char name[20] = { '\0' };
  int row, col, w, h;
  strcpy(name, outfileName);
  strcat(name,"(edge).bmp");
  outputFile = fopen(name, "wb");

  //write the header information
  fwrite(&Header, 14, 1, outputFile);
  fwrite(&InfoHeader, 40, 1, outputFile);
  //goes through each row, for col amount of times, to change each pixel
  for(row = 0; row < InfoHeader.Height; row++){
    for(col = 0; col < InfoHeader.Width; col++){
      //if within bounds, perform arithmetic to add filter
      if(row == 0 || row == InfoHeader.Height-1 || col == 0 || col == InfoHeader.Width-1){
        //sets the border of the bitmap equal to the original pixels to avoid going
        //out of bounds from the loop below
        tempBMP[row][col]->Red = inBMP[row][col] -> Red;
        tempBMP[row][col]->Green = inBMP[row][col] -> Green;
        tempBMP[row][col]->Blue = inBMP[row][col] -> Blue;
      } else {
          //sets the pixel to itself if the row and col are out of bounds
          for(h = 0; h < 3; h++){
            for(w = 0; w < 3; w++){
              //increments through each pixel, and applies the filter to the pixels
              //surrounded the selected pixel by selecting through the bounds of the
              //given matrix filter
              tempBMP[row][col]->Red = tempBMP[row][col]->Red + inBMP[row+h-1][col+w-1]->Red*Matrix[h][w];
              tempBMP[row][col]->Green = tempBMP[row][col]->Green + inBMP[row+h-1][col+w-1]->Green*Matrix[h][w];
              tempBMP[row][col]->Blue = tempBMP[row][col]->Blue + inBMP[row+h-1][col+w-1]->Blue*Matrix[h][w];
            }
          }

          //make sure the pixels are within the correct color range using ternary operations
          tempBMP[row][col]->Red = (tempBMP[row][col]->Red < 0) ? 0 :
                ((tempBMP[row][col]->Red > 255) ? 255 : tempBMP[row][col]->Red );
          tempBMP[row][col]->Green = (tempBMP[row][col]->Green < 0) ? 0 :
                ((tempBMP[row][col]->Green > 255) ? 255 : tempBMP[row][col]->Green );
          tempBMP[row][col]->Blue = (tempBMP[row][col]->Blue < 0) ? 0 :
                ((tempBMP[row][col]->Blue > 255) ? 255 : tempBMP[row][col]->Blue );
      }
      //writes the resulting pixel thats been set in the previous if statement
      // to the output edge file
      fwrite(&tempBMP[row][col]->Blue, 1, 1, outputFile);
      fwrite(&tempBMP[row][col]->Green, 1, 1, outputFile);
      fwrite(&tempBMP[row][col]->Red, 1, 1, outputFile);
    } // end of col for loop
  } // end of row for loop

fclose(outputFile);
}

//------------------------------------------------------------------------------

//this function takes in the filled input bitmap and changes the shade of it
//using the command line user input colors
void shadeIMAGE(PIXEL ***inBMP,PIXEL ***outBMP, char *outfileName){
  FILE *output;
  char name[20] = {'\0'};
  int row, col;
  short int tempR, tempG, tempB;
  strcpy(name, outfileName);
  strcat(name,"(shade).bmp");
  output = fopen(name, "wb");

  /* place the header information into output file */
  fwrite(&Header, 14, 1, output);
  fwrite(&InfoHeader, 40, 1, output);

  /* for loop to go through each pixel and adjust it by user entered shade */

  for(row = 0; row < InfoHeader.Height; row++){
    for(col = 0; col < InfoHeader.Width; col++){

      // sets temp values equal to the new color to change the output pixel to
      tempR = inBMP[row][col] -> Red + rgb.R;
      tempG = inBMP[row][col] -> Green + rgb.G;
      tempB = inBMP[row][col] -> Blue + rgb.B;

      // checks to make sure the temp values are within bounds, if not, sets them
      tempR = (tempR < 0) ? 0 : ((tempR > 255) ? 255 : tempR );
      tempG = (tempG < 0) ? 0 : ((tempG > 255) ? 255 : tempG );
      tempB = (tempB < 0) ? 0 : ((tempB > 255) ? 255 : tempB );

      // sets output to the temporary value used to keep within bounds
      (*outBMP[row][col]).Red = tempR;
      (*outBMP[row][col]).Green = tempG;
      (*outBMP[row][col]).Blue = tempB;

      //writes the resulting shaded output bitmap to the output file
      fwrite(&outBMP[row][col] -> Blue,1,1,output);
      fwrite(&outBMP[row][col] -> Green,1,1,output);
      fwrite(&outBMP[row][col] -> Red,1,1,output);

    }
  }

fclose(output);
}

//------------------------------------------------------------------------------

/* this  function saves the user colors to a structure to access else where in the
    program */
void saveUserColor(const char *red, const char *green, const char *blue){
  int r, g, b;
  //place the r g b uer colors into integer variables
  sscanf(red, "%i", &r);
  sscanf(green, "%i", &g);
  sscanf(blue, "%i", &b);
  //checks to make sure the user input is within bounds
  if((r <-200) || (r > 200) || (g <-200) || (g > 200) || (b <-200) || (b > 200)){
    printf("Color Error. Please input colors between a range of -200 up to 200.\n");
    exit(1);
  }
  rgb.R = r;
  rgb.G = g;
  rgb.B = b;


}

//------------------------------------------------------------------------------
