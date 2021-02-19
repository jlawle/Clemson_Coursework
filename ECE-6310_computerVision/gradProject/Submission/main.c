/*	John Lawler
*	ECE 6310 Semester Graduate Project
	Active Contouring GUI program

	-> 3 Options for User <-
	
		1. Left click + drag around a food item to create
	contour points, then shrink boundary to food 
		
		2. Right click a point inside a food item, grow from point 
	to outer boundary-line on food item 

		3. Shift + right or left click to manually drag a contour	
	point to a new location



*/





#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
#include <process.h>	/* needed for multithreading */
#include "resource.h"
#include "globals.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)

{
	MSG			msg;
	HWND		hWnd;
	//HWND		predBox;
	WNDCLASS	wc;

	// windows class, details how the entire window will look, etc
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc; /* event handler set up - gives pointer to a function WndProc*/
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, "ID_PLUS_ICON");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = "ID_MAIN_MENU";
	wc.lpszClassName = "PLUS";

	// registers window class
	if (!RegisterClass(&wc))
		return(FALSE);

	// using window class to create a window, with horizontal and vertical scroll bar, size 
	hWnd = CreateWindow("PLUS", "plus program",
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		CW_USEDEFAULT, 0, 400, 400, NULL, NULL, hInstance, NULL);

	// check that window is actually created
	if (!hWnd)
		return(FALSE);

	// display the window on the screen
	ShowScrollBar(hWnd, SB_BOTH, FALSE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MainWnd = hWnd;

	// global var set 
	//region = (unsigned char*)calloc(ROWS * COLS, 1);
	//intensityThreshold = 10;
	//centroidThreshold = 10;

	ShowPixelCoords = 0;
	StartBubble = 0;
	createBub = 0;
	cpCount = 1;
	ShowPixels = 0;
	LButton = 0;
	RButton = 0;

	//Play = 0;
	//Step = 0;

	// file that is ppm image to load
	strcpy(filename, "");
	OriginalImage = NULL;
	ROWS = COLS = 0;

	/* Below:
		event handling loop - howe all gui programs work.
		program stays in this loop until user says to quit

	 */
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return(msg.wParam);
}




/* function takes in what window got the event, type of event (msg), and message paramenters
	- for instance if its a button press, parameter might be where (coords) button press was
	- key press, which key press it was is potental parameter */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, 
	WPARAM wParam, LPARAM lParam) {

	HMENU				hMenu;
	OPENFILENAME		ofn;
	FILE*				fpt;
	HDC					hDC;
	char				header[320], text[320];
	int					BYTES, xPos, yPos, r, c, i;
	int					j=0, addPoint;


	// switch through different types of messages 
	switch (uMsg) {
		/* case 1: did we recieve a command from the menu? */
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		/*case ID_CLEAR_REGION:
			free(tempRegion);
			tempRegion = (unsigned char*)calloc(ROWS * COLS, sizeof(unsigned char));
			free(region);
			region = (unsigned char*)calloc(ROWS * COLS, sizeof(unsigned char));
			PaintImage();
			
			
			break;*/

		//case ID_RGB_DLG:
		///*	RGBBox = CreateDialog((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_RGB), hWnd, (DLGPROC)AboutDlgProc);
		//	ShowWindow(RGBBox, SW_SHOW);

		//	SetDlgItemTextA(RGBBox, TEXT_RED, "Red");
		//	SetDlgItemTextA(RGBBox, TEXT_BLUE, "Blue");
		//	SetDlgItemTextA(RGBBox, TEXT_GREEN, "Green");*/

		//	break;

		//case ID_PARAMETER_DLG:
		//	/*predBox = CreateDialog((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PARAMETER), hWnd, (DLGPROC)AboutDlgProc);
		//	ShowWindow(predBox, SW_SHOW);

		//	SetDlgItemTextA(predBox, TEXT_CENT, "Centroid Threshold:");
		//	SetDlgItemTextA(predBox, TEXT_INTENSITY, "Intensity Threshold:");*/
		//	break;

		/*case ID_DISPLAY_BIGDOTS:
			ShowBigDots = (ShowBigDots + 1) % 2;
			PaintImage();
			break;*/

		//case ID_PLAY:
		//	Play = (Play + 1) % 2;
		//	//PaintImage();
		//	break;

		//case ID_STEP:
		//	Step = (Step + 1) % 2;
		//	//PaintImage();
		//	break;
		case ID_CLEARSCREEN:
			
			PaintImage();
			// reset globals
			cpX = (int*)calloc(1, sizeof(int));
			cpY = (int*)calloc(1, sizeof(int));
			finX = (int*)calloc(0, sizeof(int));
			finY = (int*)calloc(0, sizeof(int));
			bpX = (int*)calloc(60, sizeof(int));
			bpY = (int*)calloc(60, sizeof(int));
			break;

		case CREATEBUBBLE:
			/* toggles this variable between 0 and 1 */

			createBub = (createBub + 1) % 2;
			PaintImage();
			break;

		case BUBBLE:
			/* toggles this variable between 0 and 1 */
			StartBubble = 1;

			if (createBub) {
				_beginthread(bubble, 0, MainWnd);
				RButton = 0;
				//		createBub = 0;
				//PaintImage();
			}
			break;

		case ID_SHOWPIXELS:
			/* toggles this variable between 0 and 1 */

			ShowPixels = (ShowPixels + 1) % 2;
			LButton = 0;
			PaintImage();
			break;
			/* menu command that occurs when clicking "load image" - uses this ID*/
		case ID_FILE_LOAD:

			// Handle PNM files -> convert to grayscale 

			if (OriginalImage != NULL)
			{
				free(OriginalImage);
				OriginalImage = NULL;
			}
			memset(&(ofn), 0, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = filename;
			filename[0] = 0;
			ofn.nMaxFile = MAX_FILENAME_CHARS;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
			ofn.lpstrFilter = "PPM files\0*.ppm\0All files\0*.*\0\0";
			/* GUI element that gives dialog box on where to open a file or what image, and retrieves the name*/
			if (!(GetOpenFileName(&ofn)) || filename[0] == '\0')
				break;		/* user cancelled load */

			// Open file for reading
			if ((fpt = fopen(filename, "rb")) == NULL) {
				MessageBox(NULL, "Unable to open file", filename, MB_OK | MB_APPLMODAL);
				break;
			}

			/* load ppm image */
			fscanf(fpt, "%s %d %d %d ", header, &COLS, &ROWS, &BYTES);
			if (BYTES != 255) {
				MessageBox(NULL, "Bytes != 255, wrong file type", filename, MB_OK | MB_APPLMODAL);
				fclose(fpt);
				break;
			}

			/* Convert to grayscale if not */
	//		header[0] = fgetc(fpt);	/* whitespace character after header */
			OriginalImage = (unsigned char*)calloc(ROWS * COLS, 1);
			if (strcmp(header, "P5") == 0) {
	
				fread(OriginalImage, 1, ROWS * COLS, fpt);
			} 
			else if (strcmp(header, "P6") == 0) {
				// Convert to Grayscale here by averaging each RGB pixel intensities
				unsigned char pixel[3];
				for (i = 0; i < ROWS*COLS; i++ ){
					fread(pixel, 3, 1, fpt);
					OriginalImage[i] =(pixel[0] + pixel[1] + pixel[2]) / 3;
				}
			}
			else {
				MessageBox(NULL, "Error determining file type - must be pnm or ppm", filename, MB_OK | MB_APPLMODAL);
				fclose(fpt);
				break;
			}


			fclose(fpt);
			SetWindowText(hWnd, filename);
			PaintImage();
			break;

			/* Once message "quit" recieved, destroys window and exits program */
		case ID_FILE_QUIT:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_SIZE:		  /* could be used to detect when window size changes */
		hDC = GetDC(MainWnd);
		PaintImage();
		ReleaseDC(MainWnd, hDC);
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_PAINT:
		PaintImage();
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;

	case WM_LBUTTONUP:
		
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		hDC = GetDC(MainWnd);

	

		// Start contouring the image
		if (LButton == 1 && !(shift < 0) && !StartBubble) {
			recontour = 0;
			_beginthread(contour, 0, MainWnd);
		}
		else if (LButton && (shift < 0) && !StartBubble) {
			scpx = xPos;
			scpy = yPos;
			finX[pointIndex] = xPos;
			finY[pointIndex] = yPos;

			recontour = 1;
			_beginthread(contour, 0, MainWnd);
		}
		else if (StartBubble && recontour && (shift < 0)) {

			scpx = xPos;
			scpy = yPos;
			finX[pointIndex] = xPos;
			finY[pointIndex] = yPos;
			recontour = 1;
			_beginthread(bubble, 0, MainWnd);
		}
		
		LButton = 0;
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_RBUTTONUP:
			// retrieve the pixel at position of click
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);

		int count = 0;
		int x = 0, y = 0;
		
		// Start contouring the image
		if (RButton && createBub) {
			RButton = 0;

			sprintf(text, "Hello\n");
			OutputDebugString(text);

			// Save the positive x values first 
			for (y = rad; y > -rad; y--) {
				x = sqrt(pow(rad, 2) - pow(y, 2));


				if ((yPos + y) < ROWS && (yPos + y) >= 0 &&
					(xPos + x) < COLS && (xPos + x) >= 0) {

					// fill two spots prior
					if (y == rad) {
						bpY[count] = yPos + y;
						bpX[count] = xPos - 2;

						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos - 1;
						count += 2;
					}


					bpY[count] = yPos + y;
					bpX[count] = xPos + x;


					// add in a few spots
					if (y == rad) {
						bpY[count + 1] = yPos + rad;
						bpX[count + 1] = xPos + 1;
						bpY[count + 2] = yPos + rad;
						bpX[count + 2] = xPos + 2;
						bpY[count + 3] = yPos + rad - 1;
						bpX[count + 3] = xPos + 3;
						count += 3;
					}
					else if (y == 9) {
						bpY[count + 1] = yPos + y - 1;
						bpX[count + 1] = xPos + x + 1;
						count += 1;
					}
					else if (y == -8) {
						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos + x - 1;
						count += 1;
					}
					else if (y == -9) {
						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos + x - 1;
						count += 1;
					}
					count++;
				}
			}

			// Save the negative x values
			for (y = -rad; y < rad; y++) {
				x = -sqrt(pow(rad, 2) - pow(y, 2));

				if ((yPos + y) < ROWS && (yPos + y) >= 0 &&
					(xPos + x) < COLS && (xPos + x) >= 0) {

					// fill two spots prior
					if (y == -rad) {
						bpY[count] = yPos + y;
						bpX[count] = xPos + 2;

						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos + 1;
						count += 2;
					}

					bpY[count] = yPos + y;
					bpX[count] = xPos + x;

					// add in a few spots
					if (y == -rad) {
						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos - 1;

						bpY[count + 2] = yPos + y;
						bpX[count + 2] = xPos - 2;

						bpY[count + 3] = yPos + y - 1;
						bpX[count + 3] = xPos - 3;
						count += 3;
					}
					else if (y == -9) {
						bpY[count + 1] = yPos - 8;
						bpX[count + 1] = xPos - 5;
						count += 1;
					}
					else if (y == 8) {
						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos - 5;
						count += 1;
					}
					else if (y == 9) {
						bpY[count + 1] = yPos + y;
						bpX[count + 1] = xPos - 3;
						count += 1;
					}

					count++;
				}
			}

			hDC = GetDC(MainWnd);
			for (i = 0; i < count; i++) {
				SetPixel(hDC, bpX[i], bpY[i], RGB(255, 150, 150));	/* color the cursor position red */
				sprintf(text, "count:%d -  %d, %d\n", i, bpX[i], bpY[i]);
				OutputDebugString(text);
			}
			ReleaseDC(MainWnd, hDC);
			bcpCount = count;
		}





		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_RBUTTONDOWN:
		hDC = GetDC(MainWnd);
	
		RButton = 1;
		recontour = 0;
		// turn off active contouring
		if (LButton) LButton = 0;

		// retrieve the pixel at position of click
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
	
		bpX = (int*)calloc(60, sizeof(int));
		bpY = (int*)calloc(60, sizeof(int));

		ReleaseDC(MainWnd, hDC);

		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_LBUTTONDOWN: 

		// Retrieve states
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		shift = GetKeyState(VK_LSHIFT);
		sprintf(text, "%d", shift);
		OutputDebugString(text);


		hDC = GetDC(MainWnd);
		LButton = 1;
		if (RButton) RButton = 0;
		
		if (ShowPixels == 1 && !(shift < 0)) {
			cpX = (int*)calloc(1, sizeof(int));
			cpY = (int*)calloc(1, sizeof(int));
			cpCount = 0; 


			// if position is valid 
			if (xPos >= 2 && xPos < COLS - 2 && yPos >= 2 && yPos < ROWS - 2) {
				sprintf(text, "%d,%d=>%d     ", xPos, yPos, OriginalImage[yPos * COLS + xPos]);
				TextOut(hDC, 0, 0, text, strlen(text));		/* draw text on the window */
				SetPixel(hDC, xPos, yPos, RGB(255, 150, 150));	/* color the cursor position red */
				ReleaseDC(MainWnd, hDC);
			}
		}
		else if (shift < 0) {
	

			// Check if there has been a contouring already
			if (cpCount > 1) {
				// reset point in the array and run the program again
				scpx = xPos;
				scpy = yPos;

				// find indexed point in cp array and set to point
				pointIndex = -1;
				for (i = 0; i < cpCount; i++) {
					if (pointIndex < 0) {
						// check for point
						if (xPos <= (finX[i] + ptWnd) && xPos >= (finX[i] - ptWnd)) {
							if (yPos <= (finY[i] + ptWnd) && yPos >= (finY[i] - ptWnd)) {
								pointIndex = i;
								scpx = finX[i];
								scpy = finY[i];
							}
						}
					}
				}
				if (pointIndex >= 0) {
					sprintf(text, " point %d x: %d,  y: %d\n", pointIndex, finX[pointIndex], finY[pointIndex]);
					OutputDebugString(text);
				}
			} 
			else {
				sprintf(text, "No contour to adjust!");
				TextOut(hDC, 0, 0, text, strlen(text));		/* draw text on the window */

				sprintf(text, "pointIndex: %d, cpCount: %d\n", pointIndex, cpCount);
				OutputDebugString(text);
			}
		}

	
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_MOUSEMOVE:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		if (ShowPixels == 1 && LButton == 1 && !(shift < 0)) {
			Sleep(10);
			addPoint = 1;

			// Set x and y position from message parameters
		
			// if appropriate bounds
			if (xPos >= 0 && xPos < COLS && yPos >= 0 && yPos < ROWS) {
			//	sprintf(text, "%d,%d=>%d     ", xPos, yPos, OriginalImage[yPos * COLS + xPos]);
				hDC = GetDC(MainWnd);
				SetPixel(hDC, xPos, yPos, RGB(255, 0, 0));	/* color the cursor position red */
				ReleaseDC(MainWnd, hDC);

				// print crosses and save every other point
				
				// check if no repeated points, if not add to list
				for (i = 0; i < cpCount; i++) {
					if (xPos == cpX[i] && yPos == cpY[i]) {
						addPoint = 0;
					}	
				}

				// if the point is not repeated already
				if (addPoint) {
					// Add given point to li
					cpX[cpCount] = xPos;
					cpY[cpCount] = yPos;

					cpCount++;

					cpX = (int*)realloc(cpX, (cpCount+1)*sizeof(int));
					cpY = (int*)realloc(cpY, (cpCount+1)*sizeof(int));

					

					sprintf(text, "contour point %d: %d,%d\n", cpCount-1, cpX[cpCount-1], cpY[cpCount-1]);
					OutputDebugString(text);
				}
			}
		}
		else if (LButton && shift < 0) {
			//scpx, scpy
			finX[pointIndex] = xPos;
			finY[pointIndex] = yPos;
			recontour = 1;


			// repaint all pixels
			PaintImage();
			hDC = GetDC(MainWnd);
			for (i = 0; i < cpCount; i++) {
				for (r = -3; r <= 3; r++)
					for (c = -3; c <= 3; c++) {
						if (r == 0) SetPixel(hDC, finX[i] + c, finY[i] + r, RGB(0, 0, 255));
						if (c == 0) SetPixel(hDC, finX[i] + c, finY[i] + r, RGB(0, 0, 255));
					}
			}
			ReleaseDC(MainWnd, hDC);

		}



		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;

		// Handle step mode - J key is 0x6A
	case WM_CHAR:
		switch (wParam) {
		//case 0x6A:
			//stepSw = 1;
		}
		break;
	case WM_KEYDOWN:

		if (wParam == 's' || wParam == 'S')
			PostMessage(MainWnd, WM_COMMAND, ID_SHOWPIXELCOORDS, 0);	  /* send message to self */
		if ((TCHAR)wParam == '1')
		{
			TimerRow = TimerCol = 0;
			SetTimer(MainWnd, TIMER_SECOND, 10, NULL);	/* start up 10 ms timer */
		}
		if ((TCHAR)wParam == '2')
		{
			KillTimer(MainWnd, TIMER_SECOND);			/* halt timer, stopping generation of WM_TIME events */
			PaintImage();								/* redraw original image, erasing animation */
		}
		if ((TCHAR)wParam == '3')
		{
			ThreadRunning = 1;
			_beginthread(AnimationThread, 0, MainWnd);	/* start up a child thread to do other work while this thread continues GUI */
		}
		if ((TCHAR)wParam == '4')
		{
			ThreadRunning = 0;							/* this is used to stop the child thread (see its code below) */
		}
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_TIMER:	  /* this event gets triggered every time the timer goes off */
		hDC = GetDC(MainWnd);
		SetPixel(hDC, TimerCol, TimerRow, RGB(0, 0, 255));	/* color the animation pixel blue */
		ReleaseDC(MainWnd, hDC);
		TimerRow++;
		TimerCol += 2;
		break;
	case WM_HSCROLL:	  /* this event could be used to change what part of the image to draw */
		PaintImage();	  /* direct PaintImage calls eliminate flicker; the alternative is InvalidateRect(hWnd,NULL,TRUE); UpdateWindow(hWnd); */
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_VSCROLL:	  /* this event could be used to change what part of the image to draw */
		PaintImage();
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
		break;
	}

	/* menu cleanup - sets the menu after message */
	hMenu = GetMenu(MainWnd);
	



	if (ShowPixels != 1)
		CheckMenuItem(hMenu, ID_SHOWPIXELS, MF_UNCHECKED);
	else
		CheckMenuItem(hMenu, ID_SHOWPIXELS, MF_CHECKED);




	// Set Play button check
	if (createBub != 1)
		CheckMenuItem(hMenu, CREATEBUBBLE, MF_UNCHECKED);
	else
		CheckMenuItem(hMenu, CREATEBUBBLE, MF_CHECKED);	

	

	DrawMenuBar(hWnd);

	return(0L);
}




void PaintImage()

{
	PAINTSTRUCT			Painter;
	HDC					hDC;
	BITMAPINFOHEADER	bm_info_header;
	BITMAPINFO* bm_info;
	int					i, r, c, DISPLAY_ROWS, DISPLAY_COLS;
	unsigned char* DisplayImage;

	if (OriginalImage == NULL)
		return;		/* no image to draw */

			  /* Windows pads to 4-byte boundaries.  We have to round the size up to 4 in each dimension, filling with black. */
	DISPLAY_ROWS = ROWS;
	DISPLAY_COLS = COLS;
	if (DISPLAY_ROWS % 4 != 0)
		DISPLAY_ROWS = (DISPLAY_ROWS / 4 + 1) * 4;
	if (DISPLAY_COLS % 4 != 0)
		DISPLAY_COLS = (DISPLAY_COLS / 4 + 1) * 4;
	DisplayImage = (unsigned char*)calloc(DISPLAY_ROWS * DISPLAY_COLS, 1);
	for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLS; c++)
			DisplayImage[r * DISPLAY_COLS + c] = OriginalImage[r * COLS + c];

	BeginPaint(MainWnd, &Painter);
	hDC = GetDC(MainWnd);
	bm_info_header.biSize = sizeof(BITMAPINFOHEADER);
	bm_info_header.biWidth = DISPLAY_COLS;
	bm_info_header.biHeight = -DISPLAY_ROWS;
	bm_info_header.biPlanes = 1;
	bm_info_header.biBitCount = 8;
	bm_info_header.biCompression = BI_RGB;
	bm_info_header.biSizeImage = 0;
	bm_info_header.biXPelsPerMeter = 0;
	bm_info_header.biYPelsPerMeter = 0;
	bm_info_header.biClrUsed = 256;
	bm_info_header.biClrImportant = 256;
	bm_info = (BITMAPINFO*)calloc(1, sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bm_info->bmiHeader = bm_info_header;
	for (i = 0; i < 256; i++)
	{
		bm_info->bmiColors[i].rgbBlue = bm_info->bmiColors[i].rgbGreen = bm_info->bmiColors[i].rgbRed = i;
		bm_info->bmiColors[i].rgbReserved = 0;
	}

	SetDIBitsToDevice(hDC, 0, 0, DISPLAY_COLS, DISPLAY_ROWS, 0, 0,
		0, /* first scan line */
		DISPLAY_ROWS, /* number of scan lines */
		DisplayImage, bm_info, DIB_RGB_COLORS);
	ReleaseDC(MainWnd, hDC);
	EndPaint(MainWnd, &Painter);

	free(DisplayImage);
	free(bm_info);
}


void AnimationThread(HWND AnimationWindowHandle) {
	HDC		hDC;
	char	text[300];

	ThreadRow = ThreadCol = 0;
	while (ThreadRunning == 1)
	{
		hDC = GetDC(MainWnd);
		SetPixel(hDC, ThreadCol, ThreadRow, RGB(0, 255, 0));	/* color the animation pixel green */
		sprintf(text, "%d,%d     ", ThreadRow, ThreadCol);
		TextOut(hDC, 300, 0, text, strlen(text));		/* draw text on the window */
		ReleaseDC(MainWnd, hDC);
		ThreadRow += 3;
		ThreadCol++;
		Sleep(100);		/* pause 100 ms */
	}
}


void bubble() {
	int i, ptCount = 0;
	char text[300];

	if (!recontour) {
		for (i = 0; i < bcpCount; i = i + 3) {
			ptCount++;
		}
	}
	else {
		ptCount = bcpCount;
	}

	int* cpCol, * cpRow, j, p, r = 0, c = 0, tr = 0, tc = 0, row = 0, col = 0;
	float min = 0, max = 0, min3 = 0, max3 = 0;;
	float dist = 0, dist2 = 0, sum = 0, avgDist = 0;
	float* intE1, * intE2, *intE3, * totalE;

	HDC hDC;

	// Calculate Sobel image
	float sobx, soby;
	float* sobelx, * sobely, * sobel, * ext_Energy;
	sobel = (float*)calloc(ROWS * COLS, sizeof(float));
	sobelx = (float*)calloc(ROWS * COLS, sizeof(float));
	sobely = (float*)calloc(ROWS * COLS, sizeof(float));
	ext_Energy = (float*)calloc(ROWS * COLS, sizeof(float));

	// Calculate sobel in x and y directions 
	for (r = sblRadius; r <= ROWS - sblRadius; r++)
		for (c = sblRadius; c <= COLS - sblRadius; c++) {
			sobx = soby = 0;

			// loop around every pixel to find x gradient
			for (tr = -sblRadius; tr <= sblRadius; tr++)
				for (tc = -sblRadius; tc <= sblRadius; tc++) {
					sobx += OriginalImage[COLS * (r + tr) + (c + tc)] * Gx[(sblRadius * 2 + 1) * (tr + sblRadius) + (tc + sblRadius)];
				}

			// loop around every pixel to find y gradient	
			for (tr = -sblRadius; tr <= sblRadius; tr++)
				for (tc = -sblRadius; tc <= sblRadius; tc++) {
					soby += OriginalImage[COLS * (r + tr) + (c + tc)] * Gy[(sblRadius * 2 + 1) * (tr + sblRadius) + (tc + sblRadius)];
				}
			sobelx[COLS * r + c] = sobx;
			sobely[COLS * r + c] = soby;

			// Calculate total gradient magnitude
 //           sobel[COLS*r + c] = sqrt(pow(sopx,2) + pow(sopy,2));
		}

	/* Calculate G and min/max */
	for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLS; c++) {
			/* Calculate overall gradient */
			sobel[COLS * r + c] = sqrt(pow(sobelx[COLS * r + c], 2) + pow(sobely[COLS * r + c], 2));

			// find min and max
			if (r == 0 && c == 0)
				min = max = sobel[COLS * r + c];
			if (sobel[COLS * r + c] < min)
				min = sobel[COLS * r + c];
			if (sobel[COLS * r + c] > max)
				max = sobel[COLS * r + c];
		}

	FILE *fpt;
	unsigned char* output;
	output = (unsigned char*)calloc(ROWS * COLS, sizeof(unsigned char));
	for (i = 0; i < ROWS * COLS; i++) {
		*(output + i) = (unsigned char)((sobel[i] - min)*255 / (max - min));


	}

	fpt = fopen("sobelOutput.ppm", "wb");
	fprintf(fpt, "P5 %d %d 255\n", COLS, ROWS);
	fwrite(output, COLS * ROWS, 1, fpt);
	fclose(fpt);

	/* Negate sobel gradient -> external energy */
	min = max = 0;
	for (i = 0; i < ROWS * COLS; i++) {
		*(ext_Energy + i) = sobel[i] * (-1);
		if (*(ext_Energy + i) > max) max = *(ext_Energy + i);
		if (*(ext_Energy + i) < min) min = *(ext_Energy + i);
	}

	/* Normalize external energy between 0 -> 1 */
	for (i = 0; i < ROWS * COLS; i++) {
		*(ext_Energy + i) = ((ext_Energy[i] - min)) / (max - min);
	}


	int* tempCol, * tempRow;
	tempCol = (int*)calloc(ptCount + 1, sizeof(int));
	tempRow = (int*)calloc(ptCount + 1, sizeof(int));

	p = 0;
	cpCol = (int*)calloc(ptCount, sizeof(int));
	cpRow = (int*)calloc(ptCount, sizeof(int));
	sprintf(text, "total points: %d, downscaled count: %d\n", bcpCount, ptCount);
	OutputDebugString(text);


	// Select every 3 points from list 
	int avgPCol = 0, avgPRow = 0;
	if (!recontour) {
		finX = (int*)calloc(ptCount, sizeof(int));
		finY = (int*)calloc(ptCount, sizeof(int));
		for (i = 0; i < bcpCount; i += 3) {
			cpCol[p] = bpX[i];
			cpRow[p] = bpY[i];
			p++;
		}
	}
	else {
		for (i = 0; i < ptCount; i++) {
			cpCol[i] = finX[i];
			cpRow[i] = finY[i];
		}
	}

	// Handle contouring for specified iterations

	for (j = 0; j < bubbleCount; j++) {
		/* find average distance of points */
		for (i = 0; i < ptCount; i++) {
			dist = sqrt(pow((cpCol[i] - cpCol[i + 1]), 2) + pow((cpRow[i] - cpRow[i + 1]), 2));
			sum += dist;
		}
		avgDist = sum / ptCount;

		// Find average center pixel
		for (i = 0; i < ptCount; i++) {
			avgPCol += cpCol[i];
			avgPRow += cpRow[i];
		}
		avgPCol /= ptCount;
		avgPRow /= ptCount;

		sprintf(text, "average row, col: %d, %d\n", avgPRow, avgPCol);
		OutputDebugString(text);
		tempCol = (int*)calloc(ptCount + 1, sizeof(int));
		tempRow = (int*)calloc(ptCount + 1, sizeof(int));

		///* find average distance of points */
		//for (i = 0; i < ptCount; i++) {
		//	dist = sqrt(pow((cpCol[i] - cpCol[i + 1]), 2) + pow((cpRow[i] - cpRow[i + 1]), 2));
		//	sum += dist;
		//}


		//avgDist = sum / ptCount;
		// Calculate internal energies at each cp
		for (p = 0; p < ptCount; p++) {
			intE1 = (float*)calloc(wndSize * wndSize, sizeof(float)); /* internalE 1*/
			intE2 = (float*)calloc(wndSize * wndSize, sizeof(float));
			intE3 = (float*)calloc(wndSize * wndSize, sizeof(float));
			totalE = (float*)calloc(wndSize * wndSize, sizeof(float));
			row = cpRow[p];
			col = cpCol[p];

		
			// Internal energy 1
			for (r = -wndE1; r <= wndE1; r++)
				for (c = -wndE1; c <= wndE1; c++) {
					dist2 = pow(((cpCol[p] + c) - cpCol[p + 1]), 2) + pow(((cpRow[p] + r) - cpRow[p + 1]), 2);
					intE1[wndSize * (r + wndE1) + (c + wndE1)] = dist2;
				}

			

			// Internal E 1 min and max
			min = max = dist2;
			for (i = 0; i < wndSize * wndSize; i++) {
				if (*(intE1 + i) > max) max = *(intE1 + i);
				if (*(intE1 + i) < min) min = *(intE1 + i);
			}

			/* Normalize internal energy 1 */
			for (i = 0; i < wndSize * wndSize; i++)
				*(intE1 + i) = ((intE1[i] - min)) / (max - min);

			// Find internal energy 2 (deviation)
			for (r = -wndE2; r <= wndE2; r++)
				for (c = -wndE2; c <= wndE2; c++) {
						dist = sqrt(pow(((cpCol[p] + c) - avgPCol), 2) + pow(((cpRow[p] + r) - avgPRow), 2));
						intE2[wndSize * (r + wndE2) + (c + wndE2)] = pow((dist - avgDist), 2);
				}

					// Calcuate min and max of int2
			min = max = dist;
			for (i = 0; i < wndSize * wndSize; i++) {
				if (*(intE2 + i) > max) max = *(intE2 + i);
				if (*(intE2 + i) < min) min = *(intE2 + i);
			}

			/* Normalize internal energy 2 */
			for (i = 0; i < wndSize * wndSize; i++) {
				*(intE2 + i) = ((intE2[i] - min)) / (max - min);
			}
		
			// Add energies together
			min = 3;
			i = 0;
			for (r = -wndET; r <= wndET; r++) {
				for (c = -wndET; c <= wndET; c++) {
					//sprintf(text, "%d. Col,Row -> %d,%d \n", p, , );
					//OutputDebugString(text);
					//sprintf(text, "%d c=x, r=y: %d, %d : ext_E index - %d\n", i++, col+c,row+r, COLS * (row + r) + (col + c));
				//	OutputDebugString(text);

					if ((COLS * (row + r) + (col + c)) < 0 || (COLS * (row + r) + (col + c)) > ROWS * COLS) continue;
					totalE[wndSize * (r + wndET) + (c + wndET)] = bint1*intE1[wndSize * (r + wndET) + (c + wndET)] + bint2*intE2[wndSize * (r + wndET) + (c + wndET)] + bubbleExt*ext_Energy[COLS * (row + r) + (col + c)];

					
					// Find minimum energy
					if (totalE[wndSize * (r + wndET) + (c + wndET)] < min) {
						min = totalE[wndSize * (r + wndET) + (c + wndET)];
						tr = row + r;
						tc = col + c;
					}
				}
			}
			if (!recontour) {
				tempRow[p] = tr;
				tempCol[p] = tc;
			}
			else {
				if (p == pointIndex) {
					tempRow[p] = scpy;
					tempCol[p] = scpx;
				}
				else {
					tempRow[p] = tr;
					tempCol[p] = tc;
				}
			}
		} /* end of c.p. loop */

		// set tempRow and tempCol equal to cpRow abd cpCol
		for (i = 0; i < ptCount; i++) {
			cpRow[i] = tempRow[i];
			cpCol[i] = tempCol[i];
			finX[i] = tempCol[i];
			finY[i] = tempRow[i];
		}

		cpRow[ptCount] = cpRow[0];
		cpCol[ptCount] = cpCol[0];


		// paint the pixels
		PaintImage();
		hDC = GetDC(MainWnd);
		for (i = 0; i < ptCount; i++) {
			for (r = -3; r <= 3; r++)
				for (c = -3; c <= 3; c++) {
					if (r == 0) SetPixel(hDC, cpCol[i] + c, cpRow[i] + r, RGB(0, 0, 255));
					if (c == 0) SetPixel(hDC, cpCol[i] + c, cpRow[i] + r, RGB(0, 0, 255));
				}
		}
		ReleaseDC(MainWnd, hDC);

		Sleep(30);
	} /* End of 30 iterations */

// print final control points
	for (i = 0; i < ptCount; i++) {
		sprintf(text, "%d. Col,Row -> %d,%d \n", i + 1, cpCol[i], cpRow[i]);
		OutputDebugString(text);
	}

	cpCount = ptCount;
	bcpCount = ptCount;
	StartBubble = 0;
}


void contour() {
	int i,ptCount=0;
	char text[300];

	if (!recontour) {
		for (i = 0; i < cpCount; i = i + 5) {
			ptCount++;
		}
	}
	else {
		ptCount = cpCount;
	}

	int *cpCol, *cpRow, j, p, r=0, c=0, tr=0, tc=0, row=0, col=0;
	float min=0, max=0;
	float dist=0, dist2=0, sum=0, avgDist=0;
	float *intE1, *intE2, *totalE;

	HDC hDC;

	float sobx, soby;
	float* sobelx, * sobely, * sobel, * ext_Energy;

	sobel = (float*)calloc(ROWS * COLS, sizeof(float));
	sobelx = (float*)calloc(ROWS * COLS, sizeof(float));
	sobely = (float*)calloc(ROWS * COLS, sizeof(float));
	ext_Energy = (float*)calloc(ROWS * COLS, sizeof(float));

	// Calculate sobel in x and y directions 
	for (r = sblRadius; r <= ROWS - sblRadius; r++)
		for (c = sblRadius; c <= COLS - sblRadius; c++) {
			sobx = soby = 0;

			// loop around every pixel to find x gradient
			for (tr = -sblRadius; tr <= sblRadius; tr++)
				for (tc = -sblRadius; tc <= sblRadius; tc++) {
					sobx += OriginalImage[COLS * (r + tr) + (c + tc)] * Gx[(sblRadius * 2 + 1) * (tr + sblRadius) + (tc + sblRadius)];
				}

			// loop around every pixel to find y gradient	
			for (tr = -sblRadius; tr <= sblRadius; tr++)
				for (tc = -sblRadius; tc <= sblRadius; tc++) {
					soby += OriginalImage[COLS * (r + tr) + (c + tc)] * Gy[(sblRadius * 2 + 1) * (tr + sblRadius) + (tc + sblRadius)];
				}
			sobelx[COLS * r + c] = sobx;
			sobely[COLS * r + c] = soby;

			// Calculate total gradient magnitude
 //           sobel[COLS*r + c] = sqrt(pow(sopx,2) + pow(sopy,2));
		}

	/* Calculate G and min/max */
	for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLS; c++) {
			/* Calculate overall gradient */
			sobel[COLS * r + c] = sqrt(pow(sobelx[COLS * r + c], 2) + pow(sobely[COLS * r + c], 2));

			// find min and max
			if (r == 0 && c == 0)
				min = max = sobel[COLS * r + c];
			if (sobel[COLS * r + c] < min)
				min = sobel[COLS * r + c];
			if (sobel[COLS * r + c] > max)
				max = sobel[COLS * r + c];
		}



	/* Sobel Gradient output Image */
	FILE* fp;
	unsigned char* sobelOutput;
	sobelOutput = (unsigned char*)calloc(ROWS * COLS, sizeof(unsigned char));
	for (i = 0; i < ROWS * COLS; i++)
		*(sobelOutput + i) = (unsigned char)(((sqrt(pow(sobelx[i], 2) + pow(sobely[i], 2)) - min) * 255) / (max - min));

	// Write normalized sobel gradient out
	fp = fopen("sobelGradientImage.ppm", "wb");
	fprintf(fp, "P5 %d %d 255\n", COLS, ROWS);
	fwrite(sobelOutput, 1, ROWS * COLS, fp);
	fclose(fp);



	/* Negate sobel gradient -> external energy */
	min = max = 0;
	for (i = 0; i < ROWS * COLS; i++) {
		*(ext_Energy + i) = sobel[i] * (-1);
		if (*(ext_Energy + i) > max) max = *(ext_Energy + i);
		if (*(ext_Energy + i) < min) min = *(ext_Energy + i);
	}

	/* Normalize external energy between 0 -> 1 */
	for (i = 0; i < ROWS * COLS; i++) {
		*(ext_Energy + i) = ((ext_Energy[i] - min)) / (max - min);
		}

	
	int* tempCol, * tempRow;
	tempCol = (int*)calloc(ptCount + 1, sizeof(int));
	tempRow = (int*)calloc(ptCount + 1, sizeof(int));

	p = 0;
	cpCol = (int*)calloc(ptCount, sizeof(int));
	cpRow = (int*)calloc(ptCount, sizeof(int));
	sprintf(text, "total points: %d, downscaled count: %d\n", cpCount, ptCount);
	OutputDebugString(text);


	// Select every 5 points from list 
	if (!recontour) {
		finX = (int*)calloc(ptCount, sizeof(int));
		finY = (int*)calloc(ptCount, sizeof(int));
		for (i = 0; i < cpCount; i += 5) {
			cpCol[p] = cpX[i];
			cpRow[p] = cpY[i];
			p++;
		}
	}
	else {
		for (i = 0; i < cpCount; i++) {
			cpCol[i] = finX[i];
			cpRow[i] = finY[i];
		}
	}


	
	// active countour outwards
	for (j = 0; j < contourCount; j++) {

		tempCol = (int*)calloc(ptCount + 1, sizeof(int));
		tempRow = (int*)calloc(ptCount + 1, sizeof(int));

		/* find average distance of points */
		for (i = 0; i < ptCount; i++) {
			dist = sqrt(pow((cpCol[i] - cpCol[i + 1]), 2) + pow((cpRow[i] - cpRow[i + 1]), 2));
			sum += dist;
		}


		avgDist = sum / ptCount;
		// Calculate internal energies at each cp
		for (p = 0; p < ptCount; p++) {
			intE1 = (float*)calloc(wndSize * wndSize, sizeof(float)); /* internalE 1*/
			intE2 = (float*)calloc(wndSize * wndSize, sizeof(float));
			totalE = (float*)calloc(wndSize * wndSize, sizeof(float));
			row = cpRow[p];
			col = cpCol[p];

			sprintf(text, "row: %d, col: %d\n", row, col);
			OutputDebugString(text);

			// Internal energy 1
			for (r = -wndE1; r <= wndE1; r++)
				for (c = -wndE1; c <= wndE1; c++) {
					dist2 = pow(((cpCol[p] + c) - cpCol[p + 1]), 2) + pow(((cpRow[p] + r) - cpRow[p + 1]), 2);
					intE1[wndSize * (r + wndE1) + (c + wndE1)] = iE1a*dist2;
				}

			// Internal E 1 min and max
			min = max = dist2;
			for (i = 0; i < wndSize*wndSize; i++) {
				if (*(intE1 + i) > max) max = *(intE1 + i);
				if (*(intE1 + i) < min) min = *(intE1 + i);
			}

			/* Normalize internal energy 1 */
			for (i = 0; i < wndSize*wndSize; i++)
				*(intE1 + i) = ((intE1[i] - min)) / (max - min);


			// Find internal energy 2 (deviation)
			if (!recontour) {
				for (r = -wndE2; r <= wndE2; r++)
					for (c = -wndE2; c <= wndE2; c++) {
						dist = sqrt(pow(((cpCol[p] + c) - cpCol[p + 1]), 2) + pow(((cpRow[p] + r) - cpRow[p + 1]), 2));
						intE2[wndSize * (r + wndE2) + (c + wndE2)] = iE2a * pow((dist - avgDist), 2);
					}
			}
			else {
				for (i = 0; i < wndSize * wndSize; i++) {
					intE2[i] = 0;
				}
			}
			// Calcuate min and max of int2
			min = max = dist;
			for (i = 0; i < wndSize*wndSize; i++) {
				if (*(intE2 + i) > max) max = *(intE2 + i);
				if (*(intE2 + i) < min) min = *(intE2 + i);
			}

			/* Normalize internal energy 2 */
			for (i = 0; i < wndSize * wndSize; i++) {
				*(intE2 + i) = ((intE2[i] - min)) / (max - min);
			}

			// Add energies together
			min = 3;
			i = 0;
			for (r = -wndET; r <= wndET; r++) {
				for (c = -wndET; c <= wndET; c++) {
					//sprintf(text, "%d. Col,Row -> %d,%d \n", p, , );
					//OutputDebugString(text);
					//sprintf(text, "%d c=x, r=y: %d, %d : ext_E index - %d\n", i++, col+c,row+r, COLS * (row + r) + (col + c));
				//	OutputDebugString(text);

					if ((COLS * (row + r) + (col + c)) < 0 || (COLS * (row + r) + (col + c)) > ROWS*COLS) continue;
					totalE[wndSize * (r + wndET) + (c + wndET)] = iE1_tot*intE1[wndSize * (r + wndET) + (c + wndET)] + iE2a_tot*intE2[wndSize * (r + wndET) + (c + wndET)] + extE_tot*ext_Energy[COLS * (row + r) + (col + c)];


					//if (r == -wndET && c == -wndET) min = totalE[wndSize * (r + wndET) + (c + wndET)];
					// Find minimum energy
					if (totalE[wndSize * (r + wndET) + (c + wndET)] < min) {
						min = totalE[wndSize * (r + wndET) + (c + wndET)];
						tr = row + r;
						tc = col + c;
					}
				}
			}
			// set temp col and row after found
			if (!recontour) {
				tempRow[p] = tr;
				tempCol[p] = tc;
			}
			else {
				if (p == pointIndex) {
					tempRow[p] = scpy;
					tempCol[p] = scpx;
				}
				else {
					tempRow[p] = tr;
					tempCol[p] = tc;
				}
			}
		} /* end of c.p. loop */

		// set tempRow and tempCol equal to cpRow abd cpCol
		for (i = 0; i < ptCount; i++) {
			cpRow[i] = tempRow[i];
			cpCol[i] = tempCol[i];
			finX[i] = tempCol[i];
			finY[i] = tempRow[i];
		}

		cpRow[ptCount] = cpRow[0];
		cpCol[ptCount] = cpCol[0];
		finX[ptCount] = finX[0];
		finY[ptCount] = finY[0];


		// paint the pixels
		PaintImage();
		hDC = GetDC(MainWnd);
		for (i = 0; i < ptCount; i++) {
			for (r = -3; r <= 3; r++)
				for (c = -3; c <= 3; c++) {
					if (r == 0) SetPixel(hDC, cpCol[i] + c, cpRow[i] + r, RGB(0, 0, 255));
					if (c == 0) SetPixel(hDC, cpCol[i] + c, cpRow[i] + r, RGB(0, 0, 255));
				}
		}
		ReleaseDC(MainWnd, hDC);

		Sleep(30);
		} /* End of 30 iterations */

	// print final control points
	for (i = 0; i < ptCount; i++) {
		sprintf(text, "%d. Col,Row -> %d,%d \n", i + 1, finX[i], finY[i]);
		OutputDebugString(text);
	}

	cpCount = ptCount;
}



