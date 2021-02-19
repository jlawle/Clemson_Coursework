/* ECE 6680 - Lab 2 - John Lawler
    This lab handles simple RLE (run-length encoding) compression and
    decompression

    PRINT FILE DATA:
    $ od -t x1 [file]

- scan in entire file  to get rows / COLS and store into memory
- close FILE
- re open file for parsing
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 4

int main(int argc, char *argv[]) {
    int flen, i, position2;
    unsigned char a, b,c;
    FILE *fpt, *output;

    if (argc != 3){
        printf("incorrect arguments!\n ./run [flag] [filename]\n flag options: -c or -d\n");
        exit(0);
    }

    if (strcmp(argv[1], "-c") == 0){
        // Handle compression of the file

        // Open the file
        fpt=fopen(argv[2], "rb");
        if (fpt == NULL){
            printf("Unable to open %s for reading\n",argv[2]);
            exit(0);
        }

        // obtain length of file
        fseek(fpt, 0, SEEK_END);
        flen = ftell(fpt);

        printf("Size of %s is %d \n", argv[2], flen);
        //place fpt back at start
        fseek(fpt, 0, SEEK_SET);

        // read in 4 bytes to compress
        output = fopen("output.rle", "wb");
        int position, runCount = 1, max = 0, dne = 0;
        fread(&a, sizeof(unsigned char), 1, fpt);
        fread(&b, sizeof(unsigned char), 1, fpt);

        while(1){

            // Check if RLE is on
            if (dne >= BUFFERSIZE){
                runCount = 0;
                fseek(fpt, 1, SEEK_CUR);
            }

            // If EOF reached
            if (feof(fpt)) {

                if (dne){
                    runCount = 1;
                    fseek(fpt, -(dne+1), SEEK_END);
                    // write rle on stuff under 4 unique letters
                    for (i = 0; i < dne; i++){
                        fread(&b, sizeof(unsigned char), 1, fpt);
                        fwrite(&runCount,  1, sizeof(unsigned char), output);
                        fwrite(&b, 1, sizeof(unsigned char), output);
                    }
                }
                fwrite(&runCount, 1, sizeof(unsigned char), output);
                fwrite(&a, 1, sizeof(unsigned char), output);
                break;
            }

            if (runCount || max){
                // RLE on
                if (a == b){
                    position = ftell(fpt);
                    if (dne){
                        runCount = 1;
                        fseek(fpt, (position - dne - 2), SEEK_SET);
                        // write rle on stuff under 4 unique letters
                        for (i = 0; i < dne; i++){
                            fread(&b, sizeof(unsigned char), 1, fpt);
                            fwrite(&runCount,  1, sizeof(unsigned char), output);
                            fwrite(&b, 1, sizeof(unsigned char), output);
                        }
                        fseek(fpt, (position), SEEK_SET);
                    }
                    if (runCount < 255) {
                        //max = 0;
                        runCount++;
                        if (runCount == 255) max = 1;
                        fread(&b, sizeof(unsigned char), 1, fpt);
                    } else if (dne == 0 && runCount == 255){
                        // Reset: write A out at max runCount
                        fwrite(&runCount, 1, sizeof(unsigned char), output);
                        fwrite(&a, 1, sizeof(unsigned char), output);
                        if (max) runCount = 1;
                        else runCount = 0;
                        //runCount = 0;
                        max=1;
                        a = b;
                        fread(&b, sizeof(unsigned char), 1, fpt);
                    }

                    // reset dne
                    if (dne) dne = 0;

                } else if (a != b){
                    // Reset: write A out at max runCount
                    if (dne == 0) position = ftell(fpt);
                    dne++;

                    if ((runCount > 1) && max == 0){
                        fwrite(&runCount, 1, sizeof(unsigned char), output);
                        fwrite(&a, 1, sizeof(unsigned char), output);
                        dne = 0;
                    }
                    if (max) {
                        fseek(fpt, -2, SEEK_CUR);
                        fread(&a, sizeof(unsigned char), 1, fpt);
                        fseek(fpt, 1, SEEK_CUR);


                        if (runCount == 0) runCount = 1;
                        fwrite(&runCount, 1, sizeof(unsigned char), output);
                        fwrite(&a, 1, sizeof(unsigned char), output);
                        a = b;
                        position2 = ftell(fpt);
                        fread(&b, sizeof(unsigned char), 1, fpt);
                        if (a == b){
                            dne = 0;
                            fseek(fpt, -2, SEEK_CUR);
                            fread(&a, sizeof(unsigned char), 1, fpt);
                            fseek(fpt, 1, SEEK_CUR);
                        } else {
                            dne--;
                            position2 = ftell(fpt);
                            // check the next value
                            fread(&c, sizeof(unsigned char), 1, fpt);
                            fseek(fpt, -1, SEEK_CUR);

                            if (b == c){
                                // write out a
                                runCount = 1;
                                fwrite(&runCount, 1, sizeof(unsigned char), output);
                                fwrite(&a, 1, sizeof(unsigned char), output);

                                a = b;
                                fseek(fpt, position2, SEEK_SET);
                                fread(&b, sizeof(unsigned char), 1, fpt);
                            }
                        }
                        max = 0;
                    } else {
                        //dne++;
                        a=b;
                        fread(&b, sizeof(unsigned char), 1, fpt);
                    }

                    runCount = 1;
                }

            } else if (runCount == 0) {

                // RLE off
                // revert 4 places
                fseek(fpt,-(dne+3), SEEK_CUR);

                // Write starting marker
                fwrite(&runCount, 1, sizeof(unsigned char), output);

                // Loop and print out until a match is met
                while (1){
                    fread(&a, sizeof(unsigned char), 1, fpt);
                    fread(&b, sizeof(unsigned char), 1, fpt);
                    if (a == b){
                         break;
                     } else fseek(fpt, -1, SEEK_CUR);
                    fwrite(&a, 1, sizeof(unsigned char), output);
                    if (feof(fpt)) break;
                }

                fwrite(&runCount, 1, sizeof(unsigned char), output);
                runCount = 1;
                dne = 0;
            }
        }

        fclose(output);
        fclose(fpt);

    } else if (strcmp(argv[1], "-d") == 0){
        // Handle decompression of the file

        // Open the compressed file
        fpt = fopen(argv[2], "rb");
        if (fpt == NULL){
            printf("Unable to open %s for reading\n",argv[2]);
            exit(0);
        }

        // open output
        output = fopen("output.dec", "wb");

        // obtain length of file
        fseek(fpt, 0, SEEK_END);
        flen = ftell(fpt);

        printf("Size of %s is %d \n", argv[2], flen);
        //place fpt back at start
        fseek(fpt, 0, SEEK_SET);



        // loop through file
        while (1){
            // read in byte
            fread(&a, sizeof(unsigned char), 1, fpt);
            if (feof(fpt)) {
                break;
            }
            // check if RLE
            if (a == 0x00){
                while (1){
                    // read in the next one
                    fread(&a, sizeof(unsigned char), 1, fpt);

                    // end loop once the ending 0 is reached
                    if (a == 0x00) break;

                    // write each consecutive byte
                    fwrite(&a, 1, sizeof(unsigned char), output);
                }
            } else {
                // a is now the runcount, read b and print b for a times
                fread(&b, sizeof(unsigned char), 1, fpt);
                // Loop for runcount times, printing b to output
                for (i = 0; i < a; i++){
                    fwrite(&b, 1, sizeof(unsigned char), output);
                }
            }
        }
        fclose(output);
        fclose(fpt);

    } else {
        printf("Incorrect Flag. please use -c or -d\n");
        exit(0);
    }

    return 0;
}
