/* ECE 6680 - Lab 3 - John Lawler

This lab compresses and decompresses uses the LZW algorithm
    It generates a dictionary on compression and decompression to run
    values against and compare/build
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *dictionary[65536];   // points to each pattern
unsigned short code[65536];
int length[65536];

// print dictionary
void printDictionary(int dsize){
    int i,j;

    // Print each line
    printf(" Code\t| Length  |\tPattern \t size = %d\n", dsize);
    for (i = 0; i < dsize; i++){
        printf(" %hu \t| %d \t", code[i], length[i]);
        for (j = 0; j < length[i]; j++){
            printf("%x ", dictionary[i][j]);
        }
        printf("\n");

    }
}

// Find value in the dictionary, return 1 if found
int findVal(int dsize, unsigned char *val, int len){
    int i, index = 0;

        // add check for same size too with length

        // loop through dictionary to find a match
        for (i = 0; i < dsize; i++){
            if (length[i] == len){
                if (memcmp(dictionary[i], val, len) == 0) {
                    index = code[i];
                    break;
                }
            }
        }

        return index;
}


int main(int argc, char *argv[]){
    FILE *fpt;
    int i, flen;

    if (argc != 3){
        printf("incorrect arguments!\n ./run [flag] [filename]\n flag options: -c or -d\n");
        exit(0);
    }


    // Open the file
    fpt=fopen(argv[2], "rb");
    if (fpt == NULL){
        printf("Unable to open %s for reading\n",argv[2]);
        exit(0);
    }

    // Obtain length of file
    fseek(fpt, 0, SEEK_END);
    flen = ftell(fpt);
    if (flen == 0) {
        printf("File empty");
        exit(0);
    }

    printf("Size of %s is %d \n", argv[2], flen);
    //place fpt back at start
    fseek(fpt, 0, SEEK_SET);


    // Read in file
    unsigned char *image = malloc(flen + 1);
    fread(image, 1, flen, fpt);

    fseek(fpt, 0, SEEK_SET);
    int dsize=0;                          // size of dictionary

    // Initialize dictionary roots
    for (i = 0; i < 256; i++) {
        length[i] = 1;
        dictionary[i] = (unsigned char*)calloc(1, sizeof(unsigned char));
        dictionary[i][0] = (unsigned char)i;
        dsize++;
    }

    // Initialize code
    for (i = 0; i < 65535; i++){
        code[i] = i+1;
    }

    printf("size is %d\n", dsize);


    // handle compression
    if (strcmp(argv[1], "-c") == 0){
        unsigned char *p, *pc, c;
        int index, codeOut, len_p = 1;

        // initialize p and pc
        p = (unsigned char*)calloc(1, sizeof(unsigned char));
        pc = (unsigned char*)calloc(1, sizeof(unsigned char));

        // Open a file for output
        FILE *output;
        output = fopen("output.lzw", "wb");


        // Read first byte (bound to be in dictionary)
        fread(&c, sizeof(unsigned char), 1, fpt);
        *pc = c;
        *p = *pc;

        while(1) {

            // Read current byte
            fread(&c, sizeof(unsigned char), 1, fpt);

            // If EOF reached
            if (feof(fpt)) {
                // output code for P
                codeOut = findVal(dsize, p, len_p);
                fwrite(&codeOut, 1, sizeof(unsigned short), output);
                break;
            }

            // Add P and C
            pc = (unsigned char*)realloc(pc, sizeof(unsigned char)*(len_p+1));

            // copy p
            for (i = 0; i < len_p; i++)
                *(pc + i) = *(p + i);

            // copy c
            *(pc + len_p) = c;

            // Is P + C in dictionary?
            index = findVal(dsize, pc, len_p+1);

            // If the value is found
            if (index){

                // Let P = P + C
                len_p++;
                p = (unsigned char*)realloc(p, sizeof(unsigned char)*len_p);

                // copy pc to p
                for (i = 0; i < len_p; i++)
                    *(p + i) = *(pc + i);

            } else {
                // Handle when P + C not in dictionary

                // Output code for P
                codeOut = findVal(dsize, p, len_p);

                fwrite(&codeOut, 1, sizeof(unsigned short int), output);

                // Add PC to dictionary
                length[dsize] = len_p+1;
                dictionary[dsize] = (unsigned char*)calloc(len_p+1, sizeof(unsigned char));

                // Copy PC to dictionary
                for (i = 0; i <= len_p; i++)
                    dictionary[dsize][i] = *(pc + i);

                // Increase dictionary size by 1
                dsize++;

                // Let P = C, calloc pointers
                pc = (unsigned char*)calloc(1, sizeof(unsigned char));
                p = (unsigned char*)calloc(1, sizeof(unsigned char));
                len_p = 1;
                *pc = c;
                *p = *pc;
            }
        }

        free(p);
        free(pc);
        fclose(output);
        fclose(fpt);

    } else {
        // Handle decompression
        unsigned char *p, *c;
        unsigned char *xy, *xz;
        unsigned short codeIn;
        int index;
        int len_c = 1;
        int len_p = 1;


        // initialize p and pc
        p = (unsigned char*)calloc(1, sizeof(unsigned char));
        c = (unsigned char*)calloc(1, sizeof(unsigned char));
        xy = (unsigned char*)calloc(1, sizeof(unsigned char));
        xz = (unsigned char*)calloc(1, sizeof(unsigned char));

        // Open a file for output
        FILE *output;
        output = fopen("output.dec", "wb");


        // Read first byte (bound to be in dictionary)
        fread(&codeIn, sizeof(unsigned short), 1, fpt);

        // loop through dictionary to find a match
        for (i = 0; i < dsize; i++)
            if (code[i] == codeIn) {
                index = i;
                break;
            }

        // Initialize c
        *c = dictionary[index][0];
        len_c = length[i];

        printf("c %x \t codeIn = %hu -> index = %d \n",c[0],codeIn,index);

        // Output pattern for C
        for (i = 0; i < len_c; i++)
            fwrite(&c[i], 1, sizeof(unsigned char), output);

        // Loop through file of codes
        while (1){

            // Read first byte (bound to be in dictionary)
            fread(&codeIn, sizeof(unsigned short), 1, fpt);

            // If EOF reached
            if (feof(fpt)) break;

            // Copy pattern c to p
            len_p = len_c;
            p = (unsigned char*)realloc(p, sizeof(unsigned char)*len_p);
            for (i = 0; i < len_p; i++)
                *(p + i) = *(c + i);

            index = 0;

            // loop through dictionary to find a match
            for (i = 0; i < dsize; i++){
                if (code[i] == codeIn) {
                    index = i;
                    break;
                }
            }

            if (index){     // If C is in dictionary


                // Set length of c
                len_c = length[index];

                // Copy pattern to c
                c = (unsigned char*)realloc(c, sizeof(unsigned char)*len_c);
                for (i = 0; i < len_c; i++)
                    *(c + i) = dictionary[index][i];

                // Write out pattern for C
                for (i = 0; i < len_c; i++)
                    fwrite(&c[i], 1, sizeof(unsigned char), output);

                // Copy p to xy
                xy = (unsigned char*)realloc(xy, sizeof(unsigned char)*(len_p + 1));
                for (i = 0; i < len_p; i++)
                    *(xy + i) = *(p + i);

                // copy c
                *(xy + len_p) = c[0];


                // Copy XY to dictionary
                length[dsize] = len_p+1;
                dictionary[dsize] = (unsigned char*)calloc(len_p+1, sizeof(unsigned char));
                for (i = 0; i <= len_p; i++)
                    dictionary[dsize][i] = *(xy + i);

                dsize++;


            } else {
                // C is not in dictionary

                // Generate XZ: P + P[0]
                xz = (unsigned char*)realloc(xz, sizeof(unsigned char)*(len_p + 1));
                for (i = 0; i < len_p; i++)
                    *(xz + i) = *(p + i);

                // Add first char of P
                *(xz + len_p) = p[0];


                // Output XZ
                for (i = 0; i <= len_p; i++)
                    fwrite(&xz[i], 1, sizeof(unsigned char), output);


                // Copy XZ to dictionary
                length[dsize] = len_p+1;
                dictionary[dsize] = (unsigned char*)calloc(len_p+1, sizeof(unsigned char));
                for (i = 0; i <= len_p; i++)
                    dictionary[dsize][i] = *(xz + i);

                dsize++;

                // Copy xz to c
                len_c = len_p + 1;
                c = (unsigned char*)realloc(c, sizeof(unsigned char)*(len_c));
                for (i = 0; i < len_c; i++)
                    *(c + i) = *(xz + i);


            }



        }

        fclose(fpt);
        fclose(output);


    } // end of flag ifelse

    // print out dictionary
    //printDictionary(dsize);

    return 0;
}
