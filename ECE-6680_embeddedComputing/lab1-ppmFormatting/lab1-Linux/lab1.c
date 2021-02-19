/* ECE 6680 - John Lawler - Lab 1
    takes 8 bit grayscale image -> 16 bit RGB
    takes 24 bit RGB image -> 16 bit RGB
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
FILE    *fpt;
Display			*Monitor;
Window			ImageWindow;
GC				ImageGC;
XWindowAttributes		Atts;
XImage			*Picture;
int				count,i, ROWS,COLS;
char      header[80];
unsigned char		*image, *displaydata;

// ./a.out filename.ppm
if (argc != 2){
  printf("argument error, incorrect number");
  exit(0);
}
// open the file

fpt=fopen(argv[1], "rb");
if (fpt == NULL){
  printf("Unable to open %s for reading\n",argv[1]);
  exit(0);
}

int BYTES;
i = fscanf(fpt, "%s %d %d %d ", header, &COLS, &ROWS, &BYTES);
if (i != 4){
  fclose(fpt);
  exit(0);
}

unsigned char a, b;

if (strcmp(header, "P5") == 0) {
  // Format file for grayscale shifting 8 bit to 16
  image = (unsigned char*)calloc(ROWS*COLS, sizeof(unsigned char));
  // read in Image
  fread(image, 1, ROWS*COLS, fpt);
  fclose(fpt);
  displaydata = (unsigned char*)calloc(ROWS*COLS*2, sizeof(unsigned char));
  // loop through size of file datat

  for (i = 0; i < ROWS*COLS; i++){
    a = b = 0;
    a = image[i] & 0xE0; // saving MS green
    a = a >> 5;
    a = a & 0x07;
    b = image[i] & 0xF8; // red bits
    // Set the 5 Red bits and 3 most sig. green bits
    displaydata[i*2 + 1] = b | a;

    a = b = 0;
    a = image[i] & 0x1C; // select lest sig 3 green bits
    a = a << 3; // place green to most sig 3 bits
    a = a & 0xE0; // clear potential bottom bits
    b = image[i] & 0xF8; // obtain the blue bits
    b = b >> 3;
    b = b & 0x1F;
    // set lest sig green to top 3, then blue the rest
    displaydata[i*2] = a | b;
}

} else if (strcmp(header, "P6") == 0) {
  // format for 24 bit to 16
  image = (unsigned char*)calloc(ROWS*COLS*3, sizeof(unsigned char));
  // read in Image
  fread(image, 1, ROWS*COLS*3, fpt);
  fclose(fpt);

  displaydata = (unsigned char*)calloc(ROWS*COLS*2, sizeof(unsigned char));
    count = 0;
    for (i = 0; i < ROWS*COLS*3; i+=3){
      a=b=0;
      a = image[i] & 0xF8; // save red 5 MS bits
      b = image[i + 1] & 0xE0; // save MS 3 green bits
      b = b >> 5;
      b = b & 0x07; // clear unused bits

      // Set the 5 Red bits and 3 most sig. green bits
      displaydata[2*count + 1] = a | b;
      a=b=0;

      a = image[i + 1] & 0x1C; // select LS 3 green bits
      a = a << 3; // place green to most sig 3 bits
      a = a & 0xE0; // clear potential bottom bits


      b = image[i + 2] & 0xF8; // set the blue bits
      b = b >> 3;
      b = b & 0x1F;
      displaydata[2*count] = a | b;
      count++;
    }

} else {
  // wrong file type
  exit(0);
}

if (displaydata == NULL) {
  printf("unable to allocate data for image");
  exit(0);
}

/* ... */

Monitor=XOpenDisplay(NULL);
if (Monitor == NULL)
  {
  printf("Unable to open graphics display\n");
  exit(0);
  }

ImageWindow=XCreateSimpleWindow(Monitor,RootWindow(Monitor,0),
	50,10,		/* x,y on screen */
	COLS,ROWS,	/* width, height */
	2, 		/* border width */
	BlackPixel(Monitor,0),
	WhitePixel(Monitor,0));

ImageGC=XCreateGC(Monitor,ImageWindow,0,NULL);

XMapWindow(Monitor,ImageWindow);
XFlush(Monitor);
while(1)
  {
  XGetWindowAttributes(Monitor,ImageWindow,&Atts);
  if (Atts.map_state == IsViewable /* 2 */)
    break;
  }

Picture=XCreateImage(Monitor,DefaultVisual(Monitor,0),
		DefaultDepth(Monitor,0),
		ZPixmap,	/* format */
		0,		/* offset */
		displaydata,/* the data */
		COLS,ROWS,	/* size of the image data */
		16,		/* pixel quantum (8, 16 or 32) */
		0);		/* bytes per line (0 causes compute) */

XPutImage(Monitor,ImageWindow,ImageGC,Picture,
		0,0,0,0,	/* src x,y and dest x,y offsets */
		COLS,ROWS);	/* size of the image data */

XFlush(Monitor);
sleep(5);
XCloseDisplay(Monitor);
return 0;
}
