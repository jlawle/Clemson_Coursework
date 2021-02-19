/* John Lawler ECE 6310 Lab 8 - Range Segmentation
 *
 * This lab takes in a given 128*128 range image then
 * calculates surface normals via cross product to find 
 * varying sides of the image. It colors in each side of the chair
 * with varying grayscale values.
 *
 * Implementation of Region Growing using queues
 *
 * */



#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_QUEUE 10000
#define ROWS	128
#define COLS	128
#define SZ      ROWS*COLS

/*
**	This routine converts the data in an Odetics range image into 3D
**	cartesian coordinate data.  The range image is 8-bit, and comes
**	already separated from the intensity image.
*/
// Compute and return angle difference
double dotProd(double ax, double ay, double az, double bx, double by, double bz);

int main(int argc, char *argv[]) {
    int	            tr, tc,r,c, i;
    double	        cp[7];
    double	        xangle, yangle, dist;
    double	        ScanDirectionFlag, SlantCorrection;
    unsigned char	RangeImage[ROWS*COLS], *ThreshImage, *region;
    double		    P[3][ROWS*COLS], SfNm[3][ROWS*COLS];
    int             ImageTypeFlag;
    char	        Filename[160],Outfile[160], header[300];
    FILE	        *fpt;
    int             temp;
    void            RegionGrow();
    
    printf("Enter range image file name: ");
    scanf("%s",Filename);

    if ((fpt = fopen(Filename, "r")) == NULL) {
      printf("Couldn't open %s\n", Filename);
      exit(0);
    }
    fscanf(fpt, "%s %d %d %d ", header, &temp, &temp, &temp);

    fread(RangeImage, 1, ROWS*COLS, fpt);
    fclose(fpt);
    
    printf("Up(-1), Down(1) or Neither(0)? ");
    scanf("%d", &ImageTypeFlag);
    
    // Calloc the thresholded image
    ThreshImage = (unsigned char*)calloc(SZ, sizeof(unsigned char));
    region = (unsigned char*)calloc(SZ, sizeof(unsigned char));
    
    // Threshold Image
    int threshold = 130;
    for (i = 0; i < SZ; i++) {
        if (RangeImage[i] < threshold && RangeImage[i] > 20) ThreshImage[i] = 0;
        else ThreshImage[i] = 255;
    }
    
    // write threshold as output
    fpt = fopen("threshOutput.ppm","w");
    fprintf(fpt, "P5 %d %d 255\n", COLS, ROWS);
    fwrite(ThreshImage, 1, ROWS*COLS, fpt);
    fclose(fpt);
    
    
    
    cp[0]=1220.7;		/* horizontal mirror angular velocity in rpm */
    cp[1]=32.0;		/* scan time per single pixel in microseconds */
    cp[2]=(COLS/2)-0.5;		/* middle value of columns */
    cp[3]=1220.7/192.0;	/* vertical mirror angular velocity in rpm */
    cp[4]=6.14;		/* scan time (with retrace) per line in milliseconds */
    cp[5]=(ROWS/2)-0.5;		/* middle value of rows */
    cp[6]=10.0;		/* standoff distance in range units (3.66cm per r.u.) */
    
    cp[0]=cp[0]*3.1415927/30.0;	/* convert rpm to rad/sec */
    cp[3]=cp[3]*3.1415927/30.0;	/* convert rpm to rad/sec */
    cp[0]=2.0*cp[0];		/* beam ang. vel. is twice mirror ang. vel. */
    cp[3]=2.0*cp[3];		/* beam ang. vel. is twice mirror ang. vel. */
    cp[1]/=1000000.0;		/* units are microseconds : 10^-6 */
    cp[4]/=1000.0;			/* units are milliseconds : 10^-3 */
    
    switch(ImageTypeFlag)
      {
      case 1:		/* Odetics image -- scan direction upward */
        ScanDirectionFlag=-1;
        break;
      case 0:		/* Odetics image -- scan direction downward */
        ScanDirectionFlag=1;
        break;
      default:		/* in case we want to do this on synthetic model */
        ScanDirectionFlag=0;
        break;
      }
    
    	/* start with semi-spherical coordinates from laser-range-finder: */
    	/*			(r,c,RangeImage[r*COLS+c])		                    */
    	/* convert those to axis-independant spherical coordinates:	    */
    	/*			(xangle,yangle,dist)			                    */
    	/* then convert the spherical coordinates to cartesian:         */
    	/*			(P => X[] Y[] Z[])			                        */
    
    if (ImageTypeFlag != 3) {
      for (r=0; r<ROWS; r++) {
        for (c=0; c<COLS; c++) {
          SlantCorrection = cp[3]*cp[1]*((double)c - cp[2]);
          xangle = cp[0] * cp[1] * ((double)c - cp[2]);
          yangle = (cp[3]*cp[4]*(cp[5]-(double)r)) +	/* Standard Transform Part */ 
              SlantCorrection*ScanDirectionFlag;	    /*  + slant correction */

          dist = (double)RangeImage[r*COLS+c] + cp[6];
          P[2][r*COLS+c] = sqrt((dist*dist) / 
                  (1.0+(tan(xangle) * tan(xangle)) + (tan(yangle) * tan(yangle))));
          P[0][r*COLS+c] = tan(xangle) * P[2][r*COLS+c];
          P[1][r*COLS+c] = tan(yangle) * P[2][r*COLS+c];
          }
        }
      }
    
    SfNm[0][0] = 0;
    SfNm[1][0] = 0;
    SfNm[2][0] = 0;

    double ax, ay, az, bx, by, bz;
    ax=ay=az=bx=by=bz=cx=cy=cz=0;
    int wnd = 3;

    // Calculate the Surface Normals
    if (ImageTypeFlag != 3) {
        // Calculates the cross product of two 3D coords A and B 
        // relative to the currently selected pixel coords, X
        for (r = 0; r < ROWS-wnd; r++)
            for(c = 0; c < COLS-wnd; c++){
                // (b - x) * (a - x)
                ax = P[0][(r + wnd)*COLS + c] - P[0][r*COLS + c];
                ay = P[1][(r + wnd)*COLS + c] - P[1][r*COLS + c];
                az = P[2][(r + wnd)*COLS + c] - P[2][r*COLS + c];
                bx = P[0][r*COLS + (c + wnd)] - P[0][r*COLS + c];
                by = P[1][r*COLS + (c + wnd)] - P[1][r*COLS + c];
                bz = P[2][r*COLS + (c + wnd)] - P[2][r*COLS + c];
                SfNm[0][r*COLS + c] = ay*bz - az*by;
                SfNm[1][r*COLS + c] = az*bx - ax*bz;
                SfNm[2][r*COLS + c] = ax*by - ay*bx;
        }
    }

    // Fill region
    int search= 1;
    int regionLabel = 0;
    int gR, gC;
    int *indicies, count;
    indicies = (int *)calloc(SZ, sizeof(int));
    
    // Perform until no more regions to seed

    int cnt = 1;
    int regionCount = 0;
    // Print Table header
    printf("\n# Pixels | Avg Sfnm:\tX\t\tY\t\tZ\n");
    int wndSr = 2;

        // check in 5 x 5 window through theshold image and region for distinctiona
        for (r = wndSr; r < ROWS - wndSr; r ++) {
            for (c = wndSr; c < COLS - wndSr; c ++) {
                cnt = 0;
                // 5 x 5 window 
                if (search) {
                    for (tr = -wndSr; tr <= wndSr; tr ++){
                        for (tc = -wndSr; tc <= wndSr; tc++){
                            if ((ThreshImage[(r + tr)*COLS + (c + tc)] == 0) 
                            && region[(r + tr)*COLS + (c + tc)] == 0)  
                               cnt++;
                        }
                    }
                }
                
                // if counted 25 adequate pixels
                if (cnt > 24) {
                    gR = r;
                    gC = c;
                    regionCount++;
                    regionLabel+=50;
                    if (regionLabel >= 255) regionLabel = 20;
                    RegionGrow(ThreshImage, SfNm, RangeImage, region, gR, gC, 0, regionLabel, indicies, &count);

                    // print results
                    if (count < 20) {
                        for (i = 0; i < count; i++){
                            region[indicies[i]] = 0;
                        }
                    }
            }
        }
    }


    // write threshold as output
    fpt = fopen("segments.ppm","w");
    fprintf(fpt, "P5 %d %d 255\n", COLS, ROWS);
    fwrite(region, 1, ROWS*COLS, fpt);
    fclose(fpt);

    sprintf(Outfile,"%s.coords", Filename);
    fpt=fopen(Outfile,"w");
    fwrite(P[0],8,128*128,fpt);
    fwrite(P[1],8,128*128,fpt);
    fwrite(P[2],8,128*128,fpt);
    fclose(fpt);
}

// Function to grow regions inside image based on surface orientation
void RegionGrow( unsigned char *ThreshImage,
    double SfNm[3][ROWS*COLS],
    unsigned char *image,	/* image data */
  unsigned char *labels,	/* segmentation labels */
  int r,int c,		/* pixel to paint from */
  int paint_over_label,	/* image label to paint over */
  int new_label,		/* image label for painting */
  int *indices,		/* output:  indices of pixels painted */
  int *count)		/* output:  count of pixels painted */
{
    int	r2,c2, ri, ci;
    int	queue[MAX_QUEUE],qh,qt;
    double	avgx, avgy, avgz;
    double totalx, totaly, totalz;	/* average and total intensity in growing region */
    
    *count=0;
    
    if (labels[r*COLS+c] != paint_over_label) return;
    
    // start averages on intial point
    avgx = totalx = SfNm[0][r*COLS + c];
    avgy = totaly = SfNm[1][r*COLS + c];
    avgz = totalz = SfNm[2][r*COLS + c];
    labels[r*COLS+c] = new_label;
    if (indices != NULL)
    indices[0] = r*COLS+c;
    queue[0] = r*COLS+c;
    qh=1;	/* queue head */
    qt=0;	/* queue tail */
    (*count)=1;
    while (qt != qh) {
    /* recalculate average after each pixel joins*/
    if ((*count)%1 == 0) {
        avgx = totalx/(*count);
        avgy = totaly/(*count);
        avgz = totalz/(*count);
    }

    // loop through the 8 pixels around selected pixel
    for (r2=-1; r2<=1; r2++)
      for (c2=-1; c2<=1; c2++) {
        ri = queue[qt]/COLS+r2;
        ci = queue[qt]%COLS+c2;


        if (r2 == 0  &&  c2 == 0)
          continue;
        if ((queue[qt]/COLS+r2) < 0  ||  (queue[qt]/COLS+r2) >= ROWS  ||
      (queue[qt]%COLS+c2) < 0  ||  (queue[qt]%COLS+c2) >= COLS)
          continue;
        if (labels[ri*COLS+ci] != paint_over_label)
          continue;
      /* test criteria to join region */
        if ( (ThreshImage[ri*COLS + ci] != 0)
          || dotProd(avgx, avgy, avgz, SfNm[0][ri*COLS + ci],SfNm[1][ri*COLS + ci],SfNm[2][ri*COLS + ci]) >= 0.9)
          continue;
        labels[ri*COLS + ci] = new_label;
        if (indices != NULL)
          indices[*count] = (queue[qt]/COLS + r2)*COLS + queue[qt]%COLS + c2;
        // Re add to totals
        totalx += SfNm[0][ri*COLS + ci];
        totaly += SfNm[1][ri*COLS + ci];
        totalz += SfNm[2][ri*COLS + ci];

        (*count)++;
        queue[qh] = (queue[qt]/COLS+r2)*COLS+queue[qt]%COLS+c2;
        qh=(qh+1)%MAX_QUEUE;
        if (qh == qt) {
          printf("Max queue size exceeded\n");
          exit(0);
          }
        }
    qt=(qt+1)%MAX_QUEUE;
    }

    // print averages
    if (*count > 20)
    printf(" %d\t\t\t%lf\t%lf\t%lf\n",*count, avgx, avgy, avgz);


}

// Find dot product of two given vector coordinates, return angle difference
double dotProd(double ax, double ay, double az, double bx, double by, double bz){
    double val;
    double avgDist, aDist;
    val = ax*bx + ay*by + az*bz;
    avgDist = sqrt(pow(bx,2) + pow(by,2) + pow(bx,2));
    aDist = sqrt(pow(ax,2) + pow(ay,2) + pow(az,2));
    val = acos(val / (avgDist*aDist));
    return abs(val);
}



