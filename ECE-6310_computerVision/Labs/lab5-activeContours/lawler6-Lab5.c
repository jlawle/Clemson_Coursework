/*  John Lawler - ECE 6310 - Lab 5
 *  Active Image Contouring
 *
 *  program loads grayscale ppm image and contour points
 *      calculates sobel gradient
 *      calculates internat energies at each contour point, sums with
 *      gradient, and outputs new result. iterates 30 times
 *
 *  input: ./run imageFile.ppm initial_contours.txt
 *
 *  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char *argv[]) {
    int  ROWS, COLS, BYTES, i;
    int r, c, tr, tc;
    int j, p, row, col, trow, tcol;
    char header[300];
    unsigned char *image, *image1;
    FILE *fp;

    // sobel gradient template
    float Gx[] = {  -1.0, 0.0, 1.0,
                    -2.0, 0.0, 2.0,
                    -1.0, 0.0, 1.0,
                    };
    float Gy[] = {  1.0, 2.0, 1.0,
                    0.0, 0.0, 0.0,
                    -1.0, -2.0, -1.0,
                 };

    // Check cmd line argument size
    if (argc != 3) {
        printf("Incorrect command arguments: ./run image.ppm countours.txt\n");
        exit(0);
    }

    // Open ppm image for handling
    if ((fp = fopen(argv[1],"rb")) == NULL) {
        printf("Unable to open hawk.ppm for reading\n");
        exit(0);
    }

    // Verify it is a grayscale ppm image
    fscanf(fp, "%s %d %d %d", header, &COLS, &ROWS, &BYTES);
    if (strcmp(header, "P5") != 0  ||  BYTES != 255) {
        printf("Input image is not an 8-bit image\n");
        exit(0);
    }

    // store image data
    image = (unsigned char*)calloc(ROWS*COLS, sizeof(unsigned char));
    image1 = (unsigned char*)calloc(ROWS*COLS, sizeof(unsigned char));
    fread(image, 1, ROWS*COLS, fp);
    fclose(fp);

    // make copy for initial contour points
    for (i = 0; i < ROWS*COLS; i++)
        image1[i] = image[i];


    // Open contour text file
    if ((fp = fopen(argv[2],"r")) == NULL) {
        printf("Unable to open parenthood_gt.ppm for reading\n");
        exit(0);
    }

    // get size of contour file
    int temp, temp1;
    int controlPointCount = 0;
    while (1) {
        if (fscanf(fp, "%d %d", &temp, &temp1) != 2) break;
        controlPointCount++;
    }
    fclose(fp);

    // Store the points into float pointers
    int *cpCol, *cpRow, *tempCol, *tempRow;
    temp = 0;
    cpCol = (int *)calloc(controlPointCount+1, sizeof(int));
    cpRow = (int *)calloc(controlPointCount+1, sizeof(int));
    tempCol = (int *)calloc(controlPointCount+1, sizeof(int));
    tempRow = (int *)calloc(controlPointCount+1, sizeof(int));
    if ((fp= fopen(argv[2],"r")) == NULL) {
        printf("Unable to open parenthood_gt.ppm for reading\n");
        exit(0);
      }

    // Place contour points into an array
    while (1) {
        if (fscanf(fp, "%d %d", &cpCol[temp], &cpRow[temp]) != 2) break;
        temp++;
    }
    fclose(fp);

    // paint crosses on original image
    for (i = 0; i < controlPointCount; i++) {
        for (r = -3; r <= 3; r++)
            for (c = -3; c <= 3; c ++) {
                if (r == 0) image1[COLS*(cpRow[i]+r) + (cpCol[i]+c)] = 0;
                if (c == 0) image1[COLS*(cpRow[i]+r) + (cpCol[i]+c)] = 0;
            }
    }



    // write intial contoured image to file
    fp = fopen("Image-InitialContours.ppm", "w");
    fprintf(fp,"P5 %d %d 255\n", COLS, ROWS);
    fwrite(image1, 1, ROWS*COLS, fp);
    fclose(fp);

    // add first cp onto end of array
    cpCol[controlPointCount] = cpCol[0];
    cpRow[controlPointCount] = cpRow[0];

    /* calculate the average distance between points */
    int sobLen = 1;

    // create a temporary image pointer
    float sopx, sopy; // sobel operator
    float min = 0, max = 0;
    float *sobel, *extE, *sobelx, *sobely, *tempWin, *tempWin2;
    sobel   = (float*)calloc(ROWS*COLS, sizeof(float));
    sobelx   = (float*)calloc(ROWS*COLS, sizeof(float));
    sobely   = (float*)calloc(ROWS*COLS, sizeof(float));
    extE    = (float*)calloc(ROWS*COLS, sizeof(float));
    tempWin  = (float*)calloc(7*7, sizeof(float));

    // start at the len of the sobel matrix out from sides
    for (r = sobLen; r <= ROWS-sobLen; r++)
        for (c = sobLen; c <= COLS-sobLen; c++) {
            sopx = sopy = 0;

            // loop around every pixel to find x gradient
            for (tr = -sobLen; tr <= sobLen; tr++)
                for (tc = -sobLen; tc <= sobLen; tc++){
                    sopx += image[COLS*(r+tr) + (c+tc)]*Gx[(sobLen*2+1)*(tr+sobLen) + (tc+sobLen)];
                }

            // loop around every pixel to find y gradient
            for (tr = -sobLen; tr <= sobLen; tr++)
                for (tc = -sobLen; tc <= sobLen; tc++){
                    sopy += image[COLS*(r+tr) + (c+tc)]*Gy[(sobLen*2+1)*(tr+sobLen) + (tc+sobLen)];
                }
            sobelx[COLS*r + c] = sopx;
            sobely[COLS*r +c] = sopy;

            // Calculate total gradient magnitude
 //           sobel[COLS*r + c] = sqrt(pow(sopx,2) + pow(sopy,2));
        }

    /* Calculate G and min/max */
   for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c ++) {
            /* Calculate overall gradient */
            sobel[COLS*r+c] = sqrt(pow(sobelx[COLS*r+c],2) + pow(sobely[COLS*r+c],2));
            temp = sqrt(pow(sobelx[COLS*r+c],2) + pow(sobely[COLS*r+c],2));
            if (r == 0 && c == 0)
                min = max = sobel[COLS*r+c];
            if ( sobel[COLS*r+c] < min)
                    min = sobel[COLS*r+c];
            if (sobel[COLS*r+c] > max)
                    max = sobel[COLS*r+c];

        }

    /* Debug min and max */
    printf("\nmin %f \n max %f \n", min, max);

    /* Sobel Gradient output Image */
    unsigned char *sobelOutput;
    sobelOutput = (unsigned char*)calloc(ROWS*COLS, sizeof(unsigned char));
    for (i = 0; i < ROWS*COLS; i++)
        *(sobelOutput + i) =(unsigned char)(((sqrt(pow(sobelx[i],2) + pow(sobely[i],2)) - min)*255) /(max - min));

    // Write normalized sobel gradient out
    fp = fopen("sobelGradientImage.ppm", "w");
    fprintf(fp,"P5 %d %d 255\n", COLS, ROWS);
    fwrite(sobelOutput, 1, ROWS*COLS, fp);
    fclose(fp);


    /* Negate sobel gradient -> external energy */
    min = max = 0;
    for (i = 0; i < ROWS*COLS; i++){
        *(extE + i) = sobel[i] * (-1);
        if (*(extE + i) > max) max = *(extE + i);
        if (*(extE + i) < min) min = *(extE + i);
    }


    /* Normalize extE between 0 -> 1 */
    for (i = 0; i < ROWS*COLS; i++){
        *(extE + i) = ((extE[i] - min))/(max - min);
    }


    float dist, dist2, sum, avgDist;
    float *tempWin3; // used for storing sums of 3 energies

    // Calculate new contour points 30 times
    for (j = 0; j < 30; j++) {

        tempCol = (int *)calloc(controlPointCount+1, sizeof(int));
        tempRow = (int *)calloc(controlPointCount+1, sizeof(int));

        /* find average distance of points */
        for (i = 0; i < controlPointCount; i++){
            dist = sqrt(pow((cpCol[i] - cpCol[i+1]),2) + pow((cpRow[i] - cpRow[i+1]), 2));
            sum += dist;
        }


        avgDist= sum/controlPointCount;
        printf("avgDist = %f \n", avgDist);

        // Calculate internal energies at each cp
        for (p = 0; p < controlPointCount; p++){
            tempWin  = (float*)calloc(7*7, sizeof(float)); /* internalE 1*/
            tempWin2  = (float*)calloc(7*7, sizeof(float));
            tempWin3  = (float*)calloc(7*7, sizeof(float));
            row = cpRow[p];
            col = cpCol[p];

            // Internal energy 1
            for (r = -3; r <= 3; r ++)
                for (c = -3; c <= 3; c ++) {
                    dist2 = pow(((cpCol[p]+c) - cpCol[p+1]),2) + pow(((cpRow[p]+r) - cpRow[p+1]), 2);
                    tempWin[7*(r+3) + (c+3)] = dist2;
                }

            // Internal E 1 min and max
            min = max = dist2;
            for (i = 0; i < 7*7; i++){
                if (*(tempWin + i) > max) max = *(tempWin + i);
                if (*(tempWin + i) < min) min = *(tempWin + i);
            }

            /* Normalize internal energy 1 */
            for (i = 0; i < 7*7; i++)
                *(tempWin + i) = ((tempWin[i] - min))/(max - min);


            // Find internal energy 2 (deviation)
            for (r = -3; r <= 3; r ++)
                for (c = -3; c <= 3; c ++) {
                    dist = sqrt(pow(((cpCol[p]+c) - cpCol[p+1]),2) + pow(((cpRow[p]+r) - cpRow[p+1]), 2));
                    tempWin2[7*(r+3) + (c+3)] = pow((dist - avgDist),2);
                }

            // Calcuate min and max of int2
            min = max = dist;
            for (i = 0; i < 7*7; i++){
                if (*(tempWin2 + i) > max) max = *(tempWin2 + i);
                if (*(tempWin2 + i) < min) min = *(tempWin2 + i);
            }

            /* Normalize internal energy 2 */
            for (i = 0; i < 7*7; i++){
                *(tempWin2 + i) = ((tempWin2[i] - min))/(max - min);
            }

            // Add energies together
            min = 3;
            for (r = -3; r <= 3; r++) {
                for (c = -3; c <= 3; c ++) {
                    tempWin3[7*(r+3)+(c+3)] = tempWin[7*(r+3)+(c+3)] + tempWin2[7*(r+3) + (c+3)] + extE[COLS*(row + r) + (col + c)];

                    // Find minimum energy
                    if (tempWin3[7*(r+3) + (c+3)] < min) {
                        min = tempWin3[7*(r+3)+(c+3)];
                        trow = row + r;
                        tcol = col + c;
                    }
                }
            }
                // set temp col and row after found
                tempRow[p] = trow;
                tempCol[p] = tcol;
        } /* end of c.p. loop */

        // set tempRow and tempCol equal to cpRow abd cpCol
        for (i = 0; i < controlPointCount; i++){
            cpRow[i] = tempRow[i];
            cpCol[i] = tempCol[i];
        }

        cpRow[controlPointCount] = cpRow[0];
        cpCol[controlPointCount] = cpCol[0];
    } /* End of 30 iterations */

    // print final control points
    for (i = 0; i < controlPointCount; i++){
        printf ("%d. Col,Row -> %d,%d \n",i+1, cpCol[i], cpRow[i]);
    }

    // paint crosses on new image
    for (i = 0; i < controlPointCount; i++) {
        for (r = -3; r <= 3; r++)
            for (c = -3; c <= 3; c ++) {
                if (r == 0) image[COLS*(cpRow[i]+r) + (cpCol[i]+c)] = 0;
                if (c == 0) image[COLS*(cpRow[i]+r) + (cpCol[i]+c)] = 0;
            }
    }

    // write intial contoured image to file
    fp = fopen("Image-FinalContours.ppm", "w");
    fprintf(fp,"P5 %d %d 255\n", COLS, ROWS);
    fwrite(image, 1, ROWS*COLS, fp);
    fclose(fp);
}
