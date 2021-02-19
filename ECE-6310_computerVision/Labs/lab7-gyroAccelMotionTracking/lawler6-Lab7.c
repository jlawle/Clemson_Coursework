/*  John Lawler - ECE 6310 - Lab 7
 *  Motion tracking Gyrscope and Accelerometer data
 *  Iphone movements
 *
 *  This lab takes in gyro and accelerometer movements and convert
 *  rad/s -> deg 
 *  G/s^2 -> distance
 *
 *  finds periods of motion in data given 2 Thresholds for 
 *  gyro and accelerometer.  
 *
 * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// use multiple of total lines = 1250
#define WNDSZ 5

int main (){
    char tempc[3000];
    int i;
    FILE *fp;

    // Text file data
    int samples = 0;                // # of data entries in txtfile
    float *time;              // time is increments of 0.05
    float *accX, *accY, *accZ;      // accel axes
    float *pitch, *roll, *yaw;      // gyro axes

    // Open data text file for reading
    if ((fp = fopen("acc_gyro.txt","r")) == NULL) {
        printf("Unable to open text data for reading\n");
        exit(0);
    }

    // Skip first line of text
    i = fscanf(fp, "%[^\n]\n", tempc);

    // get length of text file
    while (1) {
        if (fscanf(fp, "%[^\n]\n", tempc) != 1) break;
        samples++;
    }
    fclose(fp);
    
    // Number of lines 
    printf("%d %d # of wnds: %d \n", i, samples, samples / WNDSZ);   

    // Allocate memory for each 
    time    = (float *)calloc(samples, sizeof(float));
    accX    = (float *)calloc(samples, sizeof(float));
    accY    = (float *)calloc(samples, sizeof(float));
    accZ    = (float *)calloc(samples, sizeof(float));
    yaw     = (float *)calloc(samples, sizeof(float));
    pitch   = (float *)calloc(samples, sizeof(float));
    roll    = (float *)calloc(samples, sizeof(float));

    
    // Fill arrays
    if ((fp = fopen("acc_gyro.txt","r")) == NULL) {
        printf("Unable to open text data for reading\n");
        exit(0);
    }

    // Skip first line of text
    fscanf(fp, "%[^\n]\n", tempc);

    i = 0;
    while (1) {
        if (fscanf(fp, "%f %f %f %f %f %f %f", 
            &time[i], &accX[i], &accY[i], &accZ[i], 
            &pitch[i], &roll[i], &yaw[i]) != 7) break;
        i++;
    }
    fclose(fp);


    // Create variance arrays
    int wndCount;
    float *varX, *varY, *varZ, *varP, *varR, *varYw, *initTime;

    // Check for remainders and add extra window
    if (samples % WNDSZ) wndCount = (samples / WNDSZ) + 1;
    else wndCount = samples / WNDSZ; 

    initTime= (float *)calloc(wndCount, sizeof(float));
    varX    = (float *)calloc(wndCount, sizeof(float));
    varY    = (float *)calloc(wndCount, sizeof(float));
    varZ    = (float *)calloc(wndCount, sizeof(float));
    varP    = (float *)calloc(wndCount, sizeof(float));
    varR    = (float *)calloc(wndCount, sizeof(float));
    varYw   = (float *)calloc(wndCount, sizeof(float));

    float sum, avg;
    int varCount=0, j;

    // Calculate variance at each window
    printf("Time\t\tX\tY\t\tZ\tP\t\tR\tY\n");
    for (i = 0; i < samples; i+=WNDSZ){
        if (varCount >= wndCount) break;
        sum = avg = 0;

        // Save initial time of each period's starting time
        initTime[varCount] = time[i];

        /* Calculate the variances for each axis */
        // X axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += accX[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(accX[i+j] - avg, 2);
        varX[varCount] = sum / (WNDSZ - 1);

        // Y axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += accY[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(accY[i+j] - avg, 2);
        varY[varCount] = sum / (WNDSZ - 1);

        // Z axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += accZ[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(accZ[i+j] - avg, 2);
        varZ[varCount] = sum / (WNDSZ - 1);

        // Pitch axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += pitch[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(pitch[i+j] - avg, 2);
        varP[varCount] = sum / (WNDSZ - 1);

        // Roll axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += roll[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(roll[i+j] - avg, 2);
        varR[varCount] = sum / (WNDSZ - 1);

        // Yaw axis
        sum = avg = 0;
        for (j = 0; j < WNDSZ; j++) sum += yaw[i+j];
        avg = sum / WNDSZ;
        sum = 0;
        for (j = 0; j < WNDSZ; j++) 
            sum += pow(yaw[i+j] - avg, 2);
        varYw[varCount] = sum / (WNDSZ - 1);


        // print to a csv file to find good thresholds in excel
        printf("%f, %0.8f, %0.8f, %0.8f, %0.8f, %0.8f, %0.8f, \n", 
                initTime[varCount], varX[varCount], varY[varCount], varZ[varCount], 
                 varP[varCount], varR[varCount], varYw[varCount]);
        varCount++;
    } /* end of variance for-loop */


    // seperate gyro and acc thresholds
    float accThr = 0.001;
    float gyroThr = 0.01;
    float VavgX, VavgY, VavgZ;
    float Period = time[0] * WNDSZ;
    float *velX, *velY, *velZ, distX, distY, distZ;
    velX    = (float *)calloc(samples, sizeof(float));
    velY    = (float *)calloc(samples, sizeof(float));
    velZ    = (float *)calloc(samples, sizeof(float));

    int motion = 0;
    int detected = 0;
    float radDeg = 57.22958;
    float degP, degR, degY;
    degP = degR = degY = 0;
    
    // Loop through variance list and find motion for each point
    printf("Count  Time Detected\t  X\t \t Y\t\t Z\t\tPitch\t\tYaw\t\tRoll \n");
    for (i = 0; i < wndCount; i++){

        // check for motion
        if (varX[i] > accThr || varY[i] > accThr || varZ[i] > accThr
            || varP[i] > gyroThr || varR[i] > gyroThr || varYw[i] > gyroThr) {

            // Count motion if previous sample exhibited no motion
            if (i != 0){
                if (motion == 0){ 
                    detected++;
                    velX[i*WNDSZ + j - 1] = 0;
                    velY[i*WNDSZ + j - 1] = 0;
                    velZ[i*WNDSZ + j - 1] = 0;
                }
                motion = 1;
            }

            /* Calculate gyro motion */

            // find middle value to perform integration
            for(j = 0; j < WNDSZ; j++){

                // Integrate gyro data rad/s -> rads
                 degP += 0.05 * pitch[i*WNDSZ + j];
                 degR += 0.05 * roll[i*WNDSZ + j];
                 degY += 0.05 * yaw[i*WNDSZ + j];
                
                // find velocity
                velX[i*WNDSZ + j] = 0.05 * (9.81*(accX[i*WNDSZ+j]-accX[i*WNDSZ]));
                velY[i*WNDSZ + j] = 0.05 * (9.81*(accY[i*WNDSZ+j]-accY[i*WNDSZ]));
                velZ[i*WNDSZ + j] = 0.05 * (9.81*(accZ[i*WNDSZ+j]-accZ[i*WNDSZ]));
            }
        
            // Calculate cumlative distance
            VavgX = (velX[i*WNDSZ-1] + velX[i*WNDSZ + WNDSZ - 1]) / 2;
            VavgY = (velY[i*WNDSZ-1] + velY[i*WNDSZ + WNDSZ - 1]) / 2;
            VavgZ = (velZ[i*WNDSZ-1] + velZ[i*WNDSZ + WNDSZ - 1]) / 2;
            distX += VavgX * Period;
            distY += VavgY * Period;
            distZ += VavgZ * Period;


        } else {
            if (motion ){
                // If end of period of motion, print values
                printf(" %d\t%0.2f", detected, time[i*WNDSZ]);
                printf("\t\t%f\t%f\t%f\t%f\t%f\t%f\n", 1000*distX, 1000*distY, 1000*distZ, degP*radDeg, degY*radDeg, degR*radDeg);
                distX = distY = distZ = degP = degY = degR = 0;
                motion = 0;

            }
            // no motion detected
        }
    }
    printf("\n Detected motion at %d spots\n",detected);
}
