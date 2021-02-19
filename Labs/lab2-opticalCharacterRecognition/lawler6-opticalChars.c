/*  John Lawler - ECE 6310 - Lab 2
 *  Optical Character recognition 
 *
 *  This program implements matched spatial filtering to select a specified 
 *  grouping of letters from a given text image. Currently, it is used to find
 *  all of the "e"s in the given image. 
 *
 *  It takes in a template image of a letter, calculates the zero-mean center 
 *      - sum pixels and divide by area, then subtract mean from every pixel
 *
 *  The zmc is convolved with a given input image with text to create an msf.
 *  This msf must be normalized to 8 bits, i.e. the max and min pixel are found
 *  and all the pixels are scaled within 0-255 relative to max and min
 *
 *  Next the normalized output is converted to a binary file to select where 
 *  all possible occurences of the letter are selected above a given threshold
 *  (cmdline input). This file is checked against a ground truth file for false 
 *  positives and true positives.
 *  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int *msf, *zmc;
    int r, sum = 0;
    int TROWS, TCOLS, IROWS, ICOLS, BYTES, i;
    char header1[320], header2[320];
    unsigned char *image, *output, *template;
    FILE *imageFile, *templateFile, *gtFile, *outputFile, *binaryFile;

    // Set threshold
    int threshold = atoi(argv[1]);

    // Open ppm image for handling
    if ((imageFile = fopen("parenthood.ppm","rb")) == NULL) {
        printf("Unable to open parenthood.ppm for reading\n");
        exit(0);
      }
    fscanf(imageFile, "%s %d %d %d", header1, &ICOLS, &IROWS, &BYTES);
    if (strcmp(header1, "P5") != 0  ||  BYTES != 255) {
        printf("Input image is not an 8-bit image\n");
        exit(0);
      }

    image = (unsigned char*)calloc(IROWS*ICOLS, sizeof(unsigned char));
    fread(image, 1, IROWS*ICOLS, imageFile);
    fclose(imageFile);


    // read template
    if ((templateFile = fopen("parenthood_e_template.ppm","rb")) == NULL) {
        printf("Unable to open template for reading\n");
        exit(0);
    }
    fscanf(templateFile,"%s %d %d %d ", header2, &TCOLS, &TROWS, &BYTES);
    if (strcmp(header2, "P5") != 0  ||  BYTES != 255) {
        printf("Template is not an 8-bit image\n");
        exit(0);
    }
    template = (unsigned char*)calloc(TCOLS*TROWS, sizeof(unsigned char));
    fread(template, 1, TROWS*TCOLS, templateFile);
    fclose(templateFile);
    
    msf = (int*)calloc(IROWS*ICOLS, sizeof(int));
    zmc = (int*)calloc(TCOLS*TROWS, sizeof(int));

    // Find sum of template
    for (r = 0; r < TROWS*TCOLS; r++){
        sum += *(template + r);
    }

    // find ZMC
    for (r = 0; r < TROWS*TCOLS; r++){
        zmc[r] = *(template + r) - (sum / (TROWS*TCOLS));
    }

    // find min and max overall while msf is created
    int c, tr, tc;
    int pixelVal = 0, min = 0, max = 0;
    for (r = 7; r < IROWS-7; r++) {
        for (c = 4; c < ICOLS-4; c++) {
            pixelVal = 0; 
            // Inside loop for handling the 7x7 around indexed pixel to convolve
            for (tr = -7; tr <= 7; tr++) {
                for (tc = -4; tc <= 4; tc++)
                     pixelVal += image[(r+tr)*ICOLS + (c+tc)]*zmc[TCOLS*(tr+7) + (tc+4)];
            }
            // Deternmine min and max
            msf[ICOLS*r + c] = pixelVal;        // appropriate msf pixel value
            if (r==4 && c==7) max=min=pixelVal;
            if (pixelVal < min) min = pixelVal; // Find min out of all msf values
            if (pixelVal > max) max = pixelVal; // Find max out of all msf values
       }
    }
 
    // Normalize MSF and write to output file 
    output = (unsigned char*)calloc(IROWS*ICOLS, sizeof(unsigned char));
    for (i = 0; i < IROWS*ICOLS; i++)
        *(output + i) = (((msf[i] - min)*255) / (max - min));

    // Write normalized MSF to output file
    outputFile = fopen("msfOutputImage.ppm", "w");
    fprintf(outputFile,"P5 %d %d 255\n", ICOLS, IROWS);
    fwrite(output, 1, IROWS*ICOLS, outputFile);
    fclose(outputFile);

    // set threshold to find actual matches
    unsigned char *binaryImage;
    binaryImage = (unsigned char*)calloc(IROWS*ICOLS, sizeof(unsigned char));

    // Set threshold for new binary file
    for (i = 0; i < IROWS*ICOLS; i++)
        if (output[i] > threshold) binaryImage[i] = 255;
        else binaryImage[i] = 0;

    // Write binary image to file
    binaryFile = fopen("binaryOutput.ppm", "wb");
    fprintf(binaryFile,"P5 %d %d 255\n", ICOLS, IROWS);
    fwrite(binaryImage, 1, IROWS*ICOLS, binaryFile);
    fclose(binaryFile);
    
    // Ground truth file and verification
    int gtRow, gtCol;
    int FP = 0, TP = 0, FN = 0, TN = 0, detected;
    char gt_letter;
    if ((gtFile = fopen("parenthood_gt.txt","r")) == NULL) {
        printf("Unable to open parenthood_gt.ppm for reading\n");
        exit(0);
      }

    // Loop through groud truth file until end and verify each location
     while (1) {
        i = fscanf(gtFile, "%s %d %d", &gt_letter, &gtCol, &gtRow);
        if (i != 3) break;                  // if number of items returned is not 3, i.e end of file
        detected = 0;
        // Search for a detected pixel at each letter
        for (tr = -7; tr <= 7; tr++)
            for (tc = -4; tc <= 4; tc++) {
                if(binaryImage[ICOLS*(gtRow + tr)+(gtCol + tc)] == 255) detected = 1;
            }
        // check for FP, TP, TN, FN
        if (detected == 1) {
            if (gt_letter == 'e'){ 
                TP++;
            } else FP++;
        } else if ( gt_letter == 'e'){
            FN++;   
        } else TN++;
    }
    fclose(gtFile);

    // Print out the resulting values
    printf("\nThreshold\tFP\tTP\tFN\tTN\n");
    printf("%d\t\t%d\t%d\t%d\t%d\n", threshold, FP, TP, FN, TN);
}
