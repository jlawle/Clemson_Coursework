
#define SQR(x) ((x)*(x))	/* macro for square */
#ifndef M_PI			/* in case M_PI not found in math.h */
#define M_PI 3.1415927
#endif
#ifndef M_E
#define M_E 2.718282
#endif

#define MAX_FILENAME_CHARS	320
#define SOBELSIZE 

char	filename[MAX_FILENAME_CHARS];

HWND	MainWnd;
HWND	predBox;
HWND    RGBBox;


// make globals for all averages, centroidavg, ine
int intensityThreshold;
int centroidThreshold;
int redINT, greenINT, blueINT;
float averageIntensity, averageIntensitySum;
float centr_r, centr_r_sum; // y var
float centr_c, centr_c_sum; // x vart

 // sobel gradient template

float *externalEnergy;
int sblRadius = 1;
float Gx[] = { -1.0, 0.0, 1.0,
                -2.0, 0.0, 2.0,
                -1.0, 0.0, 1.0,
};
float Gy[] = { 1.0, 2.0, 1.0,
                0.0, 0.0, 0.0,
                -1.0, -2.0, -1.0,
};


// bubble 
int* bpX, * bpY;

// Iterations
int contourCount = 40;
int bubbleCount = 100;
int rad = 10;
int bcpCount= 0;

// Shift constants
int scpx, scpy;
int ptWnd = 2;
int pointIndex;
int recontour;



// internal energy windows
int wndSize = 7;
int wndE1 = 3;
int wndE2 = 3;
int wndET = 3;
int wndE3 = 3;
// energies
float iE1a = 1.0;
float iE2a = 1.0;
float iE1_tot = 1.7;
float iE2a_tot = 1.15;
float extE_tot = 1.7;

float bubbleExt = 2.5;
float bubbleInt = 1;
float bint1 = -1.0;
float bint2 = -1.5;
float bint3 = -0.7;

		// Display flags
int		shift;
int		createBub;
int		ShowPixelCoords;
int		ShowPixels;
int		StartBubble;
int		cpCount;
int		LButton;
int		RButton;

		// Image data
float			*extE;
unsigned char	*OriginalImage;
int *cpX, *cpY;
int* finX, * finY;

//unsigned char	*tempRegion;
//unsigned char	*region;
//unsigned char   *perimeterToCheck;
int				ROWS, COLS, pixelsInRegion;

#define TIMER_SECOND	1			/* ID of timer used for animation */

		// Drawing flags
int		TimerRow,TimerCol;
int		ThreadRow,ThreadCol;
int		ThreadRunning;

		// Function prototypes
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void PaintImage();
void AnimationThread(void *);		/* passes address of window */
INT_PTR CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
//float distance(int r, int c);
//float absDiff(int r, int c);
//void growRegion();
//int copy(int r, int c);
//void sgradient();
void contour();
void bubble();
void setpx();