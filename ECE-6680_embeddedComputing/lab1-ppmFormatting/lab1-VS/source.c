/* ECE 6680 - John Lawler - Lab 1 - VS source code
  This program performs 8 bit and 24 bit .ppm image type to 16  bit RGB display
  This is intended for windows OS use with VS 2010
*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LRESULT CALLBACK EventProcessor (HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{ return(DefWindowProc(hWnd,uMsg,wParam,lParam)); }

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,int nCmdShow) // lpcmdlne holds aargs, no indexinng, nCmdShow is count
{
WNDCLASS		wc;
HWND			WindowHandle;
char			header[80];
int				r,g,bl,count,i,ROWS,COLS,BYTES;
unsigned char	a,b,*image,*displaydata;
BITMAPINFO		*bm_info;
HDC			hDC;
FILE      *fpt;



if (nCmdShow != 1){
  printf("argument error, incorrect number");
  exit(0);
}
// open the file

fpt=fopen(lpCmdLine, "rb");
if (fpt == NULL){
  printf("Unable to open %s for reading\n",lpCmdLine);
  exit(0);
}

i = fscanf(fpt, "%s %d %d %d ", header, &COLS, &ROWS, &BYTES);
if (i != 4){
  fclose(fpt);
  exit(0);
}


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
	// save and place 5 red bits
    a = image[i] & 0xF8;
    a = a >> 1;
    a = a & 0x7C;
	// save and place the 2 green bits
    b = image[i] & 0xC0;
	  b = b >> 6;
	  b = b & 0x03;
    displaydata[i*2 + 1] = b | a;

    a = b = 0;
    a = image[i] & 0x38; // select lest sig 3 green bits
    a = a << 2; // place green to most sig 3 bits
    a = a & 0xE0; // clear potential bottom bits
	// Save and place 5 blue bits
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

  r = 0;
  g = 1;
  bl = 2;


  displaydata = (unsigned char*)calloc(ROWS*COLS*2, sizeof(unsigned char));
    count = 0;
    for (i = 0; i < ROWS*COLS*3; i+=3){
      a=b=0;

	  // save 5 red bits
      a = image[i + r] & 0xF8;
	  a = a >> 1; // shift to position
	  a = a & 0x7C; // set bits to position, clear unwanted bits

	  // save 2 MS green bits
      b = image[i + g] & 0xC0;
      b = b >> 6;
      b = b & 0x03; // clear unused bits

      // Set the 5 Red bits and 3 most sig. green bits
      displaydata[2*count + 1] = a | b;
      a=b=0;

	  // save 3 LS green bits
      a = image[i + g] & 0x38;
      a = a << 2; // place green to most sig 3 bits
      a = a & 0xE0; // clear potential bottom bits

	  // save MS 5 blue bits
      b = image[i + bl] & 0xF8;
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

wc.style=CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc=(WNDPROC)EventProcessor;
wc.cbClsExtra=wc.cbWndExtra=0;
wc.hInstance=hInstance;
wc.hIcon=wc.lpszMenuName=NULL;
wc.hCursor=LoadCursor(NULL,IDC_ARROW);
wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
wc.lpszClassName="Image Window Class";
if (RegisterClass(&wc) == 0)
  exit(0);

WindowHandle=CreateWindow("Image Window Class",L"ECE668 Lab1",
						  WS_OVERLAPPEDWINDOW,
						  10,10,COLS,ROWS,
						  NULL,NULL,hInstance,NULL);
if (WindowHandle == NULL)
  {
  MessageBox(NULL,"No window","Try again",MB_OK | MB_APPLMODAL);
  exit(0);
  }
ShowWindow (WindowHandle, SW_SHOWNORMAL);

bm_info=(BITMAPINFO *)calloc(1,sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
hDC=GetDC(WindowHandle);

bm_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bm_info->bmiHeader.biWidth = COLS; // make neg if reveresd
bm_info->bmiHeader.biHeight = -ROWS;
bm_info->bmiHeader.biPlanes = 1;
bm_info->bmiHeader.biBitCount = 16; // might need to change to 8
bm_info->bmiHeader.biCompression = BI_RGB;
bm_info->bmiHeader.biXPelsPerMeter = 0;
bm_info->bmiHeader.biYPelsPerMeter = 0;
bm_info->bmiHeader.biClrUsed = 0;
bm_info->bmiHeader.biClrImportant = 0;

/* ... set up bmiHeader field of bm_info ... */

for (i=0; i<256; i++)	/* colormap */
  {
  bm_info->bmiColors[i].rgbBlue=bm_info->bmiColors[i].rgbGreen=bm_info->bmiColors[i].rgbRed=i;
  bm_info->bmiColors[i].rgbReserved=0;
  }
SetDIBitsToDevice(hDC,0,0,COLS,ROWS,0,0,
			  0, /* first scan line */
			  ROWS, /* number of scan lines */
			  displaydata,bm_info,DIB_RGB_COLORS);
ReleaseDC(WindowHandle,hDC);
free(bm_info);
MessageBox(NULL,L"Press OK to continue","",MB_OK | MB_APPLMODAL);
}
