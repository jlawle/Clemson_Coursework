
// Predicate 2
float distance(int r, int c) {
	float temp = pow(centr_c - c, 2) + pow(centr_r - r, 2);
	return sqrt(temp);
}

// Predicate 1
float absDiff(int r, int c) {
	return abs(OriginalImage[COLS*r+c] - averageIntensity);
}

// Fill region
void growRegion() {
	char text[300];
	HDC hDC;
	hDC = GetDC(MainWnd);
	int r, c, tr, tc, found=1;
	tempRegion = (unsigned char*)calloc(ROWS * COLS, 1);
	// loop through original image to check if compatible
	while (found) {
		found = 0;
		for (r = 1; r < ROWS-1; r++)
			for (c = 1; c < COLS-1; c++) {
				// if not in region
				if (!(region[COLS * r + c] == 1)) {
					// Loop through surrounding 8 pixels to check if any one of them are in the region
					for (tr = -1; tr <= 1; tr++)
						for (tc = -1; tc <= 1; tc++) {
								// if not on outside border
								if (r != 0 && r != ROWS - 1 &&
									c != 0 && c != COLS - 1) {
									// if found an adjacent region pixel
									if (region[COLS * (r + tr) + (c + tc)] == 1) {
										// Handle user predicates
										if ((absDiff(r, c) <= intensityThreshold) && (distance(r, c) <= centroidThreshold)) {
											tempRegion[COLS * r + c] = 1;
											found = 1;
										}
									}
								}
						}
				}
			} /* end of col */
		//sprintf(text, "region: %d avgint: %f centr_c: %f centr_r: %f", pixelsInRegion, averageIntensity, centr_c, centr_r);
		//TextOut(hDC, 300, 0, text, strlen(text));		/* draw text on the window */
		if (found) {
			// copy new perimeter over
			for (r = 0; r < ROWS; r++){
				for (c = 0; c < COLS; c++) {

					// if temp region copies over a new pixel value to original region
					if (copy(r, c)) {

						// recalculate averages
						averageIntensitySum += OriginalImage[COLS * r + c];
						averageIntensity = averageIntensitySum / pixelsInRegion;
						centr_r_sum += r;
						centr_c_sum += c;
						centr_r = centr_r_sum / pixelsInRegion;
						centr_c = centr_c_sum / pixelsInRegion;

						// Handle stepping through, must have play active to step through
						if (Step == 1 && stepSw == 1 && Play == 0) {
							stepSw = 0;
							while (!stepSw) {}
						}

						// Sleep for play when active or wait until play is turned on
						if (Play) {
							Sleep(0.5);
						}
						else if (Play == 0 && Step == 0) {
							while (!Play) {}
							Sleep(0.5);
						}
						sprintf(text, "region: %d avgint: %f centr_c: %f centr_r: %f", pixelsInRegion, averageIntensity, centr_c, centr_r);
						TextOut(hDC, 300, 0, text, strlen(text));
						// paint the pixels
						hDC = GetDC(MainWnd);
						SetPixel(hDC, c, r, RGB(redINT, greenINT, blueINT));
						ReleaseDC(MainWnd, hDC);
					}
				}
			}
		}
	}
}
